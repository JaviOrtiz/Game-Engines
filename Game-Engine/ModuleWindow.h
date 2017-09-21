#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "Application.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	int		Window_Width = 0;
	int		Window_Height = 0;
	bool	Fullscreen = false;
	bool	Fullscreen_Desktop = false;
	bool	Resizable;
	bool	Borderless;
	float	Brightness;



	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);
	void SetFullscreen(bool fullscreen);
	void SetResizable(bool resizable);
	void SetBorderless(bool borderless);
	void SetFullScreenDesktop(bool fullscreendesktop);
	void SetBrightness(float brightness);

	bool IsFullscreen();
	bool IsResizable();
	bool IsBorderless();
	bool IsFullscreenDesktop();
	float GetBrightness();



public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

};

#endif // __ModuleWindow_H__