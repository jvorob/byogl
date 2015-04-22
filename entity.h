#ifndef ENTITY_H
#define ENTITY_H

#include <string>

#include "matrix.h"

class Entity {
	private:
		Vect4 scale;
		Vect4 loc;
		Vect4 rotation; //holds onto values that created rotmat
		Mat4 rotmat;

	public:

		Entity(); //Initializes it at the origin
		
		void setLocation(Vect4 v);
		void setScale(Vect4 v);
		void setScale(double s);//sets all of them to s
		void setRotation(Vect4 v); //goes around z, y, then x (in rads)

		Vect4 getLocation();
		Vect4 getScale();
		Vect4 getRotation(); //returns a vect4 containing the x, y, and z rots
		
		Mat4 forwardMat(); //Returns a transform that moves something here
		Mat4 inverseMat(); //Moves this to the origin

		std::string to_string();
};

#endif
