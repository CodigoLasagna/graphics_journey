#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE

#define STB_IMAGE_IMPLEMENTATION

#include <stb/stb_image.h>



#include <math.h>


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "source/ShaderManager.h"


void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);


int main(int argc, char* argv[])
{
	GLFWwindow* window;
	/* rectangle
	float vertices[] =
	{
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
	};
	*/
	float vertices[] =
	{
		/* positions        	colors           	texture coords*/
		 0.5f, -0.5f, 0.0f, 	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 	0.0f, 0.0f, 1.0f,	0.5f, 1.0f,
	};

	float texCoords[] =
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f,
	};


	/* rectangle
	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};
	*/

	/*triangle*/
	unsigned int indices[] =
	{
		0, 1, 2,
		0, 2,
	};

	Shader shader_program;
	int success;
	char infoLog[512];

	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;

	float timeValue = 0;
	float greenValue = 0;
	float blueValue = 0;
	int vertexColorLocation = 0;

	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };

	int width, height, nrChannels;
	unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
	unsigned int texture1;
	unsigned int texture2;

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
	glViewport(0, 0, 500, 500);

	if (glewInit() != GLEW_OK)
	{
		printf("Error on initialization\n");
		return -1;
	}

	ShaderConstructor(&shader_program, "shaders/shader.vert", "shaders/shader.frag");



	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/*coord attrib array*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/*color attrib array*/
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	/*texture attrib array*/
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	/*
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	*/

	/*textures*/
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
	data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load texture\n");
	}
	stbi_image_free(data);




	/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */ /*wireframe*/
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	useShader(&shader_program);
	glUniform1i(glGetUniformLocation(shader_program.ID, "texture1"), 0);
	ShaderSetInt(&shader_program ,"texture2", 1);

	/*bind texture before draw glDrawElements*/


	while(!glfwWindowShouldClose(window))
	{
		timeValue = glfwGetTime();
		blueValue = (sin(timeValue) / 0.5f) - 0.5;
		greenValue = (sin(timeValue) / 2.0f) + 0.7f;
		vertexColorLocation = glGetUniformLocation(shader_program.ID, "transColor");
		glUseProgram(shader_program.ID);
		glUniform3f(vertexColorLocation, 0.0f, greenValue, blueValue);
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/*glDrawArrays(GL_TRIANGLES, 0, 3);*/
		/*glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); rectangle*/ 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 5, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}
