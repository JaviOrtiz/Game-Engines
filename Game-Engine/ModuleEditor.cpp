#include "Application.h"
#include "ModuleEditor.h"
#include "GeometryLoader.h"
#include "ModuleRenderer3D.h"
#include "mmgr\mmgr.h"
#include "CompCamera.h"
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
	CompCamera* cam = new CompCamera();
	root->AddComponent(cam);
	Quadroot = new Octree(AABB(float3(-100, -20, -100), float3(100, 120, 100)));

	return true;
}

update_status ModuleEditor::Update(float dt)
{
	
	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	delete Quadroot;
	delete root;
	return true;
}

void ModuleEditor::Render()
{
	root->Update();
	Quadroot->DrawDebug(Orange);

	if (selected != nullptr)
	{
		selected->ShowInspector();
	}

}

void ModuleEditor::ShowEditor()
{
	root->OnEditor();
}

GameObject * ModuleEditor::GetRoot()
{
	return root;
}

Octree * ModuleEditor::GetQuadtree()
{
	return Quadroot;
}

GameObject * ModuleEditor::CreateNewGameObject(const char * path)
{
	GameObject* ret = App->geometryloader->LoadGameObject(path);
	if (ret != nullptr)
	{
		//root->DeleteChilds();
		root->AddChild(ret);
		//App->camera->CenterToGO(ret);
	}
	else
	{
		LOG("Couldn't load .fbx file!");
	}

	return ret;
}

void ModuleEditor::SelectGameObject(GameObject * Selected)
{
	if (this->selected != Selected)
	{
		if (this->selected != nullptr)
		{
			this->selected->selected = false;
		}
		Selected->selected = true;
		this->selected = Selected;
	}
	else
	{
		this->selected->selected = false;
		this->selected = nullptr;
	}
}
