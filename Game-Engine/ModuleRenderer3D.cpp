#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "GeometryLoader.h"
#include <vector>



#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Renderer 3D";
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	grid = new PPlane(0,0,0,10);

	LOG("Creating 3D Renderer context");
	App->Console.AddLog("Creating 3D Renderer context");
	bool ret = true;


	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	GLenum err = glewInit();
	App->Console.AddLog("Using Glew %s", glewGetString(GLEW_VERSION));
	App->Console.AddLog("Vendor: %s", glewGetString(GL_VENDOR));
	App->Console.AddLog("Renderer: %s", glewGetString(GL_RENDERER));
	App->Console.AddLog("OpenGL version supported %s", glGetString(GL_VERSION));
	App->Console.AddLog("GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));


	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		App->Console.AddLog("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (ret == true)
	{
		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
		{
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
			App->Console.AddLog("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		}
		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			App->Console.AddLog("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			App->Console.AddLog("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.14f, 0.41f, 0.75f, 1);

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			App->Console.AddLog("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();

		GLfloat MaterialAmbient[] = { 1.0f, 0.5f, 0.0f, 0.7f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 0.5f, 0.0f, 0.7f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		//glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	OnResize(App->window->Window_Width, App->window->Window_Height);
	// Projection matrix for

	return ret;
}

bool ModuleRenderer3D::Start()
{
	
	return true;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());
	glClearColor(BackgroundColor[0], BackgroundColor[1], BackgroundColor[2], 1);
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	grid->Render();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (std::vector<Geometry*>::iterator it = App->editor->geometryvector.begin(); it != App->editor->geometryvector.end(); ++it)
	{
		for (std::vector<ModelMesh*>::iterator re = (*it)->meshvector.begin(); re != (*it)->meshvector.end(); ++re)
		{
			App->renderer3D->Render(**re);
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleRenderer3D::Render(ModelMesh toDraw)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, toDraw.idDevilImage);
	glColor4f(ColorOverMaterial[0], ColorOverMaterial[1], ColorOverMaterial[2] , 1);
	if (WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPushMatrix();

	if (drawnormals && toDraw.normals != nullptr) {
		for (uint i = 0; i < toDraw.numVertices * 3; i += 3)
		{
			glLineWidth(2.0f);
			glColor3f(1.0f, 0.0f, 0.0f);

			glBegin(GL_LINES);
			glVertex3f(toDraw.vertices[i], toDraw.vertices[i + 1], toDraw.vertices[i + 2]);
			glVertex3f(toDraw.vertices[i] + toDraw.normals[i], toDraw.vertices[i + 1] + toDraw.normals[i + 1], toDraw.vertices[i + 2] + toDraw.normals[i + 2]);
			glEnd();

			glLineWidth(1.0f);
			glColor3f(1.0f, 1.0f, 1.0f);
		}
	}

	if (toDraw.textures != nullptr) {

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, toDraw.idTexture);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, toDraw.idVertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, toDraw.idIndices);
	glDrawElements(GL_TRIANGLES, toDraw.numIndices, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_ELEMENT_ARRAY_BUFFER);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	glUseProgram(0);
}


void ModuleRenderer3D::ImGuiDrawer()
{
	ImGui::Text("Background Color");
	ImGui::ColorEdit4("##Background", &BackgroundColor[0]);

	ImGui::Text("Color Over Texture");
	ImGui::ColorEdit4("##Texture Color", &ColorOverMaterial[0]);

	if (ImGui::Checkbox("Gl_Deepth_Test", &App->renderer3D->DeepTest))
	{
		App->renderer3D->Gl_State(App->renderer3D->DeepTest, GL_DEPTH_TEST);
	}
	if (ImGui::Checkbox("Gl_Cull_Face", &App->renderer3D->CullFace))
	{
		App->renderer3D->Gl_State(App->renderer3D->CullFace, GL_CULL_FACE);
	}
	if (ImGui::Checkbox("Gl_Lighting", &App->renderer3D->GlLighting))
	{
		App->renderer3D->Gl_State(App->renderer3D->GlLighting, GL_LIGHTING);
	}
	if (ImGui::Checkbox("Gl_Color_Material", &App->renderer3D->ColorMaterial))
	{
		App->renderer3D->Gl_State(App->renderer3D->ColorMaterial, GL_COLOR_MATERIAL);
	}
	if (ImGui::Checkbox("Gl_Texture_2D", &App->renderer3D->Texture2D))
	{
		App->renderer3D->Gl_State(App->renderer3D->Texture2D, GL_TEXTURE_2D);
	}
	if (ImGui::Checkbox("WireFrame", &App->renderer3D->WireFrame))
	{
	}
	if (ImGui::Checkbox("Draw Normals", &App->renderer3D->drawnormals))
	{
	}
}
// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");
	App->Console.AddLog("Destroying 3D Renderer");
	ImGui_ImplSdlGL3_Shutdown();
	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::Gl_State(bool state, GLenum dummy) {


	if (state) {

		glEnable(dummy);
	}

	else {

		glDisable(dummy);
	}



}

void ModuleRenderer3D::SaveConfig(JSON_Object * root)
{
	json_object_set_number(root, "Red", ColorOverMaterial[0]);
	json_object_set_number(root, "Green", ColorOverMaterial[1]);
	json_object_set_number(root, "Blue", ColorOverMaterial[2]);

	json_object_set_number(root, "BackGroundRed", BackgroundColor[0]);
	json_object_set_number(root, "BackGroundGreen", BackgroundColor[1]);
	json_object_set_number(root, "BackGroundBlue", BackgroundColor[2]);

	json_object_set_boolean(root, "GL_DEPTH_TEST", DeepTest);
	json_object_set_boolean(root, "GL_CULL_FACE", CullFace);
	json_object_set_boolean(root, "GL_LIGHTNING", GlLighting);
	json_object_set_boolean(root, "GL_COLOuR_MATERIAL", ColorMaterial);
	json_object_set_boolean(root, "GL_TEXTURE_2D", Texture2D);
	json_object_set_boolean(root, "WIREFRAME", WireFrame);
}

void ModuleRenderer3D::LoadConfig(JSON_Object* root)
{
	if (json_object_get_value(root, "Red") == NULL) {
		json_object_set_value(root, "Red", json_value_init_object());
		ColorOverMaterial[0] = 1;
		json_object_set_number(root, "Red", ColorOverMaterial[0]);

	}
	else
	{
		ColorOverMaterial[0] = json_object_get_number(root, "Red");
	}
	if (json_object_get_value(root, "Green") == NULL) {
		json_object_set_value(root, "Green", json_value_init_object());
		ColorOverMaterial[1] = 1;
		json_object_set_number(root, "Green", ColorOverMaterial[1]);

	}
	else
	{
		ColorOverMaterial[1] = json_object_get_number(root, "Blue");
	}
	if (json_object_get_value(root, "Blue") == NULL) {
		json_object_set_value(root, "Blue", json_value_init_object());
		ColorOverMaterial[2] = 1;
		json_object_set_number(root, "Blue", ColorOverMaterial[2]);

	}
	else
	{
		ColorOverMaterial[2] = json_object_get_number(root, "Blue");
	}
	if (json_object_get_value(root, "BackGroundRed") == NULL) {
		json_object_set_value(root, "BackGroundRed", json_value_init_object());
		BackgroundColor[0] = 1;
		json_object_set_number(root, "BackGroundRed", BackgroundColor[0]);

	}
	else
	{
		BackgroundColor[0] = json_object_get_number(root, "BackGroundGreen");
	}
	if (json_object_get_value(root, "BackGroundGreen") == NULL) {
		json_object_set_value(root, "BackGroundGreen", json_value_init_object());
		BackgroundColor[1] = 1;
		json_object_set_number(root, "BackGroundGreen", BackgroundColor[1]);

	}
	else
	{
		BackgroundColor[1] = json_object_get_number(root, "BackGroundGreen");
	}
	if (json_object_get_value(root, "BackGroundBlue") == NULL) {
		json_object_set_value(root, "BackGroundBlue", json_value_init_object());
		BackgroundColor[2] = 1;
		json_object_set_number(root, "BackGroundBlue", BackgroundColor[2]);

	}
	else
	{
		BackgroundColor[2] = json_object_get_number(root, "BackGroundBlue");
	}

	if (json_object_get_boolean(root, "GL_DEPTH_TEST") == NULL) {
		json_object_set_boolean(root, "GL_DEPTH_TEST", true);
		DeepTest = true;
		json_object_set_number(root, "GL_DEPTH_TEST", DeepTest);

	}
	else
	{
		CullFace = json_object_get_number(root, "GL_DEPTH_TEST");
	}
	if (json_object_get_boolean(root, "GL_CULL_FACE") == NULL) {
		json_object_set_boolean(root, "GL_CULL_FACE", true);
		CullFace = true;
		json_object_set_number(root, "GL_CULL_FACE", CullFace);

	}
	else
	{
		CullFace = json_object_get_number(root, "GL_CULL_FACE");
	}

	if (json_object_get_boolean(root, "GL_LIGHTNING") == NULL) {
		json_object_set_boolean(root, "GL_LIGHTNING", true);
		GlLighting = true;
		json_object_set_number(root, "GL_LIGHTNING", GlLighting);

	}
	else
	{
		GlLighting = json_object_get_number(root, "GL_LIGHTNING");
	}
	if (json_object_get_boolean(root, "GL_COLOuR_MATERIAL") == NULL) {
		json_object_set_boolean(root, "GL_COLOuR_MATERIAL", true);
		ColorMaterial = true;
		json_object_set_number(root, "GL_COLOuR_MATERIAL", ColorMaterial);

	}
	else
	{
		ColorMaterial = json_object_get_number(root, "GL_COLOuR_MATERIAL");
	}
	if (json_object_get_boolean(root, "GL_TEXTURE_2D") == NULL) {
		json_object_set_boolean(root, "GL_TEXTURE_2D", true);
		Texture2D = true;
		json_object_set_number(root, "GL_TEXTURE_2D", Texture2D);

	}
	else
	{
		Texture2D = json_object_get_number(root, "GL_TEXTURE_2D");
	}
	if (json_object_get_boolean(root, "WIREFRAME") == NULL) {
		json_object_set_boolean(root, "WIREFRAME", true);
		WireFrame = false;
		json_object_set_number(root, "WIREFRAME", WireFrame);

	}
	else
	{
		WireFrame = json_object_get_number(root, "WIREFRAME");
	}

}

