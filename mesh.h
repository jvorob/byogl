#ifndef MESH_H
#define MESH_H

#include <vector>
#include <fstream>

class Vect4 {
	public:
		double x, y, z, h;
};

class Face {
	public:
		int v1, v2, v3; //Vertices
		//char r, g, b;
};

class Mesh {
	public:
		std::vector<Vect4> verts;
		std::vector<Face> faces;
		
		Mesh();
		~Mesh();

		void addVert(Vect4 v);
		void addFace(Face f); 

		void loadFromObjFile(std::ifstream file);

};

#endif
