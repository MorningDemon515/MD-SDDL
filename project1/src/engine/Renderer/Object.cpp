#define ENGINE_DLL
#include "../engine.h"
#include "../manager.h"
#include "Object.h"

#include "../Math/Math.h"

Handle M_R_Mesh_Plane(Manager* manager, Renderer* renderer, float bright[], float color[], int isTex)
{
    float pos[] = {
        1.0f, 0.0f, 1.0f,
        -1.0f, 0.0f, 1.0f,
        -1.0f, 0.0f, -1.0f,
        1.0f, 0.0f, -1.0f
    };

    float texCoords[] = {
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
    };

    unsigned int indices[] = {
       0, 2, 1,  
       2, 0, 3  
    };

    return M_R_Mesh(manager, renderer, bright, pos, texCoords, color, indices, 6, 4, isTex);
}

Handle M_R_Mesh_Cube(Manager* manager, Renderer* renderer, float bright[], float color[], int isTex)
{
    float pos[] = {
    -0.5f, -0.5f,  0.5f, 
     0.5f, -0.5f,  0.5f, 
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f, 

    -0.5f, -0.5f, -0.5f, 
    -0.5f,  0.5f, -0.5f, 
     0.5f,  0.5f, -0.5f, 
     0.5f, -0.5f, -0.5f, 

    -0.5f,  0.5f, -0.5f, 
    -0.5f,  0.5f,  0.5f, 
     0.5f,  0.5f,  0.5f, 
     0.5f,  0.5f, -0.5f, 

    -0.5f, -0.5f, -0.5f, 
     0.5f, -0.5f, -0.5f, 
     0.5f, -0.5f,  0.5f, 
    -0.5f, -0.5f,  0.5f, 

     0.5f, -0.5f, -0.5f, 
     0.5f,  0.5f, -0.5f, 
     0.5f,  0.5f,  0.5f,
     0.5f, -0.5f,  0.5f, 

    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f  
    };

    float texCoords[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,

    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,

    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,

    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,

    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,

    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f
    };

    unsigned int indices[] = {
     0,  1,  2,     2,  3,  0,
     7,  4,  5,     5,  6,  7,
     9, 10, 11,    11,  8,  9,
    12, 13, 14,    14, 15, 12,
    19, 16, 17,    17, 18, 19,
    20, 21, 22,    22, 23, 20
    };

    return M_R_Mesh(manager, renderer, bright, pos, texCoords, color, indices, 36, 24, isTex);
}

void generateUnitSphere(
    int stacks, 
    int sectors,
    std::vector<float>& pos,
    std::vector<float>& texCoords,
    std::vector<unsigned int>& indices
) {
    pos.clear();
    texCoords.clear();
    indices.clear();

    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = Math::pi / 2.0f - i * (Math::pi / stacks);
        float xy = std::cos(stackAngle); 
        float y  = std::sin(stackAngle); 

        float v = (float)i / stacks;

        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * (2.0f * Math::pi / sectors);

            float x = xy * std::cos(sectorAngle);
            float z = xy * std::sin(sectorAngle);

            pos.push_back(x);
            pos.push_back(y);
            pos.push_back(z);

            float u = (float)j / sectors;
            texCoords.push_back(u);
            texCoords.push_back(v);
        }
    }

    for (int i = 0; i < stacks; ++i) {
        int k1 = i * (sectors + 1);  
        int k2 = k1 + sectors + 1;   

        for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k1 + 1);
                indices.push_back(k2);
            }

            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2 + 1);
                indices.push_back(k2);
            }
        }
    }
}

Handle M_R_Mesh_Sphere(Manager* manager, Renderer* renderer, int stacks, int sectors, float bright[], float color[], int isTex)
{
    std::vector<float> pos;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;

    generateUnitSphere(stacks, sectors, pos, texCoords, indices);

    return M_R_Mesh(manager, renderer, bright, pos.data(), texCoords.data(), color, indices.data(), indices.size(), pos.size() / 3, isTex);
}

