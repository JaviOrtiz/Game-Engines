#include "Application.h"
#include "ModuleEditor.h"
#include "GeometryLoader.h"
#include "ModuleRenderer3D.h"
#include "mmgr\mmgr.h"
#include "CompCamera.h"
#include "CompMesh.h"
#include "CompTransform.h"
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
		delete Quadroot;
		root->AddChild(ret);
		AABB Enclosing_Box;
		Enclosing_Box.SetNegativeInfinity();
		for (std::vector<GameObject*>::const_iterator tmp = Static_Vector.begin(); tmp != Static_Vector.end(); tmp++)
		{
			CompMesh* tmpmesh = (CompMesh*)(*tmp)->FindComponent(Component_Mesh);
			CompTransform* tmptransf = (CompTransform*)(*tmp)->FindComponent(Component_Transform);
			//Enclosing_Box.TransformAsAABB(tmptransf->GetTransMatrix());
			Enclosing_Box.Enclose((float3*)tmpmesh->vertices, tmpmesh->numVertices);

		}

		Quadroot = new Octree(Enclosing_Box);
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
