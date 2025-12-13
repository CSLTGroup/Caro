#pragma once
#include "../global.h"

struct Menu {
    const int ID = 0;
    vector<Button> listButton;
    const int howToPlayID = 0, newGameID = 1, loadGameID = 2, settingID = 3, creditsID = 4;
    float widthBoxMenu = 0, heightBoxMenu = 0;
    int selectedButton = 0;
    vector<Button> modeButtons;
    int selectedModeButton = 0;
    bool awaitingModeSelection = false;
    vector <RecordData> records;
    int currentVersion = 0;
    bool fromLoadGame = 0;

    // draw background image
    Sprite spriteBgImg;
    Texture bgImg;

    // draw title
    Text titleGame[3];
    Texture shadowTexTitle;
    Sprite shadowSpriteTitle;

    // design 
    void drawBackGround(RenderWindow& window);
    void drawMenu(RenderWindow& window);
    void drawTitle(RenderWindow& window);
    void handleUI(RenderWindow& window);
    void handleNewGame(RenderWindow& window);
    void handleLoadGame(RenderWindow& window);
    void handleSettings(RenderWindow& window);
    void drawModeSelection(RenderWindow& window);

    // logic
    void initModeButtons(RenderWindow& window);
    void updateState(RenderWindow& window);
    void handleModeSelection(RenderWindow& window);
    void initMenuButtons();
};