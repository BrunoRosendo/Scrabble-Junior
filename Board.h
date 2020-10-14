#ifndef BOARD_H
#define BOARD_H
#include <string>
#include <iomanip>
#include <Windows.h>
#include "Player.h"
using namespace std;

extern HANDLE h;
extern const int RED;
extern const int WHITE;

class Board {
public:
	Board();
	Board(const string& file);
	char getLetter(int l, int c) const;
	int getLines() const;
	int getCols() const;
	int getPossPlaysSize() const;
	void putLetter(char letter, int line, int col);
	void showBoard() const;
	bool checkAndPlayLetter(char letter,const string &pos, Player &p);
	bool possiblePlay(char letter);
private:
	int lines, cols;
	vector<vector <char>> letters;
	vector<vector <char>> playedLetters;
	vector<string> possiblePlays;
};


#endif 
