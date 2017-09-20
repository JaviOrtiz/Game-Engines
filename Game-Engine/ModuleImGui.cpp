
#include "Globals.h"
#include "Application.h"
#include "ModuleImGui.h"

#include "Glew\include\glew.h"
#include "ImGui\imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */



ModuleImGui::ModuleImGui(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleImGui::~ModuleImGui()
{
}

bool ModuleImGui::Init()
{
	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->window);
	ImGuiIO& io = ImGui::GetIO();
	io.WantTextInput = true;
	io.IniFilename = "imgui.ini";
	Console.AddLog("hey");
	return true;
}

bool ModuleImGui::Start()
{
	return true;
}

update_status ModuleImGui::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleImGui::Update(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem("HomeWork"))
			{
				
				Homework = !Homework;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Test Window"))
			{
				Showtest = !Showtest;
			}
			if (ImGui::MenuItem("Console"))
			{
				Showconsole = !Showconsole;
			}
			ImGui::EndMenu();
		}
		if (ImGui::Button("Exit"))
		{
			return UPDATE_STOP;
		}
	}
	ImGui::EndMainMenuBar();

	if (Homework)
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove;
		ImGui::SetNextWindowSize(ImVec2(300,300), ImGuiCond_FirstUseEver);
		ImGui::Begin("Menu", (bool*)false, window_flags);
		if (ImGui::Button("Exitbutton"))
		{
			return UPDATE_STOP;
		}
		if (ImGui::Button("Show/Hide Test Window"))
		{
			Showtest = !Showtest;

		}


		ImGui::DragIntRange2("Int Range", &First, &Last, 0.25f, 0.0f, 100.0f, "Min: %.1f ", "Max: %.1f ");
		//ImGui::InputFloat("Int Range", &Test, 0.25f, 0.25f, -1, 0);    Input con ++/--
		if (ImGui::Button("Random Number Generator / Int")) {

			Random_Num_Int = Random_Num_2.Int(First, Last);

		}

		//ImGui::Text("%f", Test);

		ImGui::Text("Random Float Number between %i And %i  Is: %0.f", First, Last, Random_Num_Int);

		if (ImGui::Button("Random Number Generator / Float"))
		{
			Random_Num_Float = Random_Num_1.Float();
		}
		ImGui::Text("Random Float Number between 0-1: %.3f", Random_Num_Float);

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();

		ImGui::Text("Sphere 1(0,0,0)   Sphere 2 (1,0,0)  Sphere 3 (3,0,0)");

		if (ImGui::Button("Sphere Intersect Check between 1 and 2")) 
		{
		 SphereTest = S1Temp->Intersects(*S2Temp);
		}

		ImGui::SameLine(300); 
		if (ImGui::Button("Sphere Intersect Check between 1 and 3")) 
		{

			SphereTest = S1Temp->Intersects(*S3Temp);
		}

		ImGui::SameLine(600); 
		if (ImGui::Button("Sphere Intersect Check between 2 and 3")) 
		{

			SphereTest = S2Temp->Intersects(*S3Temp);


		}


		ImGui::Text("1 Means True/ 0 Means False:  %i", SphereTest);



		ImGui::End();
	}

	if (Showtest) ImGui::ShowTestWindow();
	if (Showconsole) Console.Draw("Super Console", (bool*)false);
	ImGui::Render();

	return UPDATE_CONTINUE;
};


update_status ModuleImGui::PostUpdate(float dt)
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp()
{
	return false;
}
