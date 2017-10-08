#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "MathGeoLib.h"
#include "Glew\include\glew.h"
#include "ImGui\imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GeometryLoader.h"
#include "Devil\include\il.h"

#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib  */  
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib  */ 

#define MAX_LIGHTS 8
 struct Mesh
 {		
	uint idVertices = 0; // id in VRAM 
	uint numVertices = 0;
	float* vertices = nullptr;

	uint idIndices = 0; // id in VRAM 
	uint numIndices = 0;
	unsigned short* indices = nullptr;
};

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);
	void Gl_State(bool, GLenum);
	
	void SaveConfig(JSON_Object *root);
	void LoadConfig(JSON_Object *root);

	void Render(Geometry toDraw);



public:
	float red = 1.0f, green = 1.0f, blue = 1.0f, bred = 0.14f, bgreen = 0.41f, bblue = 0.75f;
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	bool	DeepTest;
	bool	CullFace;
	bool	GlLighting;
	bool	ColorMaterial;
	bool	Texture2D;
	GLuint CubeDataID;
	bool	WireFrame = false;
	bool directDraw = false;
	bool ArrayDraw = false;
	bool IndexDraw = false;
	bool drawnormals = false;
	PPlane* grid;
	PSphere* circle;
	PCylinder* cyl;
};