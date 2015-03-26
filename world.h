#ifndef WORLD_H
#define WORLD_H

#include "mesh.h"

#include <vector>

class World {
	public:
		std::vector<Mesh *> meshes;
		std::vector<std::string> names;
	
		World();
		//Only deallocs meshes that are deleteonremove
		~World();

		void buryDead();
		int addMesh(Mesh *m);//returns its index

	private:
		int count;
};


#endif
