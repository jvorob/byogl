#ifndef ENTITY_H
#define ENTITY_H

#include <string>

#include "matrix.h"

class Entity {
	private:
		Vect4 scale;
		Vect4 loc;
		Mat4 rotation;

	public:

		Entity(); //Initializes it at the origin
		
		void setLocation(Vect4 v);
		void setScale(Vect4 v);
		void setScale(double s);//sets all of them to s
		void setRotation(double x, double y, double z); //goes around z, y, then x (in rads)

		Vect4 getLocation();
		Vect4 getScale();
		
		Mat4 forwardMat(); //Returns a transform that moves something here
		Mat4 inverseMat(); //Moves this to the origin

		std::string to_string();
};

#endif
