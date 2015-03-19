#include <iostream>
#include <fstream>

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

#include <agar/core.h>
#include <agar/gui.h>

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

Uint32 timerCallback(AG_Timer *t, AG_Event *e);
void runAgar();
void runSDL();

World *world;
Mesh *mesh;
Camera *camera;
Surface *surface;
UI_Agar *ui_agar;
UI_SDL *ui_sdl;

int main() {
	int width = 800;
	int height = 800;

	//Prepare world
	World w;

	//Prepare mesh
	Mesh m(FALSE); //Not on heap
	//m.loadFromObjFile("wt_teapot.obj");
	m.setLocation(Vect4 (0, -1, -4));
	m.setScale(1);
	cerr << m.to_string() << '\n';
	w.addMesh(&m);
	/*
	m.genPrimCircle(Vect4(0,0,0), 2);
	m.genPrimCircle(Vect4(0,0,0), 0.8);
	m.genPrimCircle(Vect4(0,0,0), 0.1);
	m.genPrimCircle(Vect4(0,0,0), 0.01);
	*/
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

	//Get the other mesh
	Mesh *tempm;
	tempm = new Mesh();	
	tempm->loadFromObjFile("wt_teapot.obj");
	tempm->setLocation(Vect4(-2,-1,-5));
	tempm->setScale(1);
	w.addMesh(tempm);

	//Prepare render
	Surface s (width, height);
	s.clear(255);
	Camera c (90);


	//Hold onto things
	world = &w;
	mesh = &m;
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

Uint32 timerCallback(AG_Timer *t, AG_Event *e) {
	surface->clear(255);

	mesh->setLocation(ui_agar->transform);
	mesh->setScale(ui_agar->scale);
	mesh->setRotation(ui_agar->rotate[0],ui_agar->rotate[1],ui_agar->rotate[2]);

	camera->renderMesh(mesh, surface);
	Graphics g(surface);
	//g.drawLine(800,0,0,800);
	//g.drawLine(0,0,800,800);
	ui_agar->draw();
	return 50;
}

void runAgar() {
	//Prepare windows
	UI_Agar uia(surface);

	ui_agar = &uia;

	ui_agar->transform[1] = -1;
	ui_agar->transform[2] = -3;

	uia.addTimedEvent(timerCallback);
	uia.mainloop();

	//End things
	//uis.cleanup();
	uia.cleanup();
}

void runSDL() {
	UI_SDL uis(surface);
	ui_sdl = &uis;

	double i = 0;
	while(!ui_sdl->quit) {
		if(!ui_sdl->isPaused()) {
			//Prepare mesh
			i+=5;
			mesh->setRotation(d2r(i / 3.92), d2r(i), 0);

			//Render Mesh
			surface->clear(255);
			camera->renderMeshes(world, surface);
		}

		//Handle UI
		ui_sdl->mainloop();

		usleep(20 * 1000);
	}
}
