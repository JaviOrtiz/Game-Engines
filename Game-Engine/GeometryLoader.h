#pragma once

#include "Application.h"
#include "Globals.h"
#include <vector>
struct Geometry
{
	uint idVertices = 0; // id in VRAM 
	uint numVertices = 0;
	float* vertices = nullptr;

	uint idIndices = 0; // id in VRAM 
	uint numIndices = 0;
	uint* indices = nullptr;

	uint idNormals = 0; // id in VRAM
	float* normals = nullptr;

	uint num_textcoord = 0;
	uint idTexture = 0; // id in VRAM
	uint idDevilImage = 0;
	float* textures = nullptr;
	char* tex_name;

};

class GeometryLoader : public Module
{
public:

	GeometryLoader(Application* app, bool start_enabled = true);
	~GeometryLoader();

	bool Start();

	//PreUpdate
	update_status PreUpdate(float dt);

	//PostUpdate
	update_status PostUpdate(float dt);

	bool CleanUp();

	bool LoadFile(const char* full_path);
	//void BindMeshToBuffer();

	GLuint LoadImage_devil(const char* theFileName, GLuint *buff);
	bool loadTextureFromPixels32(GLuint * id_pixels, GLuint width, GLuint height, GLuint *buff);

	void ClearGeometryvector();
public:

	std::vector<Geometry*> geometryvector;
	AABB BoundingBox;

	uint index_id = 0;
	uint vertices_id = 0;
};