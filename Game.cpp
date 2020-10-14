#include "Game.h"

/*
Reads a given file and creates a board and a pool for the game to take place in
*/
Game::Game(const string& file) {
	Board b(file);
	vector<char> tiles;
	for (int l = 0; l < b.getLines(); ++l) {
		for (int c = 0; c < b.getCols(); ++c) {
			if (b.getLetter(l, c) != ' ') {
				tiles.push_back(b.getLetter(l, c));
			}
		}
	}
	Pool p(tiles);
	board = b;
	pool = p;
	totalTiles = tiles.size();
}

/*
Asks the user for the number of players and creates the given number of objects of the class Player
*/
void Game::gameIntro() {
	int num;
	cout << "How many players will participate in the game? (2-4)" << endl;
	cin >> num;
	while (cin.fail() || cin.eof() || num < 2 || num > 4 || num > totalTiles / 7) {	
		if (cin.eof()) exit(0);
		else if (cin.fail()) 
			cout << "Insert a valid number" << endl;
		else if (num < 2 || num > 4) 
			cout << "Insert a number between 2 and 4" << endl;
		else														// Checks if there are enough letters in the pool for at least 7 tiles per player
			cout << "This board doesn't contain enough letters for that many players" << endl;
		cin.clear();
		cin.ignore(1000, '\n');
		cin >> num;
	}
	cin.ignore(1000, '\n');
	for (int i = 1; i <= num; ++i) {
		Player p(pool, i);
		players.push_back(p);
	}
}

