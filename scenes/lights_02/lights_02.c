#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE

#define SCENE_PATH "scenes/lights_02/"

#include "lights_02.h"

#include <cglm/cglm.h>

#include <stb/stb_image.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../source/ShaderManager.h"
#include "../../source/CameraManager.h"


typedef struct EnvAttribs
{
	unsigned int window_width;
	unsigned int window_height;
}EnvAttribs;

typedef struct AppContex
{
	Camera *camera;
	EnvAttribs *env;
}AppContex;


static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
static void processInput(GLFWwindow *window, Camera *camera);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


/*int main(int argc, char* argv[])*/
int light_scene_02(void)
{
	GLFWwindow* window;
	float cube_vertices[] =
	{
		/* positions         	normals           	texture coords*/
		/*front */
		-0.5f, -0.5f,  0.5f, 	 0.0f,  0.0f,  1.0f,	
		 0.5f, -0.5f,  0.5f, 	 0.0f,  0.0f,  1.0f,	
		 0.5f,  0.5f,  0.5f, 	 0.0f,  0.0f,  1.0f,	
		-0.5f,  0.5f,  0.5f, 	 0.0f,  0.0f,  1.0f,	

		/*back */
		 0.5f, -0.5f, -0.5f, 	 0.0f,  0.0f, -1.0f,	
		-0.5f, -0.5f, -0.5f, 	 0.0f,  0.0f, -1.0f,	
		-0.5f,  0.5f, -0.5f, 	 0.0f,  0.0f, -1.0f,	
		 0.5f,  0.5f, -0.5f, 	 0.0f,  0.0f, -1.0f,	

		/*left*/
		-0.5f, -0.5f, -0.5f, 	-1.0f,  0.0f,  0.0f,	
		-0.5f, -0.5f,  0.5f, 	-1.0f,  0.0f,  0.0f,	
		-0.5f,  0.5f,  0.5f, 	-1.0f,  0.0f,  0.0f,	
		-0.5f,  0.5f, -0.5f, 	-1.0f,  0.0f,  0.0f,	

		/*right*/
		 0.5f, -0.5f,  0.5f, 	 1.0f,  0.0f,  0.0f,	
		 0.5f, -0.5f, -0.5f, 	 1.0f,  0.0f,  0.0f,	
		 0.5f,  0.5f, -0.5f, 	 1.0f,  0.0f,  0.0f,	
		 0.5f,  0.5f,  0.5f, 	 1.0f,  0.0f,  0.0f,	

		 /*top*/
		-0.5f,  0.5f,  0.5f, 	 0.0f,  1.0f,  0.0f,	
		 0.5f,  0.5f,  0.5f, 	 0.0f,  1.0f,  0.0f,	
		 0.5f,  0.5f, -0.5f, 	 0.0f,  1.0f,  0.0f,	
		-0.5f,  0.5f, -0.5f, 	 0.0f,  1.0f,  0.0f,	

		/*bottom*/
		-0.5f, -0.5f, -0.5f, 	 0.0f, -1.0f,  0.0f,	
		 0.5f, -0.5f, -0.5f, 	 0.0f, -1.0f,  0.0f,	
		 0.5f, -0.5f,  0.5f, 	 0.0f, -1.0f,  0.0f,	
		-0.5f, -0.5f,  0.5f, 	 0.0f, -1.0f,  0.0f,	


	};

	/* rectangle*/
	unsigned int indices[] =
	{
		/*front*/
		0, 1, 2,
		2, 3, 0,

		/*back*/
		4, 5, 6,
		6, 7, 4,

		/*left*/
		8, 9, 10,
		10, 11, 8,

		/*right*/
		12, 13, 14,
		14, 15, 12,

		/*top*/
		16, 17, 18,
		18, 19, 16,

		/*bottom*/
		20, 21, 22,
		22, 23, 20,

	};


	Shader lightingShader;
	Shader lightCubeShader;

	unsigned int VBO;
	unsigned int cubeVAO;
	unsigned int lightCubeVAO;
	unsigned int EBO;
	float TimeValue = 0;





	/*
	int tex_width, tex_height, nrChannels;
	unsigned char *data = stbi_load(SCENE_PATH "textures/crate_px.png", &tex_width, &tex_height, &nrChannels, 0);
	*/

	vec3 lightPos = {1.2f, 1.0f, 2.0f};
	vec3 lightColor = {1.0f, 1.0f, 1.0f};

	mat4 model;

	Camera basic_cam;
	EnvAttribs env_attribs;
	AppContex ctx;


	env_attribs.window_height = 1080;
	env_attribs.window_width = 1420;
	ctx.camera = &basic_cam;
	ctx.env = &env_attribs;


	SetupBasicCamera(&basic_cam, env_attribs.window_width, env_attribs.window_height, 0.1f, 2.5f);



	glm_mat4_identity(model);
	glm_mat4_identity(basic_cam.view);

	glm_rotate(model, glm_rad(-55.0f), (vec3){1.0f, 0.0f, 0.0f});

	glm_translate(basic_cam.view, (vec3){0.0f, 0.0f, -3.0f});


	glm_perspective(glm_rad(45.0f), (float)env_attribs.window_width/env_attribs.window_height, 0.1f, 100.0f, basic_cam.projection);




	if (!glfwInit())
	{
		return -1;
	}


	window = glfwCreateWindow(500, 500, "hello", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return 1;
	}

	stbi_set_flip_vertically_on_load(true);

	/*contexto de opengl usando glfw*/
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetWindowUserPointer(window, &ctx);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
	glViewport(0, 0, 500, 500);

	if (glewInit() != GLEW_OK)
	{
		printf("Error on initialization\n");
		return -1;
	}

	ShaderConstructor(&lightingShader, SCENE_PATH "shaders/normal_light.vert", SCENE_PATH "shaders/color_shader.frag");
	ShaderConstructor(&lightCubeShader, SCENE_PATH "shaders/shader.vert", SCENE_PATH "shaders/light_shader.frag");




	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 *sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 *sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);





	/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */ /*wireframe*/
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	glEnable(GL_DEPTH_TEST);


	while(!glfwWindowShouldClose(window))
	{
		processInput(window, &basic_cam);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		UpdateViewMatrix(&basic_cam);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		TimeValue = glfwGetTime();

		basic_cam.currentFrame = glfwGetTime();
		basic_cam.deltaTime = basic_cam.currentFrame - basic_cam.lastFrame;
		basic_cam.lastFrame = basic_cam.currentFrame;

		useShader(&lightingShader);
		lightColor[0] = sin(TimeValue * 2.0f);
		lightColor[1] = sin(TimeValue * 0.7f);
		lightColor[2] = sin(TimeValue * 1.3f);
		ShaderSetVec3(&lightingShader, "lightPos", lightPos);
		ShaderSetVec3(&lightingShader, "viewPos", basic_cam.Position);
		ShaderSetVec3(&lightingShader, "material.ambient", (vec3){0.50f, 0.862f, 0.15f});
		ShaderSetVec3(&lightingShader, "material.diffuse", (vec3){0.50f, 0.862f, 0.15f});
		ShaderSetVec3(&lightingShader, "material.specular", (vec3){0.2f, 0.2f, 0.2f});
		ShaderSetFloat(&lightingShader, "material.shininess", 32.0f);

		ShaderSetVec3(&lightingShader, "light.ambient", (vec3){lightColor[0] * 0.5, lightColor[1] * 0.5, lightColor[2] * 0.5});
		ShaderSetVec3(&lightingShader, "light.diffuse", (vec3){lightColor[0] * 0.2, lightColor[1] * 0.2, lightColor[2] * 0.2});
		ShaderSetVec3(&lightingShader, "light.specular", (vec3){1.0f, 1.0f, 1.0f});
		lightPos[0] = sin(TimeValue) * 3;
		lightPos[1] = cos(TimeValue * 2) * 5 ;
		lightPos[2] = cos(TimeValue) * 5;

		glm_perspective(glm_rad(basic_cam.fov), (float)env_attribs.window_width/env_attribs.window_height, 0.1f, 100.0f, basic_cam.projection);
		ShaderSetMat4(&lightingShader, "projection", basic_cam.projection);
		ShaderSetMat4(&lightingShader, "view", basic_cam.view);


		glm_mat4_identity(model);
		ShaderSetMat4(&lightingShader, "model", model);

		glBindVertexArray(cubeVAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		/*draw lamp*/
		useShader(&lightCubeShader);
		ShaderSetMat4(&lightCubeShader, "projection", basic_cam.projection);
		ShaderSetMat4(&lightCubeShader, "view", basic_cam.view);
		ShaderSetVec3(&lightCubeShader, "lightColor", lightColor);
		glm_mat4_identity(model);
		glm_translate(model, lightPos);
		glm_scale(model, (vec3){0.2f, 0.2f, 0.2f});
		ShaderSetMat4(&lightCubeShader, "model", model);

		glBindVertexArray(lightCubeVAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);







		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	AppContex *ctx = glfwGetWindowUserPointer(window);
	(void) window;
	ctx->env->window_height = height;
	ctx->env->window_width = width;
	printf("window width: %d\n", width);
	printf("window height: %d\n", height);
	glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow *window, Camera *camera)
{

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		ProcessKeyboard(camera, CAM_FORWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		ProcessKeyboard(camera, CAM_BACKWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		ProcessKeyboard(camera, CAM_LEFT);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		ProcessKeyboard(camera, CAM_RIGHT);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		ProcessKeyboard(camera, CAM_UP);
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		ProcessKeyboard(camera, CAM_DOWN);
	}
}

static void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	AppContex *ctx = glfwGetWindowUserPointer(window);
	float xoffset = xpos - ctx->camera->mouseLastX;
	float yoffset = ctx->camera->mouseLastY - ypos;
	if (ctx->camera->firstMouse)
	{
		ctx->camera->mouseLastX = xpos;
		ctx->camera->mouseLastY = ypos;
		ctx->camera->firstMouse = false;
		return;
	}

	ctx->camera->mouseLastX = xpos;
	ctx->camera->mouseLastY = ypos;

	ProcessMouseMovement(ctx->camera, xoffset, yoffset, true);

}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	AppContex *ctx = glfwGetWindowUserPointer(window);
	(void)xoffset;
	ProcessMouseScroll(ctx->camera, yoffset);
	printf("camera fov: %f\n", ctx->camera->fov);

}
