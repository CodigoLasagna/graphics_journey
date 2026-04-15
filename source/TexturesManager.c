#include "TexturesManager.h"

unsigned int load_texture(char *path)
{
	unsigned int texture;
	int tex_width, tex_height, nrChannels;
	unsigned char* data = 0;
	GLenum format = 0;
	GLenum internalFormat = 0;
	stbi_set_flip_vertically_on_load(1);
	data = stbi_load(path, &tex_width, &tex_height, &nrChannels, 0);
	switch(nrChannels)
	{
		case 1: format = GL_RED;  internalFormat = GL_R8;   break;
		case 3: format = GL_RGB;  internalFormat = GL_RGB8; break;
		case 4: format = GL_RGBA; internalFormat = GL_RGBA; break;
	}
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, tex_width, tex_height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load texture\n");
	}
	stbi_image_free(data);
	return texture;
}
