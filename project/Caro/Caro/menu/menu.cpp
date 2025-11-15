#include "../global.h"
void Menu::draw(RenderWindow& window) {

    //make new buttons if not exist
    if (!listButton.size()) {
        listButton.assign(3, Button());

        listButton[newGameID].context = "New Game";
        listButton[loadGameID].context = "Load Game";
        listButton[settingID].context = "Settings";
        listButton[newGameID].ID = 1;
        listButton[loadGameID].ID = 2;
        listButton[settingID].ID = 3;

        listButton[newGameID].selected = true;
        state = ID;
    }

    // size menu
    width = window.getSize().x * 80 / 100;
    height = window.getSize().y * 80 / 100;

    // size button
    int widthButton = width * 60 / 100;
    int heightButton = height * 10 / 100;
    int spacingTop = height * 20 / 100;
    int spacingBetween = (height - (spacingTop * 2) - listButton.size() * heightButton) / (listButton.size() - 1);

    //draw
    for (int i = 0; i < 3; i++) {
        auto& button = listButton[i];
        button.setPosition(
            window.getSize().x / 2 - widthButton / 2,                                                   // x
            window.getSize().y / 2 - height / 2 + spacingTop + (heightButton + spacingBetween) * i,     // y
            widthButton,                                                                                // x_len
            heightButton                                                                                // y_len
        );
        button.draw(window);
    }
}
void Menu::handleUI(RenderWindow& window) {
    BackGroundMusic(window); // Play background music
    if (state == ID) {
        draw(window);
    }
    if (state == listButton[newGameID].ID || state == 4 || state == 5 || state == 6)
        handleNewGame(window);
    else if (state == listButton[loadGameID].ID)
        handleLoadGame(window);
    else if (state == listButton[settingID].ID)
        handleSettings(window);
}
void Menu::updateState(RenderWindow& window) {
    if (state == ID) {
        if (keyBoard.Up() ^ keyBoard.Down()) {
            listButton[selectedButton].selected = false;

            if (keyBoard.Up()) {
                --selectedButton;
                if (selectedButton < 0)
                    selectedButton = listButton.size() - 1;
            }
            else {
                ++selectedButton;
                if (selectedButton == listButton.size())
                    selectedButton = 0;
            }

            listButton[selectedButton].selected = true;
        }
        else if (keyBoard.Enter()) {
            int newState = listButton[selectedButton].ID;
            if (newState == listButton[newGameID].ID) {
                // Reset names when starting new game
                boardGame.player1Name = "";
                boardGame.player2Name = "";
                state = 6; // Go to mode selection
            }
            else {
                state = newState;
            }
        }
        else if (keyBoard.Esc()) {
            window.close();
        }
    }
    else if (state == listButton[newGameID].ID) {
        boardGame.setMove(window);
    }
    else if (state == 6) {
        // Handle mode selection keyboard input
        static int lastUpDown = 0;
        if (keyBoard.Up() && lastUpDown != 1) {
            lastUpDown = 1;
        }
        else if (keyBoard.Down() && lastUpDown != 2) {
            lastUpDown = 2;
        }
        else if (!keyBoard.Up() && !keyBoard.Down()) {
            lastUpDown = 0;
        }
    }
}
void Menu::handleNewGame(RenderWindow& window) {
    if (state == 6) {
        // Mode selection screen
        handleModeSelection(window);
    }
    else if (state == 4) {
        // Player 1 name input screen
        handlePlayerNameInput(window, 1);
    }
    else if (state == 5) {
        // Player 2 name input screen (only for PvP)
        if (boardGame.gameMode == 0) {
            handlePlayerNameInput(window, 2);
        }
        else {
            // PVC mode - skip player 2 name, set to "Computer" and start game
            boardGame.player2Name = "Computer";
            state = listButton[newGameID].ID;
            boardGame.reset();
        }
    }
    else if (state == listButton[newGameID].ID) {
        // Game is ready, draw the board
        boardGame.drawTable(window);
    }
}
void Menu::handleLoadGame(RenderWindow& window) {

}
void Menu::handleSettings(RenderWindow& window) {
    BackGroundMusic(window);
}
void Menu::handlePlayerNameInput(RenderWindow& window, int playerNum) {
    // Background
    RectangleShape bg(Vector2f(window.getSize().x, window.getSize().y));
    bg.setFillColor(Color(50, 50, 50));
    window.draw(bg);
    
    // Title
    Text titleText;
    titleText.setFont(font);
    titleText.setString("Enter Player " + to_string(playerNum) + " Name:");
    titleText.setCharacterSize(36);
    titleText.setFillColor(Color::White);
    FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition(
        window.getSize().x / 2 - titleBounds.width / 2,
        window.getSize().y / 2 - 100
    );
    window.draw(titleText);
    
    // Input box background
    RectangleShape inputBox(Vector2f(400, 50));
    inputBox.setFillColor(Color(100, 100, 100));
    inputBox.setPosition(
        window.getSize().x / 2 - 200,
        window.getSize().y / 2 - 25
    );
    window.draw(inputBox);
    
    // Current name text
    Text nameText;
    nameText.setFont(font);
    string currentName = (playerNum == 1) ? boardGame.player1Name : boardGame.player2Name;
    nameText.setString(currentName);
    nameText.setCharacterSize(30);
    nameText.setFillColor(Color::White);
    nameText.setPosition(
        window.getSize().x / 2 - 190,
        window.getSize().y / 2 - 20
    );
    window.draw(nameText);
    
    // Instruction
    Text instructionText;
    instructionText.setFont(font);
    instructionText.setString("Press Enter to confirm, Backspace to delete");
    instructionText.setCharacterSize(20);
    instructionText.setFillColor(Color(200, 200, 200));
    FloatRect instBounds = instructionText.getLocalBounds();
    instructionText.setPosition(
        window.getSize().x / 2 - instBounds.width / 2,
        window.getSize().y / 2 + 50
    );
    window.draw(instructionText);
}
void Menu::handleModeSelection(RenderWindow& window) {
    static int selectedMode = 0; // 0 = PvP, 1 = PVC
    static bool initialized = false;
    static bool lastUpState = false;
    static bool lastDownState = false;
    static bool lastEnterState = false;
    static int frameCount = 0;
    
    if (!initialized) {
        selectedMode = 0;
        initialized = true;
        lastUpState = false;
        lastDownState = false;
        lastEnterState = false;
        frameCount = 0;
    }
    
    frameCount++;
    
    // Handle mode selection - only toggle on key press, not hold
    bool currentUp = keyBoard.Up();
    bool currentDown = keyBoard.Down();
    bool currentEnter = keyBoard.Enter();
    
    if (currentUp && !lastUpState) {
        selectedMode = 0; // PvP
    }
    if (currentDown && !lastDownState) {
        selectedMode = 1; // PVC
    }
    
    lastUpState = currentUp;
    lastDownState = currentDown;
    
    // Only process Enter if it's a new press (not held from previous state)
    // Wait at least 2 frames to avoid immediate Enter from menu selection
    if (currentEnter && !lastEnterState && frameCount > 2) {
        boardGame.gameMode = selectedMode;
        state = 4; // Go to player 1 name input
        initialized = false;
    }
    
    lastEnterState = currentEnter;
    
    // Background
    RectangleShape bg(Vector2f(window.getSize().x, window.getSize().y));
    bg.setFillColor(Color(50, 50, 50));
    window.draw(bg);
    
    // Title
    Text titleText;
    titleText.setFont(font);
    titleText.setString("Select Game Mode:");
    titleText.setCharacterSize(36);
    titleText.setFillColor(Color::White);
    FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition(
        window.getSize().x / 2 - titleBounds.width / 2,
        window.getSize().y / 2 - 150
    );
    window.draw(titleText);
    
    // PvP option
    RectangleShape pvpBox(Vector2f(400, 60));
    pvpBox.setFillColor(selectedMode == 0 ? Color(100, 200, 100) : Color(100, 100, 100));
    pvpBox.setOutlineColor(selectedMode == 0 ? Color::Yellow : Color::White);
    pvpBox.setOutlineThickness(3);
    pvpBox.setPosition(
        window.getSize().x / 2 - 200,
        window.getSize().y / 2 - 50
    );
    window.draw(pvpBox);
    
    Text pvpText;
    pvpText.setFont(font);
    pvpText.setString("Player vs Player (PvP)");
    pvpText.setCharacterSize(28);
    pvpText.setFillColor(Color::White);
    FloatRect pvpBounds = pvpText.getLocalBounds();
    pvpText.setPosition(
        window.getSize().x / 2 - pvpBounds.width / 2,
        window.getSize().y / 2 - 35
    );
    window.draw(pvpText);
    
    // PVC option
    RectangleShape pvcBox(Vector2f(400, 60));
    pvcBox.setFillColor(selectedMode == 1 ? Color(100, 200, 100) : Color(100, 100, 100));
    pvcBox.setOutlineColor(selectedMode == 1 ? Color::Yellow : Color::White);
    pvcBox.setOutlineThickness(3);
    pvcBox.setPosition(
        window.getSize().x / 2 - 200,
        window.getSize().y / 2 + 30
    );
    window.draw(pvcBox);
    
    Text pvcText;
    pvcText.setFont(font);
    pvcText.setString("Player vs Computer (PVC)");
    pvcText.setCharacterSize(28);
    pvcText.setFillColor(Color::White);
    FloatRect pvcBounds = pvcText.getLocalBounds();
    pvcText.setPosition(
        window.getSize().x / 2 - pvcBounds.width / 2,
        window.getSize().y / 2 + 45
    );
    window.draw(pvcText);
    
    // Instruction
    Text instructionText;
    instructionText.setFont(font);
    instructionText.setString("Use UP/DOWN to select, ENTER to confirm");
    instructionText.setCharacterSize(20);
    instructionText.setFillColor(Color(200, 200, 200));
    FloatRect instBounds = instructionText.getLocalBounds();
    instructionText.setPosition(
        window.getSize().x / 2 - instBounds.width / 2,
        window.getSize().y / 2 + 120
    );
    window.draw(instructionText);
}