void generateCone(
    float radius,
    float height,
    int sectors,
    std::vector<float>& pos,
    std::vector<float>& texCoords,
    std::vector<unsigned int>& indices
) {
    pos.clear();
    texCoords.clear();
    indices.clear();

    for (int j = 0; j <= sectors; ++j) {
        float sectorAngle = j * (2.0f * Math::pi / sectors);
        float x = radius * std::cos(sectorAngle);
        float z = radius * std::sin(sectorAngle);
        float u = (float)j / sectors;

        pos.push_back(x);
        pos.push_back(0.0f);
        pos.push_back(z);
        texCoords.push_back(u);
        texCoords.push_back(0.0f); 

        pos.push_back(0.0f);
        pos.push_back(height);
        pos.push_back(0.0f);
        texCoords.push_back(u);
        texCoords.push_back(1.0f);
    }

    for (int j = 0; j < sectors; ++j) {
        int b1 = j * 2;       
        int t1 = b1 + 1;     
        int b2 = (j + 1) * 2; 

        indices.push_back(b1);
        indices.push_back(t1);
        indices.push_back(b2);
    }

    int centerIndex = pos.size() / 3; 

    pos.push_back(0.0f);
    pos.push_back(0.0f);
    pos.push_back(0.0f);
    texCoords.push_back(0.5f);
    texCoords.push_back(0.5f);

    for (int j = 0; j <= sectors; ++j) {
        float sectorAngle = j * (2.0f * Math::pi / sectors);
        float cosA = std::cos(sectorAngle);
        float sinA = std::sin(sectorAngle);

        pos.push_back(radius * cosA);
        pos.push_back(0.0f);
        pos.push_back(radius * sinA);

        texCoords.push_back(0.5f + 0.5f * cosA);
        texCoords.push_back(0.5f + 0.5f * sinA);
    }

    int capStart = centerIndex + 1;
    for (int j = 0; j < sectors; ++j) {
        indices.push_back(centerIndex);
        indices.push_back(capStart + j);
        indices.push_back(capStart + j + 1);
    }
}
Handle M_R_Mesh_Cone(Manager* manager, Renderer* renderer, float rad, float height, int sectors, float bright[], float color[], int isTex)
{
    std::vector<float> pos;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;

    generateCone(rad, height,sectors, pos, texCoords, indices);

    return M_R_Mesh(manager, renderer, bright, pos.data(), texCoords.data(), color, indices.data(), indices.size(), pos.size() / 3,isTex);
}

void generateCylinder(
    float radius,
    float height,
    int sectors,
    std::vector<float>& pos,
    std::vector<float>& texCoords,
    std::vector<unsigned int>& indices
) {
    pos.clear();
    texCoords.clear();
    indices.clear();

    float halfH = height * 0.5f;

    int sideStartIndex = pos.size() / 3;

    for (int j = 0; j <= sectors; ++j) {
        float sectorAngle = j * (2.0f * Math::pi / sectors);
        float x = radius * std::cos(sectorAngle);
        float z = radius * std::sin(sectorAngle);
        float u = (float)j / sectors;

        pos.push_back(x);
        pos.push_back(-halfH);
        pos.push_back(z);
        texCoords.push_back(u);
        texCoords.push_back(0.0f); 

        pos.push_back(x);
        pos.push_back(halfH);
        pos.push_back(z);
        texCoords.push_back(u);
        texCoords.push_back(1.0f); 
    }

    for (int j = 0; j < sectors; ++j) {
        int b1 = sideStartIndex + j * 2; 
        int t1 = b1 + 1;                   
        int b2 = sideStartIndex + (j + 1) * 2; 
        int t2 = b2 + 1;                      

        indices.push_back(b1);
        indices.push_back(t1);
        indices.push_back(b2);

        indices.push_back(t1);
        indices.push_back(t2);
        indices.push_back(b2);
    }

    int topStartIndex = pos.size() / 3;

    pos.push_back(0.0f);
    pos.push_back(halfH);
    pos.push_back(0.0f);
    texCoords.push_back(0.5f);
    texCoords.push_back(0.5f);

    for (int j = 0; j <= sectors; ++j) {
        float sectorAngle = j * (2.0f * Math::pi / sectors);
        float cosA = std::cos(sectorAngle);
        float sinA = std::sin(sectorAngle);

        pos.push_back(radius * cosA);
        pos.push_back(halfH);
        pos.push_back(radius * sinA);

        texCoords.push_back(0.5f + 0.5f * cosA);
        texCoords.push_back(0.5f - 0.5f * sinA);
    }

    int topCapStart = topStartIndex + 1;
    for (int j = 0; j < sectors; ++j) {
        indices.push_back(topStartIndex);
        indices.push_back(topCapStart + j + 1);
        indices.push_back(topCapStart + j);
    }

    int bottomStartIndex = pos.size() / 3;

    pos.push_back(0.0f);
    pos.push_back(-halfH);
    pos.push_back(0.0f);
    texCoords.push_back(0.5f);
    texCoords.push_back(0.5f);

    for (int j = 0; j <= sectors; ++j) {
        float sectorAngle = j * (2.0f * Math::pi / sectors);
        float cosA = std::cos(sectorAngle);
        float sinA = std::sin(sectorAngle);

        pos.push_back(radius * cosA);
        pos.push_back(-halfH);
        pos.push_back(radius * sinA);

        texCoords.push_back(0.5f + 0.5f * cosA);
        texCoords.push_back(0.5f + 0.5f * sinA);
    }

    int bottomCapStart = bottomStartIndex + 1;
    for (int j = 0; j < sectors; ++j) {
        indices.push_back(bottomStartIndex);
        indices.push_back(bottomCapStart + j);
        indices.push_back(bottomCapStart + j + 1);
    }
}

