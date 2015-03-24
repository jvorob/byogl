#ifndef MESH_H
#define MESH_H

#include <vector>
#include <fstream>
#include <iostream>

#include "matrix.h"
#include "entity.h"
#include "util.h"

class Face {
	public:
		int v1, v2, v3; //Vertices
		//char r, g, b;

		std::string to_string();
};

class Mesh: public Entity {
	public:
		std::vector<Vect4> verts;
		std::vector<Face> faces;

		//If true, the world this is part of will delete it when it garbage-collects
		int markedForDeath = FALSE;
		//If true, the world this is part of will delete it when it's marked for death
		int deleteOnRemove = TRUE; 
		
		Mesh();
		Mesh(const Mesh& other);
		Mesh(int deleteonremove);
		~Mesh();

		void clear(); //makes it blank again (verts/faces)

		//Returns the index of the new vertex
		int addVert(Vect4 v);
		void addFace(Face f); 

		void loadFromObjFile(std::string filename);

		void applyTransform(Mat4 m); //Doesn't modify the verts, only transVerts


		void genPrimEdge(Vect4 a, Vect4 b);
		//Adds edges for a circle
		//in xy plane
		void genPrimCircle(Vect4 c, double r);
		//Start, start control, end control, end
		void genPrimBezier(Vect4 a, Vect4 b, Vect4 c, Vect4 d);
		void genPrimHermite(Vect4 p0, Vect4 p1, Vect4 r0, Vect4 r1);
	private:
		char *myStrtok(char *s, char delim);
};

#endif
