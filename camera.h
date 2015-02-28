#ifndef CAMERA_H
#define CAMERA_H

#include "mesh.h"
#include "matrix.h"
#include "drawing.h"
#include "util.h"

class Camera { 
	double fov; //FOV in degrees

	public:
		Camera(double fov);
		void renderMesh(Mesh *m, Surface *s);
};

#endif
