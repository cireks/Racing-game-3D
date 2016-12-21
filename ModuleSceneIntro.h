#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	void CreateCircuit();

	int num_checkpoints;

public:
	p2List<PhysBody3D*> doors;
	p2List<PhysBody3D*> ramps;
	p2List<PhysBody3D*> balls;
	

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;
	
	PhysBody3D* meta;
	PhysBody3D* checkpoint1;
	PhysBody3D* checkpoint2;

	PhysBody3D* speedboost1;
	PhysBody3D* speedboost2;

	Timer timer;
	Timer onetlap;
	Timer seclap;
	Timer trilap;
	int lastlap = 0;
};
