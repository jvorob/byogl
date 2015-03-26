#include <iostream>
#include <cmath>

#include "camera.h"

Camera::Camera(double fov) 
: Entity() {
	this->fov = fov;
}

void Camera::renderMesh(Mesh *m, Surface *s) {
	Graphics g (s);
	Vect4 *transVerts = new Vect4[m->verts.size()]; //Temporary storage for transformed vertices
	Mat4 temp = Mat4::IdentityMat();
	double sf = 1;

	temp = m->forwardMat();
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
			//std::cerr << i << " " << transVerts[i].coord[3] << " woop\n";
		}
	}

	//Figure out mapping of points to screen
	//mapsize is how wide half the screen should be
	//In world units
	double mapsize = tan(d2r(fov / 2));
	double pixScale = s->width / (2 * mapsize); //How wide is a unit in pixels
	double offX = s->width / 2;
	double offY = s->height / 2;

	//Transform to screen coords
	for(int i = 0; i < m->verts.size(); i++) {
		transVerts[i].coord[0] = transVerts[i].coord[0] * pixScale + offX;
		transVerts[i].coord[1] = transVerts[i].coord[1] * pixScale + offY;
	}

	//Draw wireframe faces with transformed vertices
	for(int i = 0; i < m->faces.size(); i++) {
		Vect4 v1 = transVerts[m->faces[i].v1];
		Vect4 v2 = transVerts[m->faces[i].v2];
		Vect4 v3 = transVerts[m->faces[i].v3];

		if(isBackface(v1, v2, v3))
			continue;

		g.drawLine((int)v1.coord[0], (int)v1.coord[1], (int)v2.coord[0], (int)v2.coord[1]);
		g.drawLine((int)v2.coord[0], (int)v2.coord[1], (int)v3.coord[0], (int)v3.coord[1]);
		g.drawLine((int)v3.coord[0], (int)v3.coord[1], (int)v1.coord[0], (int)v1.coord[1]);
	
	}


	delete transVerts;
}

void Camera::renderMeshes(World *w, Surface *s) {
	for(int i = 0; i < w->meshes.size(); i++) {
		//Skip nulls
		if(!w->meshes[i])
			continue;

		renderMesh(w->meshes[i], s);
	}
}

bool Camera::isBackface(Vect4 v1, Vect4 v2, Vect4 v3) {
	return Vect4::cross(v2 - v1, v3 - v1)[2] <= 0;
}
