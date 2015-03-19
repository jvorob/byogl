#include "world.h"

World::World() {

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
}

void World::buryDead() {
	for(int i = 0; i < meshes.size(); i++) {
		if(meshes[i] == NULL)
			;
		else if(meshes[i]->markedForDeath) {
			if(meshes[i]->deleteOnRemove)
				delete meshes[i];

			meshes[i] = NULL;
		}
	}
}

int World::addMesh(Mesh *m) {
	//Find an empty space
	int i;
	for(i = 0; i < meshes.size(); i++) {
		if(meshes[i] == NULL) {
			meshes[i] = m;
			return i;
		}
	}

	meshes.push_back(m);
	return i;
}


