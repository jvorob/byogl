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
	return (1 - t) * a + t * b;
}