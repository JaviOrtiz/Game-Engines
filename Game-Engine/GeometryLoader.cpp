#include "Application.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"
#include "GeometryLoader.h"
#include"Devil\include\ilut.h"
#include "SDL\include\SDL_opengl.h"
#include "ModuleEditor.h"
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
	//LoadFile("Sorakilla.obj");
	return true;
}

update_status GeometryLoader::PreUpdate(float dt)
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
	Geometry* newgeo = new Geometry;
	aiNode* node = scene->mRootNode;
	if (scene != nullptr && scene->HasMeshes())
	{
		//Loading global geometry info
		node->mTransformation.Decompose(newgeo->position, newgeo->rotation, newgeo->scale);

		newgeo->name = node->mName.C_Str();
		//Loading meshes
		LoadMesh(node, scene, newgeo);

		for (int i = 0; i < newgeo->meshvector.size(); i++)
		{
			newgeo->numTriangles += newgeo->meshvector[i]->numTriangles;

		}
		App->editor->geometryvector.push_back(newgeo);
		App->Console.AddLog("The model %s with %i triangles was loaded correctly", newgeo->name.c_str() , newgeo->numTriangles);
		aiReleaseImport(scene);
		return true;
	}
	else {
		App->Console.AddLog("Error loading scene %s", full_path);
		return false;
	}
}


void GeometryLoader::ChangeTexture(const char * Path)
{
	for (std::vector<Geometry*>::iterator it = App->editor->geometryvector.begin(); it != App->editor->geometryvector.end(); ++it)
	{
		for (std::vector<ModelMesh*>::iterator re = (*it)->meshvector.begin(); re != (*it)->meshvector.end(); ++re)
		{
			(*re)->tex_name = Path;
			LoadImage_devil((*re)->tex_name.c_str(), &(*re)->idDevilImage);
		}
	}
	/*for (int i = 0; i < geometryvector.size(); i++) {
		Geometry* ActualGeometry = geometryvector[i]; //Passing through every mesh of the geometryvector to change it's texture.
		ActualGeometry->tex_name = Path;
		LoadImage_devil(ActualGeometry->tex_name, &ActualGeometry->idDevilImage);
	}*/
}

void GeometryLoader::LoadMesh(aiNode * node, const aiScene * scene, Geometry* newgeo)
{
	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ModelMesh* newMesh = new ModelMesh;
		newMesh->numVertices = mesh->mNumVertices;
		newMesh->vertices = new float[newMesh->numVertices * 3];
		memcpy(newMesh->vertices, mesh->mVertices, sizeof(float)* newMesh->numVertices * 3);
		App->Console.AddLog("New mesh with %d vertices", newMesh->numVertices);

		glGenBuffers(1, (GLuint*)&newMesh->idVertices);
		glBindBuffer(GL_ARRAY_BUFFER, newMesh->idVertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * newMesh->numVertices * 3, newMesh->vertices, GL_STATIC_DRAW);

		if (mesh->HasFaces())
		{
			newMesh->numIndices = mesh->mNumFaces * 3;
			newMesh->indices = new uint[newMesh->numIndices];
			for (uint i = 0; i < mesh->mNumFaces; ++i)
			{
				if (mesh->mFaces[i].mNumIndices != 3)
				{
					App->Console.AddLog("WARNING, geometry face with != 3 indices!");
				}
				else
				{
					memcpy(&newMesh->indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
				}
			}


			glGenBuffers(1, (GLuint*)&newMesh->idIndices);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh->idIndices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * newMesh->numIndices, newMesh->indices, GL_STATIC_DRAW);
		}
		if (mesh->HasNormals())
		{
			newMesh->normals = new float[newMesh->numVertices * 3];
			memcpy(newMesh->normals, mesh->mNormals, sizeof(float) * newMesh->numVertices * 3);

			glGenBuffers(1, (GLuint*) &(newMesh->idNormals));
			glBindBuffer(GL_ARRAY_BUFFER, newMesh->idNormals);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * newMesh->numVertices * 3, newMesh->normals, GL_STATIC_DRAW);
		}
		if (mesh->HasTextureCoords(0))
		{
			newMesh->textures = new float[newMesh->numVertices * 2];

			for (int x = 0; x < mesh->mNumVertices; ++x) {

				newMesh->textures[x * 2] = mesh->mTextureCoords[0][x].x;
				newMesh->textures[x * 2 + 1] = mesh->mTextureCoords[0][x].y;
			}
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			uint numtextures = material->GetTextureCount(aiTextureType_DIFFUSE);
			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
			newMesh->tex_name = path.C_Str();

			std::size_t pos = newMesh->tex_name.rfind("\\");     //if the texture path is in a folder, tries to take only the name of the .png file 
			std::string str3 = newMesh->tex_name.substr(pos + 1);
			newMesh->tex_name = str3.c_str();

			LoadImage_devil(newMesh->tex_name.c_str(), &newMesh->idDevilImage);

			glGenBuffers(1, (GLuint*)&(newMesh->idTexture));
			glBindBuffer(GL_ARRAY_BUFFER, newMesh->idTexture);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) *newMesh->numVertices * 2, &newMesh->textures[0], GL_STATIC_DRAW);
		}
		//SetValues
		node->mTransformation.Decompose(newMesh->position, newMesh->rotation, newMesh->scale);
		newMesh->name = node->mName.C_Str();
		newMesh->numTriangles = mesh->mNumFaces;
		//Push new mesh
		newgeo->meshvector.push_back(newMesh);
	}
	for (uint i = 0; i < node->mNumChildren; i++)
	{
		LoadMesh(node->mChildren[i], scene,newgeo);
	}

	BoundingBox.SetNegativeInfinity();
	for (std::vector<ModelMesh*>::iterator item = newgeo->meshvector.begin(); item != newgeo->meshvector.cend(); ++item)
		BoundingBox.Enclose((float3*)(*item)->vertices, (*item)->numVertices);

	App->camera->CenterCameraToGeometry(&BoundingBox);
}

