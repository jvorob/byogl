#include "mesh.h"

#include <iostream>

#include <cstring>
#include <cstdlib>
#include <cmath>

using std::endl;

std::string Face::to_string() {
	std::string temp;

	temp.append("");
	temp.append(std::to_string(v1));
	temp.append(", ");
	temp.append(std::to_string(v2));
	temp.append(", ");
	temp.append(std::to_string(v3));

	return temp;
}

Mesh::Mesh() 
: Entity() {
}

Mesh::~Mesh() {
}

int Mesh::addVert(Vect4 v) {
	verts.push_back(v);
	return verts.size() - 1;
}

void Mesh::addFace(Face f) {
	faces.push_back(f);
}

//Replaces next occurence of delim with null
//Returns start of next string, or null if end
char *Mesh::myStrtok(char *s, char delim) { 
	while(1) {
		//Checl if hit end
		if(!*s)
			return NULL;

		//Check if hit delimiter
		if(*s == delim) { 
			//null all delimiters
			while(1) {
				*s = '\0';
				s++;
				if(!*s) 
					return NULL;
				if(*s != delim) {
					return s;
				}
			}
		}

		//If not, keep going
		s++;
	}
}

void Mesh::loadFromObjFile(char *filename) {
	addVert(Vect4(0,0,0));//dummy vert to account for 1-indexing


	std::ifstream fs;
	int numread = 0;
	char buffer[1024];

	fs.open(filename);
	
	while(!fs.eof()) {
		char *next, *curr;

		fs.getline(buffer, 1024);
		
		curr = buffer;
		next = myStrtok(curr, ' ');

		//Handles blank lines
		if(*curr == '\0')
			continue;

		

		if(!strcmp(buffer, "#")) {
			std::cerr << "Ignoring comment" << endl;
		} else if(!strcmp(buffer, "o")) {
			//std::cerr << "ignoring o for now" << endl;
		} else if(!strcmp(buffer, "v")) {
			//std::cerr << "handling v" << endl;
			Vect4 v;

			char *temp = next; //points to start of first float
			for(int i = 0; i < 3; i++)
				v.coord[i] = strtod(temp, &temp);
			v.coord[3] = 1;

			addVert(v);
		} else if(!strcmp(buffer, "vt")) {
			//std::cerr << "ignoring vt for now" << endl;
		} else if(!strcmp(buffer, "vn")) {
			//std::cerr << "ignoring vn for now" << endl;
		} else if(!strcmp(buffer, "f")) {
			Face f;

			char *temp = next; //points to start of first int
			f.v1 = strtol(temp, &temp, 0);
			while(*temp++ != ' ')
				;
			f.v2 = strtol(temp, &temp, 0);
			while(*temp++ != ' ')
				;
			f.v3 = strtol(temp, &temp, 0);

			//std::cerr << f.v1 << ", " << f.v2 << ", " << f.v3 << "\n";

			addFace(f);
			//std::cerr << "Woo! f" << endl;
		} else if(!strcmp(buffer, "s")) {
			std::cerr << "ignored s" << endl;
		} else {
			//Delay
			for(int i = 0; i < 100 * 1000 * 1000; i++)
				;
			std::cerr << "Unkown start word: '" << buffer << "'" << std::endl;
		}
	}


	fs.close();
}

void Mesh::genPrimEdge(Vect4 a, Vect4 b) {
	int av = addVert(a);
	int bv = addVert(b);
	addFace(Face {av, bv, bv});
}

void Mesh::genPrimCircle(Vect4 c, double r) {
	double t = 0;

	const int edges = 50;

	double lastx, lasty, currx, curry;

	boop(c[0]);
	boop(c[1]);
	boop(c[2]);

	lasty = sin(t) * r;
	lastx = cos(t) * r;

	for(int i = 1; i <= edges; i++) {
		t = (TWOPI / edges) * i;

		currx = cos(t) * r;
		curry = sin(t) * r;

		genPrimEdge(
				Vect4(currx + c[0], curry + c[1], c[2]), 
				Vect4(lastx + c[0], lasty + c[1], c[2]));

		lastx = currx;
		lasty = curry;
	}
}
