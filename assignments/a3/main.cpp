// #####################################################################
//  Main
//  CS3451 Computer Graphics Starter Code
//  Contact: Bo Zhu (bo.zhu@gatech.edu)
// #####################################################################
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <unordered_set>

#include "OpenGLMesh.h"
#include "OpenGLMarkerObjects.h"
#include "OpenGLCommon.h"
#include "OpenGLWindow.h"
#include "OpenGLViewer.h"
#include "TinyObjLoader.h"

#ifndef __Main_cpp__
#define __Main_cpp__

#ifdef __APPLE__
#define CLOCKS_PER_SEC 100000
#endif

// Add fallback definition for M_PI
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define DegreesToRadians(degrees) (degrees * M_PI / 180)

class MyDriver : public OpenGLViewer
{
	std::vector<OpenGLTriangleMesh *> mesh_object_array; ////mesh objects, every object you put in this array will be rendered.
	OpenGLSegmentMesh *floor = nullptr;
	std::vector<OpenGLSegmentMesh *> coords, trajectory;
	clock_t startTime;

public:
	virtual void Initialize()
	{
		draw_axes = false;
		startTime = clock();
		OpenGLViewer::Initialize();
		opengl_window->camera_distance = 25.f;
		opengl_window->camera_target = Vector3f(0, 3.5, 0);
		opengl_window->Update_Clip_Planes();
	}

	void Create_Background(const OpenGLColor &color1, const OpenGLColor &color2)
	{
		auto bg = Add_Interactive_Object<OpenGLBackground>();
		bg->Set_Color(color1, color2);
		bg->Initialize();
	}

	virtual void Initialize_Data()
	{
		Create_Background(OpenGLColor(0.71f, 0.6f, 0.17f, 1.f), OpenGLColor(0.71f, 0.87f, 0.17f, 1.f));
		OpenGLShaderLibrary::Instance()->Add_Shader_From_File("a3_vert.vert", "a3_frag.frag", "a3_shading"); ////bind shader for this assignment

		// Create_Angry_Bird_Palace(); ////TODO: Comment this line when you start to implement your customized scene
		Create_Angry_Bird_Garden(); ////TODO: Uncomment this line when you start to implement your customized scene
	}

