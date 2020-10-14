#include "Board.h"


Board::Board(){
	lines = 0;
	cols = 0;
}

Board::Board(const string& file) {
	ifstream fin;
	fin.open(file);
	fin >> lines;
	fin.ignore(100, 'x');
	fin >> cols;
	fin.ignore(100, '\n');
	if (fin.fail()) {
		cout << "The file does not match the required format" << endl;
		exit(0);
	}
	vector<char> v(cols, ' ');
	for (int i = 1; i <= lines; ++i) {
		letters.push_back(v);
		playedLetters.push_back(v);
	}
	while (!fin.eof()) {
		string pos, word, end;
		char orientation;
		int line, col, size;
		fin >> pos >> orientation >> word;
		if (pos == "") continue;	// An empty line at the end of the file is ignored. However, mistakes in the board text file end the program
		if (fin.fail()) {
			cout << "The file does not match the required format" << endl;
			exit(0);
		}
		size = word.size();
		if (orientation == 'H') {
			end.push_back(pos[0]);
			end.push_back(char(int(pos[1]) + size));
		}
		else {
			end.push_back(char(int(pos[0]) + size));
			end.push_back(pos[1]);
		}
		possiblePlays.push_back(pos + orientation + end);	// This vector stores a 5 chars string which stores the position of a possible play (e.g. start of a word),
		line = int(pos[0]) - 65;							// the orientation of the word (H/V) and the position after the word ends
		col = int(pos[1]) - 97;			// AaHAc means that the playable letter is at the position (0, 0), the word is horizontal and ends before the position (0, 2)
		if (orientation == 'V') {
			for (int i = 0; i < word.size(); ++i) {
				letters.at(line + i).at(col) = word[i];
			}
		}
		else {
			for (int i = 0; i < word.size(); ++i) {
				letters.at(line).at(col + i) = word[i];
			}
		}
	}
	fin.close();
}

char Board::getLetter(int l, int c) const {
	return letters.at(l).at(c);
}

int Board::getLines() const {
	return lines;
}

int Board::getCols() const {
	return cols;
}

int Board::getPossPlaysSize() const {
	return possiblePlays.size();
}

/*
Updates the vector which manages the letters played on the board
*/
void Board::putLetter(char letter, int line, int col) {
	playedLetters.at(line).at(col) = letter;
}

/*
Checks if it's possible to play a given letter
*/
bool Board::possiblePlay(char letter) {
	for (int i = 0; i < possiblePlays.size(); ++i) {
		int l = int(possiblePlays.at(i).at(0)) - 65;
		int c = int(possiblePlays.at(i).at(1)) - 97;
		if (letters.at(l).at(c) == letter) return true;
	}
	return false;
}

void Board::showBoard() const {
	cout << endl << setw(57-1.5*cols) << right << "  ";		// 1.5 is used for scale
	for (int i = 0; i < cols; ++i) {
		cout << "  " << char(i + 97);
	}
	cout << endl << setw(56 - 1.5 * cols) << left << "" << char(218);		// Chars 218, 196, 191, 179, 196 and 217
	for (int i = 0; i <= 3*cols; ++i) {										// help drawing a clean board
		cout << char(196);													// Check ASCII table to understand them better
	}
	cout << char(191);
	for (int l = 0; l < lines; ++l) {
		cout << endl << setw(55-1.5*cols) << right << char(l + 65) << ' ' << char(179);
		for (int c = 0; c < cols; ++c) {
			if (playedLetters.at(l).at(c) == ' ') {
				cout << "  " << letters.at(l).at(c);
			}
			else {
				SetConsoleTextAttribute(h, RED);
				cout << setw(3) << playedLetters.at(l).at(c);
				SetConsoleTextAttribute(h, WHITE);
			}
		}
		cout << ' ' << char(179);
	}
	cout << endl << setw(57 - 1.5 * cols) << right << char(192);
	for (int i = 0; i <= 3 * cols; ++i) {
		cout << char(196);
	}
	cout << char(217);
}


/*
If this function returns false, nothing is changed in the board, meaning that the given position is wrong. 
Otherwise, it updates the board vectors and the player score according to the given letter and position.
This could be done separately but it'd repeat code unnecessarily.
*/
bool Board::checkAndPlayLetter(char letter, const string &pos, Player &p) {
	bool invalid = true;
	for (int i = 0; i < possiblePlays.size(); ++i) {
		if (pos == possiblePlays[i].substr(0, 2)) {	
			invalid = false;
			string start = possiblePlays[i].substr(0, 2);
			string end = possiblePlays[i].substr(3, 2);
			char orientation = possiblePlays[i][2];
			string newstart;
			char newc;
			if (orientation == 'H') {
				newstart.push_back(start[0]);
				int col = int(start[1]) - 97;
				newc = char(col + 97);
				do {
					col++;
					newc = char(col + 97);;
					string comp = newstart;
					comp.push_back(newc);
					if (comp == end) {
						possiblePlays.erase(possiblePlays.begin() + i);
						p.increaseScore();			// word is complete, player must collect points
						break;
					}
				} while (playedLetters.at(int(start[0]) - 65).at(col) != ' ');
				newstart.push_back(newc);
			}
			else {
				int line = int(start[0]) - 65;
				newc = char(line + 65);
				do {
					line++;
					newc = char(line + 65);
					string comp = newstart;
					comp.push_back(newc);
					comp.push_back(start[1]);
					if (comp == end) {
						possiblePlays.erase(possiblePlays.begin() + i);	
						p.increaseScore();
						break;
					}
				} while (playedLetters.at(line).at(int(start[1] - 97)) != ' ');
				newstart.push_back(newc);
				newstart.push_back(start[1]);
			}
			if (newstart != end) possiblePlays[i] = newstart + orientation + end;
		}
	}
	if (!invalid) {
		int line = int(pos[0]) - 65;
		int col = int(pos[1]) - 97;
		if (letter != letters[line][col]) invalid = true;
		else putLetter(letter, line, col);
	}
	return (!invalid);
}