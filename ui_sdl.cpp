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

	//Setup fonts
	Font::init(ren);

	//Prepare rendering surface
	renBuffer = SDL_CreateRGBSurfaceFrom(
			s->raster,
			s->width,
			s->height,
			s->bytesPerPixel * 8,
			s->bytesPerPixel * s->width,
			0xFF0000,
			0x00FF00,
			0x0000FF,
			0);

	//Setup tools for curve drawing
	dragMesh = Mesh(FALSE); //Dont delete on remove
	//dragMesh.setRotation(0, PI - 1, 0);
	std::cout << "Current tool: " << toolString(currtool) << "\n";
	std::cout << "[ and ] to change tools\n";

	surface = s;
	world = w;


	//Prepare Widgets
	focusedWidget = NULL;
	
	//Curent tool name label
	toolLabel = new Label({804, 80}, "");
	widgets.push_back(toolLabel);

	setTool(0);

	//Shape selector buttons
	for(int i = 0; i < T_END; i++) {
		toolButtons[i] = new Button({804 + 100 * (i / 4), 100 + 30 * (i % 4)}, toolString(i));
		toolButtons[i]->setHandler(this);

		widgets.push_back(toolButtons[i]);
	}

	/*
	testTextBox = new TextBox({804, 250}, "woop");	
	widgets.push_back(testTextBox);

	testNumBox = new NumBox({804, 270}, 3.14159);	
	widgets.push_back(testNumBox);
	*/

	//Transformation input
	std::string coords[] = {"X", "Y", "Z"};
	widgets.push_back(new Label({804, 274},  "Rotate"));
	widgets.push_back(new Label({954, 274},  "Translate"));
	widgets.push_back(new Label({1104, 274}, "Scale"));
	for(int i = 0; i < 3; i++) {
		//Create labels for these
		widgets.push_back(new Label({804, 294 + 20 * i}, coords[i]));
		widgets.push_back(new Label({954, 294 + 20 * i}, coords[i]));
		widgets.push_back(new Label({1104, 294 + 20 * i}, coords[i]));

		rotNumBoxes[i] = new NumBox({824, 290 + 20 * i}, 0);
		widgets.push_back(rotNumBoxes[i]);

		transNumBoxes[i] = new NumBox({974, 290 + 20 * i}, 0);
		widgets.push_back(transNumBoxes[i]);

		scaleNumBoxes[i] = new NumBox({1124, 290 + 20 * i}, 1);
		widgets.push_back(scaleNumBoxes[i]);

	}
	
	//Canvas widget
	canvasArea = new Widget();
	canvasArea->bounds.x = 0;
	canvasArea->bounds.y = 0;
	canvasArea->bounds.w = s->width;
	canvasArea->bounds.h = s->height;
	widgets.push_back(canvasArea);

	//Mesh selector widgets
	prevMesh = new Button({804, 400}, "Prev.");
	prevMesh->setHandler(this);
	widgets.push_back(prevMesh);

	nextMesh = new Button({874, 400}, "Next");
	nextMesh->setHandler(this);
	widgets.push_back(nextMesh);

	meshName = new Label({804, 380}, "Boop");
	widgets.push_back(meshName);
}

void UI_SDL::setupWindow() {
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
	SDL_SetRenderDrawColor(ren, 210, 200, 160, 255);
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

	//Draw widgets
	for(int i = 0; i < widgets.size(); i++) {
		if(widgets[i])
			widgets[i]->draw(ren);
	}

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

			//Set focus
			if(e.type == SDL_MOUSEBUTTONDOWN) {
				SDL_Point p;
				p.x = e.button.x;
				p.y = e.button.y;

				int i;
				for(i = 0; i < widgets.size(); i++) {
					if(SDL_PointInRect(&p, &widgets[i]->bounds)) {
						if(focusedWidget)
							focusedWidget->loseFocus();
						focusedWidget = widgets[i];
						focusedWidget->gainFocus();
						break;
					}
				}

				//If clicked nowhere
				if(i == widgets.size()) {
					if(focusedWidget)
						focusedWidget->loseFocus();
					focusedWidget = NULL;
				}

			}

			if(focusedWidget == canvasArea) {
				if(doToolEvents(e))
					continue;
			}

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
				case SDL_KEYDOWN:
					switch(e.key.keysym.sym) {
						case SDLK_p:
							paused = !paused;
							break;
					}
				case SDL_MOUSEMOTION:
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
				case SDL_KEYUP:
				case SDL_TEXTINPUT:
				case SDL_TEXTEDITING:
				case SDL_WINDOWEVENT:
					if(focusedWidget) {
						focusedWidget->doEvent(e);
					}
					break;
				default:
					//if(DEBUGPRINT) 
						fprintf(stderr, "UNKNOWN EVENT TYPE\n");
					break;
			}
		}

	for(int i = 0; i < 3; i++) {
		rotation[i] =		rotNumBoxes[i]->getNum();
		translation[i] =	transNumBoxes[i]->getNum();
		scale[i] =			scaleNumBoxes[i]->getNum();
	}

	return;
}

