#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

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

public:
	Cube s, Flooor, door6_1, door6_2, door6_3, door6_4, sensor1, sensor3, sensor5, sensor6;
	Cube ring1, ring2, ring3, ring4, ring5, ring6, ring7, ring8, ring9, ring10, ring11, ring12;
	Cylinder start, door1l, door1r , door2palo,door5palo, door2circle, door4_1, door4_2, door4_3 , door7_1, door7_2, door7_3 , sensor2, sensor4, end, sensor7;
	Cube ring13, ring14, ring15, ring16, ring17, ring18, ring19, ring20, ring21, ring22, ring23, ring24;
	
	PhysBody3D* map[40];
	PhysBody3D* Door1;
	PhysBody3D* Door2;
	PhysBody3D* Door3;
	PhysBody3D* Door4;
	PhysBody3D* Door5;
	PhysBody3D* Door6;
	PhysBody3D* Door7;
	PhysBody3D* sensor;
	uint victory, musicc, checkpoint;
	uint wasted;
	uint time = 0;
	bool youlost = false;
	float timepassed = 0;
	bool started = false, finished = false, finished2 = false;
	bool check1 = false, check2 = false, check3 = false, check4 = false, check5 = false, check6 = false;
	bool checks1 = false, checks2 = false, checks3 = false , checks4 = false , checks5 = false , checks6 = false;
	bool lost = false;
	void Restart();
	void CreateMap();
	void Lost();
};
