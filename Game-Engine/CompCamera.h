#pragma once

#include "Component.h"
#include "MathGeoLib\Geometry\Frustum.h"

class CompCamera : public Component
{
public:

	Frustum frustum;

	CompCamera(float3 pos = { 0, 0, 0 }, float3 front = { 0, 0, 1 }, float3 up = { 0, 1, 0 }, float nearPlaneDistance = 0.5f, float farPlaneDistance = 100, float verticalFov = 15, float aspectRatio = (float)16 / 9, FrustumType type = PerspectiveFrustum);

	~CompCamera();

	void Update();

	void SetPos(float3 newPos);
	void OnEditor() override;
	void SetFOV();
	void SetVerticalFOV(float value);

	void DrawDebug() const;

	bool Contains(const AABB &aabb) const;
	bool GetFrustumCulling() const;

private:
	float FOV;
	float aspectRatio;
	bool frustumCulling;
};