#include "Pool.h"

Pool::Pool(){}

Pool::Pool(vector<char> tiles) {
	this->tiles = tiles;
}

bool Pool::isEmpty() {
	return (tiles.size() == 0);
}

void Pool::putTile(char tile) {
	tiles.push_back(tile);
}

/*
Returns a random tile and erases it from the pool
*/
char Pool::getTile() {
	int i = rand() % tiles.size();
	char tile = tiles[i];
	tiles.erase(tiles.begin() + i);
	return tile;
}