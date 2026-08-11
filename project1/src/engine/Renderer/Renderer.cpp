#define ENGINE_DLL
#include "../engine.h"
#include "../manager.h"
#include "Renderer.h"

#include <glad/glad.h>

#include <stb_image.h>

#include <iostream>
#include <string>
#include <random>

void BindRenderFrameBuffer(Renderer* renderer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, renderer->rFBO);

	glBindTexture(GL_TEXTURE_2D, renderer->rTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, renderer->window->width, renderer->window->height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderer->rTex, 0);

	glBindTexture(GL_TEXTURE_2D, renderer->brightTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, renderer->window->width, renderer->window->height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, renderer->brightTex, 0);

	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, attachments);

	glBindRenderbuffer(GL_RENDERBUFFER, renderer->rRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, renderer->window->width, renderer->window->height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderer->rRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BindGFrameBuffer(Renderer* renderer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, renderer->gFBO);

	glBindTexture(GL_TEXTURE_2D, renderer->gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, renderer->window->width, renderer->window->height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderer->gNormal, 0);

	glBindTexture(GL_TEXTURE_2D, renderer->gColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, renderer->window->width, renderer->window->height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, renderer->gColor, 0);

	glBindTexture(GL_TEXTURE_2D, renderer->gMisc);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, renderer->window->width, renderer->window->height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, renderer->gMisc, 0);

	glBindTexture(GL_TEXTURE_2D, renderer->gBright);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, renderer->window->width, renderer->window->height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, renderer->gBright, 0);

	GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
	glDrawBuffers(4, attachments);

	glBindRenderbuffer(GL_RENDERBUFFER, renderer->gRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, renderer->window->width, renderer->window->height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderer->gRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BindPPFrameBuffer(Renderer* renderer)
{
	for (size_t i = 0;i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, renderer->ppFBO[i]);
            glBindTexture(GL_TEXTURE_2D, renderer->ppTex[i]);
        	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, renderer->window->width, renderer->window->height, 0, GL_RGBA, GL_FLOAT, NULL);
        	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	        glBindTexture(GL_TEXTURE_2D, 0);

	        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderer->ppTex[i], 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void BindSSFrameBuffer(Renderer* renderer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, renderer->ssFBO);

    glBindTexture(GL_TEXTURE_2D, renderer->ssTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, renderer->window->width, renderer->window->height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderer->ssTex, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Handle M_R_Create(Manager* manager, Window* window)
{
	stbi_set_flip_vertically_on_load(true);

	Renderer* renderer = new Renderer();

	renderer->window = window;
	renderer->glContext = SDL_GL_CreateContext(renderer->window->window);
	SDL_GL_MakeCurrent(renderer->window->window, renderer->glContext);

	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

	// Camera
	renderer->camera = new Camera();

	renderer->camera->aspect = (float)renderer->window->width / (float)renderer->window->height;
	renderer->camera->projection = Matrix(1.0f);
	renderer->camera->view = Matrix(1.0f);

	// Quad
	float vertices[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f,  0.0f, 0.0f
	};

	unsigned int indices[] = {
		0, 3, 2,
        0, 2, 1
	};

	glGenVertexArrays(1, &renderer->VAO);
	glGenBuffers(1, &renderer->VBO);
	glGenBuffers(1, &renderer->EBO);

	glBindVertexArray(renderer->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	//Render Frame
	glGenFramebuffers(1, &renderer->rFBO);
	glGenRenderbuffers(1, &renderer->rRBO);
	glGenTextures(1, &renderer->rTex);

	glViewport(0, 0, renderer->window->width, renderer->window->height);

	std::cout << "rShader" << std::endl;
	renderer->rShader = new Shader(quad::vertex, render::fragment);
	renderer->rShader->Use();

	glUniform1i(renderer->rShader->GetLoc("rTex"), 0);
	glUniform1i(renderer->rShader->GetLoc("r_blurTex"), 1);
	glUniform1i(renderer->rShader->GetLoc("gColor"), 2);
	glUniform1i(renderer->rShader->GetLoc("ssTex"), 3);

	glUniform1i(renderer->rShader->GetLoc("isLight"), false);

	//Ping Pong Frame
	glGenFramebuffers(2, &renderer->ppFBO[0]);
	glGenTextures(2, &renderer->ppTex[0]);
	glGenTextures(1, &renderer->brightTex);

	renderer->ppShader = new Shader(quad::vertex, pingpong::fragment);
	renderer->ppShader->Use();

	glUniform1i(renderer->ppShader->GetLoc("image"), 0);

	//Light Frame
	glGenFramebuffers(1, &renderer->gFBO);
	glGenRenderbuffers(1, &renderer->gRBO);

	glGenTextures(1, &renderer->gNormal);
	glGenTextures(1, &renderer->gColor);
	glGenTextures(1, &renderer->gMisc);
	glGenTextures(1, &renderer->gBright);

	std::cout << "gShader" << std::endl;
	renderer->gShader = new Shader(quad::vertex, g::fragment);
	renderer->gShader->Use();

	glUniform1i(renderer->gShader->GetLoc("gNormal"), 0);
	glUniform1i(renderer->gShader->GetLoc("gColor"), 1);
	glUniform1i(renderer->gShader->GetLoc("gMisc"), 2);
	glUniform1i(renderer->gShader->GetLoc("gBright"), 3);

	glUniform1i(renderer->gShader->GetLoc("skybox"), 4);

	glUniform1f(renderer->gShader->GetLoc("PI"), Math::pi);

	glGenBuffers(1, &renderer->lightSSBO);

	renderer->lightCount = 0;

	renderer->isSkyBox = false;

	renderer->isLightChance = true;

	/*Kernel and Noise*/
    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
	std::default_random_engine generator;

	int kernelNum = 16;
	for (int i = 0; i < kernelNum; i++)
	{
		Vector sampler = Vector(
			randomFloats(generator) * 2.0 - 1.0, 
            randomFloats(generator) * 2.0 - 1.0, 
            randomFloats(generator)
		);

		sampler = sampler.Normalize();
		sampler *= randomFloats(generator);

		float scale = (float)i / (float)kernelNum;
		scale = Lerp(0.1f, 1.0f, scale * scale);
		sampler *= scale;

		renderer->samplers.push_back(sampler);
	}

	for (size_t i = 0; i < 16; i++)
	{
		Vector noise = Vector(
			randomFloats(generator) * 2.0 - 1.0, 
            randomFloats(generator) * 2.0 - 1.0, 
            0.0f
		);

		renderer->noises.push_back(noise);
	}

	glGenTextures(1, &renderer->noiseTex);
	glBindTexture(GL_TEXTURE_2D, renderer->noiseTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &renderer->noises[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);;
	glBindTexture(GL_TEXTURE_2D, 0);

	/*Screen Space*/

	glGenFramebuffers(1, &renderer->ssFBO);
	glGenTextures(1, &renderer->ssTex);

	BindSSFrameBuffer(renderer);

	renderer->ssShader = new Shader(quad::vertex, ss::fragment);
	renderer->ssShader->Use();

	glUniform1i(renderer->ssShader->GetLoc("gNormal"), 0);
	glUniform1i(renderer->ssShader->GetLoc("gColor"), 1);
	glUniform1i(renderer->ssShader->GetLoc("gMisc"), 2);

	Record* newRecords = (Record*)realloc(manager->records, (manager->size + 1) * sizeof(Record));
	if (newRecords == NULL) return -1;

	manager->records = newRecords;
	int myId = manager->nextId++;

	manager->records[manager->size].id = myId;
	manager->records[manager->size].type = RENDERER; // Type !!!!!!!!
	manager->records[manager->size].ptr = renderer;

	manager->size += 1;
	return myId;
}

void R_Present(Renderer* renderer)
{
	renderer->camera->aspect = (float)renderer->window->width / (float)renderer->window->height;

	if (renderer->isLightChance)
	{

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, renderer->lightSSBO);
		glBufferData(GL_SHADER_STORAGE_BUFFER, renderer->lights.size() * sizeof(Light), renderer->lights.data(), GL_DYNAMIC_DRAW);

   		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, renderer->lightSSBO);
 	  	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, renderer->rFBO);

	glClear(GL_COLOR_BUFFER_BIT);

	renderer->gShader->Use();

    glUniform1ui(renderer->gShader->GetLoc("lightCount"), renderer->lightCount);

	glUniformMatrix4fv(renderer->gShader->GetLoc("projection"), 1, GL_TRUE, &renderer->camera->projection.data[0][0]);
	glUniformMatrix4fv(renderer->gShader->GetLoc("view"), 1, GL_TRUE, &renderer->camera->view.data[0][0]);

	glUniform1f(renderer->gShader->GetLoc("skyboxMaxMipLevel"), renderer->skyboxMaxMipLevel);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, renderer->lightSSBO);

	glBindVertexArray(renderer->VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderer->gNormal);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, renderer->gColor);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, renderer->gMisc);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, renderer->gBright);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_CUBE_MAP, renderer->skyTex);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, renderer->gFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, renderer->rFBO);
	glBlitFramebuffer(0, 0, renderer->window->width, renderer->window->height, 0, 0, renderer->window->width, renderer->window->height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, renderer->rFBO);

	if (renderer->isSkyBox)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL); 
   		renderer->skyShader->Use();
   		glUniformMatrix4fv(renderer->skyShader->GetLoc("projection"), 1, GL_TRUE, &renderer->camera->projection.data[0][0]);
		glUniformMatrix4fv(renderer->skyShader->GetLoc("view"), 1, GL_TRUE, &renderer->camera->view.data[0][0]);
    
    	glActiveTexture(GL_TEXTURE0);
    	glBindTexture(GL_TEXTURE_CUBE_MAP, renderer->skyTex);

    	glBindVertexArray(renderer->skyVAO);
        	glDrawArrays(GL_TRIANGLES, 0, 36);
    	glBindVertexArray(0);
    	glDepthFunc(GL_LESS);
		glDisable(GL_DEPTH_TEST);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLboolean horizontal = true, first_iteration = true;
    GLuint amount = 10;
    renderer->ppShader->Use();

	for (GLuint i = 0; i < amount; i++)
	{
    	glBindFramebuffer(GL_FRAMEBUFFER, renderer->ppFBO[horizontal]); 
    	glUniform1i(renderer->ppShader->GetLoc("horizontal"), horizontal);
		glActiveTexture(GL_TEXTURE0);
    	glBindTexture(
        	GL_TEXTURE_2D, first_iteration ? renderer->brightTex : renderer->ppTex[!horizontal]
    	); 
   	 	
        glBindVertexArray(renderer->VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	    glBindVertexArray(0);
 
    	horizontal = !horizontal;
    	if (first_iteration)
        	first_iteration = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, renderer->ssFBO);

    renderer->ssShader->Use();

	glUniformMatrix4fv(renderer->ssShader->GetLoc("projection"), 1, GL_TRUE, &renderer->camera->projection.data[0][0]);

	glBindVertexArray(renderer->VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderer->gNormal);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, renderer->gColor);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, renderer->gMisc);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT);

	renderer->rShader->Use();
	glBindVertexArray(renderer->VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderer->rTex);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, renderer->ppTex[!horizontal]);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, renderer->gColor);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, renderer->ssTex);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	SDL_GL_SwapWindow(renderer->window->window);

	renderer->isLightChance = false;
}

void R_Clear(Renderer* renderer, float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void R_SetViewport(Renderer* renderer, int x, int y, int w, int h)
{
	glViewport(x, y, w, h);
	BindRenderFrameBuffer(renderer);
	BindGFrameBuffer(renderer);
	BindPPFrameBuffer(renderer);
	BindSSFrameBuffer(renderer);
}

void R_Begin(Renderer* renderer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, renderer->gFBO);
	glEnable(GL_DEPTH_TEST);
}

void R_End(Renderer* renderer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
}

void R_IsLight(Renderer* renderer, int isLight)
{
	renderer->rShader->Use();
	glUniform1i(renderer->rShader->GetLoc("isLight"), (bool)isLight);
}

void R_SetGamma(Renderer* renderer, float gamma)
{
	renderer->rShader->Use();
	glUniform1f(renderer->rShader->GetLoc("gamma"), gamma);
}