#include <iostream>
#include <engine.h>

#include "Color.h"

void MousePosToNDC(float *out_x, float *out_y, float x, float y, int window_width, int window_height);

int main()
{
	
	InitEngine();
    std::cout << "Init Engine" << std::endl;

	Manager* manager = M_Create();
    std::cout << "Create Manager" << std::endl;

	/*--------------------Window--------------------*/
	Handle winHandle = M_W_Create(manager, 800, 600);
	Window* window = (Window*)M_Get(manager, winHandle);
	W_SetTitle(window, "SCP - Containment Breach");

	std::cout << "Create Window" << std::endl;
	/*----------------------------------------------*/

	/*--------------------Keyboard--------------------*/
	Handle keyHandle = M_K_Create(manager);
	Key* key = (Key*)M_Get(manager, keyHandle);

	std::cout << "Create Keyboard" << std::endl;
	/*------------------------------------------------*/

	/*--------------------Renderer--------------------*/
	Handle rendererHandle = M_R_Create(manager, window);
	Renderer* renderer = (Renderer*)M_Get(manager, rendererHandle);

	R_IsLight(renderer, true);

	R_SetGamma(renderer, 1.0f);

	R_Init_SkyBox(renderer, "res/skybox/back.jpg",
	                        "res/skybox/bottom.jpg",
						    "res/skybox/front.jpg",
						    "res/skybox/left.jpg",
						    "res/skybox/right.jpg",
						    "res/skybox/top.jpg");

	R_Set_SkyBox(renderer, false);

	R_Set_SkyBox_Light(renderer, true);

	R_SetViewport(renderer, 0, 0, 800, 600);

	std::cout << "Create Renderer" << std::endl;

	/*------------------------------------------------*/

	/*--------------------Camera--------------------*/
	float camPos[3] = { 0.0f, 0.0f, 3.0f };
	float camDir[3];
	Camera* camera = Cam_Create(renderer, camPos, 0.1f, 100.0f, 45.0f);

	std::cout << "Create Camera" << std::endl;

	/*----------------------------------------------*/

	/*--------------------Light--------------------*/
	float lightPos[3] = {2.0f, 2.0f, 2.0f};
	float sunDir[3] = {1.0f, -1.0f, 1.0f};
    id light1 = R_Light_Point(renderer, color_white, 5.0f, lightPos, 0.09f, 0.032f, 0.032f);

	float lightDir[3] = {0.0f, 0.0f, -1.0f};
	id light2 = R_Light_Spot(renderer, color_white, 0.0f, camPos, lightDir, 1.0f, 0.09f, 0.032f, 12.5f, 17.5f);

	std::cout << "Create Light" << std::endl;
	/*---------------------------------------------*/

	/*--------------------Mesh--------------------*/
	Handle planeHandle = M_R_Mesh_Plane(manager, renderer, color_black, color_gray, 0);
	Mesh* plane = (Mesh*)M_Get(manager, planeHandle);

	R_Mesh_Set_properties(plane, 0.9f, 0.1f);

	R_Mesh_Translate(plane, 0.0f, -1.0f, 1.0f);
	R_Mesh_Scale(plane, 5.0f, 5.0f, 5.0f);

    Handle bricksHandle = T_CreateFromFile(manager, "res/bricks2.jpg", ALBEDO);
	Texture* bricks = (Texture*)M_Get(manager, bricksHandle);

	Handle bricks_normalHandle = T_CreateFromFile(manager, "res/bricks2_normal.jpg", NORMAL);
    Texture* bricks_normal = (Texture*)M_Get(manager, bricks_normalHandle);

	Handle cubeHandle = M_R_Mesh_Cube(manager, renderer, color_black, color_red, 1);
	Mesh* cube = (Mesh*)M_Get(manager, cubeHandle);

	R_Mesh_Set_properties(cube, 0.1f, 0.9f);

	Handle sphereHandle = M_R_Mesh_Sphere(manager, renderer, 32, 32, color_white, color_white, 0);
	Mesh* sphere = (Mesh*)M_Get(manager, sphereHandle);

	R_Mesh_Set_properties(sphere, 0.1f, 0.9f);

	R_Mesh_Translate(sphere, lightPos[0], lightPos[1], lightPos[2]);
	R_Mesh_Scale(sphere, 0.2f, 0.2f, 0.2f);

	Handle coneandle = M_R_Mesh_Cone(manager, renderer, 1.0f, 2.0f, 32, color_black, color_blue, 0);
	Mesh* cone = (Mesh*)M_Get(manager, coneandle);

	R_Mesh_Set_properties(cone, 0.1f, 0.9f);

	R_Mesh_Translate(cone, 0.0f, -0.99f, -2.0f);

	Handle cylinderHandle = M_R_Mesh_Cylinder(manager, renderer, 1.0f, 2.0f, 32, color_black, color_green, 0);
	Mesh* cylinder = (Mesh*)M_Get(manager, cylinderHandle);

	R_Mesh_Set_properties(cylinder, 0.1f, 0.9f);

	R_Mesh_Translate(cylinder, -2.0f, 0.01f, 0.0f);

	Handle torusHandle = M_R_Mesh_Torus(manager, renderer, 1.0f, 0.3f, 32, 32, color_black, color_yellow, 0);
	Mesh* torus = (Mesh*)M_Get(manager, torusHandle);

	R_Mesh_Set_properties(torus, 0.1f, 0.9f);

	R_Mesh_Translate(torus, 2.0f, 0.0f, 0.0f);

	std::cout << "Create Mesh" << std::endl;
	/*---------------------------------------------*/

	/*--------------------AABB--------------------*/

    AABB sphereAABB = R_Mesh_Get_AABB(sphere);
	std::cout << "Local Sphere AABB min: " << sphereAABB.min[0] << " " << sphereAABB.min[1] << " " << sphereAABB.min[2] << std::endl;
	std::cout << "Local Sphere AABB max: " << sphereAABB.max[0] << " " << sphereAABB.max[1] << " " << sphereAABB.max[2] << std::endl;

	/*--------------------------------------------*/

	Test();

	bool relvative = true;
	bool big = false;
	bool flash = false;
	bool first = true;
	bool isDragging = false;
	float ori[2];

	static float lastTime = GetTime();

	float walkV = 5.0f, rotV = 50.0f;
	float pitch = 0.0f, yaw = 0.0f;
	float m_r_x, m_r_y;
	float m_x, m_y;
	int window_width, window_height;

	while (W_IsRun(window))
	{
		float currTime = GetTime();
		float timeDelta = (currTime - lastTime) * 0.001f;

		W_PollEvents(window);
		K_Event(key);

		if (K_Press(key, K_ESCAPE))
		{
			relvative = !relvative;
			first = true;
		}

		W_SetRelative(window, relvative);
        W_GetSize(window, &window_width, &window_height);

		Cam_GetPos(camera, camPos);
	    Cam_GetDir(camera, camDir);

		R_Light_Set_position(renderer, light2, camPos);
		R_Light_Set_direction(renderer, light2, camDir);

		Mouse_GetRelative(&m_r_x,&m_r_y);

		if (!relvative)
		{
			Mouse_GetPos(&m_x, &m_y);

			AABB world_sphereAABB = R_Mesh_Set_AABB(sphereAABB, sphere); 
			AABB screen_sphereAABB = R_AABB_To_Screen(camera, world_sphereAABB); 

            float ndc_x, ndc_y;
			MousePosToNDC(&ndc_x, &ndc_y, m_x, m_y, window_width, window_height);

			if (Mouse_Down(BUTTON_LEFT))
			{
				if (!isDragging)
				{
					if (screen_sphereAABB.min[0] < ndc_x && ndc_x < screen_sphereAABB.max[0] &&
						screen_sphereAABB.min[1] < ndc_y && ndc_y < screen_sphereAABB.max[1])
					{
						isDragging = true;
						ori[0] = ndc_x;
						ori[1] = ndc_y;
					}
				}
				else
				{
					float deltaX = ndc_x - ori[0];
					float deltaY = ndc_y - ori[1];
					float factor = 6.0f;

					R_Mesh_Offset(sphere, factor * deltaX, 0.0f, -factor * deltaY);
					lightPos[0] += factor * deltaX;
					lightPos[2] -= factor * deltaY;

					R_Light_Set_position(renderer, light1, lightPos);

					ori[0] = ndc_x;
					ori[1] = ndc_y;
				}
			}
			else
			{
				isDragging = false;
			}

		}

		if (relvative)
		{
			m_x = 0.0f; m_y = 0.0f;

			if (first)
			{
				m_r_x = 0.0f;
				m_r_y = 0.0f;
				first = false;
			}

		    pitch -= m_r_y * rotV * timeDelta; yaw -= m_r_x * rotV * timeDelta;

		    if (pitch >= 89.0f)
			    pitch = 89.0f;

		    if (pitch <= -89.0f)
			    pitch = -89.0f;

			if (K_Press(key, K_R))
			{
				big = !big;
			}

			if (K_Press(key, K_F))
			{
				flash = !flash;
			}

			if (big)
			{
                W_SetSize(window, 1280, 720); 
				R_SetViewport(renderer, 0, 0, 1280, 720);
			}
			else
			{
                W_SetSize(window, 800, 600);
				R_SetViewport(renderer, 0, 0, 800, 600);
			}

			if (flash)
			{
				R_Light_Set_strength(renderer, light2, 10.0f);
			}
			else
			{
				R_Light_Set_strength(renderer, light2, 0.0f);
			}

			if (K_Down(key, K_W))
			{
				Cam_Move(camera,0.0f, 0.0f, walkV * timeDelta);
			}

			if (K_Down(key, K_S))
			{
				Cam_Move(camera, 0.0f, 0.0f, -walkV * timeDelta);
			}

			if (K_Down(key, K_A))
			{
				Cam_Move(camera, -walkV * timeDelta, 0.0f, 0.0f);
			}

			if (K_Down(key, K_D))
			{
				Cam_Move(camera, walkV * timeDelta, 0.0f, 0.0f);
			}

			if (K_Down(key, K_SPACE))
			{
				Cam_Move(camera, 0.0f, walkV * timeDelta, 0.0f);
			}

			if (K_Down(key, K_LSHIFT))
			{
				Cam_Move(camera, 0.0f, -walkV * timeDelta, 0.0f);
			}

			Cam_Rotate(camera, pitch, yaw, 0.0f);
		}

		Cam_Update(camera);

		R_Begin(renderer);

		    R_Clear(renderer, 0.0f, 0.0f, 0.0f);

		    R_Mesh_Draw(plane);

			T_Enable(bricks, cube, 0);
			T_Enable(bricks_normal, cube, 1);
			R_Mesh_Draw(cube);

			R_Mesh_Draw(sphere);

			R_Mesh_Draw(cone);

			R_Mesh_Draw(cylinder);

			R_Mesh_Draw(torus);

		R_End(renderer);

		R_Present(renderer);
		K_Update(key);
		Mouse_Update();
		lastTime = currTime;
	}

	M_Release(manager);
	QuitEngine();
	return 0;
}

void MousePosToNDC(float *out_x, float *out_y, float x, float y, int window_width, int window_height)
{
	float m_x = (2.0f * x) / window_width - 1.0f; 
	float m_y = 1.0f - (2.0f * y) / window_height;

	*out_x = m_x; *out_y = m_y;
}