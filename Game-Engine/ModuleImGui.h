#pragma once

#include "Module.h"
#include "Globals.h"



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
	LCG Random_Num_1;
	LCG Random_Num_2;
	float Random_Num_Int = 0;
	float Random_Num_Float = 0;

	bool showtest = false;
};
