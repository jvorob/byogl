#include "ui.h"

/*
int SDL_PointInRect(SDL_Point *p, SDL_Rect *r) {
	return (int)(p->x >= r->x && p->y >= r->y && p->x <= r->x + r->w && p->y <= r->y + r->h);
}
*/

UI_SDL::UI_SDL(Surface *s) {
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