GLuint GeometryLoader::LoadImage_devil(const char * theFileName, GLuint *buff)
{

	//Texture loading success
	bool textureLoaded = false;

	//Generate and set current image ID
	uint imgID = 0;
	ilGenImages(1, &imgID);
	ilBindImage(imgID);

	//Load image
	ILboolean success = ilLoadImage(theFileName);

	//Image loaded successfully
	if (success == IL_TRUE)
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		//Convert image to RGBA
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (success == IL_TRUE)
		{
			textureLoaded = loadTextureFromPixels32((GLuint*)ilGetData(), (GLuint)ilGetInteger(IL_IMAGE_WIDTH), (GLuint)ilGetInteger(IL_IMAGE_HEIGHT), buff);
			//Create texture from file pixels
			textureLoaded = true;
		}

		//Delete file from memory
		ilDeleteImages(1, &imgID);
	}

	//Report error
	if (!textureLoaded)
	{

	}
	return textureLoaded;

}

bool GeometryLoader::loadTextureFromPixels32(GLuint * id_pixels, GLuint width_img, GLuint height_img, GLuint *buff)
{


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, buff);
	glBindTexture(GL_TEXTURE_2D, *buff);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_img, height_img,
		0, GL_RGBA, GL_UNSIGNED_BYTE, id_pixels);
	glBindTexture(GL_TEXTURE_2D, NULL);
	//Check for error
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error loading texture from %p pixels! %s\n", id_pixels, gluErrorString(error));
		return false;
	}

	return true;
}

ModelMesh::~ModelMesh()
{
	if (vertices != nullptr)
	{
		for (int i = 0; i < sizeof(vertices); i++)
		{
			vertices[i] = NULL;
		}
	}

	if (indices != nullptr)
	{
		for (int i = 0; i < sizeof(indices); i++)
		{
			indices[i] = NULL;
		}
	}
	if (normals != nullptr)
	{
		for (int i = 0; i < sizeof(normals); i++)
		{
			normals[i] = NULL;
		}
	}
	if (textures != nullptr)
	{
		for (int i = 0; i < sizeof(textures); i++)
		{
			textures[i] = NULL;
		}
	}
}

void ModelMesh::ImGuiDraw()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::Text("Transformation:");
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Position x %.2f y %.2f z %.2f", position.x, position.y, position.z);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Rotation x %.2f y %.2f z %.2f", rotation.GetEuler().x, rotation.GetEuler().y, rotation.GetEuler().z);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Scale x %.2f y %.2f z %.2f", scale.x, scale.y, scale.z);
		ImGui::Text("Geometry");
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Triangles %i", numTriangles);

		ImGui::Text("Texture");
		ImGui::Image((GLuint*)idTexture, ImVec2(64, 64), ImVec2(0, 0), ImVec2(1, 1));
		ImGui::PushItemWidth(150);
		ImGui::PopItemWidth();
		ImGui::TreePop();
	}
}

Geometry::~Geometry()
{
	meshvector.clear();
}

void Geometry::ImGuiDraw()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::Text("Transformation:");
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Position x %.2f y %.2f z %.2f", position.x, position.y, position.z);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Rotation x %.2f y %.2f z %.2f", rotation.GetEuler().x, rotation.GetEuler().y, rotation.GetEuler().z);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Scale x %.2f y %.2f z %.2f", scale.x, scale.y, scale.z);
		ImGui::Text("Geometry");
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Triangles %i", numTriangles);
		for (int i = 0; i < meshvector.size(); i++)
		{
			meshvector[i]->ImGuiDraw();

		}
		ImGui::TreePop();

	}
}


