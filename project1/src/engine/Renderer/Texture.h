#ifndef TEXTURE_H
#define TEXTURE_H

#include "../TextureType.h"

typedef struct Texture
{
	TextureType type;
	unsigned int id;
	int width, height, nrChannels;
	unsigned char* data;
} Texture;

#endif