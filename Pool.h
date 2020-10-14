#ifndef POOL_H
#define POOL_H
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <time.h>
using namespace std;

class Pool {
public:
	Pool();
	Pool(vector<char> tiles);
	char getTile();
	bool isEmpty();
	void putTile(char tile);
private:
	vector<char> tiles;
};

#endif 
