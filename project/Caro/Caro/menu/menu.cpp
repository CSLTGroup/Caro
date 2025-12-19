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
    float scaleX = 3.0f * titleGame[0].getLocalBounds().width / shadowSpriteTitle.getLocalBounds().width;
    float scaleY = 5.0f * titleGame[0].getLocalBounds().height / shadowSpriteTitle.getLocalBounds().height;
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
        drawBackGround(window), handleNewGame(window);
    else if (stateMenu == listButton[loadGameID].ID)
        drawBackGround(window), drawTitle(window), handleLoadGame(window);
    else if (stateMenu == listButton[settingID].ID)
        drawBackGround(window), drawTitle(window), handleSettings(window);
    else if (stateMenu == listButton[howToPlayID].ID)
        drawBackGround(window), drawTitle(window), handleHowToPlay(window);
    else if (stateMenu == listButton[creditsID].ID)
        drawBackGround(window), drawTitle(window), handleCreditScreen(window);
}


int lastState = 0;

void Menu::updateState(RenderWindow& window) {
    // 1. Trạng thái nhập tên lần đầu
    if (stateMenu == -1) {
        menuName_for_firstTimeLogic(window);
    }

    // 2. Trạng thái Menu chính (ID = 0)
    else if (stateMenu == ID) {
        awaitingModeSelection = true;
        initMenuButtons();

        // Di chuyển giữa các nút Menu
        if (keyBoard.Up() ^ keyBoard.Down()) {
            listButton[selectedButton].selected = false;
            listButton[selectedButton].needUpdate = true;

            if (keyBoard.Up()) {
                selectedButton = (selectedButton - 1 + (int)listButton.size()) % (int)listButton.size();
            }
            else {
                selectedButton = (selectedButton + 1) % (int)listButton.size();
            }

            listButton[selectedButton].selected = true;
            listButton[selectedButton].needUpdate = true;
            PlaySoundClick();
        }
        // Khi nhấn chọn một mục từ Menu chính
        else if (keyBoard.Enter()) {
            lastState = 0; // Ghi nhớ trạng thái trước đó là Menu chính
            stateMenu = listButton[selectedButton].ID;

            if (stateMenu == listButton[newGameID].ID) { // Vào chơi mới
                awaitingModeSelection = true;
                selectedModeButton = 0;
                boardGame.setUp();
                boardGame.setMode(BoardGame::GameMode::None);
            }
            else if (stateMenu == listButton[loadGameID].ID) { // Vào Load Game
                LoadGameFetch();
                LoadGameLogic();
            }
            else if (stateMenu == listButton[settingID].ID) { // Vào Settings
                setting.SettingsLogic(window);
            }
        }
        else if (keyBoard.Esc()) {
            window.close();
        }
    }

    // 3. Trạng thái trong trận đấu (State 1)
    else if (stateMenu == listButton[newGameID].ID) {
        if (awaitingModeSelection)
            handleModeSelection(window);
        else
            boardGame.setMove(window);
    }

    // 4. Trạng thái màn hình Load Game (State 2)
    else if (stateMenu == listButton[loadGameID].ID) {
        LoadGameLogic();

        // Nhấn ESC để quay lại
        if (keyBoard.Esc()) {
            stateMenu = lastState; // Quay về Menu chính (0) hoặc Game (1)
            if (lastState == 1) {
                boardGame.showExitDialog = true; // Hiện lại bảng Pause nếu đang trong game
            }
            PlaySoundClick();
        }
    }

    // 5. Trạng thái màn hình Cài đặt (State 3)
    else if (stateMenu == listButton[settingID].ID) {
        // Nhấn ESC để quay lại
        if (inGeneralSettings && keyBoard.Esc()) {
            stateMenu = lastState; // Quay về Menu chính (0) hoặc Game (1)
            if (lastState == 1) {
                boardGame.showExitDialog = true; // Hiện lại bảng Pause nếu đang trong game
            }
            PlaySoundClick();
            return;
        }
        setting.SettingsLogic(window);
    }
    else if (stateMenu == listButton[howToPlayID].ID) {
        howToPlay.HowToPlayLogic(window);
        if (keyBoard.Esc()) {
            stateMenu = 0; // Thường How to Play chỉ mở từ Menu chính
            PlaySoundClick();
        }
    }

    // 7. Trạng thái Thông tin Credits (State 5)
    else if (stateMenu == listButton[creditsID].ID) {
        credit.CreditLogic(window);
        if (keyBoard.Esc()) {
            stateMenu = 0;
            PlaySoundClick();
        }
    }
}
void Menu::handleNewGame(RenderWindow& window) {
    if (awaitingModeSelection) {
        if (fromLoadGame) { // Nếu biến này bằng true
            fromLoadGame = false;
            awaitingModeSelection = false; // Nó sẽ bỏ qua bước chọn PVP/PVC
            boardGame.showPlayerPanel = true;
            boardGame.drawTable(window); // Và vẽ thẳng bàn cờ đã load
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
void Menu::handleHowToPlay(RenderWindow& window) {
    howToPlay.draw(window);
}
void Menu::handleCreditScreen(RenderWindow& window) {
    credit.draw(window);
}
void Menu::drawModeButtons(RenderWindow& window, float panelWidth, float panelHeight) {
    const float winWidth = window.getSize().x;
    const float winHeight = window.getSize().y;
    const float buttonWidth = panelWidth * 0.40f;
    const float buttonHeight = panelHeight * 0.12f;
    const float positionY1 = winHeight / 2 - panelHeight / 2 + panelHeight * 0.37f;
    const float positionY2 = winHeight / 2 - panelHeight / 2 + panelHeight * 0.58f;
    const float buffYShadow = buttonHeight * 0.12f;

    // button PVP
    RectangleShape buttonPVP(Vector2f(buttonWidth, buttonHeight));
    buttonPVP.setPosition(winWidth / 2 - buttonWidth / 2, positionY1);
    buttonPVP.setFillColor(Color(254, 255, 195));

    // buttonPVPShadow
    RectangleShape buttonPVPShadow(Vector2f(buttonWidth, buttonHeight));
    buttonPVPShadow.setPosition(winWidth / 2 - buttonWidth / 2 + buffYShadow / 2, positionY1 + buffYShadow);
    buttonPVPShadow.setFillColor(Color(150, 153, 1));

    // text PVP
    Text textPVP;
    textPVP.setString("PVP");
    textPVP.setFont(font);
    textPVP.setCharacterSize(getCharacterSizeForLineHeight(font, buttonHeight * 0.90f));
    FloatRect bounds = textPVP.getLocalBounds();
    textPVP.setOrigin(bounds.left + bounds.getSize().x / 2.0f, bounds.top + bounds.getSize().y / 2.0f);
    textPVP.setPosition(winWidth / 2, positionY1 + buttonHeight / 2);
    textPVP.setFillColor(Color(57, 57, 57));

    // button PVE
    RectangleShape buttonPVE(Vector2f(buttonWidth, buttonHeight));
    buttonPVE.setPosition(winWidth / 2 - buttonWidth / 2, positionY2);
    buttonPVE.setFillColor(Color(223, 252, 255));

    // buttonPVEShadow
    RectangleShape buttonPVEShadow(Vector2f(buttonWidth, buttonHeight));
    buttonPVEShadow.setPosition(winWidth / 2 - buttonWidth / 2 + buffYShadow / 2, positionY2 + buffYShadow);
    buttonPVEShadow.setFillColor(Color(135, 163, 166));

    // text PVE
    Text textPVE;
    textPVE.setString("PVE");
    textPVE.setFont(font);
    textPVE.setCharacterSize(getCharacterSizeForLineHeight(font, buttonHeight * 0.90f));
    FloatRect bounds2 = textPVE.getLocalBounds();
    textPVE.setOrigin(bounds2.left + bounds2.getSize().x / 2.0f, bounds2.top + bounds2.getSize().y / 2.0f);
    textPVE.setPosition(winWidth / 2, positionY2 + buttonHeight / 2);
    textPVE.setFillColor(Color(75, 49, 123));

    // draw animated finger pointing
    static Animation finger("assets/image/point.png", 26, 24, 2, 1, 2);
    static Sprite spriteFinger;
    static Clock clock;
    static int currentFrame = 0;
    static float timer = 0.f;
    const static float fps = 5.0f;   // animation speed
    float dt = clock.restart().asSeconds();
    updateAnimation(dt, fps, finger.frameCount, currentFrame, timer);
    applyFrame(spriteFinger, finger, currentFrame);
    float fingerSizeWidth = (panelWidth - buttonWidth) / 2 / 3;
    float fingerSizeHeight = buttonHeight * 0.8f;
    float scaleX = 1.25f * fingerSizeWidth / finger.texture.getSize().x;
    float scaleY = 2.0f * fingerSizeHeight / finger.texture.getSize().y;
    if (spriteFinger.getScale().x != scaleX || spriteFinger.getScale().y != scaleY) {
        spriteFinger.setScale(scaleX, scaleY);
        FloatRect bounds3 = spriteFinger.getLocalBounds();
        spriteFinger.setOrigin(bounds3.left + bounds3.getSize().x / 2.0f, bounds3.top + bounds3.getSize().y / 2.0f);
    }
    if (selectedModeButton == 0)
        spriteFinger.setPosition(winWidth / 2 - buttonWidth / 2 - fingerSizeWidth * 1.5,
            positionY1 + buttonHeight / 2);
    else spriteFinger.setPosition(winWidth / 2 - buttonWidth / 2 - fingerSizeWidth * 1.5,
        positionY2 + buttonHeight / 2);

    // draw onto screen
    window.draw(buttonPVPShadow);
    window.draw(buttonPVEShadow);
    window.draw(buttonPVP);
    window.draw(buttonPVE);
    window.draw(textPVP);
    window.draw(textPVE);
    window.draw(spriteFinger);
}
void Menu::drawModeSelection(RenderWindow& window) {

    // draw panel
    const float panelWidth = max(window.getSize().x * 0.5f, 650.0f);
    const float panelHeight = window.getSize().y * 0.5f;
    RectangleShape panel(Vector2f(panelWidth, panelHeight));
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
    drawModeButtons(window, panelWidth, panelHeight);
    window.draw(hint);
}
void Menu::handleModeSelection(RenderWindow& window) {

    if (keyBoard.Up() ^ keyBoard.Down()) {

        if (keyBoard.Up()) {
            --selectedModeButton;
            if (selectedModeButton < 0)
                selectedModeButton = 1;
        }
        else {
            ++selectedModeButton;
            if (selectedModeButton > 1)
                selectedModeButton = 0;
        }

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
    }
    else if (keyBoard.Esc()) {
        stateMenu = ID;
        awaitingModeSelection = false;
        boardGame.setMode(BoardGame::GameMode::None);
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