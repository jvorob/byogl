#include <iostream>

#include "camera.h"

Camera::Camera(double fov) {
	this->fov = fov;
}

void Camera::renderMesh(Mesh *m, Surface *s) {
	Graphics g (s);

	for(int i = 0; i < m->faces.size(); i++) {
		Vect4 v1 = m->verts[m->faces[i].v1 - 1];
		Vect4 v2 = m->verts[m->faces[i].v2 - 1];
		Vect4 v3 = m->verts[m->faces[i].v3 - 1];

		v1.x *= 400;
		v1.x += 500;

		v1.y *= 500;
		v1.y += 300;

		v2.x *= 400;
		v2.x += 500;

		v2.y *= 500;
		v2.y += 300;

		v3.x *= 400;
		v3.x += 500;

		v3.y *= 500;
		v3.y += 300;

		g.drawLine((int)v1.x, (int)v1.y, (int)v2.x, (int)v2.y);
		g.drawLine((int)v2.x, (int)v2.y, (int)v3.x, (int)v3.y);
		g.drawLine((int)v3.x, (int)v3.y, (int)v1.x, (int)v1.y);
	
	}
}
