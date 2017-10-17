#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "MathGeoLib.h"
#include "GeometryLoader.h"
#include "GameObject.h"

class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enable = true);
	~ModuleEditor();

	bool Init();
	bool Start();
	update_status Update(float dt);

	bool CleanUp() override;

	void Render();
	void ShowEditor();

	GameObject* GetRoot();

	GameObject* CreateNewGameObject(const char* path);
public:
	
	GameObject* root;
};
