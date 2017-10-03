#include "Application.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"
#include "GeometryLoader.h"

#include "SDL\include\SDL_opengl.h"

#pragma comment (lib,"Assimp/libx86/assimp.lib")

GeometryLoader::GeometryLoader(Application * app, bool start_enabled) : Module(app, start_enabled)
{

}

GeometryLoader::~GeometryLoader()
{
}

bool GeometryLoader::Start()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	//LoadFile("warrior.FBX");
	LoadFile("Soraka.obj");
	return true;
}

update_status GeometryLoader::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status GeometryLoader::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool GeometryLoader::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

bool GeometryLoader::LoadFile(const char* full_path)
{
	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		LOG("Scene %s loaded succesfully", full_path);
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* newMesh = scene->mMeshes[i];
			Geometry* m = new Geometry;
			m->numVertices = newMesh->mNumVertices;
			m->vertices = new float[m->numVertices * 3];
			memcpy(m->vertices, newMesh->mVertices, sizeof(float)* m->numVertices * 3);
			LOG("New mesh with %d vertices", m->numVertices);

			glGenBuffers(1, (GLuint*)&m->idVertices);
			glBindBuffer(GL_ARRAY_BUFFER, m->idVertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->numVertices * 3, m->vertices, GL_STATIC_DRAW);

			if (newMesh->HasFaces())
			{
				m->numIndices = newMesh->mNumFaces * 3;
				m->indices = new uint[m->numIndices];
				for (uint i = 0; i < newMesh->mNumFaces; ++i)
				{
					if (newMesh->mFaces[i].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&m->indices[i * 3], newMesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}

				glGenBuffers(1, (GLuint*)&m->idIndices);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->idIndices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m->numIndices, m->indices, GL_STATIC_DRAW);
			}
			if (newMesh->HasNormals())
			{
				m->normals = new float[m->numVertices * 3];
				memcpy(m->normals, newMesh->mNormals, sizeof(float) * m->numVertices * 3);

				glGenBuffers(1, (GLuint*) &(m->idNormals));
				glBindBuffer(GL_ARRAY_BUFFER, m->idNormals);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->numVertices * 3, m->normals, GL_STATIC_DRAW);
			}
			geometryvector.push_back(m);
		}
		
		aiReleaseImport(scene);
		return true;
	}
	else {
		LOG("Error loading scene %s", full_path);
		return false;
	}
	/*
	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			Geometry new_geo;

			const aiMesh* new_mesh = scene->mMeshes[i];
			new_geo.num_vertices = new_mesh->mNumVertices;
			new_geo.vertices = new float[new_geo.num_vertices * 3];
			memcpy(new_geo.vertices, new_mesh->mVertices, sizeof(float) * new_geo.num_vertices * 3);
			App->Console.AddLog("New mesh with %d vertices", new_geo.num_vertices);

			new_geo.id_vertices = vertices_id;
			new_geo.id_indices = index_id;
			// Use scene->mNumMeshes to iterate on scene->mMeshes array
			if (new_mesh->HasFaces())
			{
				new_geo.num_indices = new_mesh->mNumFaces * 3;
				new_geo.indices = new uint[new_geo.num_indices]; // assume each face is a triangle
				for (uint i = 0; i < new_mesh->mNumFaces; ++i)
				{
					if (new_mesh->mFaces[i].mNumIndices != 3)
					{
						App->Console.AddLog("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&new_geo.indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}
			}
			geometryvector.push_back(new_geo);

		}
		BindMeshToBuffer();


		aiReleaseImport(scene);
	}
	else
		App->Console.AddLog("Error loading scene %s", full_path);
		*/
}

void GeometryLoader::BindMeshToBuffer()
{
	/*for (std::vector<Geometry>::iterator it = geometryvector.begin(); it != geometryvector.end(); it++)
	{
		glGenBuffers(1, &it->id_vertices);
		App->Console.AddLog("the mesh id is %d",it->id_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, it->id_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * it->numVertices * 3, it->vertices, GL_STATIC_DRAW);
	}*/
}