	void Create_Angry_Bird_Palace()
	{
		//// draw the three axes
		Add_Coord({Vector3(0, 0.01, 0), Vector3(5, 0.01, 0)}, OpenGLColor(1, 0, 0, 1)); //// X axis
		Add_Coord({Vector3(0, 0, 0), Vector3(0, 5, 0)}, OpenGLColor(0, 1, 0, 1));		//// Y axis
		Add_Coord({Vector3(0, 0.01, 0), Vector3(0, 0.01, 5)}, OpenGLColor(0, 0, 1, 1)); //// Z zxis

		//// draw the ground
		Add_Ground();

		//// Step 1: add the castle by reading the model from "castle.obj"
		//// The model needs to undergo the following transform operations in sequence:
		//// (1) rotate *counterclockwisely* around the y-axis by 90 degrees,
		//// (2) uniformly scale by a factor of 5,
		//// (3) translate upwards by 1.3 units in the y direction.
		//// Your task is to specify the values of the 4x4 transform matrix and send it to the mesh model via Set_Model_Matrix().
		//// You are allowed to use a chain of matrix multiplications to calculate the matrix.

		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */
		auto castle = Add_Obj_Mesh_Object_From_File("castle.obj", OpenGLColor(.6f, .6f, .6f, 1.f));
		{
			Matrix4f rotation, scaling, translation;
			rotation << cos(DegreesToRadians(90)), 0., sin(DegreesToRadians(90)), 0.,
				0., 1., 0., 0.,
				-sin(DegreesToRadians(90)), 0., cos(DegreesToRadians(90)), 0.,
				0., 0., 0., 1.;

			scaling << 5., 0., 0., 0.,
				0., 5., 0., 0.,
				0., 0., 5., 0.,
				0., 0., 0., 1.;

			translation << 1., 0., 0., 0.,
				0., 1., 0., 1.3,
				0., 0., 1., 0.,
				0., 0., 0., 1.;

			castle->Set_Model_Matrix(translation * scaling * rotation);
		}
		/* Your implementation ends. */

		//// Step 2: add the axes statue by reading the model from "axes.obj"
		//// The model needs to undergo the following transform operations in sequence:
		//// (1) rotate *counterclockwisely* around the y-axis by 90 degrees,
		//// (2) uniformly scale by a factor of 2,
		//// (3) translate by 6 units in the positive x direction and 1 unit in the y direction.

		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */
		auto axes = Add_Obj_Mesh_Object_From_File("axes.obj", OpenGLColor(.9f, .5f, .0f, 1.f));
		{
			Matrix4f rotation, scaling, translation;
			rotation << cos(DegreesToRadians(90)), 0., sin(DegreesToRadians(90)), 0.,
				0., 1., 0., 0.,
				-sin(DegreesToRadians(90)), 0., cos(DegreesToRadians(90)), 0.,
				0., 0., 0., 1.;

			scaling << 2., 0., 0., 0.,
				0., 2., 0., 0.,
				0., 0., 2., 0.,
				0., 0., 0., 1.;

			translation << 1., 0., 0., 6.,
				0., 1., 0., 1.,
				0., 0., 1., 0.,
				0., 0., 0., 1.;

			axes->Set_Model_Matrix(translation * scaling * rotation);
		}
		/* Your implementation ends. */

		//// Step 3: add the magic tower by reading the model from "tower.obj"
		//// The model needs to undergo the following transform operations in sequence:
		//// (1) rotate *clockwisely* around the y-axis by 45 degrees,
		//// (2) non-uniformly scale by factors of 2, 6, 2 in the x, y, z directions
		//// (3) translate by 6 units in the negative x direction and 3 units in the y direction.

		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */
		auto tower = Add_Obj_Mesh_Object_From_File("tower.obj", OpenGLColor(.0f, .5f, .5f, 1.f));
		{
			Matrix4f rotation, scaling, translation;
			rotation << cos(DegreesToRadians(-45)), 0., sin(DegreesToRadians(-45)), 0.,
				0., 1., 0., 0.,
				-sin(DegreesToRadians(-45)), 0., cos(DegreesToRadians(-45)), 0.,
				0., 0., 0., 1.;

			scaling << 2., 0., 0., 0.,
				0., 6., 0., 0.,
				0., 0., 2., 0.,
				0., 0., 0., 1.;

			translation << 1., 0., 0., -6.,
				0., 1., 0., 3.,
				0., 0., 1., 0.,
				0., 0., 0., 1.;

			tower->Set_Model_Matrix(translation * scaling * rotation);
		}
		/* Your implementation ends. */

		//// Step 4: add 24 trees by reading the model from "tree1.obj"
		//// The 24 trees need to be distributed evenly and at equal distances along the circumference of the inner circle.
		//// The circle has its center at the origin and a radius of 8.
		//// Each tree needs to be translated in the positive y axis by 0.5 unit to ensure its base is above the ground.
		//// Calculate the transform matrix for each tree in the following for-loop.

		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */
		int tree_num = 24;
		for (int i = 0; i < tree_num; i++)
		{
			auto tree = Add_Obj_Mesh_Object_From_File("tree1.obj", OpenGLColor(0.f, 1.f, 0.f, 1.f));
			{
				Matrix4f translation;
				translation << 1., 0., 0., 8. * cos(DegreesToRadians(i * (360. / tree_num))),
					0., 1., 0., 0.5,
					0., 0., 1., 8. * sin(DegreesToRadians(i * (360. / tree_num))),
					0., 0., 0., 1.;
				tree->Set_Model_Matrix(translation);
			}
		}
		/* Your implementation ends. */

		//// Step 5: add 36 trees by reading the model from "tree2.obj"
		//// The 36 trees need to be distributed evenly and at equal distances along the circumference of the outer circle.
		//// The circle has its center at the origin and a radius of 10.
		//// Each tree needs to be translated in the positive y axis by 0.5 unit to ensure its base is above the ground.
		//// Calculate the transform matrix for each tree in the following for-loop.

		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */
		int tree2_num = 36;
		for (int i = 0; i < tree2_num; i++)
		{
			auto tree = Add_Obj_Mesh_Object_From_File("tree2.obj", OpenGLColor(0.f, 1.f, 0.f, 1.f));
			{
				Matrix4f translation;
				translation << 1., 0., 0., 10. * cos(DegreesToRadians(i * (360. / tree2_num))),
					0., 1., 0., 0.5,
					0., 0., 1., 10. * sin(DegreesToRadians(i * (360. / tree2_num))),
					0., 0., 0., 1.;
				tree->Set_Model_Matrix(translation);
			}
		}
		/* Your implementation ends. */

		//// Step 6: add 5 stone steps by using the function `Add_Cube()`
		//// `Add_Cube()` adds a cube mesh to the scene with its center at the origin and each side measuring 1 unit.
		//// Each stone step has its size as 1, 0.1, and 0.5 along x, y, z axes.
		//// The center of the first stone step is in (0, 0, 3), incremented by 1 in positive z direction for the following stone steps.
		//// Calculate the transform matrix for each stone step in the following for-loop.

		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */
		for (int i = 0; i < 5; i++)
		{
			auto cube1 = Add_Cube(1.f, OpenGLColor(0.1f, 0.1f, 0.1f, 1.f));
			{
				Matrix4f scaling, translation;
				scaling << 1., 0., 0., 0.,
					0., 0.1, 0., 0.,
					0., 0., 0.5, 0.,
					0., 0., 0., 1.;

				translation << 1., 0., 0., 0.,
					0., 1., 0., 0.,
					0., 0., 1., 3. + i,
					0., 0., 0., 1.;

				cube1->Set_Model_Matrix(translation * scaling);
			}
		}
		/* Your implementation ends. */

		//// Step 7: add 6 keyframes of a throwing angry bird following a parabola.
		//// The angry bird is thrown from (-5, 0, 0) with initial velocity (5, 9.8, 0) (unit/sec)
		//// Its angular velocity omega = 150 (deg/sec) *clockwise*
		//// The gravity is g = 9.8 (unit/sec) in negative y direction
		//// The x coordinate of the bird can be calculated as x = x0 + ux * t
		//// The y coordinate of the bird can be calculated as y = uy * t - 0.5 * g * t * t
		//// Your task is to draw keyframes at time t = 0.2, 0.5, 0.8, 1.1, 1.4, 1.7 respectively.
		//// To this end, you need to calculate the transform matrix for each keyframe of the angry bird in the following for-loop.
		//// You can temporarily uncomment the following line to visualize the parabola trajectory as a reference during implementation.
		//// Comment it out again once you complete the task.

		// Add_Arc_Trajectory();

		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */
		std::vector<float> time = {0.2, 0.5, 0.8, 1.1, 1.4, 1.7};
		int bird_num = 6;
		for (int i = 0; i < bird_num; i++)
		{
			auto bird = Add_Obj_Mesh_Object_From_File("bird.obj", OpenGLColor(1.f, 0.2f, 0.f, 1.f));
			{
				Matrix4f rotation, translation;
				rotation << cos(DegreesToRadians(-150 * time[i])), -sin(DegreesToRadians(-150 * time[i])), 0., 0.,
					sin(DegreesToRadians(-150 * time[i])), cos(DegreesToRadians(-150 * time[i])), 0., 0.,
					0., 0., 1., 0.,
					0., 0., 0., 1.;

				translation << 1., 0., 0., -5. + 5 * time[i],
					0., 1., 0., 9.8 * time[i] - 0.5 * 9.8 * time[i] * time[i],
					0., 0., 1., 0.,
					0., 0., 0., 1.;

				bird->Set_Model_Matrix(translation * rotation);
			}
		}
		/* Your implementation ends. */
	}

