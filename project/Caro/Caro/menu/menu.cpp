#include "../global.h"
void Menu::draw(RenderWindow& window) {

    if (stateMenu == ID) {
        awaitingModeSelection = true;
        modeButtons.clear();
    }

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
        stateMenu = ID;
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
    draw(window);
    if (stateMenu == listButton[0].ID)
        handleNewGame(window);
    else if (stateMenu == listButton[1].ID)
        handleLoadGame(window);
    else if (stateMenu == listButton[2].ID)
        handleSettings(window);
}
void Menu::updateState(RenderWindow& window) {
    if (stateMenu == ID) {
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
            PlaySoundClick(); // Play click sound when navigating menu
        }
        else if (keyBoard.Enter()) {
            stateMenu = listButton[selectedButton].ID;
            if (stateMenu == listButton[newGameID].ID) {
                awaitingModeSelection = true;
                selectedModeButton = 0;
                modeButtons.clear();
                initModeButtons(window);
                boardGame.setMode(BoardGame::GameMode::None);
            }
        }
        else if (keyBoard.Esc()) {
            window.close();
        }
    }
    else if (stateMenu == listButton[newGameID].ID) {
        if (awaitingModeSelection)
            handleModeSelection(window);
        else
            boardGame.setMove(window);
    }
}
void Menu::handleNewGame(RenderWindow& window) {
    if (awaitingModeSelection)
        drawModeSelection(window);
    else
        boardGame.drawTable(window);
}
void Menu::handleLoadGame(RenderWindow& window) {

}
void Menu::handleSettings(RenderWindow& window) {
    setting.handleSettings(window);
}

void Menu::initModeButtons(RenderWindow& window) {
    modeButtons.assign(2, Button());

    modeButtons[0].context = "PVP ";
    modeButtons[1].context = "PVE ";
    modeButtons[0].ID = 101;
    modeButtons[1].ID = 102;
    modeButtons[0].setMenuTheme(true);
    modeButtons[1].setMenuTheme(true);

    if (selectedModeButton < 0)
        selectedModeButton = 0;
    else if (selectedModeButton > 1)
        selectedModeButton = 1;
    modeButtons[0].selected = (selectedModeButton == 0);
    modeButtons[1].selected = (selectedModeButton == 1);

    int buttonWidth = static_cast<int>(window.getSize().x * 0.35f);
    int buttonHeight = 70;
    int spacing = 30;
    int startX = static_cast<int>(window.getSize().x / 2.f - buttonWidth / 2.f);
    int startY = static_cast<int>(window.getSize().y / 2.f - buttonHeight - spacing / 2.f);

    modeButtons[0].setPosition(startX, startY, buttonWidth, buttonHeight);
    modeButtons[1].setPosition(startX, startY + buttonHeight + spacing, buttonWidth, buttonHeight);
}

void Menu::drawModeSelection(RenderWindow& window) {
    if (!modeButtons.size())
        initModeButtons(window);

    RectangleShape panel(Vector2f(window.getSize().x * 0.5f, window.getSize().y * 0.5f));
    panel.setFillColor(Color(20, 20, 20, 200));
    panel.setOutlineColor(Color::White);
    panel.setOutlineThickness(2);
    panel.setOrigin(panel.getSize().x / 2.f, panel.getSize().y / 2.f);
    panel.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
    window.draw(panel);

    Text title;
    title.setFont(font);
    title.setString("Choose Game Mode");
    title.setCharacterSize(48);
    title.setFillColor(Color::White);
    FloatRect bounds = title.getLocalBounds();
    title.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    title.setPosition(window.getSize().x / 2.f, panel.getPosition().y - panel.getSize().y / 2.f + 70.f);
    window.draw(title);

    for (auto& button : modeButtons) {
        button.draw(window);
    }

    Text hint;
    hint.setFont(font);
    hint.setCharacterSize(24);
    hint.setString("Use Arrow Keys to choose, Enter to confirm, ESC to go back");
    hint.setFillColor(Color(220, 220, 220));
    FloatRect hintBounds = hint.getLocalBounds();
    hint.setOrigin(hintBounds.left + hintBounds.width / 2.f, hintBounds.top + hintBounds.height / 2.f);
    hint.setPosition(window.getSize().x / 2.f, panel.getPosition().y + panel.getSize().y / 2.f - 40.f);
    window.draw(hint);
}

void Menu::handleModeSelection(RenderWindow& window) {
    if (!modeButtons.size())
        initModeButtons(window);

    if (keyBoard.Up() ^ keyBoard.Down()) {
        modeButtons[selectedModeButton].selected = false;

        if (keyBoard.Up()) {
            --selectedModeButton;
            if (selectedModeButton < 0)
                selectedModeButton = modeButtons.size() - 1;
        }
        else {
            ++selectedModeButton;
            if (selectedModeButton >= modeButtons.size())
                selectedModeButton = 0;
        }

        modeButtons[selectedModeButton].selected = true;
        PlaySoundClick();
    }
    else if (keyBoard.Enter()) {
        awaitingModeSelection = false;
        if (selectedModeButton == 0) {
            boardGame.setMode(BoardGame::GameMode::PVP);
        }
        else {
            boardGame.setMode(BoardGame::GameMode::PVC);
        }
        modeButtons.clear();
    }
    else if (keyBoard.Esc()) {
        stateMenu = ID;
        awaitingModeSelection = false;
        boardGame.setMode(BoardGame::GameMode::None);
        modeButtons.clear();
    }
}