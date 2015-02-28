#include "util.h"

#include <iostream>

void beep(int i) {
	std::cerr << "Beep-" << i << "\n";
}

double d2r(double d) { 
	return d / 180 * PI;
}
