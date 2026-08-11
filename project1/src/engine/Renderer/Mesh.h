#ifndef MESH_H
#define MESH_H

#include "Renderer.h"

typedef struct Vertex
{
	Vector position;
	Vector texCoords;
	Vector normal;
	Vector color;
	Vector tangent;
} Vertex;

class Mesh
{
private:
	unsigned int VAO, VBO, EBO;
	Renderer* renderer;

	Matrix model;

	Vector bright;
public:
	Shader* shader;

	Matrix translate;
	Matrix scale;
	Matrix rotate;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Vector bright,Renderer* renderer,int isTex);
	~Mesh();

	void Draw();
};

#endif