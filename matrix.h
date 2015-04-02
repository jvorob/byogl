#ifndef MATRIX_H
#define MATRIX_H

#include <string>

class Vect4 {
	public:
		double coord[4];

		Vect4();
		Vect4(double x, double y, double z);

		//w is still 1
		static Vect4 Zero();

		double& operator[] (const int& i);
		const Vect4 operator+ (const Vect4 &rhs);
		const Vect4 operator- (const Vect4 &rhs);
		const Vect4 operator* (const double &rhs);
		
		double magnitude();

		Vect4 inverse();
		std::string to_string();

		//t0 = a, t1 = b, lerps between them
		static Vect4 vLerp(Vect4 a, Vect4 b, double t);
		static Vect4 cross(Vect4 l, Vect4 r);
		static double crossZ(Vect4 l, Vect4 r);//returns just the z component
		static double dot(Vect4 l, Vect4 r);
		static Vect4 unit(Vect4 v); //returns a unit vector in same direction
};

class Mat4 {
	private:
		double mat[16]; //c1(r1234) c2(r1234) ...

	public:
		static Mat4 ZeroMat();
		static Mat4 IdentityMat();
		static Mat4 ScaleMat(Vect4 v);
		static Mat4 TranslateMat(Vect4 v);
		//All rotations in radians
		static Mat4 RotateXMat(double theta);
		static Mat4 RotateYMat(double theta);
		static Mat4 RotateZMat(double theta);

		static Mat4 ProjectPersp();//Needs to be normalized after this
		static Mat4 ProjectOrtho();//Not implemented

		static Mat4 mult(Mat4 left, Mat4 right);//Multiply left by right, return product in new matrix

		Vect4 mult(Vect4 v);//Multiply a vect4 by this
		
		static int index(int r, int c); //Indexes into the matrix (0 indexed)

		std::string to_string();
};

#endif
