#ifndef MESH_H
#define MESH_H

#include <vector>
#include <fstream>

#include "matrix.h"


class Face {
	public:
		int v1, v2, v3; //Vertices
		//char r, g, b;
};

class Mesh {
	public:
		std::vector<Vect4> verts;
		std::vector<Vect4> transVerts; //Temporary storage for transformed vertices
		std::vector<Face> faces;
		
		Mesh();
		~Mesh();

		void addVert(Vect4 v);
		void addFace(Face f); 

		void loadFromObjFile(char *filename);

		void applyTransform(Mat4 m); //Doesn't modify the verts, only transVerts

	private:
		char *myStrtok(char *s, char delim);
};

#endif
