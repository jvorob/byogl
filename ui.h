#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>

#include "drawing.h"

#define WINDOW_W 1400
#define WINDOW_H 1080

class UI {
	private: 
		SDL_Renderer *ren;
		SDL_Surface *renBuffer; //render 3d to this

		SDL_Renderer *setupWindow();
	public: 
		UI(Surface *s); //Inits, creates windows
		void cleanup();

		void draw();


	/*
	//THESE ARENT IMPLEMENTED

	void mainloop();
	//Inits sdl, window, and renderer

	Uint32 timercallback(Uint32 interval, void *param);
	//void doMouseEvent(struct _uistate *ui, sim_Sim *s);

	void starttimer();
	void stoptimer();
	*/
};

#endif
