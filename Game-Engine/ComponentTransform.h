#pragma once

#include "Component.h"
#include "MathGeoLib\Math\float3.h"
#include "MathGeoLib\Math\Quat.h"

class CompTransform : public Component
{
public:
	CompTransform(float3 pos, float3 scale, Quat rot, ComponentType type = Component_Transform);
	~CompTransform();

	void Update();

	void OnEditor() override;

private:
	float3 position;
	float3 scale;
	Quat rotation;

	bool needToMove;
};
