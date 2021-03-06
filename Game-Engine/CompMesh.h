#pragma once

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib\Geometry\AABB.h"

class CompMesh : public Component
{
public:
	CompMesh();

	~CompMesh();

	uint idVertices = 0; // id in VRAM 
	uint numVertices = 0;
	float* vertices = nullptr;

	uint idIndices = 0; // id in VRAM 
	uint numIndices = 0;
	uint* indices = nullptr;

	uint idNormals = 0; // id in VRAM
	float* normals = nullptr;

	uint idColors = 0; // id in VRAM
	float* colors = nullptr;

	uint idTexCoords = 0; // id in VRAM
	float* texCoords = nullptr;

	AABB enclosingBox;

	void Update();

	//Returns the center point of the mesh's enclosing box.
	float3 GetCenter() const;

	void DrawDebug() const;

	void OnEditor()override;

	void Move(float3 lastpos,float3 newPos);

public:
	bool drawdebug = false;
};