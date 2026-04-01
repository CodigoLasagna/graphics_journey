#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE


#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


int main(int argc, char* argv[])
{
	GLFWwindow* window;
	float vertices[] =
	{
		-0.5f, 0.0f,
		0.5f, 0.0f
	};
	GLuint VBO, VAO;

	/*shaders minimos*/
	const char* vertexShadersSrc =
		"#version 330 core\n"
		"layout";

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

	glfwMakeContextCurrent(window);

	/*context for drawing line*/
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*atributos de posición*/
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
