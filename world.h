#ifndef WORLD_H
#define WORLD_H

#include "mesh.h"

#include <vector>
#include <string>

class World {
	public:
		std::vector<Mesh *> meshes;
		std::vector<std::string> names;
	
		World();
		//Only deallocs meshes that are deleteonremove
		~World();

		void buryDead();

		//returns its index
		int addMesh(Mesh *m); //unnamed
		int addMesh(Mesh *m, std::string name);

		//Gets the next/prev non-null mesh
		int nextMesh(int n);
		int prevMesh(int n);

		Mesh *getMesh(int n);
		std::string getName(int n);

	private:
		int count;

		//worker function for next/prev mesh
		int meshCycler(int n, int delta);
};


#endif
