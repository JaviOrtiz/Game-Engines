#include "CompTransform.h"
#include "GameObject.h"
#include "ImGui\imgui.h"
#include "Application.h"


CompTransform::CompTransform(float3 pos, float3 scale, Quat rot, ComponentType type) : Component(Component_Transform), position(pos), movedposition(pos), scale(scale), rotation(rot)
{
	name = "Transform";
	needToMove = false;
	positionmatrix = new float[16];
	float3 aulerrot = rotation.ToEulerXYZ().Abs();
	rotation.x = aulerrot.x;
	rotation.y = aulerrot.y;
	rotation.z = aulerrot.z;
	//this->myGO;
	UpdatePositionMatrix();
}

CompTransform::~CompTransform()
{
	delete[] positionmatrix;
}

void CompTransform::Update()
{
	UpdatePositionMatrix();
}

void CompTransform::UpdatePositionMatrix()
{
	glm::mat4 posmat = glm::translate(glm::mat4(), glm::vec3(position.x, position.y, position.z));

	//glm::mat4 scalemat = glm::scale(scale.x, scale.y, scale.z);
	glm::vec3 myRotationAxisX(1, 0, 0);	glm::vec3 myRotationAxisY(0, 1, 0);		glm::vec3 myRotationAxisZ(0, 0, 1);
	glm::mat4 rotmat = glm::rotate(rotation.x, myRotationAxisX)*glm::rotate(rotation.y, myRotationAxisY)*glm::rotate(rotation.z, myRotationAxisZ);

	glm::mat4 finalmat = posmat*rotmat;
	const float *pSource2 = (const float*)glm::value_ptr(finalmat);
	for (int i = 0; i < 16; ++i) {
		positionmatrix[i] = pSource2[i];
		//App->Console.AddLog("%f", positionmatrix[i]);
	}
	transformmatrix = finalmat;
	if (myGO != nullptr)
	{
		GameObject* GO = myGO->GetParent();
		while (GO != nullptr)
		{
			CompTransform* transf = dynamic_cast<CompTransform*>(GO->FindComponent(Component_Transform));
			if (transf != nullptr)
			{
				transformmatrix *= transf->transformmatrix;
			}
			GO = GO->GetParent();
		}
	}
}

float * CompTransform::GetPositionMatrix()
{
	return positionmatrix;
}

void CompTransform::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		ImGui::Text("Position:");
		if (ImGui::SliderFloat("X", &position.x, -200, 200))
		{
		}
		if (ImGui::SliderFloat("Y", &position.y, -100, 100))
		{
		}
		if (ImGui::SliderFloat("Z", &position.z, -200, 200))
		{
		}

		ImGui::TextColored({0.7f, 1.0f, 1.0f,1.0f},"Scale: X=%.2f, Y=%.2f, Z=%.2f", scale.x, scale.y, scale.z);
		ImGui::TextColored({ 0.7f, 1.0f, 1.0f ,1.0f}, "Rotation: X=%.2f, Y=%.2f, Z=%.2f, W=%.2f", rotation.x, rotation.y, rotation.z, rotation.w);
		ImGui::TreePop();
	}
}