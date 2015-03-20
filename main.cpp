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
#include "world.h"
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

void runSDL();

World *world;
Mesh *mesh;
Camera *camera;
Surface *surface;
UI_SDL *ui_sdl;

int main() {
	int width = 800;
	int height = 800;

	//Prepare world
	World w;

	//Prepare mesh
	mesh = new Mesh(); //Not on heap
	mesh->loadFromObjFile("wt_teapot.obj");
	mesh->setLocation(Vect4 (0, 0, -2));
	mesh->setScale(1);
	//w.addMesh(mesh);

	/*
	m.genPrimCircle(Vect4(0,0,0), 2);
	m.genPrimCircle(Vect4(0,0,0), 0.8);
	m.genPrimCircle(Vect4(0,0,0), 0.1);
	m.genPrimCircle(Vect4(0,0,0), 0.01);
	*/
	/*
	{
		Vect4 a(-1,-1,0);
		Vect4 b(0,-1,0);
		Vect4 c(0,2,0);
		Vect4 d(1,1,0);

		m.genPrimCircle(a, 0.1);
		m.genPrimCircle(b, 0.1);
		m.genPrimCircle(c, 0.1);
		m.genPrimCircle(d, 0.1);
		m.genPrimBezier(a, b, c, d);
	}
	{
		Vect4 p0(1,0,0);
		Vect4 p1(-1,0,0);
		Vect4 r0(0,-1,0);
		Vect4 r1(0,1.5,0);

		m.genPrimCircle(p0, 0.1);
		m.genPrimCircle(p1, 0.1);
		m.genPrimCircle(p0 + r0, 0.02);
		m.genPrimCircle(p1 + r1, 0.02);
		m.genPrimHermite(p0, p1, r0, r1);
	}
	*/

	//Prepare render
	Surface s (width, height);
	s.clear(255);
	Camera c (90);


	//Hold onto things
	world = &w;
	camera = &c;
	surface = &s;

	//Render once
	s.clear(255);
	c.renderMeshes(&w, &s);

	//Write to file
	s.write_to_file("out.ppm");
	cout << "Done Writing" << endl;


	runSDL();

	cout << " done" << endl;
}

void runSDL() {
	UI_SDL uis(surface, world);
	ui_sdl = &uis;

	world->addMesh(&ui_sdl->dragMesh);

	double i = 0;
	while(!ui_sdl->quit) {
		if(!ui_sdl->isPaused()) {
			//Prepare mesh
			i+=5;
			mesh->setRotation(d2r(i / 3.92), d2r(i), 0);
		}

			//Render Mesh
			surface->clear(255);
			camera->renderMesh(&(ui_sdl->dragMesh), surface);
			camera->renderMeshes(world, surface);

		//Handle UI
		ui_sdl->mainloop();

		usleep(20 * 1000);
	}
}
