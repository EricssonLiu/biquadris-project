#ifndef BLOCK_H
#define BLOCK_H
#include <string>
#include "subject.h"
#include "blockinfo.h"
#include "board.h"
#include "cell.h"
using namespace std;

// class Cell;

class Block: public Subject<blockInfo> {
  vector<Cell> parts;
  blockInfo info;
  vector<vector<Cell>> board;
  
  int width, height;
  int remaining = 4;  // ctor will take a value for remaining ie 1
  
 public:
  
  Block(char type, int level, const vector<vector<Cell>> &gameBoard);
  
  void move(string action, int i); // int is repetitions
  void rotate(bool clockwise);
  void drop();
  void decreaseCells();
};

#endif