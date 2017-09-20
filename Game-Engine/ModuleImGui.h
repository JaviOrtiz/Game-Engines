#pragma once

#include "Module.h"
#include "Globals.h"
#include "imguiconsole.h"

class ModuleImGui : public Module
{
public:
	ModuleImGui(Application* app, bool start_enabled = true);
	~ModuleImGui();
	bool Init();

	bool Start();

	//PreUpdate
	update_status PreUpdate(float dt);

	//Update
	update_status Update(float dt);

	//PostUpdate
	update_status PostUpdate(float dt);

	bool CleanUp();

private:
	int	First = 0;
	int	Last = 100;
	LCG		Random_Num_1;
	LCG		Random_Num_2;
	float	Random_Num_Int = 0;
	float	Random_Num_Float = 0;
	bool	Showtest = false;
	bool    Showconsole = false;
	bool	Homework = false;



	vec Temp = { 0,0,0 };
	vec Temp1 = { 1,0,0 };
	vec Temp2 = { 3,0,0 };
	Sphere* S1Temp = new Sphere(Temp, 1);
	Sphere* S2Temp = new Sphere(Temp1, 1);
	Sphere* S3Temp = new Sphere(Temp2, 1);
	bool SphereTest = false;

public:
	ExampleAppConsole Console;


};

