#include <iostream>
#include <fstream>

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

#include "drawing.h"
#include "mesh.h"
#include "camera.h"
#include "ui.h"
#include "util.h"

#define MAX_HSV 1536

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

	//Prepare mesh
	Mesh m;
	m.loadFromObjFile("wt_teapot.obj");
	m.setLocation(Vect4 (0, -1, -2));
	m.setScale(1);
	cerr << m.to_string() << '\n';

	//Prepare render
	Surface s (width, height);
	s.clear(255);
	Camera c (90);

	//Prepare window
	UI ui (&s);

	//Write to file
	s.write_to_file("out.ppm");
	cout << "Done" << endl;

	for(double i = 0; i < 300; i++ ) {
		//Make/draw screen
		m.setRotation(d2r(i / 3.92), d2r(i), 0);
		s.clear(255);
		c.renderMesh(&m, &s);
		ui.draw();
		//cout << "sleeping... " << i << endl;
		//usleep(20 * 1000);
	}

	//End things
	ui.cleanup();
	cout << " done" << endl;
}
