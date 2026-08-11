#define ENGINE_DLL
#include "../engine.h"
#include "../manager.h"
#include "Renderer.h"

#define FLT_MAX 1e30f

Vector worldRight(1.0f, 0.0f, 0.0f);
Vector worldUp(0.0f, 1.0f, 0.0f);
Vector worldFront(0.0f, 0.0f, -1.0f);

Camera* Cam_Create(Renderer* renderer, float pos[3], float near, float far, float fov)
{
	renderer->camera->front = Vector(0.0f, 0.0f, -1.0f);
    renderer->camera->_1font = renderer->camera->front;
	renderer->camera->near = near;
	renderer->camera->far = far;
	renderer->camera->fov = ToRad(fov);
	renderer->camera->pos = Vector(pos[0], pos[1], pos[2]);
	
	return renderer->camera;
}

void Cam_Update(Camera* camera)
{
	camera->right = Cross(camera->front, worldUp).Normalize();
	camera->up = Cross(camera->right, camera->front).Normalize();

    camera->view._11 = camera->right.x;
    camera->view._12 = camera->right.y;
    camera->view._13 = camera->right.z;

    camera->view._21 = camera->up.x;
    camera->view._22 = camera->up.y;
    camera->view._23 = camera->up.z;

    camera->view._31 = -camera->front.x;
    camera->view._32 = -camera->front.y;
    camera->view._33 = -camera->front.z;

    camera->view._14 = -Dot(camera->pos, camera->right);
    camera->view._24 = -Dot(camera->pos, camera->up);
    camera->view._34 = Dot(camera->pos, camera->front);

    camera->projection._11 = 1.0f / (std::tan(camera->fov * 0.5f) * camera->aspect);
    camera->projection._22 = 1.0f / (std::tan(camera->fov * 0.5f));
    camera->projection._33 = (camera->near + camera->far) / (camera->near - camera->far);
    camera->projection._44 = 0.0f;

    camera->projection._34 = (2.0f * camera->near * camera->far) / (camera->near - camera->far);
    camera->projection._43 = -1.0f;
}

void Cam_Move(Camera* camera, float x, float y, float z)
{
    camera->pos += x * camera->right + y * camera->up + z * camera->front;
}

void Cam_Rotate(Camera* camera, float pitch, float yaw, float roll)
{
    Quat q1(ToRad(pitch), worldRight);
    Quat q2(ToRad(yaw), worldUp);
    Quat q3(ToRad(roll), worldFront);

    camera->front = VecRotate(q2 * q1 * q3, camera->_1font).Normalize();

}

void Cam_GetPos(Camera* camera, float *pos)
{
    pos[0] = camera->pos.x;
    pos[1] = camera->pos.y;
    pos[2] = camera->pos.z;
}

void Cam_GetDir(Camera* camera, float *dir)
{
    dir[0] = camera->front.x;
    dir[1] = camera->front.y;
    dir[2] = camera->front.z;
}

AABB R_AABB_To_Screen(Camera* camera, AABB aabb)
{
    AABB result;

    Matrix transform = camera->projection * camera->view;

    result.min[0] = FLT_MAX;
    result.min[1] = FLT_MAX;
    result.min[2] = FLT_MAX;
    result.max[0] = -FLT_MAX;
    result.max[1] = -FLT_MAX;
    result.max[2] = -FLT_MAX;

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
        Vector transformedCorner = transform * corners[i];

        if (transformedCorner.w != 0.0f) {
            transformedCorner.x /= transformedCorner.w;
            transformedCorner.y /= transformedCorner.w;
            transformedCorner.z /= transformedCorner.w;
        }

        result.min[0] = std::min(result.min[0], transformedCorner.x);
        result.min[1] = std::min(result.min[1], transformedCorner.y);
        result.min[2] = std::min(result.min[2], transformedCorner.z);

        result.max[0] = std::max(result.max[0], transformedCorner.x);
        result.max[1] = std::max(result.max[1], transformedCorner.y);
        result.max[2] = std::max(result.max[2], transformedCorner.z);
    }

    return result;
}