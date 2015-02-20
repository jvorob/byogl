#ifndef CAMERA_H
#define CAMERA_H

#include "mesh.h"
#include "drawing.h"

class Camera { 
	double fov; //is orthographic, so is just viewwidth

	public:
		Camera(double fov);
		void renderMesh(Mesh *m, Surface *s);
};

#endif
