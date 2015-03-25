#include "mesh.h"


#include <cstring>
#include <cstdlib>
#include <cmath>

using std::endl;

Face::Face() {}

Face::Face(int a, int b, int c) {
	v1 = a;
	v2 = b;
	v3 = c;
}

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

Mesh::Mesh(int deleteonremove) 
: Entity(), deleteOnRemove(deleteonremove) {
}

Mesh::Mesh(const Mesh& other) :
	markedForDeath(other.markedForDeath),
	deleteOnRemove(other.deleteOnRemove),
	verts(std::vector<Vect4>(other.verts)),
	faces(std::vector<Face>(other.faces)) {

}

Mesh::~Mesh() {
	clear();
}

void Mesh::clear() {
	verts.clear();
	faces.clear();
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

void Mesh::loadFromObjFile(std::string filename) {
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

//Start, start control, end control, end
void Mesh::genPrimBezier(Vect4 a, Vect4 b, Vect4 c, Vect4 d) {
	double t = 0;

	const int edges = 50;

	double lastx, lasty, currx, curry;

	/*
	genPrimEdge(a, b);
	genPrimEdge(c, d);
	*/

	Vect4 ab;
	Vect4 bc;
	Vect4 cd;

	Vect4 ab_bc;
	Vect4 bc_cd;

	Vect4 end;

	Vect4 last = a; //starts at a

	for(int i = 1; i <= edges; i++) {
		t = (1.0 / edges) * i;

		ab = Vect4::vLerp(a, b, t);
		bc = Vect4::vLerp(b, c, t);
		cd = Vect4::vLerp(c, d, t);

		ab_bc = Vect4::vLerp(ab, bc, t);
		bc_cd = Vect4::vLerp(bc, cd, t);

		end = Vect4::vLerp(ab_bc, bc_cd, t);

		genPrimEdge(last, end);

		last = end;
	}
}

void Mesh::genPrimHermite(Vect4 p0, Vect4 p1, Vect4 r0, Vect4 r1) {
	const int edges = 80;

	double lastx, lasty, currx, curry;

	/*
	genPrimEdge(p0, p1);
	genPrimEdge(p0, p0 + r0);
	genPrimEdge(p1, p1 + r1);
	*/

	Vect4 curr;
	Vect4 last = p0; //starts at p0

	for(int i = 1; i <= edges; i++) {
		double t = (1.0 / edges) * i;
		double t2 = t * t;
		double t3 = t2 * t;

		double h0 = 2 * t3 - 3 * t2 + 1;
		double h1 = -2 * t3 + 3 * t2;
		double h2 = t3 - 2 * t2 + t;
		double h3 = t3 - t2;

		//Do this for each coordinate
		for(int j = 0; j < 3; j++) {
			curr[j] = 
					h0 * p0[j] +
					h1 * p1[j] +
					h2 * r0[j] +
					h3 * r1[j];
		}

		genPrimEdge(last, curr);

		last = curr;
	}

}

void Mesh::genPrimBox() {
	std::cerr << "genprimbox not implemented\n";
}

void Mesh::genPrimSphere(double r) {
	const int slices = 20;
	const int pps = 10;//points per slice

	double lastx, lasty, currx, curry;

	int top = addVert(Vect4(0, r, 0));
	int bot = addVert(Vect4(0, -r, 0));

	//Contains all the vert indices except the top and bottom
	int points[slices][pps - 1];

	for(int i = 0; i < slices; i++) {
		double phi = (TWOPI / slices) * i;

		for(int j = 1; j < pps; j++) {
			double theta = (PI / pps) * j;

			//theta 0 -> top, theta pi -> bottom
			//phi 0 -> +x, goes counterclockwise from top

			double vert = r * cos(theta);
			double horiz = r * sin(theta);

			Vect4 temp;
			temp[0] = horiz * sin(phi);
			temp[1] = vert;
			temp[2] = horiz * cos(phi);

			points[i][j - 1] = addVert(temp);
		}
	}

	//make caps
	for(int i = 0; i < slices; i++) {
		addFace(Face(
				top, 
				points[i][0], 
				points[(i + 1) % slices][0]));
		addFace(Face(
				bot, 
				points[i][pps - 2], 
				points[(i + 1) % slices][pps - 2]));
	}

	//make body
	for(int i = 0; i < slices; i++) {
		for(int j = 1; j < pps - 1; j++) {
			//bottomleft
			addFace(Face(
					points[i][j - 1], 
					points[i][j], 
					points[(i + 1) % slices][j]));

			//topright
			addFace(Face(
					points[(i + 1) % slices][j - 1], 
					points[i][j - 1], 
					points[(i + 1) % slices][j]));
		}
	}
}