/*
Checks if a player can play any of his tiles this turn
*/
bool Game::possibleToPlay(Player &p) {
	for (int i = 0; i < p.getTilesSize(); ++i) {
		char tile = p.getTile(i);
		if (board.possiblePlay(tile)) return true;
	}
	return false;
}
/*
Manages a full turn of any player. This function checks if it's possible to play, what tiles are playable, 
what positions are valid and whether the pool is empty or not, besides asking the user for his play.
*/
void Game::playerTurn(int player) {
	char letter;
	string pos;
	Player &p = players[player];
	for (int i = 1;  i <= 2; ++i) {
		if (possibleToPlay(p)) {
			cout << "Player " << p.getID() << ", choose the tile you wish to play: ";
			cin >> letter;
			while (true) {
				if (cin.eof()) exit(0);
				if (cin.fail()) continue;
				cin.clear();
				cin.ignore(1000, '\n');
				bool wrong = true;
				for (int i = 0; i < p.getTilesSize(); ++i) {
					if (p.getTile(i) == letter) {
						wrong = false;
						break;
					}
				}
				if (wrong) {
					if (letter < 'A' || letter > 'Z')
						cout << "That's not a valid character: ";
					else 
						cout << "You don't have that tile: ";
					cin >> letter;
					continue;
				}
				if (!board.possiblePlay(letter)) {
					cout << "You can't play that tile: " << endl;
					cin >> letter;
				}
				else break;
			}
			cout << "Insert the position you want to play it, in the format 'Ab': ";
			cin >> pos;
			while (cin.eof() || cin.fail() || !board.checkAndPlayLetter(letter, pos, p)) {
				if (cin.eof()) exit(0);
				cin.clear();
				cin.ignore(1000, '\n');
				cout << "That's not a valid position: ";
				cin >> pos;
			}
			cin.ignore(1000, '\n');
			p.removeTile(letter);
			draw();
			if (checkEnd()) break;
		}
		else {
			if (p.getTilesSize() == 0) {
				cout << "Player " << p.getID() << " has ran out of tiles" << endl;
				system("pause");
				break;
			}
			else {
				cout << "No possible plays for Player " << p.getID() << endl;
				if (i == 1) {
					if (pool.isEmpty()) {
						cout << "The pool is empty" << endl;
						system("pause");
						break;
					}
					else if (p.getTilesSize() == 1) {
						cout << "Select 1 tile to reroll: ";
						char a;
						cin >> a;
						while (cin.fail() || cin.eof() || !p.hasTile(a)) {
							if (cin.eof()) exit(0);
							if (cin.fail()) {
								cin.clear();
								cin.ignore(100, '\n');
								cout << "Please insert a valid character: ";
								cin >> a;
							}
							if (!p.hasTile(a)) {
								cout << "You don't have that tile: ";
								cin.ignore(100, '\n');
								cin >> a;
							}
						}
						cin.ignore(1000, '\n');
						p.removeTile(a);
						pool.putTile(a);
						p.insertTiles(pool, 1);
						break;
					}
					else {
						char a, b;
						cout << "Select 2 tiles to reroll: ";
						cin >> a >> b;
						while (cin.fail() || cin.eof() || !p.hasTile(a) || !p.hasTile(b)) {
							if (cin.eof()) exit(0);
							if (cin.fail()) {
								cin.clear();
								cin.ignore(1000, '\n');
								cout << "Please insert two valid characters: ";
								cin >> a >> b;
							}
							if (!p.hasTile(a) || !p.hasTile(b)) {
								cout << "Insert two tiles you own: ";
								cin.ignore(100, '\n');
								cin >> a >> b;
							}
						}
						cin.ignore(1000, '\n');
						p.removeTile(a);
						p.removeTile(b);
						pool.putTile(a);
						pool.putTile(b);
						p.insertTiles(pool, 2);
						break;
					}
				}
				else system("pause");
			}
		}
	}
	while (p.getTilesSize() < 7) {
		if (pool.isEmpty()) break;
		p.insertTiles(pool, 1);
	}
}
/*
Returns true if there are no possible plays, meaning that the board is completed and the game is over
*/
bool Game::checkEnd() const {
	if (board.getPossPlaysSize() == 0) return true;
	return false;
}
/*
Calculates the player(s) with the most score and prints the winner(s)
*/
void Game::declareWinner() const {
	int maxScore = -1;
	vector<Player> winners;
	for (int i = 0; i < players.size(); ++i) {
		if (players.at(i).getScore() > maxScore) {
			winners.clear();
			winners.push_back(players.at(i));
			maxScore = players.at(i).getScore();
		}
		else if (players.at(i).getScore() == maxScore) {
			winners.push_back(players.at(i));
		}
	}
	if (winners.size() == 1) cout << "The winner is player " << winners[0].getID() << endl;
	else {
		cout << "It's a tie between the players ";
		for (int i = 0; i < winners.size(); ++i) {
			if (i == winners.size() - 2) cout << winners[i].getID() << " and ";
			else if (i == winners.size() - 1) cout << winners[i].getID() << endl;
			else cout << winners[i].getID() << ", ";
		}
	}
}
/*
This function draws the whole game on the screen. It should be called everytime there's a change in the board or the players
*/
void Game::draw() const {
	system("cls");
	SetConsoleTextAttribute(h, WHITE);
	cout << "------------------------------------------------ SCRABBLE JUNIOR -------------------------------------------------------" << endl;
	board.showBoard();
	cout << endl << endl << setw(5 + (4 - streamsize(players.size())) * 14) << "";		// setw argument is set to a scale
	for (int i = 0; i < players.size(); ++i) {
		string header = "Player " + to_string(players[i].getID()) + " (" + to_string(players[i].getScore()) + " points)";
		cout << setw(29) << left << header;
	}
	cout << endl << setw(5 + (4 - streamsize(players.size())) * 14) << "";
	for (int i = 0; i < players.size(); ++i) {
		string tilestring = "   ";
		for (int t = 0; t < players[i].getTilesSize(); ++t) {
			tilestring += players[i].getTile(t);
			tilestring += ' ';
		}
		cout << setw(29) << left << tilestring;
	}
	cout << endl << endl;
}
/*
Main loop of the game. This manages the order of the turns and only breaks when the game is over
*/
void Game::gameLoop() {
	gameIntro();
	int i = -1;
	while (true) {
		i = (i + 1) % players.size();
		draw();
		if (checkEnd()) {
			declareWinner();
			break;
		}
		playerTurn(i);
	}
}