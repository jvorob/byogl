#include "world.h"

World::World() {
	count = 0;
}

World::~World() {
	//Delete all meshes that dont dealloc themselves
	for(int i = 0; i < meshes.size(); i++) {
		if(meshes[i] == NULL)
			;
		else if(meshes[i]->deleteOnRemove) {
			delete meshes[i];
			meshes[i] = NULL;
		}
	}

	meshes.clear();
}

void World::buryDead() {
	for(int i = 0; i < meshes.size(); i++) {
		if(meshes[i] == NULL)
			;
		else if(meshes[i]->markedForDeath) {
			if(meshes[i]->deleteOnRemove)
				delete meshes[i];

			meshes[i] = NULL;
			count--;
		}
	}
}

int World::addMesh(Mesh *m) {
	//Find an empty space
	int i;
	for(i = 0; i < meshes.size(); i++) {
		if(meshes[i] == NULL) {
			meshes[i] = m;
			names[i] = std::to_string(count);
			count++;
			return i;
		}
	}

	meshes.push_back(m);
	names.push_back(std::to_string(count));
	count++;
	return i;
}


