#include "matrix.h"
#include "util.h"

#include <cstring>
#include <cmath>

Mat4 Mat4::ZeroMat() {
	Mat4 temp;

	for(int i = 0; i < 16; i++)
		temp.mat[i] = 0;

	return temp;
}

Mat4 Mat4::IdentityMat() {
	Mat4 temp = ZeroMat();

	temp.mat[index(0,0)] = 1;
	temp.mat[index(1,1)] = 1;
	temp.mat[index(2,2)] = 1;
	temp.mat[index(3,3)] = 1;

	return temp;
}

Mat4 Mat4::ScaleMat(Vect4 v) {
	Mat4 temp = IdentityMat();

	double x = v[0] / v[3];
	double y = v[1] / v[3];
	double z = v[2] / v[3];

	temp.mat[index(0,0)] = x;
	temp.mat[index(1,1)] = y;
	temp.mat[index(2,2)] = z;

	return temp;
}


Mat4 Mat4::TranslateMat(Vect4 v) {
	Mat4 temp = IdentityMat();

	double x = v[0] / v[3];
	double y = v[1] / v[3];
	double z = v[2] / v[3];

	temp.mat[index(0,3)] = x;
	temp.mat[index(1,3)] = y;
	temp.mat[index(2,3)] = z;

	return temp;
}


Mat4 Mat4::RotateXMat(double theta) {
	Mat4 temp = IdentityMat();

	temp.mat[index(1,1)] = cos(theta);
	temp.mat[index(1,2)] = sin(theta);
	temp.mat[index(2,1)] = -1 * sin(theta);
	temp.mat[index(2,2)] = cos(theta);

	return temp;
}

Mat4 Mat4::RotateYMat(double theta) {
	Mat4 temp = IdentityMat();

	temp.mat[index(0,0)] = cos(theta);
	temp.mat[index(0,2)] = sin(theta);
	temp.mat[index(2,0)] = -1 * sin(theta);
	temp.mat[index(2,2)] = cos(theta);

	return temp;
}

Mat4 Mat4::RotateZMat(double theta) {
	Mat4 temp = IdentityMat();

	temp.mat[index(0,0)] = cos(theta);
	temp.mat[index(0,1)] = sin(theta);
	temp.mat[index(1,0)] = -1 * sin(theta);
	temp.mat[index(1,1)] = cos(theta);

	return temp;
}

Mat4 Mat4::ProjectPersp() {//Needs to be normalized after this
	Mat4 temp = IdentityMat();

	temp.mat[index(3,3)] = 0;
	temp.mat[index(3,2)] = 1;

	return temp;
}

/*
projectortho
*/

Mat4 Mat4::mult(Mat4 left, Mat4 right) {
	Mat4 temp = ZeroMat();

	for(int r = 0; r < 4; r++) {
		for(int c = 0; c < 4; c++) {
			double sum = 0;
			for(int i = 0; i < 4; i++) {
				sum += left.mat[index(r, i)] * right.mat[index(i, c)];
			}
			temp.mat[index(r, c)] = sum;
		}
	}

	return temp;
}

std::string Mat4::to_string() {
	std::string temp;

	temp.append("[\n");

	for(int c = 0; c < 4; c++) {
		for(int r = 0; r < 4; r++) {
			temp.append(std::to_string(mat[index(r, c)]));
			temp.append(", ");
		}
		temp.append("\n");
	}
	temp.append("]\n");

	return temp;
}

Vect4::Vect4() {}
Vect4::Vect4(double x, double y, double z) {
	coord[0] = x;
	coord[1] = y;
	coord[2] = z;
	coord[3] = 1;
}

Vect4 Mat4::mult(Vect4 v) { //Multiply a vect4 by this
	Vect4 temp;

	for(int r = 0; r < 4; r++) {
		temp.coord[r] = 0;
		for(int i = 0; i < 4; i++) {
			temp.coord[r] += mat[index(r, i)] * v.coord[i];
		}
	}

	return temp;
}

int Mat4::index(int r, int c) { //0 indexed
	return r + c * 4;
}

Vect4 Vect4::Zero() {
	Vect4 temp;

	temp.coord[0] = 0;
	temp.coord[1] = 0;
	temp.coord[2] = 0;
	temp.coord[3] = 1;

	return temp;
}

double& Vect4::operator[] (const int& i) {
	return coord[i];
}

Vect4 Vect4::inverse() {
	return Vect4 (-1 * coord[0], -1 * coord[1], -1 * coord[2]);
}

std::string Vect4::to_string() {
	std::string temp;

	temp.append("");
	temp.append(std::to_string(coord[0]));
	temp.append(", ");
	temp.append(std::to_string(coord[1]));
	temp.append(", ");
	temp.append(std::to_string(coord[2]));
	temp.append(", ");
	temp.append(std::to_string(coord[3]));

	return temp;
}


Vect4 Vect4::vLerp(Vect4 a, Vect4 b, double t) {
	return Vect4(
			lerp(a[0],b[0], t),
			lerp(a[1],b[1], t),
			lerp(a[2],b[2], t));
}
