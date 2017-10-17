#pragma once

#include "Component.h"
#include "MathGeoLib\Geometry\Frustum.h"

class CompCamera : public Component
{
public:
	CompCamera(float3 pos = { 0, 0, 0 }, float3 front = { 0, 0, 1 }, float3 up = { 0, 1, 0 }, float nearPlaneDistance = 20, float farPlaneDistance = 100, float verticalFov = 100, float aspectRatio = 16 / 9, FrustumType type = PerspectiveFrustum);

	~CompCamera();

	void Update();

	void SetPos(float3 newPos);
	void SetVerticalFOV(float value);

	void DrawDebug();

	bool Contains(const AABB &aabb) const;

private:
	float aspectRatio;
	Frustum frustum;
};