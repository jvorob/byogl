#include <iostream>

#include "entity.h"

Entity::Entity() {
	scale = Vect4(1,1,1);
	loc = Vect4::Zero();
	rotation = Vect4::Zero();
	rotmat = Mat4::IdentityMat();
}


void Entity::setLocation(Vect4 v) {loc = v;}
void Entity::setScale(Vect4 v) {scale = v;}
void Entity::setScale(double s) {//sets all of them to s
	scale[0] = s;
	scale[1] = s;
	scale[2] = s;
	scale[3] = 1;
}
void Entity::setRotation(Vect4 v) { // In rads
	rotation = v;

	rotmat = Mat4::IdentityMat();
	rotmat = Mat4::mult(Mat4::RotateZMat(v[2]), rotmat);
	rotmat = Mat4::mult(Mat4::RotateYMat(v[1]), rotmat);
	rotmat = Mat4::mult(Mat4::RotateXMat(v[0]), rotmat);
}

Vect4 Entity::getLocation() { return loc; }
Vect4 Entity::getScale() { return scale; }
Vect4 Entity::getRotation() { return rotation; }

Mat4 Entity::forwardMat() {
	Mat4 temp;

	//First scale things
	//Then rotate things
	temp = Mat4::mult(rotmat, Mat4::ScaleMat(scale));

	//Then translate them
	temp = Mat4::mult(Mat4::TranslateMat(loc), temp);

	return temp;
}

Mat4 Entity::inverseMat() {
	Mat4 temp;

	//First translate things
	temp = Mat4::TranslateMat(loc.inverse());

	//Then rotate things
	//Then scale them up
	temp = Mat4::mult(rotmat, temp);
	temp = Mat4::mult(Mat4::ScaleMat(scale), temp);


	return temp;
}

std::string Entity::to_string() {
	std::string temp;

	temp.append("loc: ");
	temp.append(loc.to_string());
	temp.append("\n");

	temp.append("scale: ");
	temp.append(scale.to_string());
	temp.append("\n");

	return temp;
}
