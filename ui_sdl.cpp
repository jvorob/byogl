#include "ui.h"
#include <iostream>

/*
int SDL_PointInRect(SDL_Point *p, SDL_Rect *r) {
	return (int)(p->x >= r->x && p->y >= r->y && p->x <= r->x + r->w && p->y <= r->y + r->h);
}
*/

UI_SDL::UI_SDL(Surface *s, World *w) {
	//Init sdl, window, renderer
	paused = 1;
	setupWindow();

	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

	renBuffer = SDL_CreateRGBSurfaceFrom(
			s->raster,
			s->width,
			s->height,
			s->bpp,
			s->bpp / 8 * s->width,
			0xFF0000,
			0x00FF00,
			0x0000FF,
			0);
	

	dragMesh = Mesh(FALSE); //Dont delete on remove
	//dragMesh.setRotation(0, PI - 1, 0);

	surface = s;
	world = w;
}

SDL_Renderer *UI_SDL::setupWindow() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		printf("SDL_Init Error: %s",SDL_GetError());
		exit(-1);
	}

	SDL_Window *win = SDL_CreateWindow(
		"Graphics Demo 0.1", 
		SDL_WINDOWPOS_CENTERED, 
		100, 
		WINDOW_W, 
		WINDOW_H, 
		SDL_WINDOW_SHOWN);
	if (win == NULL){
		printf("SDL_CreateWindow Error: %s",SDL_GetError());
		SDL_Quit();
		exit(-1);
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == NULL){
		SDL_DestroyWindow(win);
		printf("SDL_CreateRenderer Error: %s",SDL_GetError());
		SDL_Quit();
		exit(-1);
	}

}

void UI_SDL::cleanup() {
	SDL_Quit();
}

void UI_SDL::draw() {
	//Clear
	SDL_RenderClear(ren);

	//Draw Buffer
	SDL_Texture *tempTexture = SDL_CreateTextureFromSurface(ren, renBuffer);

	SDL_Rect srcrect, dstrect;
	srcrect.x = 0;
	srcrect.y = 0;
	srcrect.w = renBuffer->w;
	srcrect.h = renBuffer->h;

	dstrect.x = 0;
	dstrect.y = 0;
	dstrect.w = renBuffer->w;
	dstrect.h = renBuffer->h;

	SDL_RenderCopy(ren, tempTexture, &srcrect, &dstrect);

	SDL_DestroyTexture(tempTexture);

	//Flush Buffer
	SDL_RenderPresent(ren);
}


void UI_SDL::mainloop() {
		SDL_Event e;
		

		//droppedframes = MAXDROPPEDFRAMES;

		draw();
		SDL_RenderPresent(ren);

		
		//fprintf(stderr, "b");
		/*
		if(SDL_GetTicks() - updatetime > 1000) {
			updatetime = SDL_GetTicks();
			if(DEBUGPRINT)
				fprintf(stderr, "%d\n", tickspersecond);
			tickspersecond = 0;
		}
		*/

		//Get all events
		while (SDL_PollEvent(&e) != 0 && !quit) {
			if(doToolEvents(e))
				continue;

			switch (e.type) {
				case SDL_QUIT:
					quit = 1;
					break;
				case SDL_USEREVENT:
					/*
					if(!droppedframes || uistate.paused)
						break; //If it drops too many frames, clear the event queue
					droppedframes--;
					//fprintf(stderr, "c");
					tickspersecond++;
					for(i = 0; i < STEPSPERTICK; i++) {
						doDemos(uistate.demoselected, mysim);
						sim_step(mysim, TICKTIME);
					}
					//printGrid(mysim);
					tparams.timerlock = 0;
					*/
					break;
				case SDL_MOUSEMOTION:
					break;
				case SDL_MOUSEBUTTONDOWN:
					break;
				case SDL_MOUSEBUTTONUP:
					break;
				case SDL_KEYDOWN:
					switch(e.key.keysym.sym) {
						case SDLK_p:
							paused = !paused;
							break;
					}
					break;
				case SDL_KEYUP:
				case SDL_TEXTINPUT:
				case SDL_TEXTEDITING:
					break;
				case SDL_WINDOWEVENT:
					switch (e.window.event) {
						case SDL_WINDOWEVENT_ENTER:
							//uistate.mouseinwindow = 1;
						case SDL_WINDOWEVENT_LEAVE:
							//uistate.mouseinwindow = 0;
							break;
					}
					break;
				default:
					//if(DEBUGPRINT) 
						fprintf(stderr, "UNKNOWN EVENT TYPE\n");
					break;
			}
		}

	return;
}

int UI_SDL::isPaused() { return paused;}

//Tools:

void UI_SDL::changeTool(int delta) {
	currtool += delta;

	if(currtool < 0) {
		currtool += END;
	}
	currtool %= END;

	//Clear any partial progress
	toolstate = 0;
	dragMesh.clear();

	std::cerr << "Current tool: " << toolString(currtool) << "\n";
}

std::string UI_SDL::toolString(int n) {
	switch (n) {
		case Circle:
			return "Circle";
			break;
		case Line:
			return "Line";
			break;
		case Hermite:
			return "Hermite";
			break;
		case Bezier:
			return "Bezier";
			break;
	}

	return "Unkown";
}

