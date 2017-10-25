#include "Application.h"
#include "Quadtree.h"
#include "GameObject.h"
#include "CompMesh.h"
#include "MathGeoLib\MathGeoLib.h"
#include "MathGeoLib\Geometry\AABB.h"

#define MAX_OBJECTS 4
#define MIN_SIZE 1

OctreeNode::OctreeNode(const AABB& box) : box(box)
{
	parent = nullptr;
	for (int i = 0; i < 8; i++)
	{
		childs[i] = nullptr;
	}
}

OctreeNode::~OctreeNode()
{
	for (int i = 0; i < 8; i++)
	{
		delete childs[i];
		childs[i] = nullptr;
	}
}

bool OctreeNode::IsLeaf() const
{
	if (childs[0] == nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void OctreeNode::Insert(GameObject* toInsert)
{
	if (objects.size() < MAX_OBJECTS || box.Size().x <= MIN_SIZE)
	{
		objects.push_back(toInsert);
	}
	else
	{
		if (IsLeaf() == true)
		{
			CreateChilds();
		}
		RedistributeChilds();
	}
}

//TODO: Change function to an iterative one.
void OctreeNode::Remove(GameObject* toRemove)
{
	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		if ((*it) == toRemove)
		{
			objects.erase(it);
			return;
		}
	}
	if (IsLeaf() == false)
	{
		for (int i = 0; i < 8; i++)
		{
			childs[i]->Remove(toRemove);
		}
	}
}

void OctreeNode::CreateChilds()
{
	AABB childBox;

	float3 boxMin = this->box.minPoint;
	float3 boxMax = this->box.maxPoint;
	float3 boxSize = this->box.Size();

	//Child 0
	childBox.minPoint = float3(boxMin.x, boxMin.y, boxMin.z);
	childBox.maxPoint = float3(boxMax.x - boxSize.x * 0.5f, boxMax.y - boxSize.y *0.5, boxMax.z - boxSize.z * 0.5f);

	childs[0] = new OctreeNode(childBox);

	//Child 1
	childBox.minPoint = float3(boxMin.x + boxSize.x * 0.5f, boxMin.y, boxMin.z);
	childBox.maxPoint = float3(boxMax.x, boxMax.y - boxSize.y *0.5, boxMax.z - boxSize.z * 0.5f);

	childs[1] = new OctreeNode(childBox);

	//Child 2
	childBox.minPoint = float3(boxMin.x, boxMin.y, boxMin.z + boxSize.z * 0.5f);
	childBox.maxPoint = float3(boxMax.x - boxSize.x * 0.5f, boxMax.y - boxSize.y *0.5, boxMax.z);

	childs[2] = new OctreeNode(childBox);

	//Child 3
	childBox.minPoint = float3(boxMax.x - boxSize.x * 0.5f, boxMin.y, boxMax.z - boxSize.z * 0.5f);
	childBox.maxPoint = float3(boxMax.x, boxMax.y - boxSize.y *0.5, boxMax.z);

	childs[3] = new OctreeNode(childBox);

	//Child 4
	childBox.minPoint = float3(boxMin.x, boxMax.y - boxSize.y *0.5, boxMin.z);
	childBox.maxPoint = float3(boxMax.x - boxSize.x * 0.5f, boxMax.y, boxMax.z - boxSize.z * 0.5f);

	childs[4] = new OctreeNode(childBox);

	//Child 5
	childBox.minPoint = float3(boxMin.x + boxSize.x * 0.5f, boxMax.y - boxSize.y *0.5, boxMin.z);
	childBox.maxPoint = float3(boxMax.x, boxMax.y, boxMax.z - boxSize.z * 0.5f);

	childs[5] = new OctreeNode(childBox);

	//Child 6
	childBox.minPoint = float3(boxMin.x, boxMax.y - boxSize.y *0.5, boxMin.z + boxSize.z * 0.5f);
	childBox.maxPoint = float3(boxMax.x - boxSize.x * 0.5f, boxMax.y , boxMax.z);

	childs[6] = new OctreeNode(childBox);

	//Child 7
	childBox.minPoint = float3(boxMax.x - boxSize.x * 0.5f, boxMax.y - boxSize.y *0.5, boxMax.z - boxSize.z * 0.5f);
	childBox.maxPoint = float3(boxMax.x, boxMax.y, boxMax.z);

	childs[7] = new OctreeNode(childBox);
	for (int i = 0; i < 8; i++)
	{
		childs[i]->parent = this;
	}
}

void OctreeNode::RedistributeChilds()
{
	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end();)
	{
		for (int i = 0; i < 8; i++)
		{
			CompMesh* tmp = (CompMesh*)(*it)->FindComponent(Component_Mesh);
			if (tmp != nullptr)
			{
				if (childs[i]->box.Intersects(tmp->enclosingBox))
				{
					childs[i]->Insert((*it));
				}
			}
		}
		it = objects.erase(it);
	}
}

void OctreeNode::DrawDebug(Color color) const
{
	if (IsLeaf() == true)
	{
		box.DrawDebug(color);
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			childs[i]->DrawDebug(color);
		}
	}
}

template<typename TYPE>
void OctreeNode::CollectIntersections(std::vector<GameObject*>& objects, const TYPE & primitive) const
{
	if (primitive.Intersects(box))
	{
		for (std::list<GameObject*>::const_iterator it = this->objects.begin(); it != this->objects.end(); ++it)
		{
			ComponentMesh* tmp = (ComponentMesh*)(*it)->FindComponent(Component_Mesh);
			if (tmp != nullptr)
			{
				if (primitive.Intersects(tmp->enclosingBox))
				{
					objects.push_back(*it);
				}
			}
		}
		for (int i = 0; i < 8; ++i)
			if (childs[i] != nullptr) childs[i]->CollectIntersections(objects, primitive);
	}
}

Octree::Octree(const AABB& box)
{
	root = new OctreeNode(box);
}

Octree::~Octree()
{
	Clear();
}

void Octree::Insert(GameObject* toInsert)
{
	CompMesh* tmp = (CompMesh*)toInsert->FindComponent(Component_Mesh);
	if (root != nullptr && root->box.Contains(tmp->enclosingBox))
	{
		root->Insert(toInsert);
	}
}

void Octree::Remove(GameObject* toRemove)
{
	CompMesh* tmp = (CompMesh*)toRemove->FindComponent(Component_Mesh);
	if (root != nullptr && root->box.Contains(tmp->enclosingBox))
	{
		root->Remove(toRemove);
	}
}

void Octree::Clear()
{
	delete root;
	root = nullptr;
}

void Octree::DrawDebug(Color color) const
{
	root->DrawDebug(color);
}
