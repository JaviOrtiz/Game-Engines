#include "Application.h"
#include "ModuleEditor.h"
#include "GeometryLoader.h"
#include "ModuleRenderer3D.h"

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
	return false;
}

void ModuleEditor::ClearGeometryVector()
{
	geometryvector.clear();
}
