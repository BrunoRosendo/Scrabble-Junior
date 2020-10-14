#include "Player.h"

Player::Player(Pool &pool, int id) {
	this->id = id;
	score = 0;
	for (int i = 1; i <= 7; ++i) {
		if (pool.isEmpty()) break;
		tiles.push_back(pool.getTile());
	}
}

int Player::getScore() const {
	return score;
}

int Player::getID() const {
	return id;
}

/*
Player draws a given quantity of tiles from the pool
*/
void Player::insertTiles(Pool& pool, int quant) {
	for (int i = 1; i <= quant; ++i) {
		if (pool.isEmpty()) break;
		tiles.push_back(pool.getTile());
	}
}

void Player::increaseScore() {
	++score;
}

void Player::removeTile(char tile) {
	for (int i = 0; i < tiles.size(); ++i) {
		if (tiles[i] == tile) {
			tiles.erase(tiles.begin() + i);
			break;
		}
	}
}

int Player::getTilesSize() const {
	return tiles.size();
}

char Player::getTile(int index) const {
	return tiles.at(index);
}

bool Player::hasTile(char tile) const {
	for (int i = 0; i < tiles.size(); ++i) {
		if (tiles.at(i) == tile) return true;
	}
	return false;
}