int UI_SDL::isPaused() { return paused;}

void UI_SDL::handleButton(Button *b) {
	for(int i = 0; i < T_END; i++) {
		if(b == toolButtons[i]) {
			setTool(i);
			return;
		}
	}

	if(b == prevMesh) {
	} else if(b == nextMesh) {
	}
}

//Tools:

void UI_SDL::changeTool(int delta) {
	setTool(currtool + delta);
}

void UI_SDL::setTool(int t) {
	currtool = t;

	if(currtool < 0) {
		currtool += T_END;
	}
	currtool %= T_END;

	//Clear any partial progress
	toolstate = 0;
	dragMesh.clear();

	//std::cout << "Current tool: " << toolString(currtool) << "\n";
	toolLabel->setText(toolString(currtool));
}

std::string UI_SDL::toolString(int n) {
	switch (n) {
		case T_Circle:
			return "Circle";
			break;
		case T_Line:
			return "Line";
			break;
		case T_Hermite:
			return "Hermite";
			break;
		case T_Bezier:
			return "Bezier";
			break;
		case T_Box:
			return "Box";
			break;
		case T_Sphere:
			return "Sphere";
			break;
		case T_Torus:
			return "Torus";
			break;
	}

	return "Unknown";
}

Vect4 UI_SDL::screenToWorld(const SDL_Point p) {
		Vect4 temp;
		temp[0] = -1 * ((double)p.x - (surface->width / 2)) / (surface->width / 2);
		temp[1] = -1 * ((double)p.y - (surface->height / 2)) / (surface->width / 2);
		temp[2] = -1;

		return temp;
}

bool UI_SDL::doToolEvents(SDL_Event e) {
	SDL_Point p;
	switch (e.type) {
		case SDL_MOUSEMOTION:
			p.x = e.motion.x;
			p.y = e.motion.y;
			switch(currtool) {
				case T_Circle:
					dragMesh.clear();

					//If a center has been picked, draw the temp circle
					if(toolstate == 1) {
						Vect4 delta = screenToWorld(p) - clicks[0];
						double mag = delta.magnitude();

						dragMesh.genPrimCircle(clicks[0], mag);
					}
					break;
				case T_Line:
					dragMesh.clear();

					//If a center has been picked, draw the temp circle
					if(toolstate == 1) {
						Vect4 end = screenToWorld(p);

						dragMesh.genPrimEdge(clicks[0], end);
					}
					break;
				case T_Hermite:
					dragMesh.clear();
					
					if(toolstate >= 1) {
						Vect4 v[4];

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
				case T_Bezier:
					dragMesh.clear();
					
					if(toolstate >= 1) {
						Vect4 v[4];

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
				case T_Box:
					dragMesh.clear();

					//If a start point has been picked, draw the temp box
					if(toolstate == 1) {
						Vect4 end = screenToWorld(p);

						dragMesh.genPrimBox(end[0], end[1], end[1]);
					}
					break;
				case T_Sphere:
					dragMesh.clear();

					//If a center has been picked, draw the temp sphere
					if(toolstate == 1) {

						Vect4 delta = screenToWorld(p) - clicks[0];
						double mag = delta.magnitude();

						dragMesh.genPrimSphere(mag);
					}
					break;
				case T_Torus:
					dragMesh.clear();

					//If a start point has been picked, draw the temp torus
					if(toolstate == 1) {
						Vect4 end = screenToWorld(p);

						dragMesh.genPrimTorus(end[0], end[1]);
					}
					break;
				default:
					break;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			p.x = e.button.x;
			p.y = e.button.y;
			switch(currtool) {
				//keep track of the starting click
				case T_Circle:
				case T_Line:
				case T_Box:
				case T_Sphere:
				case T_Torus:
					//All of these just store the start point
					clicks[0] = screenToWorld(p);
					toolstate = 1;
					break;
				case T_Hermite:
					if(toolstate == 0 || toolstate == 2) {
						clicks[toolstate] = screenToWorld(p);
						toolstate++;
					} else {
						std::cerr << "ERR in mousedown Hermite\n";
					}
					break;
				case T_Bezier:
					if(toolstate == 0 || toolstate == 2) {
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
				case T_Circle:
				case T_Line:
				case T_Sphere:
				case T_Box:
				case T_Torus:
					//Make it so
					if(toolstate == 1) {
						world->addMesh(new Mesh(dragMesh));
						dragMesh.clear();
						toolstate = 0;
					}
					break;
				case T_Hermite:
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
				case T_Bezier:
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

