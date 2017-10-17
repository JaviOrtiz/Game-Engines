#include "ComponentTransform.h"
#include "GameObject.h"

#include "ImGui\imgui.h"

CompTransform::CompTransform(float3 pos, float3 scale, Quat rot, ComponentType type) : Component(Component_Transform), position(pos), scale(scale), rotation(rot)
{
	name = "Transform";
	needToMove = false;
}

CompTransform::~CompTransform()
{}

void CompTransform::Update()
{
	if (needToMove && myGO != nullptr)
	{
		myGO->Move(position);
		needToMove = false;
	}
}

void CompTransform::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		ImGui::Text("Position:");
		if (ImGui::SliderFloat("X", &position.x, -200, 200))
		{
			needToMove = true;
		}
		if (ImGui::SliderFloat("Y", &position.y, -100, 100))
		{
			needToMove = true;
		}
		if (ImGui::SliderFloat("Z", &position.z, -200, 200))
		{
			needToMove = true;
		}

		ImGui::Text("Scale: X=%.2f, Y=%.2f, Z=%.2f", scale.x, scale.y, scale.z);
		ImGui::Text("Rotation: X=%.2f, Y=%.2f, Z=%.2f, W=%.2f", rotation.x, rotation.y, rotation.z, rotation.w);
		ImGui::TreePop();
	}
}