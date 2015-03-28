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
	///*
	mesh->loadFromObjFile("wt_teapot.obj");
	mesh->setLocation(Vect4 (0, 0, -2));
	mesh->setScale(1);
	//*/
	//mesh->genPrimSphere(2);
	//mesh->genPrimTorus(4, 2);
	//mesh->genPrimBox(4, 2, 3);
	w.addMesh(mesh);


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

	cout << "Initialized\n";

	double i = 0;
	while(!ui_sdl->quit) {
		if(!ui_sdl->isPaused()) {
			//Prepare mesh
			i+=5;
			mesh->setRotation(d2r(i / 3.92), d2r(i), 0);
		}

		//mesh->setRotation(ui_sdl->rotation[0], ui_sdl->rotation[1], ui_sdl->rotation[2]);
		mesh->setLocation(ui_sdl->translation);
		mesh->setScale(ui_sdl->scale);

		//Render Mesh
		surface->clear(255);
		camera->renderMesh(&(ui_sdl->dragMesh), surface);
		camera->renderMeshes(world, surface);

	/*
		Point p1 {100, 300};
		Point p2 {200, 100};
		Point p3 {600, 100};

		Graphics g(surface);
		g.setColor(200,200,200);
		g.fillTri(p1,p2,p3);
		g.setColor(0,0,0);
		g.drawLine(p1.x, p1.y, p2.x, p2.y);
		g.drawLine(p1.x, p1.y, p3.x, p3.y);
	*/

		//Handle UI
		ui_sdl->mainloop();

		usleep(20 * 1000);
	}
}