	//// Step 8: Create a new garden scene by using the mesh objects we provided, or download your own from online resources.
	//// Practise matrix transformation by mimicking the way we setup matrices in the `Create_Angry_Bird_Palace()` function.

	void Create_Angry_Bird_Garden()
	{
		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */

		//// draw the three axes, comment them out if you don't need them
		Add_Coord({Vector3(0, 0.01, 0), Vector3(5, 0.01, 0)}, OpenGLColor(1, 0, 0, 1)); //// X axis
		Add_Coord({Vector3(0, 0, 0), Vector3(0, 5, 0)}, OpenGLColor(0, 1, 0, 1));		//// Y axis
		Add_Coord({Vector3(0, 0.01, 0), Vector3(0, 0.01, 5)}, OpenGLColor(0, 0, 1, 1)); //// Z zxis

		//// draw the ground, comment them out if you don't need them
		Add_Ground();

		//// Central Fountain as the main focal point
		auto fountain = Add_Obj_Mesh_Object_From_File("garden/fountain1.obj", OpenGLColor(0.8f, 0.9f, 1.0f, 1.f));
		{
			Matrix4f scaling, translation;
			scaling << 2.5, 0., 0., 0.,
				0., 2.5, 0., 0.,
				0., 0., 2.5, 0.,
				0., 0., 0., 1.;

			translation << 1., 0., 0., 0.,
				0., 1., 0., 1.3,
				0., 0., 1., 0.,
				0., 0., 0., 1.;

			fountain->Set_Model_Matrix(translation * scaling);
		}

		// // Garden Building (Palace Guest House)
		// auto building = Add_Obj_Mesh_Object_From_File("garden/building1.obj", OpenGLColor(0.9f, 0.8f, 0.7f, 1.f));
		// {
		// 	Matrix4f rotation, scaling, translation;

		// 	rotation << cos(DegreesToRadians(180)), 0., sin(DegreesToRadians(180)), 0.,
		// 		0., 1., 0., 0.,
		// 		-sin(DegreesToRadians(180)), 0., cos(DegreesToRadians(180)), 0.,
		// 		0., 0., 0., 1.;

		// 	scaling << 3., 0., 0., 0.,
		// 		0., 3., 0., 0.,
		// 		0., 0., 3., 0.,
		// 		0., 0., 0., 1.;

		// 	translation << 1., 0., 0., 0.,
		// 		0., 1., 0., 1.,
		// 		0., 0., 1., -8.,
		// 		0., 0., 0., 1.;

		// 	building->Set_Model_Matrix(translation * scaling * rotation);
		// }

		//// Garden Perimeter Fence - On 20x20 Grid Boundary with Gates
		// Create fence segments along each side of the grid boundary
		// Each side has fences with 2-grid gaps in the middle as gates

		// North side fences (z = 9.5, from x = -10 to 10 with 2-grid gap in center)
		for (int x = -10; x <= -2; x += 2) // Left section of north side
		{
			auto fence = Add_Obj_Mesh_Object_From_File("garden/fence1.obj", OpenGLColor(0.6f, 0.4f, 0.2f, 1.f));
			{
				Matrix4f rotation, scaling, translation;

				rotation << cos(DegreesToRadians(0)), 0., sin(DegreesToRadians(0)), 0.,
					0., 1., 0., 0.,
					-sin(DegreesToRadians(0)), 0., cos(DegreesToRadians(0)), 0.,
					0., 0., 0., 1.;

				scaling << 2.0, 0., 0., 0.,
					0., 2.0, 0., 0.,
					0., 0., 2.0, 0.,
					0., 0., 0., 1.;

				translation << 1., 0., 0., x + 1,
					0., 1., 0., 0.3,
					0., 0., 1., 9.5,
					0., 0., 0., 1.;

				fence->Set_Model_Matrix(translation * scaling * rotation);
			}
		}
		for (int x = 2; x <= 8; x += 2) // Right section of north side
		{
			auto fence = Add_Obj_Mesh_Object_From_File("garden/fence1.obj", OpenGLColor(0.6f, 0.4f, 0.2f, 1.f));
			{
				Matrix4f rotation, scaling, translation;

				rotation << cos(DegreesToRadians(0)), 0., sin(DegreesToRadians(0)), 0.,
					0., 1., 0., 0.,
					-sin(DegreesToRadians(0)), 0., cos(DegreesToRadians(0)), 0.,
					0., 0., 0., 1.;

				scaling << 2.0, 0., 0., 0.,
					0., 2.0, 0., 0.,
					0., 0., 2.0, 0.,
					0., 0., 0., 1.;

				translation << 1., 0., 0., x + 1,
					0., 1., 0., 0.3,
					0., 0., 1., 9.5,
					0., 0., 0., 1.;

				fence->Set_Model_Matrix(translation * scaling * rotation);
			}
		}

		// South side fences (z = -9.5, from x = -10 to 10 with 2-grid gap in center)
		for (int x = -10; x <= -2; x += 2) // Left section of south side
		{
			auto fence = Add_Obj_Mesh_Object_From_File("garden/fence2.obj", OpenGLColor(0.6f, 0.4f, 0.2f, 1.f));
			{
				Matrix4f rotation, scaling, translation;

				rotation << cos(DegreesToRadians(180)), 0., sin(DegreesToRadians(180)), 0.,
					0., 1., 0., 0.,
					-sin(DegreesToRadians(180)), 0., cos(DegreesToRadians(180)), 0.,
					0., 0., 0., 1.;

				scaling << 2.0, 0., 0., 0.,
					0., 2.0, 0., 0.,
					0., 0., 2.0, 0.,
					0., 0., 0., 1.;

				translation << 1., 0., 0., x + 1,
					0., 1., 0., 0.3,
					0., 0., 1., -9.5,
					0., 0., 0., 1.;

				fence->Set_Model_Matrix(translation * scaling * rotation);
			}
		}
		for (int x = 2; x <= 8; x += 2) // Right section of south side
		{
			auto fence = Add_Obj_Mesh_Object_From_File("garden/fence2.obj", OpenGLColor(0.6f, 0.4f, 0.2f, 1.f));
			{
				Matrix4f rotation, scaling, translation;

				rotation << cos(DegreesToRadians(180)), 0., sin(DegreesToRadians(180)), 0.,
					0., 1., 0., 0.,
					-sin(DegreesToRadians(180)), 0., cos(DegreesToRadians(180)), 0.,
					0., 0., 0., 1.;

				scaling << 2.0, 0., 0., 0.,
					0., 2.0, 0., 0.,
					0., 0., 2.0, 0.,
					0., 0., 0., 1.;

				translation << 1., 0., 0., x + 1,
					0., 1., 0., 0.3,
					0., 0., 1., -9.5,
					0., 0., 0., 1.;

				fence->Set_Model_Matrix(translation * scaling * rotation);
			}
		}

		// East side fences (x = 9.5, from z = -10 to 10 with 2-grid gap in center)
		for (int z = -10; z <= -2; z += 2) // Lower section of east side
		{
			auto fence = Add_Obj_Mesh_Object_From_File("garden/fence3.obj", OpenGLColor(0.6f, 0.4f, 0.2f, 1.f));
			{
				Matrix4f rotation, scaling, translation;

				rotation << cos(DegreesToRadians(90)), 0., sin(DegreesToRadians(90)), 0.,
					0., 1., 0., 0.,
					-sin(DegreesToRadians(90)), 0., cos(DegreesToRadians(90)), 0.,
					0., 0., 0., 1.;

				scaling << 2.0, 0., 0., 0.,
					0., 2.0, 0., 0.,
					0., 0., 2.0, 0.,
					0., 0., 0., 1.;

				translation << 1., 0., 0., 9.5,
					0., 1., 0., 0.3,
					0., 0., 1., z + 1,
					0., 0., 0., 1.;

				fence->Set_Model_Matrix(translation * scaling * rotation);
			}
		}
		for (int z = 2; z <= 8; z += 2) // Upper section of east side
		{
			auto fence = Add_Obj_Mesh_Object_From_File("garden/fence3.obj", OpenGLColor(0.6f, 0.4f, 0.2f, 1.f));
			{
				Matrix4f rotation, scaling, translation;

				rotation << cos(DegreesToRadians(90)), 0., sin(DegreesToRadians(90)), 0.,
					0., 1., 0., 0.,
					-sin(DegreesToRadians(90)), 0., cos(DegreesToRadians(90)), 0.,
					0., 0., 0., 1.;

				scaling << 2.0, 0., 0., 0.,
					0., 2.0, 0., 0.,
					0., 0., 2.0, 0.,
					0., 0., 0., 1.;

				translation << 1., 0., 0., 9.5,
					0., 1., 0., 0.3,
					0., 0., 1., z + 1,
					0., 0., 0., 1.;

				fence->Set_Model_Matrix(translation * scaling * rotation);
			}
		}

		// West side fences (x = -9.5, from z = -10 to 10 with 2-grid gap in center)
		for (int z = -10; z <= -2; z += 2) // Lower section of west side
		{
			auto fence = Add_Obj_Mesh_Object_From_File("garden/fence1.obj", OpenGLColor(0.6f, 0.4f, 0.2f, 1.f));
			{
				Matrix4f rotation, scaling, translation;

				rotation << cos(DegreesToRadians(-90)), 0., sin(DegreesToRadians(-90)), 0.,
					0., 1., 0., 0.,
					-sin(DegreesToRadians(-90)), 0., cos(DegreesToRadians(-90)), 0.,
					0., 0., 0., 1.;

				scaling << 2.0, 0., 0., 0.,
					0., 2.0, 0., 0.,
					0., 0., 2.0, 0.,
					0., 0., 0., 1.;

				translation << 1., 0., 0., -9.5,
					0., 1., 0., 0.3,
					0., 0., 1., z + 1,
					0., 0., 0., 1.;

				fence->Set_Model_Matrix(translation * scaling * rotation);
			}
		}
		for (int z = 2; z <= 8; z += 2) // Upper section of west side
		{
			auto fence = Add_Obj_Mesh_Object_From_File("garden/fence1.obj", OpenGLColor(0.6f, 0.4f, 0.2f, 1.f));
			{
				Matrix4f rotation, scaling, translation;

				rotation << cos(DegreesToRadians(-90)), 0., sin(DegreesToRadians(-90)), 0.,
					0., 1., 0., 0.,
					-sin(DegreesToRadians(-90)), 0., cos(DegreesToRadians(-90)), 0.,
					0., 0., 0., 1.;

				scaling << 2.0, 0., 0., 0.,
					0., 2.0, 0., 0.,
					0., 0., 2.0, 0.,
					0., 0., 0., 1.;

				translation << 1., 0., 0., -9.5,
					0., 1., 0., 0.3,
					0., 0., 1., z + 1,
					0., 0., 0., 1.;

				fence->Set_Model_Matrix(translation * scaling * rotation);
			}
		}

		//// Garden Trees in Two Concentric Circles
		// Inner circle with garden_tree1
		int inner_trees = 12;
		for (int i = 0; i < inner_trees; i++)
		{
			auto tree = Add_Obj_Mesh_Object_From_File("garden/garden_tree1.obj", OpenGLColor(0.1f, 0.6f, 0.1f, 1.f));
			{
				Matrix4f scaling, translation;

				scaling << 1.2, 0., 0., 0.,
					0., 1.2, 0., 0.,
					0., 0., 1.2, 0.,
					0., 0., 0., 1.;

				float angle = i * (360.0f / inner_trees);
				translation << 1., 0., 0., 6. * cos(DegreesToRadians(angle)),
					0., 1., 0., 0.5,
					0., 0., 1., 6. * sin(DegreesToRadians(angle)),
					0., 0., 0., 1.;

				tree->Set_Model_Matrix(translation * scaling);
			}
		}

		// Outer circle with garden_tree2
		int outer_trees = 16;
		for (int i = 0; i < outer_trees; i++)
		{
			auto tree = Add_Obj_Mesh_Object_From_File("garden/garden_tree2.obj", OpenGLColor(0.2f, 0.7f, 0.2f, 1.f));
			{
				Matrix4f rotation, scaling, translation;

				// Slight random rotation for natural look
				float tree_rotation = i * 15.0f;
				rotation << cos(DegreesToRadians(tree_rotation)), 0., sin(DegreesToRadians(tree_rotation)), 0.,
					0., 1., 0., 0.,
					-sin(DegreesToRadians(tree_rotation)), 0., cos(DegreesToRadians(tree_rotation)), 0.,
					0., 0., 0., 1.;

				scaling << 1.4, 0., 0., 0.,
					0., 1.4, 0., 0.,
					0., 0., 1.4, 0.,
					0., 0., 0., 1.;

				float angle = i * (360.0f / outer_trees);
				translation << 1., 0., 0., 9. * cos(DegreesToRadians(angle)),
					0., 1., 0., 0.5,
					0., 0., 1., 9. * sin(DegreesToRadians(angle)),
					0., 0., 0., 1.;

				tree->Set_Model_Matrix(translation * scaling * rotation);
			}
		}

		//// Garden Seating Area - Tables and Chairs
		std::vector<Vector3> seating_positions = {
			Vector3(4, 0, 4),  // Southeast
			Vector3(-4, 0, 4), // Southwest
			Vector3(4, 0, -4), // Northeast
			Vector3(-4, 0, -4) // Northwest
		};

		for (int area = 0; area < 4; area++)
		{
			Vector3 center = seating_positions[area];

			// Add table
			auto table = Add_Obj_Mesh_Object_From_File("garden/table1.obj", OpenGLColor(0.6f, 0.4f, 0.2f, 1.f));
			{
				Matrix4f scaling, translation;

				scaling << 1.2, 0., 0., 0.,
					0., 1.2, 0., 0.,
					0., 0., 1.2, 0.,
					0., 0., 0., 1.;

				translation << 1., 0., 0., center[0],
					0., 1., 0., 0.4,
					0., 0., 1., center[2],
					0., 0., 0., 1.;

				table->Set_Model_Matrix(translation * scaling);
			}

			// Add 4 chairs around each table
			std::vector<Vector3> chair_offsets = {
				Vector3(1.5, 0, 0),	 // East
				Vector3(-1.5, 0, 0), // West
				Vector3(0, 0, 1.5),	 // South
				Vector3(0, 0, -1.5)	 // North
			};
			std::vector<float> chair_rotations = {-90, 90, 0, 180};

			for (int chair_idx = 0; chair_idx < 4; chair_idx++)
			{
				// Cycle through chair types
				std::string chair_file = "garden/chair" + std::to_string((chair_idx % 4) + 1) + ".obj";
				auto chair = Add_Obj_Mesh_Object_From_File(chair_file, OpenGLColor(0.5f, 0.3f, 0.1f, 1.f));
				{
					Matrix4f rotation, scaling, translation;

					rotation << cos(DegreesToRadians(chair_rotations[chair_idx])), 0., sin(DegreesToRadians(chair_rotations[chair_idx])), 0.,
						0., 1., 0., 0.,
						-sin(DegreesToRadians(chair_rotations[chair_idx])), 0., cos(DegreesToRadians(chair_rotations[chair_idx])), 0.,
						0., 0., 0., 1.;

					scaling << 1., 0., 0., 0.,
						0., 1., 0., 0.,
						0., 0., 1., 0.,
						0., 0., 0., 1.;

					Vector3 chair_pos = center + chair_offsets[chair_idx];
					translation << 1., 0., 0., chair_pos[0],
						0., 1., 0., 0.2,
						0., 0., 1., chair_pos[2],
						0., 0., 0., 1.;

					chair->Set_Model_Matrix(translation * scaling * rotation);
				}
			}
		}

		//// Flying Birds - 2 birds around each table (8 total)
		// Use the same seating positions as tables
		std::vector<Vector3> table_positions = {
			Vector3(4, 0, 4),  // Southeast
			Vector3(-4, 0, 4), // Southwest
			Vector3(4, 0, -4), // Northeast
			Vector3(-4, 0, -4) // Northwest
		};

		// Define distinct color palettes for birds
		std::vector<OpenGLColor> bird_colors = {
			OpenGLColor(1.0f, 0.2f, 0.2f, 1.f), // Bright Red
			OpenGLColor(0.2f, 0.8f, 0.2f, 1.f), // Bright Green
			OpenGLColor(0.2f, 0.3f, 1.0f, 1.f), // Bright Blue
			OpenGLColor(1.0f, 0.8f, 0.1f, 1.f), // Golden Yellow
			OpenGLColor(0.9f, 0.1f, 0.9f, 1.f), // Magenta
			OpenGLColor(1.0f, 0.5f, 0.0f, 1.f), // Orange
			OpenGLColor(0.6f, 0.3f, 0.8f, 1.f), // Purple
			OpenGLColor(0.1f, 0.9f, 0.9f, 1.f)	// Cyan
		};

		for (int table_idx = 0; table_idx < 4; table_idx++)
		{
			Vector3 table_center = table_positions[table_idx];

			// Face toward center of garden
			float angle_to_center = atan2(-table_center[2], -table_center[0]) * 180.0f / M_PI;

			// Bird 1 - perched on table (use first set of colors)
			auto bird1 = Add_Obj_Mesh_Object_From_File("bird.obj", bird_colors[table_idx]);
			{
				Matrix4f rotation, scaling, translation;

				rotation << cos(DegreesToRadians(angle_to_center)), 0., sin(DegreesToRadians(angle_to_center)), 0.,
					0., 1., 0., 0.,
					-sin(DegreesToRadians(angle_to_center)), 0., cos(DegreesToRadians(angle_to_center)), 0.,
					0., 0., 0., 1.;

				scaling << 0.8, 0., 0., 0., // Smaller bird for table
					0., 0.8, 0., 0.,
					0., 0., 0.8, 0.,
					0., 0., 0., 1.;

				// Position on table surface
				translation << 1., 0., 0., table_center[0],
					0., 1., 0., 1.1, // Table height + bird height
					0., 0., 1., table_center[2],
					0., 0., 0., 1.;

				bird1->Set_Model_Matrix(translation * scaling * rotation);
			}

			// Bird 2 - near table (use second set of colors)
			auto bird2 = Add_Obj_Mesh_Object_From_File("bird.obj", bird_colors[table_idx + 4]);
			{
				Matrix4f rotation, scaling, translation;

				// Different orientation for variety
				float bird_angle = angle_to_center + 45.0f + (table_idx * 30.0f);
				rotation << cos(DegreesToRadians(bird_angle)), 0., sin(DegreesToRadians(bird_angle)), 0.,
					0., 1., 0., 0.,
					-sin(DegreesToRadians(bird_angle)), 0., cos(DegreesToRadians(bird_angle)), 0.,
					0., 0., 0., 1.;

				scaling << 0.9, 0., 0., 0.,
					0., 0.9, 0., 0.,
					0., 0., 0.9, 0.,
					0., 0., 0., 1.;

				// Position near table but offset
				Vector3 offset_positions[4] = {
					Vector3(1.0, 0, 1.2),  // Southeast table offset
					Vector3(-1.0, 0, 1.2), // Southwest table offset
					Vector3(1.0, 0, -1.2), // Northeast table offset
					Vector3(-1.0, 0, -1.2) // Northwest table offset
				};

				Vector3 bird_pos = table_center + offset_positions[table_idx];
				translation << 1., 0., 0., bird_pos[0],
					0., 1., 0., 0.3, // Ground level + small height
					0., 0., 1., bird_pos[2],
					0., 0., 0., 1.;

				bird2->Set_Model_Matrix(translation * scaling * rotation);
			}
		}

		//// Decorative Garden Lights
		std::vector<Vector3> light_positions = {
			Vector3(3, 0, 0),  // East
			Vector3(-3, 0, 0), // West
			Vector3(0, 0, 3),  // South
			Vector3(0, 0, -3), // North
			Vector3(7, 0, 7),  // Southeast corner
			Vector3(-7, 0, 7), // Southwest corner
			Vector3(7, 0, -7), // Northeast corner
			Vector3(-7, 0, -7) // Northwest corner
		};

		for (int i = 0; i < 8; i++)
		{
			// Alternate between light1 and light2
			std::string light_file = "garden/light" + std::to_string((i % 2) + 1) + ".obj";
			auto light = Add_Obj_Mesh_Object_From_File(light_file, OpenGLColor(1.0f, 1.0f, 0.8f, 1.f));
			{
				Matrix4f scaling, translation;

				scaling << 1.5, 0., 0., 0.,
					0., 1.5, 0., 0.,
					0., 0., 1.5, 0.,
					0., 0., 0., 1.;

				translation << 1., 0., 0., light_positions[i][0],
					0., 1., 0., 0.8,
					0., 0., 1., light_positions[i][2],
					0., 0., 0., 1.;

				light->Set_Model_Matrix(translation * scaling);
			}
		}

		//// Decorative Vases with Different Arrangements
		std::vector<Vector3> vase_positions = {
			Vector3(2, 0, 2), Vector3(-2, 0, 2),   // Front vases
			Vector3(2, 0, -2), Vector3(-2, 0, -2), // Back vases
			Vector3(5, 0, 0), Vector3(-5, 0, 0)	   // Side vases
		};

		for (int i = 0; i < 6; i++)
		{
			// Alternate between vase1 and vase2
			std::string vase_file = "garden/vase" + std::to_string((i % 2) + 1) + ".obj";
			auto vase = Add_Obj_Mesh_Object_From_File(vase_file, OpenGLColor(0.8f, 0.6f, 0.4f, 1.f));
			{
				Matrix4f rotation, scaling, translation;

				// Random rotation for variety
				float angle = i * 33.0f;
				rotation << cos(DegreesToRadians(angle)), 0., sin(DegreesToRadians(angle)), 0.,
					0., 1., 0., 0.,
					-sin(DegreesToRadians(angle)), 0., cos(DegreesToRadians(angle)), 0.,
					0., 0., 0., 1.;

				scaling << 1.2, 0., 0., 0.,
					0., 1.2, 0., 0.,
					0., 0., 1.2, 0.,
					0., 0., 0., 1.;

				translation << 1., 0., 0., vase_positions[i][0],
					0., 1., 0., 0.3,
					0., 0., 1., vase_positions[i][2],
					0., 0., 0., 1.;

				vase->Set_Model_Matrix(translation * scaling * rotation);
			}
		}

		//// Grass Patches for Natural Ground Cover - Grid Pattern
		// Create a 20x20 grid of grass patches covering the ground
		for (int x = -4; x < 4; x++)
		{
			for (int z = -4; z < 4; z++)
			{
				// Skip the center area around the fountain to avoid overlap
				if ((x < 1 && x > -2) && (z < 1 && z > -2))
					continue;

				auto grass = Add_Obj_Mesh_Object_From_File("garden/grass3.obj", OpenGLColor(0.2f, 0.8f, 0.1f, 1.f));
				{
					Matrix4f rotation, scaling, translation;

					// Slight size variation for natural look
					float scale = 1.2;
					scaling << scale, 0., 0., 0.,
						0., scale, 0., 0.,
						0., 0., scale, 0.,
						0., 0., 0., 1.;

					// Position at each grid cell (1 unit per cell)
					translation << 1., 0., 0., x + 0.5f, // Offset by 0.5 to center in grid cell
						0., 1., 0., 0.01f,				 // Slightly above ground to avoid z-fighting
						0., 0., 1., z + 0.5f,
						0., 0., 0., 1.;

					grass->Set_Model_Matrix(translation * scaling);
				}
			}
		}

		//// Flying Birds - Multiple birds at different positions and orientations
		// std::vector<Vector3> bird_positions = {
		// 	Vector3(8, 4, 3),	// High right
		// 	Vector3(-6, 3, -2), // Left side
		// 	Vector3(2, 5, -8),	// Back high
		// 	Vector3(-9, 2, 5),	// Far left
		// 	Vector3(4, 6, 0),	// High center-right
		// 	Vector3(-3, 3, 7),	// Front left
		// 	Vector3(10, 4, -4), // Far right
		// 	Vector3(0, 7, -6)	// High back center
		// };

		// std::vector<float> bird_rotations = {45, -30, 180, 90, -45, 135, 0, -90};		   // Different orientations
		// std::vector<float> bird_scales = {1.0f, 0.8f, 1.2f, 0.9f, 1.1f, 0.7f, 1.3f, 1.0f}; // Size variety

		// for (int i = 0; i < bird_positions.size(); i++)
		// {
		// 	auto bird = Add_Obj_Mesh_Object_From_File("bird.obj",
		// 											  OpenGLColor(0.8f + 0.2f * (i % 2), 0.2f + 0.1f * (i % 3), 0.1f, 1.f)); // Varied colors
		// 	{
		// 		Matrix4f rotation, scaling, translation;

		// 		// Rotation around Y-axis for flight direction
		// 		rotation << cos(DegreesToRadians(bird_rotations[i])), 0., sin(DegreesToRadians(bird_rotations[i])), 0.,
		// 			0., 1., 0., 0.,
		// 			-sin(DegreesToRadians(bird_rotations[i])), 0., cos(DegreesToRadians(bird_rotations[i])), 0.,
		// 			0., 0., 0., 1.;

		// 		// Varied sizes for natural look
		// 		float scale = bird_scales[i];
		// 		scaling << scale, 0., 0., 0.,
		// 			0., scale, 0., 0.,
		// 			0., 0., scale, 0.,
		// 			0., 0., 0., 1.;

		// 		translation << 1., 0., 0., bird_positions[i][0],
		// 			0., 1., 0., bird_positions[i][1],
		// 			0., 0., 1., bird_positions[i][2],
		// 			0., 0., 0., 1.;

		// 		bird->Set_Model_Matrix(translation * scaling * rotation);
		// 	}
		// }

		/* Your implementation ends. */
	}

