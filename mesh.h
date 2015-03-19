#ifndef MESH_H
#define MESH_H

#include <vector>
#include <fstream>

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
		Mesh(int deleteonremove);
		~Mesh();

		//Returns the index of the new vertex
		int addVert(Vect4 v);
		void addFace(Face f); 

		void loadFromObjFile(char *filename);

		void applyTransform(Mat4 m); //Doesn't modify the verts, only transVerts


		//Adds edges for a circle
		//in xy plane
		void genPrimEdge(Vect4 a, Vect4 b);
		void genPrimCircle(Vect4 c, double r);
		void genPrimBezier(Vect4 a, Vect4 b, Vect4 c, Vect4 d);
	private:
		char *myStrtok(char *s, char delim);
};

#endif
