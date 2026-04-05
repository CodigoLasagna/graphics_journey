#version 330 core
out vec4 FragColor;
in vec3 ourColor;
uniform vec3 transColor;
void main()
{
	FragColor = vec4(transColor, 1.0);
}