	//////////////////////////////////////////////////////////////
	//// The following functions are auxiliary functions to add mesh objects into the scene
	//////////////////////////////////////////////////////////////

	//// This function adds a mesh object from an obj file
	int Add_Obj_Mesh_Object(std::string obj_file_name)
	{
		auto mesh_obj = Add_Interactive_Object<OpenGLTriangleMesh>();

		Array<std::shared_ptr<TriangleMesh<3>>> meshes;
		Obj::Read_From_Obj_File(obj_file_name, meshes);
		mesh_obj->mesh = *meshes[0];
		std::cout << "load tri_mesh from obj file, #vtx: " << mesh_obj->mesh.Vertices().size() << ", #ele: " << mesh_obj->mesh.Elements().size() << std::endl;

		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size() - 1;
	}

	//// This function adds a mesh object from an .obj file and allows the user to specify its model matrix
	OpenGLTriangleMesh *Add_Obj_Mesh_Object_From_File(std::string file_name, OpenGLColor color = OpenGLColor::White())
	{
		int obj_idx = Add_Obj_Mesh_Object(file_name);
		auto obj = mesh_object_array[obj_idx];
		obj->color = color; // set color

		obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("a3_shading"));
		Set_Polygon_Mode(obj, PolygonMode::Fill);
		Set_Shading_Mode(obj, ShadingMode::A2);
		obj->Set_Data_Refreshed();
		obj->Initialize();

