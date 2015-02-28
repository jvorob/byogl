#include <iostream>

#include "camera.h"

Camera::Camera(double fov) {
	this->fov = fov;
}

void Camera::renderMesh(Mesh *m, Surface *s) {

	Graphics g (s);


	Mat4 temp = Mat4::IdentityMat();
	temp = Mat4::mult(Mat4::ScaleMat(400, 500, 400), temp);
	temp = Mat4::mult(Mat4::TranslateMat(500, 300, 0), temp);

	m->applyTransform(temp);

	for(int i = 0; i < m->faces.size(); i++) {
		Vect4 v1 = m->transVerts[m->faces[i].v1 - 1];
		Vect4 v2 = m->transVerts[m->faces[i].v2 - 1];
		Vect4 v3 = m->transVerts[m->faces[i].v3 - 1];


		g.drawLine((int)v1.coord[0], (int)v1.coord[1], (int)v2.coord[0], (int)v2.coord[1]);
		g.drawLine((int)v2.coord[0], (int)v2.coord[1], (int)v3.coord[0], (int)v3.coord[1]);
		g.drawLine((int)v3.coord[0], (int)v3.coord[1], (int)v1.coord[0], (int)v1.coord[1]);
	
	}
}
