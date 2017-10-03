/*#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "MathGeoLib.h"


class ModuleWorld : public Module
{
public:

	ModuleWorld(bool start_enable = true);
	~ModuleWorld();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();


public:

	Sphere* sphere;
	vec* temp;

	GLuint vertexbuffer;
	GLuint normalbuffer;
	std::vector<vec> vect;
	std::vector<vec> norms;
	std::vector<unsigned int> indices;
	GLuint my_indices = 0;
	//std::vector<GLfloat> g_vertex_buffer_data;
	std::vector<GLfloat> g_vertex_buffer_data;


	GLuint vao;
	GLuint vbo;
	GLuint ibo;
};
*/