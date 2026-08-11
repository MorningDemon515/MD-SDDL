#define ENGINE_DLL
#include "engine.h"
#include "manager.h"
#include <string.h>

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>

#include "Window/Window.h"
#include "Renderer/Renderer.h"
#include "Keyboard.h"
#include "Renderer/Mesh.h"
#include "Renderer/Texture.h"

Manager* M_Create()
{
	Manager* manager = (Manager*)malloc(sizeof(Manager));

	if (manager != NULL)
	{
		manager->records = nullptr;
		manager->size = 0;
		manager->nextId = 0;
	}

	return manager;
}

void* M_Get(Manager* manager, Handle handle)
{
	for (int i = 0; i < manager->size; i++) {
		if (manager->records[i].id == handle) {
			return manager->records[i].ptr;
		}
	}
	return NULL;
}

void ReleaseData(Manager* manager, int index)
{
	void* itemAddr = manager->records[index].ptr;
	if (itemAddr != nullptr) {
		switch (manager->records[index].type)
		{
		case WINDOW:
			SDL_DestroyWindow(((Window*)itemAddr)->window);
			delete (Window*)itemAddr;
			printf("[Removal] Releasing Window, Handle: %d\n", manager->records[index].id);
			break;

		case KEY:
			delete (Key*)itemAddr;
			printf("[Removal] Releasing Key, Handle: %d\n", manager->records[index].id);
			break;

		case RENDERER:
			SDL_GL_DestroyContext(((Renderer*)itemAddr)->glContext);
			glDeleteBuffers(1, &((Renderer*)itemAddr)->VBO);
			glDeleteBuffers(1, &((Renderer*)itemAddr)->EBO);
			glDeleteVertexArrays(1, &((Renderer*)itemAddr)->VAO);

			glDeleteFramebuffers(1, &((Renderer*)itemAddr)->rFBO);
			glDeleteRenderbuffers(1, &((Renderer*)itemAddr)->rRBO);
			glDeleteTextures(1, &((Renderer*)itemAddr)->rTex);

			glDeleteFramebuffers(2, &((Renderer*)itemAddr)->ppFBO[0]);
			glDeleteTextures(2, &((Renderer*)itemAddr)->ppTex[0]);
			glDeleteTextures(1, &((Renderer*)itemAddr)->brightTex);

			glDeleteFramebuffers(1, &((Renderer*)itemAddr)->gFBO);
			glDeleteRenderbuffers(1, &((Renderer*)itemAddr)->gRBO);

			glDeleteTextures(1, &((Renderer*)itemAddr)->gNormal);
			glDeleteTextures(1, &((Renderer*)itemAddr)->gColor);
			glDeleteTextures(1, &((Renderer*)itemAddr)->gMisc);
			glDeleteTextures(1, &((Renderer*)itemAddr)->gBright);

			glDeleteBuffers(1, &((Renderer*)itemAddr)->lightSSBO);

			glDeleteBuffers(1, &((Renderer*)itemAddr)->skyVBO);
			glDeleteVertexArrays(1, &((Renderer*)itemAddr)->skyVAO);
			glDeleteTextures(1, &((Renderer*)itemAddr)->skyTex);

			glDeleteTextures(1, &((Renderer*)itemAddr)->noiseTex);

			glDeleteFramebuffers(1, &((Renderer*)itemAddr)->ssFBO);
			glDeleteTextures(1, &((Renderer*)itemAddr)->ssTex);

            delete ((Renderer*)itemAddr)->ssShader;

			delete ((Renderer*)itemAddr)->rShader;

            delete ((Renderer*)itemAddr)->ppShader;

            delete ((Renderer*)itemAddr)->skyShader;

			delete ((Renderer*)itemAddr)->gShader;

			delete ((Renderer*)itemAddr)->camera;

			delete (Renderer*)itemAddr;
			printf("[Removal] Releasing Rendere, Handle: %dr\n", manager->records[index].id);
			break;

		case MESH:
			delete (Mesh*)itemAddr;
			printf("[Removal] Releasing Mesh, Handle: %d\n", manager->records[index].id);
			break;

		case TEXTURE:
			glDeleteTextures(1, &((Texture*)itemAddr)->id);
			delete (Texture*)itemAddr;
		default:
			break;
		}
	}
}

void M_Remove(Manager* manager, Handle handle)
{
	int targetIndex = -1;

	for (int i = 0; i < manager->size; i++) {
		if (manager->records[i].id == handle) {
			targetIndex = i;
			break;
		}
	}

	if (targetIndex == -1) {
		printf("Handle not found!\n");
		return;
	}

	ReleaseData(manager, targetIndex);

	for (int i = targetIndex; i < manager->size - 1; i++) {
		manager->records[i] = manager->records[i + 1];
	}

	manager->size -= 1;

	if (manager->size > 0) {
		void* temp = realloc(manager->records, manager->size * sizeof(Record));
		if (temp != nullptr) {
			manager->records = (Record*)temp;
		}
	}
	else {
		free(manager->records);
		manager->records = nullptr;
	}
}

void M_Release(Manager* manager)
{
	if (manager == NULL) return;

	for (int i = 0; i < manager->size; i++) {

		ReleaseData(manager, i);
	}

	printf("Released");

	free(manager->records);
	free(manager);
}