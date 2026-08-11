#define ENGINE_DLL
#include "../engine.h"
#include "../manager.h"
#include "Mesh.h"

#include "Shaders.h"
#include <glad/glad.h>

#include <iostream>

#include "../AABB.h"

#include <cfloat>
#include <algorithm>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Vector bright, Renderer* renderer, int isTex)
{
	this->vertices = vertices;
	this->indices = indices;

	this->renderer = renderer;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	    glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
		glEnableVertexAttribArray(4);

	glBindVertexArray(0);

	shader = new Shader(mesh::vertex, mesh::fragment);
	shader->Use();

	glUniform1i(shader->GetLoc("isTex"), isTex);
	glUniform1f(shader->GetLoc("epsilon"), Math::epsilon);

	translate = Matrix(1.0f);
	scale = Matrix(1.0f);
	rotate = Matrix(1.0f);

	this->bright = bright;
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);

	delete shader;
}

void Mesh::Draw()
{
	shader->Use();
	glUniformMatrix4fv(shader->GetLoc("projection"), 1, GL_TRUE, &renderer->camera->projection.data[0][0]);
	glUniformMatrix4fv(shader->GetLoc("view"), 1, GL_TRUE, &renderer->camera->view.data[0][0]);
    model =  translate * rotate * scale;
	glUniformMatrix4fv(shader->GetLoc("model"), 1, GL_TRUE, &model.data[0][0]);

	glUniform3f(shader->GetLoc("bright"), bright.x, bright.y, bright.z);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Handle M_R_Mesh(Manager* manager, Renderer* renderer, float bright[],float pos[], float texCoords[], float color[], unsigned int indices[], int indexNum, int posNum, int isTex)
{
	int myId = manager->nextId++;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> _indices;

	std::cout << "Mesh Handle: " << myId << std::endl;

	for (size_t i = 0; i < indexNum; i++)
	{
		_indices.push_back(indices[i]);
	}

	unsigned int index = 0, index_2 = 0;
	for (size_t i = 0; i < posNum; i++)
	{
		Vertex vertex;

		vertex.position = Vector(pos[index], pos[index + 1], pos[index + 2]);
		vertex.texCoords = Vector(texCoords[index_2], texCoords[index_2 + 1]);
		vertex.color = Vector(color[0], color[1], color[2]);

		vertices.push_back(vertex);
		index += 3;
		index_2 += 2;
 	}

	for (size_t i = 0; i < _indices.size(); i+=3)
	{
		Vector p1 = vertices[_indices[i]].position;
  		Vector p2 = vertices[_indices[i + 1]].position;
  	    Vector p3 = vertices[_indices[i + 2]].position;

   		Vector t1 = vertices[_indices[i]].texCoords;
  	    Vector t2 = vertices[_indices[i + 1]].texCoords;
  	    Vector t3 = vertices[_indices[i + 2]].texCoords;

        Vector e1 = p2 - p1;
        Vector e2 = p3 - p1;

        float deltaU_1 = t2.x - t1.x, deltaV_1 = t2.y - t1.y;
        float deltaU_2 = t3.x - t1.x, deltaV_2 = t3.y - t1.y;

        Vector tangent(0.0f, 0.0f, 0.0f);

        float det = deltaU_1 * deltaV_2 - deltaU_2 * deltaV_1;
        if (std::fabs(det) >= Math::epsilon)
        {
            float f = 1.0f / det;
        
            tangent = f * (deltaV_2 * e1 - deltaV_1 * e2);
        }

        Vector normal = Cross(e1, e2);

        vertices[_indices[i]].normal += normal;
        vertices[_indices[i + 1]].normal += normal;
        vertices[_indices[i + 2]].normal += normal;

        vertices[_indices[i]].tangent += tangent;
        vertices[_indices[i + 1]].tangent += tangent;
        vertices[_indices[i + 2]].tangent += tangent;

	}

	Mesh* mesh = new Mesh(vertices, _indices, Vector(bright[0], bright[1], bright[2]),renderer, isTex);

	Record* newRecords = (Record*)realloc(manager->records, (manager->size + 1) * sizeof(Record));
	if (newRecords == NULL) return -1;

	manager->records = newRecords;

	manager->records[manager->size].id = myId;
	manager->records[manager->size].type = MESH; // Type !!!!!!!!
	manager->records[manager->size].ptr = mesh;

	manager->size += 1;
	return myId;
}

void R_Mesh_Draw(Mesh* mesh)
{
	mesh->Draw();
}

void R_Mesh_Translate(Mesh* mesh, float x, float y, float z)
{
	mesh->translate = Matrix(
		1.0f, 0.0f, 0.0f, x,
		0.0f, 1.0f, 0.0f, y,
		0.0f, 0.0f, 1.0f, z,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

void R_Mesh_Scale(Mesh* mesh, float x, float y, float z)
{
	mesh->scale = Matrix(
		x, 0.0f, 0.0f, 0.0f,
		0.0f, y, 0.0f, 0.0f,
		0.0f, 0.0f, z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

void R_Mesh_Rotate(Mesh* mesh, float axis[3], float angle)
{
	Quat q = Quat(angle, Vector(axis[0], axis[1], axis[2]));
	mesh->rotate = q.ToMat();
}

void R_Mesh_Set_properties(Mesh* mesh, float metallic, float roughness)
{
	mesh->shader->Use();
	glUniform1f(mesh->shader->GetLoc("metallic"), metallic);
    glUniform1f(mesh->shader->GetLoc("roughness"), roughness);
}

AABB R_Mesh_Get_AABB(Mesh* mesh)
{
	AABB result;

    if (!mesh || mesh->vertices.empty())
    {
        result.min[0] = result.min[1] = result.min[2] = 0.0f;
        result.max[0] = result.max[1] = result.max[2] = 0.0f;
        return result;
    }

    Vector minPos = { FLT_MAX,  FLT_MAX,  FLT_MAX };
    Vector maxPos = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

    for (size_t i = 0; i < mesh->vertices.size(); i++)
    {
        const Vector& pos = mesh->vertices[i].position;

        minPos.x = std::min(minPos.x, pos.x);
        minPos.y = std::min(minPos.y, pos.y);
        minPos.z = std::min(minPos.z, pos.z);

        maxPos.x = std::max(maxPos.x, pos.x);
        maxPos.y = std::max(maxPos.y, pos.y);
        maxPos.z = std::max(maxPos.z, pos.z);
    }

    result.min[0] = minPos.x; result.min[1] = minPos.y; result.min[2] = minPos.z;
    result.max[0] = maxPos.x; result.max[1] = maxPos.y; result.max[2] = maxPos.z;

    return result;
}

AABB R_Mesh_Set_AABB(AABB aabb, Mesh* mesh)
{
	Matrix modelMatrix = mesh->translate * mesh->rotate * mesh->scale;

	AABB worldAABB;

	worldAABB.min[0] = FLT_MAX;
    worldAABB.min[1] = FLT_MAX;
    worldAABB.min[2] = FLT_MAX;
    worldAABB.max[0] = -FLT_MAX;
    worldAABB.max[1] = -FLT_MAX;
    worldAABB.max[2] = -FLT_MAX;

	Vector corners[8];
    corners[0] = Vector(aabb.min[0], aabb.min[1], aabb.min[2], 1.0f);
    corners[1] = Vector(aabb.max[0], aabb.min[1], aabb.min[2], 1.0f);
    corners[2] = Vector(aabb.min[0], aabb.max[1], aabb.min[2], 1.0f);
    corners[3] = Vector(aabb.min[0], aabb.min[1], aabb.max[2], 1.0f);
    corners[4] = Vector(aabb.max[0], aabb.max[1], aabb.min[2], 1.0f);
    corners[5] = Vector(aabb.max[0], aabb.min[1], aabb.max[2], 1.0f);
    corners[6] = Vector(aabb.min[0], aabb.max[1], aabb.max[2], 1.0f);
    corners[7] = Vector(aabb.max[0], aabb.max[1], aabb.max[2], 1.0f);

	for (int i = 0; i < 8; ++i)
    {
        Vector transformedCorner = modelMatrix * corners[i];
        worldAABB.min[0] = std::min(worldAABB.min[0], transformedCorner.x);
        worldAABB.min[1] = std::min(worldAABB.min[1], transformedCorner.y);
        worldAABB.min[2] = std::min(worldAABB.min[2], transformedCorner.z);

        worldAABB.max[0] = std::max(worldAABB.max[0], transformedCorner.x);
        worldAABB.max[1] = std::max(worldAABB.max[1], transformedCorner.y);
        worldAABB.max[2] = std::max(worldAABB.max[2], transformedCorner.z);
    }
	return worldAABB;
}

void R_Mesh_Offset(Mesh* mesh, float x, float y, float z)
{
	mesh->translate._14 += x;  
	mesh->translate._24 += y;
	mesh->translate._34 += z;
}