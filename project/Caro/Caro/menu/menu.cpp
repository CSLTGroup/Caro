#include "../global.h"


void Menu::drawBackGround(RenderWindow& window) {
    static bool initialized = false;
    if (!initialized) {
        bgImg.loadFromFile("assets/image/backgroundapex02.png");
        spriteBgImg.setTexture(bgImg);
    }
    sf::Vector2u textureSize = bgImg.getSize();
    sf::Vector2u windowSize = window.getSize();
    float scaleX = (float)windowSize.x / textureSize.x;
    float scaleY = (float)windowSize.y / textureSize.y;
    if (!(spriteBgImg.getScale().x == scaleX && spriteBgImg.getScale().y == scaleY))
        spriteBgImg.setScale(scaleX, scaleY);

    window.draw(spriteBgImg);

    // draw animated snow falling
    static Animation snowfall("assets/image/snowfall.png", 2600, 3600, 7, 2, 4);
    static Sprite spriteSnow;
    static Clock clock;
    static int currentFrame = 0;
    static float timer = 0.f;
    const static float fps = 5.0f;   // animation speed
    float dt = clock.restart().asSeconds();
    updateAnimation(dt, fps, snowfall.frameCount, currentFrame, timer);
    applyFrame(spriteSnow, snowfall, currentFrame);
    scaleX = 2.0f * windowSize.x / snowfall.texture.getSize().x;
    scaleY = 4.0f * windowSize.y / snowfall.texture.getSize().y;
    if (spriteSnow.getScale().x != scaleX || spriteSnow.getScale().y != scaleY)
        spriteSnow.setScale(scaleX, scaleY);
    
    window.draw(spriteSnow);
    initialized = true;
}
void Menu::drawTitle(RenderWindow& window) {
    const float winHeight = window.getSize().y;
    const float winWidth = window.getSize().x;


    // draw title
    float distancePerText[] = { winWidth / 300.0f, winHeight / 200.0f };
    float positionYText = winHeight * 34 * 33 / 10000.0f;
    float heightText = winHeight * 44 * 30 / 10000.0f;
    
    // draw 3 layer text
    static bool initialized = false;
    if (!initialized) {
        for (int i = 0; i < 3; i++) {
            titleGame[i].setFont(font);
            titleGame[i].setString("Caro Game"); // noi dung title
            titleGame[i].setStyle(Text::Bold);
        }
    }
    for (int i = 0; i < 3; i++) {
        titleGame[i].setCharacterSize(getCharacterSizeForLineHeight(font, heightText)); // character's size = chieu cao text
        FloatRect bounds = titleGame[i].getLocalBounds();
        titleGame[i].setOrigin(bounds.width / 2.0f, bounds.height / 2.0f); // set origin point in the middle
    }

    /// third layer
    titleGame[2].setFillColor(Color(150, 120, 11));
    titleGame[2].setPosition(winWidth / 2.0f + 2 * distancePerText[0], positionYText + 2 * distancePerText[1]);

    /// second layer
    titleGame[1].setFillColor(Color(177, 166, 11));
    titleGame[1].setPosition(winWidth / 2.0f + distancePerText[0], positionYText + distancePerText[1]);

    /// first layer
    titleGame[0].setFillColor(Color(255, 255, 0));
    titleGame[0].setPosition(winWidth / 2.0f, positionYText);

    // draw shadow image
    if (!initialized) {
        shadowTexTitle.loadFromFile("assets/image/shadow.png");
        shadowSpriteTitle.setTexture(shadowTexTitle);
        FloatRect bounds = shadowSpriteTitle.getLocalBounds();
        shadowSpriteTitle.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    }
    float scaleX = 3.0f * titleGame[0].getLocalBounds().width/ shadowSpriteTitle.getLocalBounds().width;
    float scaleY = 5.0f * titleGame[0].getLocalBounds().height/ shadowSpriteTitle.getLocalBounds().height;
    if (shadowSpriteTitle.getScale().x != scaleX || shadowSpriteTitle.getScale().y != scaleY)
        shadowSpriteTitle.setScale(scaleX, scaleY);
    shadowSpriteTitle.setPosition(titleGame[0].getPosition().x, titleGame[0].getPosition().y + heightText / 2.0f); // set in the middle
    shadowSpriteTitle.setColor(sf::Color(255, 255, 255, 100));

    // draw onto the screen
    window.draw(shadowSpriteTitle);
    window.draw(titleGame[2]);
    window.draw(titleGame[1]);
    window.draw(titleGame[0]);

    initialized = true;
}
void Menu::drawMenu(RenderWindow& window) {

    initMenuButtons();

    // draw buttons
    const float winWidth = window.getSize().x;
    const float winHeight = window.getSize().y;

    /// size box menu (contain 3 buttons)
    widthBoxMenu = winWidth * 54.0f / 100;
    heightBoxMenu = winHeight * 70.0f / 100;

    // size button
    float widthButton = widthBoxMenu;
    float heightButton = heightBoxMenu * 14.0f / 100;
    float spacingBetween = 1.0f * heightBoxMenu / listButton.size() - heightButton;
    float spacingBuffBot = winHeight * 4 / 100.0f;

    //draw
    for (int i = listButton.size() - 1; i >= 0; i--) {
        auto& button = listButton[i];
        button.setPosition(
            winWidth / 2 - widthButton / 2,                                             // x
            winHeight - (heightButton + spacingBetween) * (listButton.size() - i) - spacingBuffBot,     // y
            widthButton,                                                               // x_len
            heightButton                                                                     // y_len
        );
        button.draw(window);
    }

    // draw title game
    drawTitle(window);

}
void Menu::handleUI(RenderWindow& window) {
    if (stateMenu == -1)
        drawMenuName_for_firstTime(window);
    else if (stateMenu == 0)
        drawBackGround(window), drawMenu(window);
    else if (stateMenu == listButton[newGameID].ID)
        drawBackGround(window), drawTitle(window), handleNewGame(window);
    else if (stateMenu == listButton[loadGameID].ID)
        drawBackGround(window), drawTitle(window), handleLoadGame(window);
    else if (stateMenu == listButton[settingID].ID)
        drawBackGround(window), drawTitle(window), handleSettings(window);
}
void Menu::updateState(RenderWindow& window) {
    if (stateMenu == -1) { // first time playing
        menuName_for_firstTimeLogic(window);
    }
    else if (stateMenu == ID) {
        awaitingModeSelection = true;
        modeButtons.clear();
        initMenuButtons();

        if (keyBoard.Up() ^ keyBoard.Down()) {
            listButton[selectedButton].selected = false;
            listButton[selectedButton].needUpdate = true;

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
            listButton[selectedButton].needUpdate = true;
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
            else if (stateMenu == listButton[loadGameID].ID) {
                // Load game functionality can be added here
                LoadGameFetch();
                LoadGameLogic();
            }
            else if (stateMenu == listButton[settingID].ID) {
                setting.SettingsLogic(window);
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
    else if (stateMenu == listButton[loadGameID].ID) {
        // Load game functionality can be added here
        LoadGameLogic();
    }
    else if (stateMenu == listButton[settingID].ID) {
        setting.SettingsLogic(window);
	}
}
void Menu::handleNewGame(RenderWindow& window) {
    if (awaitingModeSelection)
    {
        if (fromLoadGame)
        {
            fromLoadGame = false;
            awaitingModeSelection = false;
            boardGame.showPlayerPanel = true;
            boardGame.drawTable(window);
        }
        else drawModeSelection(window);

    }
    else boardGame.drawTable(window);
}
void Menu::handleLoadGame(RenderWindow& window) {
    LoadGameUI(window);
}
void Menu::handleSettings(RenderWindow& window) {
    setting.draw(window);
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

    // draw panel
    RectangleShape panel(Vector2f(max(window.getSize().x * 0.5f, 650.0f), window.getSize().y * 0.5f));
    panel.setFillColor(Color(20, 20, 20, 200));
    panel.setOutlineColor(Color::White);
    panel.setOutlineThickness(2);
    panel.setOrigin(panel.getSize().x / 2.f, panel.getSize().y / 2.f);
    panel.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);

    // draw hint
    Text hint;
    hint.setFont(font);
    hint.setCharacterSize(24);
    hint.setString("Use Arrow Keys to choose, Enter to confirm, ESC to go back");
    hint.setFillColor(Color(220, 220, 220));
    FloatRect hintBounds = hint.getLocalBounds();
    hint.setOrigin(hintBounds.left + hintBounds.width / 2.f, hintBounds.top + hintBounds.height / 2.f);
    hint.setPosition(window.getSize().x / 2.f, panel.getPosition().y + panel.getSize().y / 2.f - 40.f);

    // draw title
    Text title;
    title.setFont(font);
    title.setString("Choose Game Mode");
    title.setCharacterSize(48);
    title.setFillColor(Color::White);
    FloatRect bounds = title.getLocalBounds();
    title.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    title.setPosition(window.getSize().x / 2.f, panel.getPosition().y - panel.getSize().y / 2.f + 70.f);

    // final draw onto the screen
    window.draw(panel);
    window.draw(title);
    for (auto& button : modeButtons) {
        button.draw(window);
    }
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
void Menu::initMenuButtons() {
    //make new buttons if not exist
    if (!listButton.size()) {
        listButton.assign(5, Button());

        listButton[newGameID].context = "New Game";
        listButton[loadGameID].context = "Load Game";
        listButton[settingID].context = "Settings";
        listButton[creditsID].context = "Credits";
        listButton[howToPlayID].context = "How to Play";
        listButton[newGameID].ID = 1;
        listButton[loadGameID].ID = 2;
        listButton[settingID].ID = 3;
        listButton[howToPlayID].ID = 4;
        listButton[creditsID].ID = 5;

        listButton[0].selected = true;
        stateMenu = ID;
    }
}