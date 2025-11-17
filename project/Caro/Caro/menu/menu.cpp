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
    draw(window);
    if (state == listButton[0].ID)
        handleNewGame(window);
    else if (state == listButton[1].ID)
        handleLoadGame(window);
    else if (state == listButton[2].ID)
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
            PlaySoundClick(); // Play click sound when navigating menu
        }
        else if (keyBoard.Enter()) {
            state = listButton[selectedButton].ID;
        }
        else if (keyBoard.Esc()) {
            window.close();
        }
    }
    else if (state == listButton[newGameID].ID) {
        boardGame.setMove(window);
    }
}
void Menu::handleNewGame(RenderWindow& window) {
    boardGame.drawTable(window);
}
void Menu::handleLoadGame(RenderWindow& window) {

}
void Menu::handleSettings(RenderWindow& window) {
    setting.handleSettings(window);
}