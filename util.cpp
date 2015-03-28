#include "util.h"

#include <iostream>

void boop(int i) {
	std::cerr << "Beep-" << i << "\n";
}

void boopd(double d) {
	fprintf(stderr, "Beep : %0.3lf\n", d);
}

double d2r(double d) { 
	return d / 180 * PI;
}

double lerp(double a, double b, double t) {
	return (1.0 - t) * a + t * b;
}

void printSDLError() {
	std::fprintf(stderr, "SDL Errror:%s\n", SDL_GetError());
}

bool SDL_PointInRect(SDL_Point *p, SDL_Rect *r) {
	return (bool)(p->x >= r->x && p->y >= r->y && p->x <= r->x + r->w && p->y <= r->y + r->h);
}
