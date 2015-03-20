#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <agar/core.h>
#include <agar/gui.h>


#include "drawing.h"
#include "util.h"
#include "matrix.h"
#include "mesh.h"
#include "world.h"

#define WINDOW_W 1400
#define WINDOW_H 1080

class UI_SDL{
	public:
		int quit = 0;

		UI_SDL(Surface *s, World *w); //Inits, creates windows
		void cleanup();
		void mainloop();
		void draw();
		Surface *surface;
		SDL_Surface *renBuffer; //3d is rendered to this

		int isPaused();

		Mesh dragMesh;
	private: 
		SDL_Renderer *ren;

		//Inits sdl, window, and renderer
		SDL_Renderer *setupWindow();


		World *world;


		enum Tool{ 
			Circle, 
			Line,
			END,
		};

		void changeTool(int delta);

		int paused = 0;

		int currtool = 0;
		int toolstate = 0;
	

		SDL_Point clicks[4];

		Vect4 screenToWorld(const SDL_Point p);
};

class UI_Agar{
	public:
		UI_Agar(Surface *s); //Inits, creates windows

		void cleanup();

		void draw();

		void addTimedEvent(Uint32 (*fn)(AG_Timer *, AG_Event *));
		void mainloop();

		Vect4 transform;
		Vect4 rotate;
		Vect4 scale;
	private: 
		Surface *surf;
		AG_Window *win;
		AG_Timer *timer;
		AG_Surface *renBuffer;//3d rendered to this
		AG_Surface *pixBuffer;//pixmap draws this
		AG_Pixmap *pixmap;

		int surfhandle;
		AG_PixelFormat *pixfrmt;
};

#endif