		return obj;
	}
	//// This function adds a cube with a specified length and color
	OpenGLTriangleMesh *Add_Cube(float length = 1, OpenGLColor color = OpenGLColor::White())
	{
		auto obj = Add_Interactive_Object<OpenGLTriangleMesh>();
		mesh_object_array.push_back(obj);
		obj->color = color; // set color
		// set up vertices and elements
		std::vector<Vector3> vertices{Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(1, 1, 0), Vector3(0, 0, 1), Vector3(1, 0, 1), Vector3(0, 1, 1), Vector3(1, 1, 1)};
		std::vector<Vector3i> elements{Vector3i(4, 5, 7), Vector3i(4, 7, 6),
									   Vector3i(5, 1, 7), Vector3i(7, 1, 3),
									   Vector3i(2, 3, 1), Vector3i(0, 2, 1),
									   Vector3i(6, 2, 4), Vector3i(2, 0, 4),
									   Vector3i(2, 6, 3), Vector3i(6, 7, 3),
									   Vector3i(0, 1, 4), Vector3i(1, 5, 4)};
		for (auto &v3 : vertices)
		{
			v3 -= Vector3(0.5, 0.5, 0.5);
			v3 *= length;
		};
		obj->mesh.Vertices() = vertices;
		obj->mesh.Elements() = elements;

		obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("a3_shading"));
		Set_Polygon_Mode(obj, PolygonMode::Fill);
		Set_Shading_Mode(obj, ShadingMode::A2);
		obj->Set_Data_Refreshed();
		obj->Initialize();

		return obj;
	}

	//// This function adds an axis with a specified color
	void Add_Coord(std::vector<Vector3> pts, OpenGLColor color)
	{
		auto axis = Add_Interactive_Object<OpenGLSegmentMesh>();
		coords.push_back(axis);
		Set_Polygon_Mode(axis, PolygonMode::Fill);
		Set_Shading_Mode(axis, ShadingMode::None);
		auto line_mesh = &axis->mesh;
		line_mesh->Vertices() = pts;
		line_mesh->Elements() = {Vector2i(0, 1)};
		Set_Line_Width(axis, 2.5f);
		Set_Color(axis, color);
		axis->Set_Data_Refreshed();
		axis->Initialize();
	}

	//// This function adds a wireframe ground
	void Add_Ground()
	{
		floor = Add_Interactive_Object<OpenGLSegmentMesh>();
		Set_Polygon_Mode(floor, PolygonMode::Fill);
		Set_Shading_Mode(floor, ShadingMode::None);
		auto line_mesh = &floor->mesh;
		std::vector<Vector3> pts;
		std::vector<Vector2i> eles;
		for (int i = -10; i <= 10; i++)
		{
			pts.push_back(Vector3(i, 0, -10));
			pts.push_back(Vector3(i, 0, 10));
			pts.push_back(Vector3(-10, 0, i));
			pts.push_back(Vector3(10, 0, i));
		}
		for (int i = 0; i < pts.size(); i += 2)
		{
			eles.push_back(Vector2i(i, i + 1));
		}
		line_mesh->Vertices() = pts;
		line_mesh->Elements() = eles;
		Set_Line_Width(floor, 3.f);
		Set_Color(floor, OpenGLColor(0, 0, 0, 1));
		floor->Set_Data_Refreshed();
		floor->Initialize();
	}

	//// This function adds a parabola trajectory
	void Add_Arc_Trajectory()
	{
		float ux = 5, uy = 9.8, g = 9.8;
		std::vector<Vector3> pts;
		std::vector<Vector2i> eles;
		for (float t = 0; t <= 2; t += 0.02)
		{
			float x = ux * t - 5;
			float y = uy * t - 0.5 * g * t * t;
			pts.push_back(Vector3(x, y, 0));
		}
		for (int i = 0; i < pts.size() - 1; i++)
		{
			eles.push_back(Vector2i(i, i + 1));
		}
		auto traj = Add_Interactive_Object<OpenGLSegmentMesh>();
		trajectory.push_back(traj);
		Set_Polygon_Mode(traj, PolygonMode::Fill);
		Set_Shading_Mode(traj, ShadingMode::None);
		auto line_mesh = &traj->mesh;
		line_mesh->Vertices() = pts;
		line_mesh->Elements() = eles;
		Set_Line_Width(traj, 2.f);
		Set_Color(traj, OpenGLColor(0, 0, 1, 1));
		traj->Set_Data_Refreshed();
		traj->Initialize();
	}

	void Init_Bird_Vertices(OpenGLTriangleMesh *obj)
	{
		std::vector<Vector3> &vertices = obj->mesh.Vertices();
		for (auto &v : vertices)
		{
			v += Vector3(-8.35, 0, 0);
			auto x = v.x();
			v.x() = v.z();
			v.z() = -x;
			v /= 10.f;
		}
	}

	//// Go to next frame
	virtual void Toggle_Next_Frame()
	{
		for (auto &mesh_obj : mesh_object_array)
		{
			mesh_obj->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);
		}
		OpenGLViewer::Toggle_Next_Frame();
	}

	virtual void Run()
	{
		OpenGLViewer::Run();
	}
};

int main(int argc, char *argv[])
{
	MyDriver driver;
	driver.Initialize();
	driver.Run();
}

#endif