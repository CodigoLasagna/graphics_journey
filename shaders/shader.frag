#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform vec3 transColor;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	//FragColor = vec4(transColor, 1.0);
	//FragColor = texture(texture1, TexCoord);
	//FragColor = texture(texture1, TexCoord) * vec4(TexCoord.x, TexCoord.y, 0.5, 1.0);
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}
