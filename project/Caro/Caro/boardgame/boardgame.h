#pragma once
#include "../global.h"

struct BoardGame {
    enum class GameMode {
        None,
        PVP,
        PVC
    };

    const int defaultSize = 12;
    int size = defaultSize;
    int resultGame = 0; // 0: chua ket thuc, 1: player 1 thang, 2: player 2 thang, 3: hoa
    vector<vector<int> > board;
    int curX = 0, curY = 0;
    int curPlayer = 1;
    GameMode mode = GameMode::None;
    int aiPlayer = 2;
	vector<pair<int, int> > winningPositions; // store winning positions for highlighting

    // Player info panel (shared by PVP / PVC)
    string player1Name = "Player 1";
    string player2Name = "Player 2";
    int player1Score = 0;
    int player2Score = 0;
    Texture player1Photo;
    Texture player2Photo;
    bool player1PhotoLoaded = false;
    bool player2PhotoLoaded = false;
    bool showPlayerPanel = false;

    // Exit confirmation dialog
    bool showExitDialog = false;
    int selectedExitOption = 0; // 0: Save & Exit, 1: Reset & Exit

    // UI
    float spacingLeft;
    float spacingTop;
    float spacingCellX;
    float spacingCellY;
    float cellLenX;
    float cellLenY;

    // player photo
    Sprite spritePlayer1Photo;
    Sprite spritePlayer2Photo;

    // functions
    void setUp();
    void reset();
    void drawTable(RenderWindow& window);
    void drawPosition(int x, int y, RenderWindow& window);
    void setChoice(RenderWindow& window);
    void setMove(RenderWindow& window);
    bool checkTheSame(vector<int> listCheck);
    int checkResult();
    int result();
    void drawWinnerMessage(RenderWindow& window);
    void setMode(GameMode newMode);
    bool isPVCMode() const { return mode == GameMode::PVC; }
    void makeBotMove();
    void ensurePlayerAssets();
    void drawPlayerInfoPanel(RenderWindow& window);
    void drawExitConfirmationDialog(RenderWindow& window);
    void handleExitConfirmationDialog(RenderWindow& window);
};