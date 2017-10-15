#include "Application.h"
#include "ModuleEditor.h"
#include "GeometryLoader.h"
#include "ModuleRenderer3D.h"
#include "mmgr\mmgr.h"
ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Editor";
}

ModuleEditor::~ModuleEditor()
{
	
}

bool ModuleEditor::Init()
{
	return true;
}

bool ModuleEditor::Start()
{
	return true;
}

update_status ModuleEditor::Update(float dt)
{
	for (std::vector<Geometry*>::iterator it = App->editor->geometryvector.begin(); it != App->editor->geometryvector.end(); ++it)
	{
		(**it).ImGuiDraw();
	}

	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	ClearGeometryVector();
	return true;
}

void ModuleEditor::ClearGeometryVector()
{
	while (!geometryvector.empty()) 
	{
		delete geometryvector.back();
		geometryvector.pop_back();
	}
		geometryvector.clear();
}
