#pragma once

#include "Application.h"
#include "Globals.h"
#include <vector>
#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"
#include <string>
struct ModelMesh
{
	//~ModelMesh();
	std::string name;
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
	std::string tex_name;

	uint numTriangles = 0;
	aiVector3D position;
	aiQuaternion rotation;
	aiVector3D scale;
	
	void ImGuiDraw();
};


struct Geometry
{
	//~Geometry();

	std::string name;
	uint numTriangles = 0;
	aiVector3D position;
	aiQuaternion rotation;
	aiVector3D scale;

	std::vector<ModelMesh*> meshvector;

	void ImGuiDraw();
};

class GeometryLoader : public Module
{
public:

	GeometryLoader(Application* app, bool start_enabled = true);
	~GeometryLoader();

	bool Start();

	//PreUpdate
	update_status PreUpdate(float dt);

	bool CleanUp();

	bool LoadFile(const char* full_path);
	//void BindMeshToBuffer();
	void LoadMesh(aiNode * node, const aiScene * scene, Geometry* newgeo);
	GLuint LoadImage_devil(const char* theFileName, GLuint *buff);
	bool loadTextureFromPixels32(GLuint * id_pixels, GLuint width, GLuint height, GLuint *buff);

	void ClearGeometryvector();

	void ChangeTexture(const char* Path);
public:

	std::vector<Geometry*> geometryvector;
	AABB BoundingBox;

	uint index_id = 0;
	uint vertices_id = 0;
};