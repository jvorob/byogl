#ifndef UTIL_H
#define UTIL_C

#define PI 3.141592653
#define TWOPI (2 * 3.141592653)

#define TRUE 1
#define FALSE 0

void boop(int i); //prints i to cerr
void boopd(double d); //prints d to cerr
double d2r(double d); //converts degrees to radians

//t0=a, t1=b
double lerp(double a, double b, double t);

#endif
