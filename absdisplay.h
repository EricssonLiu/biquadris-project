#ifndef ABSDISPLAY_H
#define ABSDISPLAY_H
#include "observer.h"
#include "cellinfo.h"
#include "playerinfo.h"

class AbsDisplay: public Observer<cellInfo> {
 protected:
  const int rows;
  const int cols;

  playerInfo player1;
  playerInfo player2;
  int highscore;

 public:
  AbsDisplay(int rows, int cols) : rows{rows}, cols{cols} {};

  void notify(Subject<cellInfo> &whoNotified) = 0;
  // void updateInfo(playerInfo player1, playerInfo player2);
};

#endif
