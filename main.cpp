#include <iostream>
#include <fstream>

#include <cmath>
#include <cstdlib>
#include <cstdio>

#include "drawing.h"
#include "mesh.h"
#include "camera.h"

#define MAX_HSV 1536
#define PI 3.14159265358979323846264
#define TWOPI (2 * PI)

using namespace std;

double max(double a, double b) {
	if(a < b) 
		return b;
	else 
		return a;
}
double min(double a, double b) {
	if(a < b) 
		return a;
	else 
		return b;
}


int main() {
	int width = 1000;
	int height = 1000;

	Surface s (width, height);

	s.clear(255);

	Mesh m;

	m.loadFromObjFile("wt_teapot.obj");
	
	Camera c (4);
	c.renderMesh(&m, &s);

	s.write_to_file("out.ppm");
	cout << "Done" << endl;
}
