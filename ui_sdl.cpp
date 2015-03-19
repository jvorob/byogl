#include "ui.h"
#include <iostream>

/*
int SDL_PointInRect(SDL_Point *p, SDL_Rect *r) {
	return (int)(p->x >= r->x && p->y >= r->y && p->x <= r->x + r->w && p->y <= r->y + r->h);
}
*/

UI_SDL::UI_SDL(Surface *s) {
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

	dstrect.x = 100;
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
		
		std::cerr << currtool << "\n";

		//droppedframes = MAXDROPPEDFRAMES;

		//doMouseEvent(&uistate, mysim);
			
		//drawGrid(ren, mysim, &gridviewport);
		//drawIso(ren, mysim, &isoviewport);
		//drawTools(ren, &uistate, &toolbox);
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
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					//doMouseEvent(&e, &selected, &hovered, mysim);
					break;
				case SDL_KEYUP:
					switch(e.key.keysym.sym) {
						case SDLK_RIGHTBRACKET:
							changeTool(1);
							break;
						case SDLK_LEFTBRACKET:
							changeTool(-1);
							break;
						case SDLK_p:
							paused = !paused;
							break;
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
	
}
