#ifndef UI_H
#define UI_H

#include <vector>

#include <SDL2/SDL.h>


#include "drawing.h"
#include "util.h"
#include "matrix.h"
#include "mesh.h"
#include "world.h"
#include "widgets.h"

#define WINDOW_W 1400
#define WINDOW_H 1080

class UI_SDL : public ButtonHandler{
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

		Vect4 rotation;
		Vect4 translation;
		Vect4 scale;
	private: 
		//Big Picture stuff ======
		int paused = 0;
		World *world;
		SDL_Renderer *ren;

		//Inits sdl, window, and renderer
		void setupWindow();

		enum Tool{ 
			Circle, 
			Line,
			Hermite,
			Bezier,
			Box,
			Sphere,
			Torus,
			END,
		};

		//Widgets
		std::vector<Widget *> widgets;
		Label *toolLabel;
		Button *toolButtons[END];
		Widget *canvasArea;
		//TextBox *testTextBox;
		//NumBox *testNumBox;

		NumBox *rotNumBoxes[3];
		NumBox *scaleNumBoxes[3];
		NumBox *transNumBoxes[3];
		
		
		Widget *focusedWidget;

		void handleButton(Button *b);


		//Drawing tools
		int currtool = 0;
		int toolstate = 0;
		Vect4 clicks[4];


		void changeTool(int delta);
		void setTool(int n);
		std::string toolString(int n);
		Vect4 screenToWorld(const SDL_Point p);
		bool doToolEvents(SDL_Event e); //return true if event handled

};

#endif
