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
	root = new GameObject();
	root->SetName("Root");

	return true;
}

update_status ModuleEditor::Update(float dt)
{

	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	delete root;
	return true;
}

void ModuleEditor::Render()
{
	root->Update();
}

void ModuleEditor::ShowEditor()
{
	root->OnEditor();
}

GameObject * ModuleEditor::GetRoot()
{
	return root;
}

GameObject * ModuleEditor::CreateNewGameObject(const char * path)
{
	GameObject* ret = App->geometryloader->LoadGameObject(path);
	if (ret != nullptr)
	{
		root->DeleteChilds();
		root->AddChild(ret);
		//App->camera->CenterToGO(ret);
	}
	else
	{
		LOG("Couldn't load .fbx file!");
	}

	return ret;
}
