#include "mesh.h"

#include <iostream>

Mesh::Mesh() {
}

Mesh::~Mesh() {
}

void Mesh::addVert(Vect4 v) {
	verts.push_back(v);
}

void Mesh::addFace(Face f) {
	faces.push_back(f);
}

void loadFromObjFile(char *filename) {
	std::ifstream fs;
	char buffer[1024];



	fs.open(filename);
	fs >> buffer;
	std::cout << buffer;

	fs.close();
}
