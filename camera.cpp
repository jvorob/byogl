#include <iostream>
#include <cmath>

#include "camera.h"

Camera::Camera(double fov) {
	this->fov = fov;
}

void Camera::renderMesh(Mesh *m, Surface *s) {
	Graphics g (s);
	Vect4 *transVerts = new Vect4[m->verts.size()]; //Temporary storage for transformed vertices
	Mat4 temp = Mat4::IdentityMat();
	double sf = 1;

	//Prepare transform
	temp = Mat4::mult(Mat4::ScaleMat(4, 5, 4), temp);
	temp = Mat4::mult(Mat4::ScaleMat(sf, sf, sf), temp);
	//temp = Mat4::mult(Mat4::RotateYMat(30), temp);
	//temp = Mat4::mult(Mat4::RotateXMat(30), temp);
	temp = Mat4::mult(Mat4::TranslateMat(0, -2, -8), temp);
	temp = Mat4::mult(Mat4::ProjectPersp(), temp);

	//Transform points
	for(int i = 0; i < m->verts.size(); i++) {
		transVerts[i] = temp.mult(m->verts[i]);
	}

	//Normalize w (only for x and y)

	for(int i = 0; i < m->verts.size(); i++) {
		transVerts[i].coord[0] /= transVerts[i].coord[3];
		transVerts[i].coord[1] /= transVerts[i].coord[3];

		if(transVerts[i].coord[3] < 0.01 && transVerts[i].coord[3] > -0.01) {
			std::cerr << i << " " << transVerts[i].coord[3] << " woop\n";
		}
	}

	//Figure out mapping of points to screen
	//mapsize is how wide half the screen should be
	//In world units
	double mapsize = tan(d2r(fov / 2));
	double pixScale = s->width / (2 * mapsize); //How wide is a unit in pixels
	double offX = s->width / 2;
	double offY = s->height / 2;

	for(int i = 0; i < m->verts.size(); i++) {
		transVerts[i].coord[0] = transVerts[i].coord[0] * pixScale + offX;
		transVerts[i].coord[1] = transVerts[i].coord[1] * pixScale + offY;
	}

	//Draw faces with transformed vertices
	for(int i = 0; i < m->faces.size(); i++) {
		Vect4 v1 = transVerts[m->faces[i].v1 - 1];
		Vect4 v2 = transVerts[m->faces[i].v2 - 1];
		Vect4 v3 = transVerts[m->faces[i].v3 - 1];

		g.drawLine((int)v1.coord[0], (int)v1.coord[1], (int)v2.coord[0], (int)v2.coord[1]);
		g.drawLine((int)v2.coord[0], (int)v2.coord[1], (int)v3.coord[0], (int)v3.coord[1]);
		g.drawLine((int)v3.coord[0], (int)v3.coord[1], (int)v1.coord[0], (int)v1.coord[1]);
	
	}

	delete transVerts;
}


