#include "../global.h"

struct BoardGame {
    const int defaultSize = 12;
    int size = defaultSize;
    int resultGame = 0; // 0: chua ket thuc, 1: player 1 thang, 2: player 2 thang, 3: hoa
    vector<vector<int> > board;
    int curX = 0, curY = 0;
    int curPlayer = 1;

    float spacingTop = 0;
    float spacingLeft = 0;
    float spacingBoardBetween = 0;
    float widthBoard = 0;
    float heightBoard = 0;
    float spacingCellX = 0;
    float spacingCellY = 0;
    float cellLenX = 0;
    float cellLenY = 0;

    void setUp();
    void reset();
    void drawTable(RenderWindow& window);
    void drawPosition(int x, int y, RenderWindow& window);
    void setChoice(RenderWindow& window);
    void setMove(RenderWindow& window);
    bool checkTheSame(vector<int> listCheck);
    int checkResult();
    int result();
};