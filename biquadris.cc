#include "biquadris.h"
using namespace std;

Biquadris::~Biquadris() {
    delete td;
    delete gd;
}
  
Biquadris::Biquadris(int start_level, int newseed, bool onlyText, string scriptfile1, string scriptfile2, int rows, int cols)
: player1{Board(newseed, start_level, 1, scriptfile1)}, player2{Board(newseed, start_level, 2, scriptfile2)}, scriptfile1{scriptfile1}, scriptfile2{scriptfile2} {
    boardHeight = rows;
    boardWidth = cols;
    textOnly = onlyText;
    seed = newseed;

    player1.setOppBoard(&player2);
    player2.setOppBoard(&player1);

    td = new TextDisplay{rows, cols};
    player1.attach(td);
    player2.attach(td);

    player1.endTurn();
    player2.endTurn();

    if (!textOnly) {
        gd = new GraphicsDisplay{player1.getInfo(), player2.getInfo(), rows, cols, player1.getCurrBlock()->blockSettings};
        player1.attach(gd);
        player2.attach(gd);
    }
}

void Biquadris::restartGame() {
    player1 = Board{seed, player1.getInfo().level, 1, scriptfile1};
    player2 = Board{seed, player2.getInfo().level, 2, scriptfile2};

    delete td;
    td = new TextDisplay{boardHeight, boardWidth};
    player1.attach(td);
    player2.attach(td);

    player1.endTurn();
    player2.endTurn();

    if (!textOnly) {
        delete gd;
        gd = new GraphicsDisplay{player1.getInfo(), player2.getInfo(), boardHeight, boardWidth, player1.getCurrBlock()->blockSettings};
        player1.attach(gd);
        player2.attach(gd);
    }

    turn = 1;

    boardsPrint();
}

void Biquadris::setRandom() {
    if (turn == 1) player1.setRand();
    if (turn == 2) player2.setRand();
}

void Biquadris::setNoRandom(string file) {
    if (turn == 1) player1.setNoRand(file);
    if (turn == 2) player2.setNoRand(file);
}

void Biquadris::setCurrBlock(char newType) {
    if (turn == 1) player1.setCurrBlock(newType);
    if (turn == 2) player2.setCurrBlock(newType);
} 


void Biquadris::move(string action, int repeats) {
    if (repeats == 0) return;

    // for when heavy debuff is on
    bool forceDrop = false;

    if (action == "drop") { // for "drop" actions, we want to end the player's turn afterwards
        for (int i = 0; i < repeats; ++i) {
            if (turn == 1) {
                player1.move(action);
                if (highscore < player1.getInfo().score) highscore = player1.getInfo().score;
            } else {
                player2.move(action);
                if (highscore < player2.getInfo().score) highscore = player2.getInfo().score;
            }
        }
        toggleTurn();
    } else {
        if (turn == 1) {
            forceDrop = player1.move(action, repeats);
            if (forceDrop) {
                player1.move("drop");
                toggleTurn();
            }
            forceDrop = false;
        }
        else {
            forceDrop = player2.move(action, repeats);
            if (forceDrop) {
                player2.move("drop");
                toggleTurn();
            }
            forceDrop = false;
        }
    }
}

void Biquadris::levelChange(int change) {
    if (turn == 1) player1.levelChange(change);
    if (turn == 2) player2.levelChange(change);
}

// prints the textdisplays of both players side by side
// blind(i) refers to if player(i) is blind
// blind: columns 3-9 and rows 3-12
void Biquadris::boardsPrint() {
    bool blind1 = player1.getInfo().isBlind;
    bool blind2 = player2.getInfo().isBlind;

    string space = "\t\t";
    string sep = "-----------";
    cout << "Level:    " << player1.getInfo().level << space << "Level:    " << player2.getInfo().level << endl;
    cout << "Score:    " << player1.getInfo().score << space << "Score:    " << player2.getInfo().score << endl;
    cout << sep << space << sep << endl;
    
    
    for (int i = 0; i < boardHeight; ++i) {
        if (blind1) {
            if (3+2 <= i && i <= 12+2)
                cout << td->rowString(1, i, "row");
            else if (i < 3)
                cout << td->rowString(1, i);
            else
                cout << td->rowString(1, i, "col");
        }
        else {
            cout << td->rowString(1, i);
        }

        cout << space;

        if (blind2) {
            if (3+2 <= i && i <= 12+2)
                cout << td->rowString(2, i, "row");
            else if (i < 3)
                cout << td->rowString(2, i);
            else
                cout << td->rowString(2, i, "col");
        }
        else {
            cout << td->rowString(2, i);
        }

        cout << endl;
    }

    cout << sep << space << sep << endl;
    cout << "Next:      " << space << "Next:      " << endl;

    char block1 = player1.getInfo().nextBlock;
    vector<vector<bool>> setting1 = player1.getCurrBlock()->blockSettings[block1];
    char block2 = player2.getInfo().nextBlock;
    vector<vector<bool>> setting2 = player2.getCurrBlock()->blockSettings[block2];

    for (int j = 0; j < max(setting1.size(), setting2.size()); ++j) {
        if (setting1.size() > j) {
            for (int i = 0; i < setting1[0].size(); ++i) {
                setting1[j][i] ? cout << block1 : cout << ' ';
            }
        }

        cout << space << "\t";

        if (setting2.size() > j) {
            for (int i = 0; i < setting2[0].size(); ++i) {
                setting2[j][i] ? cout << block2 : cout << ' ';
            }
        }

        cout << endl;
    }
}

int Biquadris::getHighscore() const {
    return highscore;
}

int Biquadris::loserIs() const {
    if (player1.getInfo().gameOver) {
        return 1;
    } else if (player2.getInfo().gameOver) {
        return 2;
    } else {
        return 0;
    }
}

void Biquadris::toggleTurn() {
    if (turn == 1) {
        turn = 2;
    } else {
        turn = 1;
    }
}