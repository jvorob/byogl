#include "matrix.h"

#include <cstring>

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

Mat4 Mat4::ScaleMat(double x, double y, double z) {
	Mat4 temp = IdentityMat();

	temp.mat[index(0,0)] = x;
	temp.mat[index(1,1)] = y;
	temp.mat[index(2,2)] = z;

	return temp;
}


Mat4 Mat4::TranslateMat(double x, double y, double z) {
	Mat4 temp = IdentityMat();

	temp.mat[index(0,3)] = x;
	temp.mat[index(1,3)] = y;
	temp.mat[index(2,3)] = z;

	return temp;
}

/*
Mat4 Mat4::RotateMat(double x, double y, double z);
*/

Mat4 Mat4::mult(Mat4 left, Mat4 right) {
	Mat4 temp = ZeroMat();

	for(int r = 0; r < 4; r++) {
		for(int c = 0; c < 4; c++) {
			int sum = 0;
			for(int i = 0; i < 4; i++) {
				sum += left.mat[index(r, i)] * right.mat[index(i, c)];
			}
			temp.mat[index(r, c)] = sum;
		}
	}

	return temp;
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
