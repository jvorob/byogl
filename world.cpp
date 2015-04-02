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
	addMesh(m, "Unnamed");
}

int World::addMesh(Mesh *m, std::string name) {
	//Check name duplicacy
	static int boop = 0;
	name += " " + std::to_string(boop);
	boop++;


	//Find an empty space
	int i;
	for(i = 0; i < meshes.size(); i++) {
		if(meshes[i] == NULL) {
			break;
		}
	}

	//Insert it
	if(i == meshes.size()) {
		meshes.push_back(m);
		names.push_back(name);
	} else {
		meshes[i] = m;
		names[i] = name;
	}

	count++;

	return i;
}

int World::nextMesh(int n) {
	return meshCycler(n, 1);
}

int World::prevMesh(int n) {
	return meshCycler(n, -1);
}

Mesh *World::getMesh(int n) {
	if(n == -1 || !meshes[n])
		return NULL;
	return meshes[n];
}

std::string World::getName(int n) {
	if(n == -1 || !meshes[n])
		return NULL;
	return names[n];
}

//Private
int World::meshCycler(int n, int delta) {
	if(n < 0 || n > meshes.size())
		return -1;

	if(delta != 1 && delta != -1) {
		std::cerr << "INVALID PARAMS IN MESHCYCLER\n";
		return -1;
	}

	if(meshes.size() == 0) {
		std::cerr << "Empty world\n";
		return -1;
	}

	int temp = n + delta;
	while(temp != n) {
		//Loop around
		if(temp == meshes.size())
			temp = 0;

		if(temp < 0)
			temp = meshes.size() - 1;

		//Return first not-null one
		if(meshes[temp]) {
			boop(temp);
			return temp;
		}
		
		temp += delta;
	}

	return n;
}
