#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE

#define SCENE_PATH "scenes/lights_01/"

#include "lights_01.h"

#include <cglm/cglm.h>


#include <stb/stb_image.h>


#include <math.h>


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../source/ShaderManager.h"

typedef struct Camera
{
	vec3 cameraPos;
	vec3 cameraFront;
	vec3 cameraUp;
	vec3 cameraDir;
	mat4 projection;
	mat4 view;
	float deltaTime;
	float lastFrame;
	float currentFrame;

	float mouseLastX;
	float mouseLastY;
	float yaw;
	float pitch;
	float fov;
	bool firstMouse;
}Camera;

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
int light_scene(void)
{
	GLFWwindow* window;
	float cube_vertices[] =
	{
		/* positions         	colors           	texture coords*/
		/*front */
		-0.5f, -0.5f,  0.5f, 	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 	0.0f, 0.0f, 1.0f,	0.0f, 1.0f,

		/*back */
		 0.5f, -0.5f, -0.5f, 	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 	0.0f, 0.0f, 1.0f,	0.0f, 1.0f,

		/*left*/
		-0.5f, -0.5f, -0.5f, 	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 	0.0f, 0.0f, 1.0f,	0.0f, 1.0f,

		/*right*/
		 0.5f, -0.5f,  0.5f, 	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 	0.0f, 0.0f, 1.0f,	0.0f, 1.0f,

		 /*top*/
		-0.5f,  0.5f,  0.5f, 	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 	0.0f, 0.0f, 1.0f,	0.0f, 1.0f,

		/*bottom*/
		-0.5f, -0.5f, -0.5f, 	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 	0.0f, 0.0f, 1.0f,	0.0f, 1.0f,


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
	unsigned int VAO;
	unsigned int cubeVAO;
	unsigned int lightCubeVAO;
	unsigned int EBO;

	float timeValue = 0;
	float greenValue = 0;
	float blueValue = 0;
	float angle = 0;
	int vertexColorLocation = 0;

	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };


	int tex_width, tex_height, nrChannels;


	unsigned char *data = stbi_load(SCENE_PATH "textures/crate_px.png", &tex_width, &tex_height, &nrChannels, 0);
	unsigned int texture1;
	unsigned int texture2;

	vec3 lightPos = {1.2f, 1.0f, 2.0f};


	mat4 trans;
	vec3 rot = {0.0f, 0.0f, 1.0f};
	vec3 scale = {0.5f, 0.5f, 0.5f};
	vec3 center;

	mat4 model;

	Camera basic_cam;
	EnvAttribs env_attribs;
	AppContex ctx;

	int modelLoc;
	int viewLoc;
	int projLoc;

	env_attribs.window_height = 1080;
	env_attribs.window_width = 1420;
	ctx.camera = &basic_cam;
	ctx.env = &env_attribs;


	glm_vec3_copy((vec3){0.0f, 0.0f, 3.0f}, basic_cam.cameraPos);
	glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, basic_cam.cameraFront);
	glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, basic_cam.cameraUp);
	basic_cam.deltaTime = 0.0f;
	basic_cam.lastFrame = 0.0f;
	basic_cam.mouseLastX = (int) (env_attribs.window_width / 2);
	basic_cam.mouseLastY = (int) (env_attribs.window_height / 2);
	basic_cam.yaw = -90;
	basic_cam.pitch = 0;
	basic_cam.firstMouse = true;
	basic_cam.fov = 90;


	glm_mat4_identity(trans);
	glm_rotate(trans, glm_rad(90.f), rot);
	glm_scale(trans, scale);

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

	ShaderConstructor(&lightingShader, SCENE_PATH "shaders/shader.vert", SCENE_PATH "shaders/color_shader.frag");
	ShaderConstructor(&lightCubeShader, SCENE_PATH "shaders/shader.vert", SCENE_PATH "shaders/light_shader.frag");



	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/*coord attrib array*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/*color attrib array*/
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	/*texture attrib array*/
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 *sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 *sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


	/*textures*/
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load texture\n");
	}
	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load(SCENE_PATH "textures/awesomeface.png", &tex_width, &tex_height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load texture\n");
	}
	stbi_image_free(data);




	/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */ /*wireframe*/
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/*transform the drawing*/

	/*bind texture before draw glDrawElements*/

	modelLoc = glGetUniformLocation(lightingShader.ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);
	viewLoc = glGetUniformLocation(lightingShader.ID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)basic_cam.view);
	projLoc = glGetUniformLocation(lightingShader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float*)basic_cam.projection);

	glEnable(GL_DEPTH_TEST);


	while(!glfwWindowShouldClose(window))
	{
		timeValue = glfwGetTime();
		blueValue = (sin(timeValue) / 0.5f) - 0.5;
		greenValue = (sin(timeValue) / 2.0f) + 0.7f;
		vertexColorLocation = glGetUniformLocation(lightingShader.ID, "transColor");
		glUseProgram(lightingShader.ID);
		glUniform3f(vertexColorLocation, 0.0f, greenValue, blueValue);
		processInput(window, &basic_cam);
		/*
		glClearColor(0.47f, 0.65f, 0.28f, 1.0f);
		*/
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm_perspective(glm_rad(basic_cam.fov), (float)env_attribs.window_width/env_attribs.window_height, 0.1f, 100.0f, basic_cam.projection);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float*)basic_cam.projection);
		basic_cam.currentFrame = glfwGetTime();
		basic_cam.deltaTime = basic_cam.currentFrame - basic_cam.lastFrame;
		basic_cam.lastFrame = basic_cam.currentFrame;

		useShader(&lightingShader);
		ShaderSetVec3(&lightingShader, "objectColor", (vec3){1.0f, 0.5f, 0.31f});
		ShaderSetVec3(&lightingShader, "lightColor", (vec3){1.0f, 1.0f, 0.31f});

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
		glm_mat4_identity(model);
		glm_translate(model, lightPos);
		glm_scale(model, (vec3){0.2f, 0.2f, 0.2f});
		ShaderSetMat4(&lightCubeShader, "model", model);

		glBindVertexArray(lightCubeVAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);





		glm_vec3_add(basic_cam.cameraPos, basic_cam.cameraFront, center);
		glm_lookat(basic_cam.cameraPos, center, basic_cam.cameraUp, basic_cam.view);


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
	const float cameraSpeed = 2.5f * camera->deltaTime;
	vec3 temp;

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		glm_vec3_muladds(camera->cameraFront, cameraSpeed, camera->cameraPos);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		glm_vec3_mulsubs(camera->cameraFront, cameraSpeed, camera->cameraPos);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		glm_cross(camera->cameraFront, camera->cameraUp, temp);
		glm_vec3_normalize(temp);
		glm_vec3_mulsubs(temp, cameraSpeed, camera->cameraPos);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		glm_cross(camera->cameraFront, camera->cameraUp, temp);
		glm_vec3_normalize(temp);
		glm_vec3_muladds(temp, cameraSpeed, camera->cameraPos);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		glm_vec3_muladds(camera->cameraUp, cameraSpeed, camera->cameraPos);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		glm_vec3_mulsubs(camera->cameraUp, cameraSpeed, camera->cameraPos);
	}
}