Vect4 UI_SDL::screenToWorld(const SDL_Point p) {
		Vect4 temp;
		temp[0] = -1 * ((double)p.x - (surface->width / 2)) / (surface->width / 2);
		temp[1] = -1 * ((double)p.y - (surface->height / 2)) / (surface->width / 2);
		temp[2] = -1;

		return temp;
}

bool UI_SDL::doToolEvents(SDL_Event e) {
	switch (e.type) {
		case SDL_MOUSEMOTION:
			switch(currtool) {
				case Circle:
					dragMesh.clear();

					//If a center has been picked, draw the temp circle
					if(toolstate == 1) {
						SDL_Point p;
						p.x = e.motion.x;
						p.y = e.motion.y;

						Vect4 delta = screenToWorld(p) - clicks[0];
						double mag = delta.magnitude();

						dragMesh.genPrimCircle(clicks[0], mag);
					}
					break;
				case Hermite:
					dragMesh.clear();
					
					if(toolstate >= 1) {
						Vect4 v[4];

						SDL_Point p;
						p.x = e.motion.x;
						p.y = e.motion.y;

						Vect4 curr = screenToWorld(p);

						if(toolstate == 1) {
							dragMesh.genPrimEdge(clicks[0], curr);
						} else {
							dragMesh.genPrimEdge(clicks[0], clicks[1]);
							if(toolstate == 3) {
								dragMesh.genPrimEdge(clicks[2], curr);
								dragMesh.genPrimHermite(
										clicks[0],
										clicks[2],
										clicks[1],
										curr);
							}
						}
					}
					break;
				case Bezier:
					dragMesh.clear();
					
					if(toolstate >= 1) {
						Vect4 v[4];

						SDL_Point p;
						p.x = e.motion.x;
						p.y = e.motion.y;

						Vect4 curr = screenToWorld(p);

						if(toolstate == 1) {
							dragMesh.genPrimEdge(clicks[0], curr);
						} else {
							dragMesh.genPrimEdge(clicks[0], clicks[1]);
							if(toolstate == 3) {
								dragMesh.genPrimEdge(clicks[2], curr);
								dragMesh.genPrimBezier(
										clicks[0],
										clicks[1],
										curr,
										clicks[2]);
							}
						}
					}
					break;
				default:
					break;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch(currtool) {
				//keep track of the starting click
				case Circle:
					SDL_Point p;
					p.x = e.button.x;
					p.y = e.button.y;
					clicks[0] = screenToWorld(p);
					toolstate = 1;
					break;
				case Hermite:
					if(toolstate == 0 || toolstate == 2) {
						SDL_Point p;
						p.x = e.button.x;
						p.y = e.button.y;
						clicks[toolstate] = screenToWorld(p);
						toolstate++;
					} else {
						std::cerr << "ERR in mousedown Hermite\n";
					}
					break;
				case Bezier:
					if(toolstate == 0 || toolstate == 2) {
						SDL_Point p;
						p.x = e.button.x;
						p.y = e.button.y;
						clicks[toolstate] = screenToWorld(p);
						toolstate++;
					} else {
						std::cerr << "ERR in mousedown Bezier\n";
					}
					break;
				default:
					break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch(currtool) {
				case Circle:
					//Make it so
					if(toolstate == 1) {
						world->addMesh(new Mesh(dragMesh));
						dragMesh.clear();
						toolstate = 0;
					}
					break;
				case Hermite:
					if(toolstate == 1 || toolstate == 3) {
						SDL_Point p;
						p.x = e.button.x;
						p.y = e.button.y;
						clicks[toolstate] = screenToWorld(p);
						toolstate++;
					} else {
						std::cerr << "ERR in mouseup Hermite\n";
					}

					if(toolstate == 4) { //finished
						dragMesh.clear();
						dragMesh.genPrimHermite(
								clicks[0],
								clicks[2],
								clicks[1],
								clicks[3]);
						world->addMesh(new Mesh(dragMesh));
						dragMesh.clear();
						toolstate = 0;
					}
					break;
				case Bezier:
					if(toolstate == 1 || toolstate == 3) {
						SDL_Point p;
						p.x = e.button.x;
						p.y = e.button.y;
						clicks[toolstate] = screenToWorld(p);
						toolstate++;
					} else {
						std::cerr << "ERR in mouseup Bezier\n";
					}

					if(toolstate == 4) { //finished
						dragMesh.clear();
						dragMesh.genPrimBezier(
								clicks[0],
								clicks[1],
								clicks[3],
								clicks[2]);
						world->addMesh(new Mesh(dragMesh));
						dragMesh.clear();
						toolstate = 0;
					}
					break;
				default:
					break;
			}
			break;
		case SDL_KEYDOWN:
			switch(e.key.keysym.sym) {
				case SDLK_RIGHTBRACKET:
					changeTool(1);
					break;
				case SDLK_LEFTBRACKET:
					changeTool(-1);
					break;
				default:
					return false;
			}
			break;
		case SDL_WINDOWEVENT:
			switch (e.window.event) {
				case SDL_WINDOWEVENT_ENTER:
					//uistate.mouseinwindow = 1;
				case SDL_WINDOWEVENT_LEAVE:
					//uistate.mouseinwindow = 0;
					break;
			}
			return false;
			break;
		default:
			return false;
	}
	return true;
}

