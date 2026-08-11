#ifndef MANAGER_H
#define MANAGER_H

typedef enum Type
{
	WINDOW,
	KEY,
	RENDERER,
	MESH,
	TEXTURE
} Type;

typedef struct Record {
	unsigned int id;
	Type type;
	void* ptr;
} Record;

typedef struct Manager
{
	unsigned int size;      
	Record* records;
	unsigned int nextId;
} Manager;

#endif