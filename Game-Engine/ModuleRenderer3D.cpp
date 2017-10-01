#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"





#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
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


	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	if (directDraw)
	{
		glBegin(GL_TRIANGLES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(2.0f, 2.0f, 0.0f);
		glVertex3f(2.0f, 0.0f, 0.0f);//
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 2.0f, 0.0f);
		glVertex3f(2.0f, 2.0f, 0.0f);//Delante

		glVertex3f(0.0f, 2.0f, 0.0f);
		glVertex3f(2.0f, 2.0f, 2.0f);//
		glVertex3f(2.0f, 2.0f, 0.0f);
		glVertex3f(0.0f, 2.0f, 0.0f);
		glVertex3f(0.0f, 2.0f, 2.0f);//Arriba
		glVertex3f(2.0f, 2.0f, 2.0f);


		glVertex3f(2.0f, 0.0f, 0.0f);
		glVertex3f(2.0f, 2.0f, 2.0f);//
		glVertex3f(2.0f, 0.0f, 2.0f);
		glVertex3f(2.0f, 0.0f, 0.0f);
		glVertex3f(2.0f, 2.0f, 0.0f);//Derecha
		glVertex3f(2.0f, 2.0f, 2.0f);


		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(2.0f, 0.0f, 0.0f);//
		glVertex3f(2.0f, 0.0f, 2.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(2.0f, 0.0f, 2.0f);//Abajo
		glVertex3f(0.0f, 0.0f, 2.0f);


		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 2.0f);
		glVertex3f(0.0f, 2.0f, 2.0f);//
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 2.0f, 2.0f);
		glVertex3f(0.0f, 2.0f, 0.0f);//Izquierda

		glVertex3f(0.0f, 0.0f, 2.0f);
		glVertex3f(2.0f, 0.0f, 2.0f);//
		glVertex3f(2.0f, 2.0f, 2.0f);
		glVertex3f(0.0f, 0.0f, 2.0f);
		glVertex3f(2.0f, 2.0f, 2.0f);//Detras
		glVertex3f(0.0f, 2.0f, 2.0f);
		glEnd();
	}
	if (ArrayDraw) {
		GLuint VertexArrayID;
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		static const GLfloat g_vertex_buffer_data[] = {
			-1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f
		};
		GLuint vertexbuffer;
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

			// 1rst attribute buffer : vertex
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,
			3,                  // size
			GL_FLOAT,           // class
			GL_FALSE,           // normalized??
			0,
			(void*)0            // buffer gap
		);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
		glDisableVertexAttribArray(0);
		static const GLfloat g_color_buffer_data[] = {
			0.583f,  0.771f,  0.014f,
			0.609f,  0.115f,  0.436f,
			0.327f,  0.483f,  0.844f,
			0.822f,  0.569f,  0.201f,
			0.435f,  0.602f,  0.223f,
			0.310f,  0.747f,  0.185f,
			0.597f,  0.770f,  0.761f,
			0.559f,  0.436f,  0.730f,
			0.359f,  0.583f,  0.152f,
			0.483f,  0.596f,  0.789f,
			0.559f,  0.861f,  0.639f,
			0.195f,  0.548f,  0.859f,
			0.014f,  0.184f,  0.576f,
			0.771f,  0.328f,  0.970f,
			0.406f,  0.615f,  0.116f,
			0.676f,  0.977f,  0.133f,
			0.971f,  0.572f,  0.833f,
			0.140f,  0.616f,  0.489f,
			0.997f,  0.513f,  0.064f,
			0.945f,  0.719f,  0.592f,
			0.543f,  0.021f,  0.978f,
			0.279f,  0.317f,  0.505f,
			0.167f,  0.620f,  0.077f,
			0.347f,  0.857f,  0.137f,
			0.055f,  0.953f,  0.042f,
			0.714f,  0.505f,  0.345f,
			0.783f,  0.290f,  0.734f,
			0.722f,  0.645f,  0.174f,
			0.302f,  0.455f,  0.848f,
			0.225f,  0.587f,  0.040f,
			0.517f,  0.713f,  0.338f,
			0.053f,  0.959f,  0.120f,
			0.393f,  0.621f,  0.362f,
			0.673f,  0.211f,  0.457f,
			0.820f,  0.883f,  0.371f,
			0.982f,  0.099f,  0.879f
		};

		GLuint colorbuffer;
		glGenBuffers(1, &colorbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // Atributo. No hay razón especial para el 1, pero debe corresponder al número en el shader.
			3,                                // tamaño
			GL_FLOAT,                         // tipo
			GL_FALSE,                         // normalizado?
			0,                                // corrimiento
			(void*)0                          // corrimiento de buffer
		);
		
		
	}

	if (IndexDraw) {
		GLfloat vertices[] = {
			3.0f,0.0f,0.0f, //0
			4.0f,0.0f,0.0f, //1
			3.0f,1.0f,0.0f, //2
			4.0f,1.0f,0.0f, //3
			3.0f,0.0f,1.0f, //4
			4.0f,0.0f,1.0f, //5
			3.0f,1.0f,1.0f, //6
			4.0f,1.0f,1.0f, //7
		};

		GLubyte indices[] =
		{ 0,1,3, 0,3,2,   // 36 of indices
			1,5,7, 1,7,3,
			6,4,0, 0,2,6,
			2,3,7, 2,7,6,
			0,4,5, 0,5,1,
			4,7,5, 4,6,7 };

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, vertices);

		// draw a cube
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

		// deactivate vertex arrays after drawing
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	return UPDATE_CONTINUE;
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
	json_object_set_number(root, "Red", red);
	json_object_set_number(root, "Green",green);
	json_object_set_number(root, "Blue",blue);

	json_object_set_number(root, "BackGroundRed", bred);
	json_object_set_number(root, "BackGroundGreen", bgreen);
	json_object_set_number(root, "BackGroundBlue", bblue);

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
		red = 1;
		json_object_set_number(root, "Red", red);

	}
	else
	{
		red = json_object_get_number(root, "Red");
	}
	if (json_object_get_value(root, "Green") == NULL) {
		json_object_set_value(root, "Green", json_value_init_object());
		green = 1;
		json_object_set_number(root, "Green", green);

	}
	else
	{
		blue = json_object_get_number(root, "Blue");
	}
	if (json_object_get_value(root, "Blue") == NULL) {
		json_object_set_value(root, "Blue", json_value_init_object());
		blue = 1;
		json_object_set_number(root, "Blue", blue);

	}
	else
	{
		blue = json_object_get_number(root, "Blue");
	}
	if (json_object_get_value(root, "BackGroundRed") == NULL) {
		json_object_set_value(root, "BackGroundRed", json_value_init_object());
		bred = 1;
		json_object_set_number(root, "BackGroundRed", bred);

	}
	else
	{
		bred = json_object_get_number(root, "BackGroundGreen");
	}
	if (json_object_get_value(root, "BackGroundGreen") == NULL) {
		json_object_set_value(root, "BackGroundGreen", json_value_init_object());
		bgreen = 1;
		json_object_set_number(root, "BackGroundGreen", bgreen);

	}
	else
	{
		bgreen = json_object_get_number(root, "BackGroundGreen");
	}
	if (json_object_get_value(root, "BackGroundBlue") == NULL) {
		json_object_set_value(root, "BackGroundBlue", json_value_init_object());
		bblue = 1;
		json_object_set_number(root, "BackGroundBlue", bblue);

	}
	else
	{
		bblue = json_object_get_number(root, "BackGroundBlue");
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
