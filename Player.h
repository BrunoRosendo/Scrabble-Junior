#ifndef PLAYER_H
#define PLAYER_H
#include <algorithm>
#include "Pool.h"
using namespace std;

class Player {
public:
	Player(Pool &pool, int id);
	int getScore() const;
	int getID() const;
	int getTilesSize() const;
	char getTile(int index) const;
	void insertTiles(Pool &pool, int quant);
	void increaseScore();
	void removeTile(char tile);
	bool hasTile(char tile) const;
private:
	int score, id;
	vector<char> tiles;
};

#endif
