#ifndef UTIL_H
#define UTIL_H

#define PI 3.141592653
#define TWOPI (2 * 3.141592653)

#define TRUE 1
#define FALSE 0

#include <SDL2/SDL.h>

void boop(int i); //prints i to cerr
void boopd(double d); //prints d to cerr
double d2r(double d); //converts degrees to radians

//t0=a, t1=b
double lerp(double a, double b, double t);

void printSDLError();

bool SDL_PointInRect(SDL_Point *p, SDL_Rect *r);
#endif
