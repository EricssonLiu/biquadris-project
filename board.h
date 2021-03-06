#ifndef BOARD_H
#define BOARD_H
#include <string>
#include "cell.h"
#include "observer.h"
#include "blockinfo.h"
#include "playerinfo.h"
#include "textdisplay.h"
#include "gdisplay.h"
using namespace std;

class Block;

class Board : public Subject<playerInfo> {
  vector<vector<Cell>> myBoard;
  Board* oppBoard = nullptr;
  int score = 0;
  int player;
  int level;
  string sequenceFile;
  vector<char> blockOrder;

  bool isRand = true;
  string noRandFile;
  vector<char> noRandOrder;

  int width = 11;
  int height = 18;

  Block* currBlock = nullptr;
  char nextBlock;
  
  bool isHeavy = false; // from opponent's special action
  bool isBlind = false;
  bool gameOver = false;

  int blocksDropped = 0;

 public:
  ~Board();
  
  Board(int seed, int level, int player, string scriptfile = "");
  void levelChange(int change);
  bool move(string action, int i = 0);

  void endTurn(bool special = false);
  Block* createBlock();

  void specialAction();
  void toggleBlind();
  void setHeavy();

  void dropCenterBlock();
  void setNextBlock(char newtype);
  Block* getCurrBlock();
  void setCurrBlock(char newtype);
  void setNoRand(string file);
  void setRand();

  bool isRowFull(int rownum) const;
  int clearRow(int rownum);

  char generateNext(int level);
  vector<vector<Cell>> &getBoard();
  void setOppBoard(Board* opponentBoard);
  
  void printBoard();
  playerInfo getInfo() const;
};

#endif
