
#include "Globals.h"
#include "Application.h"
#include "ModuleImGui.h"

#include "mmgr\mmgr.h"
#include "mmgr\nommgr.h"

#include "Glew\include\glew.h"
#include "ImGui\imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */



ModuleImGui::ModuleImGui(Application * app, bool start_enabled) : Module(app, start_enabled),
Fps_Plot_Data(FPS_AND_MS_PLOT_DATA_LENGTH), Ms_Plot_Data(FPS_AND_MS_PLOT_DATA_LENGTH)
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

	Brightness = App->window->GetBrightness();
	Screen_Width = 0;
	Screen_Height = 0;
	Fullscreen = App->window->IsFullscreen();
	Resizable = App->window->IsResizable();
	Borderless = App->window->IsBorderless();
	Full_Desktop = App->window->IsFullscreenDesktop();




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


	//		MAIN MENU		//

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
			if (ImGui::MenuItem("Documentation"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/JaviOrtiz/Game-Engines/wiki", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("Download Latest"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/JaviOrtiz/Game-Engines/releases", NULL, NULL, SW_SHOWNORMAL);

			}
			if (ImGui::MenuItem("Report a bug"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/JaviOrtiz/Game-Engines/issues", NULL, NULL, SW_SHOWNORMAL);

			}
			if (ImGui::MenuItem("Console"))
			{
				Showconsole = !Showconsole;
			}
			if (ImGui::MenuItem("About"))
			{
				About = !About;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Exit"))
		{
			return UPDATE_STOP;
		}

	}
	ImGui::EndMainMenuBar();


	//			BEGIN MENU INFORMATION		//


	//Start Homework//
	if (Homework)
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove;
		ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
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

		if (ImGui::Button("Random Number Generator / Int"))
		{
			Random_Num_Int = Random_Num_2.Int(First, Last);
		}

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
	// End Homework//

	// Start About? //
	if (About)
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize;
		ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
		ImGui::Begin("About", (bool*)false, window_flags);
		ImGui::Text("Pochinki's Engine 0.1");
		ImGui::Text("We are a pair of students who are working on a 3d Game Engine c++ based.");		//NEED TO BE FINISHED//
		ImGui::Text("Authors:		Daniel Lopez and Javier Ortiz");
		ImGui::End();

	}
	//End About//


	//  Start Window Options//
	if (ImGui::CollapsingHeader("Window Options"))
	{
		if (ImGui::SliderFloat("Brightness", &Brightness, 0.0f, 1.0f))
		{
			App->window->SetBrightness(Brightness);
		}

		ImGui::SliderInt("Screen Width", &App->window->Window_Width, 100, 1920);
		ImGui::SliderInt("Screen Height", &App->window->Window_Height, 100, 1080);

		if (ImGui::Button("Save Window Size"))
		{
			SDL_SetWindowSize(App->window->window, App->window->Window_Width, App->window->Window_Height);
		}

		if (ImGui::Checkbox("Fullscreen", &Fullscreen))
		{
			App->window->SetFullscreen(Fullscreen);
		}

		ImGui::SameLine();

		if (ImGui::Checkbox("Resizable", &Resizable))
		{
			App->window->SetResizable(Resizable);
		}
		if (ImGui::Checkbox("Borderless", &Borderless))
		{
			App->window->SetBorderless(Borderless);
		}
	}

	if (ImGui::CollapsingHeader("Hardware"))
	{
		uint title_size = 50;
		char title[50];
		Pc_Cache = SDL_GetCPUCacheLineSize();
		ImGui::Text("Cache: %i", Pc_Cache);

		Pc_Cpu = SDL_GetCPUCount();
		ImGui::Text("CPU: %i", Pc_Cpu);

		//Memory Statistics//
		sMStats MemoryStats = m_getMemoryStatistics();
		sprintf_s(title, title_size, "totalReportedMemory: %i", MemoryStats.totalReportedMemory);
		ImGui::Text(title);
		sprintf_s(title, title_size, "totalActualMemory: %i", MemoryStats.totalActualMemory);
		ImGui::Text(title);
		sprintf_s(title, title_size, "peakReportedMemory: %i", MemoryStats.peakReportedMemory);
		ImGui::Text(title);
		sprintf_s(title, title_size, "peakActualMemory: %i", MemoryStats.peakActualMemory);
		ImGui::Text(title);
		sprintf_s(title, title_size, "accumulatedReportedMemory: %i", MemoryStats.accumulatedReportedMemory);
		ImGui::Text(title);
		sprintf_s(title, title_size, "accumulatedActualMemory: %i", MemoryStats.accumulatedActualMemory);
		ImGui::Text(title);
		sprintf_s(title, title_size, "accumulatedAllocUnitCount: %i", MemoryStats.accumulatedAllocUnitCount);
		ImGui::Text(title);
		sprintf_s(title, title_size, "totalAllocUnitCount: %i", MemoryStats.totalAllocUnitCount);
		ImGui::Text(title);
		sprintf_s(title, title_size, "peakAllocUnitCount: %i", MemoryStats.peakAllocUnitCount);
		ImGui::Text(title);


		sprintf_s(title, title_size, "RAM: %i", SDL_GetSystemRAM());
		ImGui::Text(title);

		ImGui::Separator();

		const GLubyte* GPUData = nullptr;
		GPUData = glGetString(GL_VENDOR);
		sprintf_s(title, title_size, "GL_Vendor: %s", GPUData);
		ImGui::Text(title);
		GPUData = glGetString(GL_RENDERER);
		sprintf_s(title, title_size, "GL_Renderer: %s", GPUData);
		ImGui::Text(title);
		GPUData = glGetString(GL_VERSION);
		sprintf_s(title, title_size, "GL_Versions: %s", GPUData);
		ImGui::Text(title);


	}
	const Performance* PerformanceData = App->GetPerformanceStruct();
	PushFPSandMSPlot(PerformanceData->Frames_Last_Second, PerformanceData->Miliseconds_Per_Frame);

	// End Window Options//

	// Start Application//
	if (ImGui::CollapsingHeader("Application"))
	{
		uint title_size = 50;
		char title[50];
		//Framerate PlotHistogram
		sprintf_s(title, title_size, "Framerate: %i", PerformanceData->Frames_Last_Second);
		ImGui::PlotHistogram("##Framerate", &Fps_Plot_Data[0], Fps_Plot_Data.size(), 0, title, 0.0f, 150.0f, ImVec2(310, 100));

		//Miliseconds PlotHistogram
		sprintf_s(title, title_size, "Frame Miliseconds: %i", PerformanceData->Miliseconds_Per_Frame);
		ImGui::PlotHistogram("##Frame Miliseconds", &Ms_Plot_Data[0], Ms_Plot_Data.size(), 0, title, 0.0f, 50.0f, ImVec2(310, 100));
	}


	if (ImGui::CollapsingHeader("OpenGL Options"))
	{
		if (ImGui::Checkbox("Gl_Deepth_Test", &DeepTest))
		{
			App->renderer3D->Gl_State(DeepTest, GL_DEPTH_TEST);
		}
		if (ImGui::Checkbox("Gl_Cull_Face", &CullFace))
		{
			App->renderer3D->Gl_State(DeepTest, GL_CULL_FACE);
		}
		if (ImGui::Checkbox("Gl_Lighting", &GlLighting))
		{
			App->renderer3D->Gl_State(DeepTest, GL_LIGHTING);
		}
		if (ImGui::Checkbox("Gl_Color_Material", &ColorMaterial))
		{
			App->renderer3D->Gl_State(DeepTest, GL_COLOR_MATERIAL);
		}
		if (ImGui::Checkbox("Gl_Texture_2D", &Texture2D))
		{
			App->renderer3D->Gl_State(DeepTest, GL_TEXTURE_2D);
		}
		/*if (ImGui::Checkbox("Borderless", &Borderless))
		{
			App->window->SetBorderless(Borderless);
		}
		if (ImGui::Checkbox("Resizable", &Resizable))
		{
			App->window->SetResizable(Resizable);
		}
		*/





	}
	//End Application//
	App->Options();


	if (Showtest) ImGui::ShowTestWindow();
	if (Showconsole) App->Console.Draw("Super Console", (bool*)false);
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

void ModuleImGui::PushFPSandMSPlot(uint fps, uint ms)
{
	static uint count = 0;

	if (count >= FPS_AND_MS_PLOT_DATA_LENGTH)
		for (int i = 0; i < FPS_AND_MS_PLOT_DATA_LENGTH - 1; i++)
		{
			Fps_Plot_Data[i] = Fps_Plot_Data[i + 1];
			Ms_Plot_Data[i] = Ms_Plot_Data[i + 1];
		}
	else
		count++;

	Fps_Plot_Data[count - 1] = fps;
	Ms_Plot_Data[count - 1] = ms;
}


void ModuleImGui::ChangeName(char* new_name)
{

	App->window->EngineName = new_name;


}
