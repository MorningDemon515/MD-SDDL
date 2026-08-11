#define ENGINE_DLL
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "../engine.h"
#include "../manager.h"
#include "Texture.h"
#include "Mesh.h"

#include <glad/glad.h>

#include <iostream>

Handle T_Create(Manager* manager, int width, int height, int nrChannels, unsigned char* data, TextureType type)
{
	Texture* texture = new Texture();
	glGenTextures(1, &texture->id);
    texture->data = data;
    texture->width = width;
    texture->height = height;
    texture->nrChannels = nrChannels;
    texture->type = type;

    GLenum format;
    switch (texture->nrChannels)
    {
        case 1:
            format = GL_RED;
            break;

        case 3:
            format = GL_RGB;
            break;

        case 4:
            format = GL_RGBA;
            break;

        default:
            format = GL_RGB;
            break;
    }

    glBindTexture(GL_TEXTURE_2D, texture->id); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, format, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, texture->data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    int myId = manager->nextId++;

    Record* newRecords = (Record*)realloc(manager->records, (manager->size + 1) * sizeof(Record));
    if (newRecords == NULL) return -1;

    manager->records = newRecords;

    manager->records[manager->size].id = myId;
    manager->records[manager->size].type = TEXTURE; // Type !!!!!!!!
    manager->records[manager->size].ptr = texture;

    manager->size += 1;
    return myId;

}

Handle T_CreateFromFile(Manager* manager, const char* file, TextureType type)
{
    int myId = manager->nextId++;

    Texture* texture = new Texture();
    glGenTextures(1, &texture->id);

    texture->data = stbi_load(file, &texture->width, &texture->height, &texture->nrChannels, 0);
    texture->type = type;

    GLenum format;
    switch (texture->nrChannels)
    {
    case 1:
        format = GL_RED;
        break;

    case 3:
        format = GL_RGB;
        break;

    case 4:
        format = GL_RGBA;
        break;

    default:
        format = GL_RGB;
        break;
    }

    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (texture->data)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(GL_TEXTURE_2D, 0, format, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, texture->data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to Load Texture! Handle: " << myId << std::endl;
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    Record* newRecords = (Record*)realloc(manager->records, (manager->size + 1) * sizeof(Record));
    if (newRecords == NULL) return -1;

    manager->records = newRecords;

    manager->records[manager->size].id = myId;
    manager->records[manager->size].type = TEXTURE; // Type !!!!!!!!
    manager->records[manager->size].ptr = texture;

    manager->size += 1;
    return myId;
}

void T_Enable(Texture* texture, Mesh* mesh, unsigned int level)
{
    mesh->shader->Use();
    switch (texture->type)
    {
    case ALBEDO:
        glUniform1i(mesh->shader->GetLoc("texture_albedo"), level);
        break;

    case NORMAL:
        glUniform1i(mesh->shader->GetLoc("texture_normal"), level);
        break;

    case METALLIC:
        glUniform1i(mesh->shader->GetLoc("texture_metallic"), level);
        break;

    case ROUGHNESS:
        glUniform1i(mesh->shader->GetLoc("texture_roughness"), level);
        break;
    }

    if (texture->nrChannels == 1)
    {
        glUniform1i(mesh->shader->GetLoc("isRed"), true);
    }

    glActiveTexture(GL_TEXTURE0 + level);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}