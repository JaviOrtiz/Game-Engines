#include "CompTransform.h"
#include "GameObject.h"
#include "ImGui\imgui.h"
#include "Application.h"


CompTransform::CompTransform(float3 pos, float3 scale, Quat rot, ComponentType type) : Component(Component_Transform), position(pos), movedposition(pos), scale(scale), rotation(rot)
{
	name = "Transform";
	needToMove = false;
	positionmatrix = new float[16];
	float3 eulerrot = rotation.ToEulerXYZ().Abs() * RADTODEG;
	rotation.x = eulerrot.x;
	rotation.y = eulerrot.y;
	rotation.z = eulerrot.z;
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
	glm::vec3 scalevec = { scale.x, scale.y, scale.z };
	glm::mat4 scalemat = glm::scale(scalevec);
	glm::vec3 myRotationAxisX(1, 0, 0);	glm::vec3 myRotationAxisY(0, 1, 0);		glm::vec3 myRotationAxisZ(0, 0, 1);
	rotation.x *= DEGTORAD;
	rotation.y *= DEGTORAD;
	rotation.z *= DEGTORAD;
	glm::mat4 rotmat = glm::rotate(rotation.x, myRotationAxisX)*glm::rotate(rotation.y, myRotationAxisY)*glm::rotate(rotation.z, myRotationAxisZ);
	rotation.x *= RADTODEG;
	rotation.y *= RADTODEG;
	rotation.z *= RADTODEG;
	glm::mat4 finalmat = posmat*rotmat*scalemat;

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
	const float *pSource2 = (const float*)glm::value_ptr(transformmatrix);
	for (int i = 0; i < 16; ++i) {
		positionmatrix[i] = pSource2[i];

	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
		{
			TransMatrix[i][j] = positionmatrix[i + i*j];
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
		if (ImGui::SliderFloat("rotationX", &rotation.x, 0, 360))
		{
		}
		if (ImGui::SliderFloat("rotationY", &rotation.y, 0, 360))
		{
		}
		if (ImGui::SliderFloat("rotationZ", &rotation.z, 0, 360))
		{
		}
		if (ImGui::SliderFloat("scaleX", &scale.x, 0, 30))
		{
		}
		if (ImGui::SliderFloat("scaleY", &scale.y, 0, 30))
		{
		}
		if (ImGui::SliderFloat("scaleZ", &scale.z, 0, 30))
		{
		}

		ImGui::TextColored({0.7f, 1.0f, 1.0f,1.0f},"Scale: X=%.2f, Y=%.2f, Z=%.2f", scale.x, scale.y, scale.z);
		ImGui::TextColored({ 0.7f, 1.0f, 1.0f ,1.0f}, "Rotation: X=%.2f, Y=%.2f, Z=%.2f, W=%.2f", rotation.x, rotation.y, rotation.z, rotation.w);
		ImGui::TreePop();
	}
}

float4x4 CompTransform::GetTransMatrix()
{
	return TransMatrix;
}
