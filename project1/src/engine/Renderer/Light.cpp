#define ENGINE_DLL
#include "../engine.h"
#include "../manager.h" 
#include "Renderer.h"

float size = 10.0f;
float near = 0.1f;
float far = 100.0f;

unsigned int R_Light_Paralle(Renderer* renderer, float color[3], float strength, float direction[3])
{
    unsigned int id = renderer->lightCount++;

    Light light;
    light.type = 0;

    light.color = Vector(color[0], color[1], color[2]);
    light.direction = Vector(direction[0], direction[1], direction[2], 0.0f);
    light.strength = strength;

    renderer->lights.push_back(light);

    return id;
}

unsigned int R_Light_Point(Renderer* renderer, float color[3], float strength, float position[3], float constant, float linear, float quadratic)
{
    unsigned int id = renderer->lightCount++;

    Light light;
    light.type = 1;

    light.color = Vector(color[0], color[1], color[2]);
    light.position = Vector(position[0], position[1], position[2], 1.0f);
    light.strength = strength;

    light.constant = constant;
    light.linear = linear;
    light.quadratic = quadratic;

    renderer->lights.push_back(light);

    return id;
}

unsigned int R_Light_Spot(Renderer* renderer, float color[3], float strength, float position[3], float direction[3], float constant, float linear, float quadratic, float cutOff, float outerCutOff)
{
    unsigned int id = renderer->lightCount++;

    Light light;
    light.type = 2;

    light.color = Vector(color[0], color[1], color[2]);
    light.position = Vector(position[0], position[1], position[2], 1.0f);
    light.direction = Vector(direction[0], direction[1], direction[2], 0.0f);
    light.strength = strength;

    light.constant = constant;
    light.linear = linear;
    light.quadratic = quadratic;

    light.cutOff = std::cos(ToRad(cutOff));
    light.outerCutOff = std::cos(ToRad(outerCutOff));

    renderer->lights.push_back(light);

    return id;
}

void R_Light_Set_position(Renderer* renderer, unsigned int lightid, float position[3])
{
    renderer->lights[lightid].position = Vector(position[0], position[1], position[2], 1.0f);
    renderer->isLightChance = true;
}

void R_Light_Set_direction(Renderer* renderer, unsigned int lightid, float direction[3])
{
    renderer->lights[lightid].direction = Vector(direction[0], direction[1], direction[2], 0.0f);
    renderer->isLightChance = true;
}

void R_Light_Set_attenuation(Renderer* renderer, unsigned int lightid, float constant, float linear, float quadratic)
{
    renderer->lights[lightid].constant = constant;
    renderer->lights[lightid].linear = linear;
    renderer->lights[lightid].quadratic = quadratic;
    renderer->isLightChance = true;
}

void R_Light_Set_angle(Renderer* renderer, unsigned int lightid, float cutOff, float outerCutOff)
{
    renderer->lights[lightid].cutOff = std::cos(ToRad(cutOff));
    renderer->lights[lightid].outerCutOff = std::cos(ToRad(outerCutOff));
    renderer->isLightChance = true;
}

void R_Light_Set_color(Renderer* renderer, unsigned int lightid, float color[3])
{
    renderer->lights[lightid].color = Vector(color[0], color[1], color[2]);
    renderer->isLightChance = true;
}

void R_Light_Set_strength(Renderer* renderer, unsigned int lightid, float strength)
{
    renderer->lights[lightid].strength = strength;
    renderer->isLightChance = true;
}