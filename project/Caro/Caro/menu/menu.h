#pragma once
#include "../global.h"

struct Menu {
    const int ID = 0;
    vector<Button> listButton;
    const int newGameID = 0, loadGameID = 1, settingID = 2;
    int width = 0, height = 0;
    int selectedButton = 0;

    void draw(RenderWindow& window);
    void handleUI(RenderWindow& window);
    void updateState(RenderWindow& window);
    void handleNewGame(RenderWindow& window);
    void handleLoadGame(RenderWindow& window);
    void handleSettings(RenderWindow& window);
    void handlePlayerNameInput(RenderWindow& window, int playerNum);
    void handleModeSelection(RenderWindow& window);
};