#ifndef GAME_H
#define GAME_H
#include "Board.h"
#include "Player.h"
#include "Pool.h"

/*
This class manages all functions and classes necessary to run the game
*/
class Game {
public:
	Game(const string& file);
	void gameIntro();
	bool possibleToPlay(Player &p);
	void playerTurn(int player);
	bool checkEnd() const;
	void declareWinner() const;
	void draw() const;
	void gameLoop();
private:
	Board board;
	Pool pool;
	vector<Player> players;
	int totalTiles;
};

#endif

