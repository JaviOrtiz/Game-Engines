#include "Application.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"
#include "GeometryLoader.h"
#include"Devil\include\ilut.h"
#include "SDL\include\SDL_opengl.h"

#pragma comment (lib,"Assimp/libx86/assimp.lib")
#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")


GeometryLoader::GeometryLoader(Application * app, bool start_enabled) : Module(app, start_enabled)
{

}

GeometryLoader::~GeometryLoader()
{
}

bool GeometryLoader::Start()
{

	ilutRenderer(ILUT_OPENGL);
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	ilClearColour(255, 255, 255, 000);

	//Check for error
	ILenum ilError = ilGetError();
	if (ilError != IL_NO_ERROR)
	{
		LOG("IlInit error!!");
	}


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
			if (scene->mMeshes[i]->HasTextureCoords(0))
			{
				m->textures = new float[m->numVertices * 2];

				for (int x = 0; x < scene->mMeshes[i]->mNumVertices; ++x) {

					m->textures[x * 2] = scene->mMeshes[i]->mTextureCoords[0][x].x;
					m->textures[x * 2 + 1] = scene->mMeshes[i]->mTextureCoords[0][x].y;
				}

				//memcpy(m->mesh.textures_coord, scene->mMeshes[i]->mTextureCoords[0], sizeof(float) * m->mesh.num_vertices * 2);
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
}
