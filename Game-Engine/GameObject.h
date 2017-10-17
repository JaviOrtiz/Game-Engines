#pragma once
#include "Component.h"
#include "MathGeoLib\Math\float3.h"

#include <string>
#include <vector>

class GameObject
{
public:
	GameObject(GameObject* parent = nullptr);
	~GameObject();

	void Update();
	void AddChild(GameObject* child);
	void DeleteChilds();
	void AddComponent(Component* component);
	void SetName(const char* name);
	void OnEditor();

	void Move(float3 newPos);

	Component* FindComponent(ComponentType type) const;

	std::vector<Component*> components;
	std::vector<GameObject*> childs;
protected:
	GameObject* parent;
	std::string name;
};