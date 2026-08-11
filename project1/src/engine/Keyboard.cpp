#define ENGINE_DLL
#include "engine.h"
#include "manager.h"
#include "Keyboard.h"

#include <stdlib.h>

Handle M_K_Create(Manager* manager)
{
	Key* key = new Key();
	key->keyborads = SDL_GetKeyboardState(NULL);

	Record* newRecords = (Record*)realloc(manager->records, (manager->size + 1) * sizeof(Record));
	if (newRecords == NULL) return -1;

	manager->records = newRecords;
	int myId = manager->nextId++;

	manager->records[manager->size].id = myId;
	manager->records[manager->size].type = KEY; // Type !!!!!!!!
	manager->records[manager->size].ptr = key;

	manager->size += 1;
	return myId;
}

int K_Down(Key* key, int num)
{
	return key->keyborads[num];
}

void K_Event(Key* key)
{
	SDL_PumpEvents();
	key->keyborads = SDL_GetKeyboardState(&key->keyNum);
}

void K_Update(Key* key)
{
	memcpy(key->lastKey, key->keyborads, key->keyNum * sizeof(bool));
}

int K_Up(Key* key, int num)
{
	return (key->lastKey[num] && !key->keyborads[num]);
}

int K_Press(Key* key, int num)
{
	return (!key->lastKey[num] && key->keyborads[num]);
}