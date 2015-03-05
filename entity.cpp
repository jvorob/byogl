#include <iostream>

#include "entity.h"

Entity::Entity() {
	scale = Vect4::Zero();
	loc = Vect4::Zero();
	rotation = Mat4::IdentityMat();
}


void Entity::setLocation(Vect4 v) {loc = v;}
void Entity::setScale(Vect4 v) {scale = v;}
void Entity::setScale(double s) {//sets all of them to s
	scale[0] = s;
	scale[1] = s;
	scale[2] = s;
	scale[3] = 1;
}
void Entity::setRotation(double x, double y, double z) { // In rads
	rotation = Mat4::IdentityMat();
	rotation = Mat4::mult(Mat4::RotateZMat(z), rotation);
	rotation = Mat4::mult(Mat4::RotateYMat(y), rotation);
	rotation = Mat4::mult(Mat4::RotateXMat(x), rotation);
}

Vect4 Entity::getLocation() { return loc; }
Vect4 Entity::getScale() { return scale; }

Mat4 Entity::forwardMat() {
	Mat4 temp;

	//First rotate things
	//Then scale them up
	temp = Mat4::mult(Mat4::ScaleMat(scale), rotation);

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
	temp = Mat4::mult(rotation, temp);
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
