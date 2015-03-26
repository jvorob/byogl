#ifndef CAMERA_H
#define CAMERA_H

#include "mesh.h"
#include "matrix.h"
#include "drawing.h"
#include "util.h"
#include "entity.h"
#include "world.h"

class Camera: public Entity { 
	double fov; //FOV in degrees

	public:
		Camera(double fov);
		void renderMesh(Mesh *m, Surface *s);
		void renderMeshes(World *w, Surface *s);

		static bool isBackface(Vect4 v1, Vect4 v2, Vect4 v3);
};

#endif
