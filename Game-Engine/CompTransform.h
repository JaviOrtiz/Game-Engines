#pragma once

#include "Component.h"
#include "MathGeoLib\Math\float3.h"
#include "MathGeoLib\Math\Quat.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\type_ptr.hpp"
class CompTransform : public Component
{
public:
	CompTransform(float3 pos, float3 scale, Quat rot, ComponentType type = Component_Transform);
	~CompTransform();

	void Update();
	void UpdatePositionMatrix();
	float* GetPositionMatrix();
	void OnEditor() override;

private:
	float3 movedposition;
	float3 position;
	float3 scale;
	Quat rotation;
	glm::mat4 transformmatrix;
	float* positionmatrix;

	bool needToMove;
};