static void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	AppContex *ctx = glfwGetWindowUserPointer(window);
	float xoffset = xpos - ctx->camera->mouseLastX;
	float yoffset = ctx->camera->mouseLastY - ypos;
	const float sensitivity = 0.1f;
	vec3 direction;
	if (ctx->camera->firstMouse)
	{
		ctx->camera->mouseLastX = xpos;
		ctx->camera->mouseLastY = ypos;
		ctx->camera->firstMouse = false;
		return;
	}

	ctx->camera->mouseLastX = xpos;
	ctx->camera->mouseLastY = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	ctx->camera->yaw += xoffset;
	ctx->camera->pitch += yoffset;

	if (ctx->camera->pitch > 89.0f)
	{
		ctx->camera->pitch = 89.0f;
	}
	if (ctx->camera->pitch < -89.0f)
	{
		ctx->camera->pitch = -89.0f;
	}

	direction[0] = cos(glm_rad(ctx->camera->yaw)) * cos(glm_rad(ctx->camera->pitch));
	direction[1] = sin(glm_rad(ctx->camera->pitch));
	direction[2] = sin(glm_rad(ctx->camera->yaw)) * cos(glm_rad(ctx->camera->pitch));
	glm_normalize_to(direction, ctx->camera->cameraFront);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	AppContex *ctx = glfwGetWindowUserPointer(window);
	(void)xoffset;
	ctx->camera->fov -= (float)yoffset;
	if (ctx->camera->fov < 1.0f)
		ctx->camera->fov = 1.0f;
	if (ctx->camera->fov > 120.0f)
		ctx->camera->fov = 120.0f; 
	printf("camera fov: %f\n", ctx->camera->fov);

}
