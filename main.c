#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE



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
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

	/* rectangle
	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};
	*/

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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);





	/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */ /*wireframe*/
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	while(!glfwWindowShouldClose(window))
	{
		timeValue = glfwGetTime();
		blueValue = (sin(timeValue) / 2.0f);
		greenValue = (sin(timeValue) / 2.0f) + 0.7f;
		vertexColorLocation = glGetUniformLocation(shader_program.ID, "transColor");
		glUseProgram(shader_program.ID);
		glUniform3f(vertexColorLocation, 0.0f, greenValue, blueValue);
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/*glDrawArrays(GL_TRIANGLES, 0, 3);*/
		/*glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); rectangle*/ 
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
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}
