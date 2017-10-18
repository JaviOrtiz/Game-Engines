#include "CompTransform.h"
#include "GameObject.h"

#include "ImGui\imgui.h"

CompTransform::CompTransform(float3 pos, float3 scale, Quat rot, ComponentType type) : Component(Component_Transform), position(pos), movedposition(pos), scale(scale), rotation(rot)
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
		myGO->Move(position, movedposition);
		needToMove = false;
		position = movedposition;
	}
}

void CompTransform::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		ImGui::Text("Position:");
		if (ImGui::SliderFloat("X", &movedposition.x, -200, 200))
		{
			needToMove = true;
		}
		if (ImGui::SliderFloat("Y", &movedposition.y, -100, 100))
		{
			needToMove = true;
		}
		if (ImGui::SliderFloat("Z", &movedposition.z, -200, 200))
		{
			needToMove = true;
		}

		ImGui::TextColored({0.7f, 1.0f, 1.0f,1.0f},"Scale: X=%.2f, Y=%.2f, Z=%.2f", scale.x, scale.y, scale.z);
		ImGui::TextColored({ 0.7f, 1.0f, 1.0f ,1.0f}, "Rotation: X=%.2f, Y=%.2f, Z=%.2f, W=%.2f", rotation.x, rotation.y, rotation.z, rotation.w);
		ImGui::TreePop();
	}
}