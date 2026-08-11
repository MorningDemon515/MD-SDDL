#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL.h>

#include "Shader.h"
#include "Shaders.h"

#include "../Window/Window.h"
#include "Camera.h"

#include "Light.h"

typedef struct Renderer
{
	Window* window;

	SDL_GLContext glContext;

	//quad Object
	unsigned int VAO, VBO, EBO;

	//Render Frame
	unsigned int rFBO, rRBO;
	unsigned int rTex;
	Shader* rShader;

	//Light Frame
	unsigned int gFBO, gRBO;
	unsigned int gNormal, gColor, gMisc, gBright;
	// depth, metallic, roughness
	Shader* gShader;
	unsigned int lightSSBO;
	bool isLightChance;
	std::vector<Light> lights;
	unsigned int lightCount;

	//Ping Pong Frame
	unsigned int ppFBO[2];
	unsigned int ppTex[2]; 
	unsigned int brightTex;
	Shader* ppShader;

	//Kernel and Noise
	std::vector<Vector> samplers;
	std::vector<Vector> noises;
	unsigned int noiseTex;

	//Screen Space
	unsigned int ssFBO;
	unsigned int ssTex;
	Shader* ssShader;

	//SkyBox
	unsigned int skyVAO, skyVBO;
    unsigned int skyTex;
	Shader* skyShader;
	float skyboxMaxMipLevel;

	bool isSkyBox;

	//Camera
	Camera* camera;

} Renderer;

#endif