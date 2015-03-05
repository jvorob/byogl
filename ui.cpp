#include "ui.h"

int SDL_PointInRect(SDL_Point *p, SDL_Rect *r) {
	return (int)(p->x >= r->x && p->y >= r->y && p->x <= r->x + r->w && p->y <= r->y + r->h);
}

UI::UI(Surface *s) {
	//Init sdl, window, renderer
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
	

	/*
	if((iconSurf = SDL_LoadBMP("icons.bmp")) == NULL) {
		printf("Failed to load icons.bmp");
		exit(-1);
	}
	iconText = SDL_CreateTextureFromSurface(ren, iconSurf);

	isoSurf = SDL_LoadBMP("tile.bmp");
	isoText = SDL_CreateTextureFromSurface(ren, isoSurf);
	*/

	/*
	if((tparams.CustomTimerEvent = SDL_RegisterEvents(1)) == -1){
		printf("Failed to register customevent\n");
		exit(-1);
	}
	tparams.timerlock = 0;

	if((timerid = SDL_AddTimer(TIMERINTERVAL, timercallback, (void *) &tparams)))
		;
	else
		fprintf(stderr,"Timer failed to start\n");
	*/


	//updatetime = SDL_GetTicks();
}

SDL_Renderer *UI::setupWindow() {
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

void UI::cleanup() {
	SDL_Quit();
}

void UI::draw() {
	//Clear
	SDL_RenderClear(ren);

	//Draw Buffer
	SDL_Texture *tempTexture = SDL_CreateTextureFromSurface(ren, renBuffer);

	SDL_Rect srcrect, dstrect;
	srcrect.x = 0;
	srcrect.y = 0;
	srcrect.w = renBuffer->w;
	srcrect.h = renBuffer->h;
	dstrect = srcrect;

	SDL_RenderCopy(ren, tempTexture, &srcrect, &dstrect);

	SDL_DestroyTexture(tempTexture);

	//Flush Buffer
	SDL_RenderPresent(ren);
}

/*
void UI::mainloop() {
	while (!quit) {
		droppedframes = MAXDROPPEDFRAMES;

		doMouseEvent(&uistate, mysim);
			
		drawGrid(ren, mysim, &gridviewport);
		drawIso(ren, mysim, &isoviewport);
		drawTools(ren, &uistate, &toolbox);
		SDL_RenderPresent(ren);

		
		//fprintf(stderr, "b");
		if(SDL_GetTicks() - updatetime > 1000) {
			updatetime = SDL_GetTicks();
			if(DEBUGPRINT)
				fprintf(stderr, "%d\n", tickspersecond);
			tickspersecond = 0;
		}


		while ((temp = SDL_PollEvent(&e)) != 0 && !quit) {
			switch (e.type) {
				case SDL_QUIT:
					quit = 1;
					break;
				case SDL_USEREVENT:
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
					break;
				case SDL_MOUSEMOTION:
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					//doMouseEvent(&e, &selected, &hovered, mysim);
					break;
				case SDL_WINDOWEVENT:
					switch (e.window.event) {
						case SDL_WINDOWEVENT_ENTER:
							uistate.mouseinwindow = 1;
						case SDL_WINDOWEVENT_LEAVE:
							uistate.mouseinwindow = 0;
							break;
					}
					break;
				default:
					if(DEBUGPRINT) 
						fprintf(stderr, "UNKNOWN EVENT TYPE\n");
					break;
			}
		}
	}
}

void drawGrid(SDL_Renderer *r, sim_Sim *s, SDL_Rect *viewport) {
	int i, j;
	sim_Cell *cell;
	SDL_Rect rect;
	rect.w = CELLSIZE;
	rect.h = CELLSIZE;

	SDL_SetRenderDrawColor(r, 200, 255, 200, 255);
	SDL_RenderClear(r);


	for(i = 0; i < s->h; i++) {
		for(j = 0; j < s->w; j++) {
			cell = &(cellAt(j,i,s));

			int rg = 255 - (int)cell->height;
			rg = rg < 0 ? 0 : rg;
			rect.x = j * CELLSIZE + viewport->x;
			rect.y = i * CELLSIZE + viewport->y;

			SDL_SetRenderDrawColor(r, rg, rg, 255, 255); 
			SDL_RenderFillRect(r, &rect);

			SDL_SetRenderDrawColor(r, 0, 0, 0, 255);


		}
	}	
	if(DRAWFLOW)
		for(i = 0; i < s->h; i++) {
			for(j = 0; j < s->w; j++) {
				cell = &(cellAt(j,i,s));
				rect.x = j * CELLSIZE + viewport->x;
				rect.y = i * CELLSIZE + viewport->y;
				int len;
				len = cell->flow[RT];
				SDL_SetRenderDrawColor(r, len > 0 ? 0 : 255, 0, 0, 255); 
				len = abs(len);
				SDL_RenderDrawLine(r, 
					rect.x + CELLSIZE / 2,
					rect.y + CELLSIZE / 2,
					rect.x + CELLSIZE / 2 + len,
					rect.y + CELLSIZE / 2);
				len = cell->flow[LF];
				SDL_SetRenderDrawColor(r, len > 0 ? 0 : 255, 0, 0, 255); 
				len = abs(len);
				SDL_RenderDrawLine(r, 
					rect.x + CELLSIZE / 2,
					rect.y + CELLSIZE / 2,
					rect.x + CELLSIZE / 2 - len,
					rect.y + CELLSIZE / 2);
				len = cell->flow[DN];
				SDL_SetRenderDrawColor(r, len > 0 ? 0 : 255, 0, 0, 255); 
				len = abs(len);
				SDL_RenderDrawLine(r, 
					rect.x + CELLSIZE / 2,
					rect.y + CELLSIZE / 2,
					rect.x + CELLSIZE / 2,
					rect.y + CELLSIZE / 2 + len);
				len = cell->flow[UP];
				SDL_SetRenderDrawColor(r, len > 0 ? 0 : 255, 0, 0, 255); 
				len = abs(len);
				SDL_RenderDrawLine(r, 
					rect.x + CELLSIZE / 2,
					rect.y + CELLSIZE / 2,
					rect.x + CELLSIZE / 2,
					rect.y + CELLSIZE / 2 - len);
			}
		}
}

void drawIso(SDL_Renderer *r, sim_Sim *s, SDL_Rect *viewport) {
	int i, j;
	sim_Cell *cell;
	SDL_Rect srcrect, dstrect,vertrect;

	srcrect.x = 0;
	srcrect.y = 0;
	srcrect.w = ISOWIDTH;
	srcrect.h = ISOHEIGHT;

	dstrect.w = ISOWIDTH;
	dstrect.h = ISOHEIGHT;

	vertrect.w = ISOWIDTH / 2;


	for(i = 0; i < s->h; i++) {
		for(j = s->w - 1; j >= 0; j--) {
			cell = &(cellAt(j,i,s));

			dstrect.x = j * ISOWIDTH / 2 + i * ISOWIDTH / 2 + viewport->x;
			dstrect.y = i * ISOHEIGHT / 2 - j * ISOHEIGHT / 2 + viewport->y;

			vertrect.x = dstrect.x;
			vertrect.y = dstrect.y + ISOHEIGHT / 2;
			vertrect.h = cell->height * HEIGHTMULT;
			vertrect.y -= vertrect.h;

			dstrect.y -= cell->height * HEIGHTMULT;


			SDL_SetRenderDrawColor(r, 0, 0, 100, 255);
			SDL_RenderFillRect(r, &vertrect);

			vertrect.x += ISOWIDTH / 2;
			SDL_SetRenderDrawColor(r, 0, 0, 150, 255);
			SDL_RenderFillRect(r, &vertrect);


			SDL_RenderCopy(r, isoText, &srcrect, &dstrect);
		}
	}	
}

void drawTools(SDL_Renderer *r, struct _uistate *ui, SDL_Rect *viewport) {
	SDL_Rect temp;
	temp.x = 0;//copy from origin on src texture
	temp.y = 0;
	temp.w = viewport->w;
	temp.h = viewport->h;
	SDL_RenderCopy(r, iconText, &temp, viewport);

	if(ui->hoverx != -1) {//Draw hover rectangle
		temp.x = viewport->x;
		temp.y = viewport->y;
		temp.w = 100;
		temp.h = 100;

		temp.y += 100 * ui->hovery;
		temp.x += 100 * ui->hoverx;

		SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
		SDL_RenderDrawRect(r, &temp);
	}

	temp.w = 100;
	temp.h = 100;

	temp.x = viewport->x;
	temp.y = viewport->y + 100 * ui->toolselected;
	SDL_SetRenderDrawColor(r, 0, 0, 255, 100);
	SDL_RenderFillRect(r, &temp);


	temp.x = viewport->x + 200;
	temp.y = viewport->y + 100 * ui->demoselected;
	SDL_SetRenderDrawColor(r, 0, 0, 255, 100);
	SDL_RenderFillRect(r, &temp);

	if(ui->paused) {
		temp.x = viewport->x + 300;
		temp.y = viewport->y;
		SDL_SetRenderDrawColor(r, 0, 0, 255, 100);
		SDL_RenderFillRect(r, &temp);
	}
}

void doMouseEvent(struct _uistate *ui, sim_Sim *s) {
	static int laststate, clicked;
	Uint32 buttons;
	SDL_Point loc;
	
	buttons = SDL_GetMouseState(&(loc.x), &(loc.y));
	clicked = laststate & SDL_BUTTON(SDL_BUTTON_LEFT) && !(buttons & SDL_BUTTON(SDL_BUTTON_LEFT));
	buttons &= SDL_BUTTON(SDL_BUTTON_LEFT);

	ui->hoverx = -1;

	if(SDL_PointInRect(&loc, &toolbox)) {//Handle clicks in the ui
		ui->hovery = (loc.y - toolbox.y) / 100;
		ui->hoverx = (loc.x - toolbox.x) / 100;
		switch(ui->hoverx) {
			case 0://Tools
				if(buttons)
					ui->toolselected = ui->hovery;
				break;
			case 1://None
				break;
			case 2://demo
				if(buttons)
					ui->demoselected = ui->hovery;
				break;
			case 3://Pause
				if(clicked && ui->hovery == 0)
					ui->paused = !ui->paused;
				break;
		}
		//Each button in the toolbox is 100px high
	} else if(SDL_PointInRect(&loc, &gridviewport)) {//Handle clicks in the wave grid
		switch(ui->toolselected) {
			case TOOL_ADD: //Add water
				if(!clicked)
					break;
				loc.x -= gridviewport.x;
				loc.y -= gridviewport.y;
				loc.x /= CELLSIZE;
				loc.y /= CELLSIZE;
				cellAt(loc.x, loc.y, s).height += 200;
				//cellAt(loc.x - 1, loc.y, s).height += 200;
				//cellAt(loc.x + 1, loc.y, s).height += 200;
				//cellAt(loc.x, loc.y - 1, s).height += 200;
				//cellAt(loc.x, loc.y + 1, s).height += 200;
				break;
			case TOOL_REMOVE:
				if(!clicked)
					break;
				loc.x -= gridviewport.x;
				loc.y -= gridviewport.y;
				loc.x /= CELLSIZE;
				loc.y /= CELLSIZE;
				cellAt(loc.x, loc.y, s).height = 0;
				//Remove water

				break;
		}
	} else {//???
		
	}

	laststate = buttons;

}


Uint32 timercallback(Uint32 interval, void *param) {
	struct timerparams *tparams = (struct timerparams *) param;
	SDL_Event event;
	SDL_zero(event);
	event.type = tparams->CustomTimerEvent;

	//if(!tparams->timerlock){
		SDL_PushEvent(&event);
		tparams->timerlock = 1;
	//}

	return interval;
}

*/