Handle M_R_Mesh_Cylinder(Manager* manager, Renderer* renderer, float rad, float height, int sectors, float bright[], float color[], int isTex)
{
    std::vector<float> pos;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;

    generateCylinder(rad, height,sectors, pos, texCoords, indices);

    return M_R_Mesh(manager, renderer, bright, pos.data(), texCoords.data(), color, indices.data(), indices.size(), pos.size() / 3,isTex);
}

void generateTorus(
    float mainRadius,
    float tubeRadius,
    int mainSectors,
    int tubeSectors,
    std::vector<float>& pos,
    std::vector<float>& texCoords,
    std::vector<unsigned int>& indices
) {
    pos.clear();
    texCoords.clear();
    indices.clear();

    for (int i = 0; i <= mainSectors; ++i) {
        float uAngle = i * (2.0f * Math::pi / mainSectors);
        float cosU = std::cos(uAngle);
        float sinU = std::sin(uAngle);

        float u = (float)i / mainSectors;

        for (int j = 0; j <= tubeSectors; ++j) {
            float vAngle = j * (2.0f * Math::pi / tubeSectors);
            float cosV = std::cos(vAngle);
            float sinV = std::sin(vAngle);

            float v = (float)j / tubeSectors;

            float x = (mainRadius + tubeRadius * cosV) * cosU;
            float y = tubeRadius * sinV;
            float z = (mainRadius + tubeRadius * cosV) * sinU;

            pos.push_back(x);
            pos.push_back(y);
            pos.push_back(z);

            texCoords.push_back(u);
            texCoords.push_back(v);
        }
    }

    for (int i = 0; i < mainSectors; ++i) {
        int k1 = i * (tubeSectors + 1);     
        int k2 = k1 + tubeSectors + 1;     

        for (int j = 0; j < tubeSectors; ++j, ++k1, ++k2) {

            indices.push_back(k1);
            indices.push_back(k1 + 1);
            indices.push_back(k2);

            indices.push_back(k1 + 1);
            indices.push_back(k2 + 1);
            indices.push_back(k2);
        }
    }
}

Handle M_R_Mesh_Torus(Manager* manager, Renderer* renderer, float mainRad, float tubeRad, int mainSec, int tubeSec,float bright[], float color[], int isTex)
{
    std::vector<float> pos;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;

    generateTorus(mainRad, tubeRad, mainSec, tubeSec, pos, texCoords, indices);

    return M_R_Mesh(manager, renderer, bright, pos.data(), texCoords.data(), color, indices.data(), indices.size(), pos.size() / 3,isTex);
}