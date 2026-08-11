#define ENGINE_DLL
#include "../engine.h"
#include "SkyBox.h"

#include "Renderer.h"

#include "../Math/Math.h"

#include "Shaders.h"

#include <glad/glad.h>

#include <stb_image.h>

#include <iostream>
#include <vector>
#include <string>

void R_Init_SkyBox(Renderer* renderer, const char* back,
                                        const char* bottom,
                                        const char* front,
                                        const char* left,
                                        const char* right,
                                        const char* top)
{
    stbi_set_flip_vertically_on_load(false);

    float skyboxVertices[] = {         
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &renderer->skyVAO);
    glGenBuffers(1, &renderer->skyVBO);

    glBindVertexArray(renderer->skyVAO);

      glBindBuffer(GL_ARRAY_BUFFER, renderer->skyVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(renderer->skyVAO);

    glGenTextures(1, &renderer->skyTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, renderer->skyTex);

    int width, height, nrChannels;
    unsigned char *data;

    std::vector<const char*> faces = {
        right,
        left,
        top,
        bottom,
        front,
        back
    };

    data = stbi_load(faces[0], &width, &height, &nrChannels, 0);
    if (data) {
        stbi_image_free(data);
    } else {
        width = 1; height = 1;
        std::cerr << "Failed to load first cubemap face for dimension check." << std::endl;
    }
    renderer->skyboxMaxMipLevel = floor(log2(std::max(width, height)));

    for (unsigned int i = 0; i < faces.size(); i++)
    {
        data = stbi_load(faces[i], &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    renderer->skyShader = new Shader(skybox::vertex, skybox::fragment);
    renderer->skyShader->Use();
    glUniform1i(renderer->skyShader->GetLoc("skybox"), 0);

    stbi_set_flip_vertically_on_load(true);
}

void R_Set_SkyBox(Renderer* renderer, int issb)
{
    renderer->isSkyBox = (bool)issb;
}

void R_Set_SkyBox_Light(Renderer* renderer, int issb)
{
    if (renderer->isSkyBox)
    {
        renderer->gShader->Use();
        glUniform1i(renderer->gShader->GetLoc("isSkyBox"), (bool)issb);
    }
}