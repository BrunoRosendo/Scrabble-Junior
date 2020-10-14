#include "Board.h"
#include "Player.h"
#include "Pool.h"
#include "Game.h"
using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);     // Use the variable 'h' to change the color of cout
const int RED = 12;
const int WHITE = 15;

int main()  
{
    srand(time(NULL));
    string file;
    ifstream fin;
    SetConsoleTextAttribute(h, WHITE);
    cout << "------------------------------------------------ SCRABBLE JUNIOR -------------------------------------------------------" << endl << endl;
    cout << "What's the name of the file containing the board?" << endl;
    cin >> file;
    fin.open(file);
    while (cin.fail() || cin.eof() || fin.fail()) {
        if (cin.eof()) exit(0);
        else {
            cout << "That file doesn't exist" << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            fin.clear();
            cin >> file;
            fin.open(file);
        }
    }
    cin.ignore(1000, '\n');
    Game game(file);
    game.gameLoop();
}
