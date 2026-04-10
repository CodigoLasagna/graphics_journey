#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform vec3 transColor;
uniform sampler2D ourTexture;

void main()
{
	//FragColor = vec4(transColor, 1.0);
	FragColor = texture(ourTexture, TexCoord);
}
