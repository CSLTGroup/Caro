#include "../global.h"


// for expanding settings in the future (please add ID buttons down below AND a
// section for specific buttons' variables if needed)
static const int totalSetButtons = 3;
static string contextSetButtons[totalSetButtons] = { "Enter your name here", "Press Enter to confirm", "Press Esc to exit game" };

// general settings variables
static int SelectPage = 0;     // 0: first 3 buttons; 1: next 3 buttons; ...
static int numSettingsCurPage = 3;
static const int totalPage = (totalSetButtons + 2) / 3;
static bool initialized = false;

// id buttons in settings
static const int ID = 0;

// username variables
static string tmp_name = "";
static bool isTypingName = false; // track if we're currently typing a name
static const int MAX_LENGTH_NAME = 14;
bool confirmedNameFirstTime = false;

// picture selection variables
extern string selectedPlayer1PicturePath;
static bool isSelectingPicture = false;
static bool nameConfirmed = false;
static int selectedPictureIndex = 0;
static const int totalAvailablePictures = 5;
static string availablePictures[totalAvailablePictures] = {
    "assets/image/apex2.png",
    "assets/image/apex3.png",
    "assets/image/apex4.png",
    "assets/image/apex5.png",
    "assets/image/apex6.png",
};

// UI handle
static float boxWidth = 0.0f;  // window.getSize().x * 0.6f; (shared with overlay)
static float boxHeight = 0.0f; // window.getSize().y * 0.5f; (shared with overlay)

void menuName_for_firstTimeLogic(RenderWindow& window) {
    if (confirmedNameFirstTime) {
		stateMenu = 0; // go to main menu
        return;
    }
    if (!initialized) {
        SelectPage = 0;
        numSettingsCurPage = min(3, totalSetButtons - SelectPage * 3);
		boxWidth = window.getSize().x * 0.5f;
        boxHeight = window.getSize().y * 0.5f;
		initialized = true;
		isTypingName = false;
        isSelectingPicture = false;
        nameConfirmed = false;
        selectedPictureIndex = 0;
    }
    
    // Picture selection stage
    if (nameConfirmed && !confirmedNameFirstTime) {
        isSelectingPicture = true;
        if (keyBoard.Left() || keyBoard.Right()) {
            if (keyBoard.Left()) {
                selectedPictureIndex--;
                if (selectedPictureIndex < 0)
                    selectedPictureIndex = totalAvailablePictures - 1;
            } else {
                selectedPictureIndex++;
                if (selectedPictureIndex >= totalAvailablePictures)
                    selectedPictureIndex = 0;
            }
            PlaySoundClick();
        }
        else if (keyBoard.Enter()) {
            // Apply chosen picture for Player 1
            selectedPlayer1PicturePath = availablePictures[selectedPictureIndex];
            confirmedNameFirstTime = true;
            isSelectingPicture = false;
            stateMenu = 0; // go to main menu
            setting.SaveSettings();
            PlaySoundClick();
        }
        else if (keyBoard.Esc()) {
            // Go back to name entry
            nameConfirmed = false;
            isSelectingPicture = false;
            PlaySoundClick();
        }
        return;
    }
    
    // Name entry stage
    if (!isTypingName) { // exit game
        if (keyBoard.Esc()) {
            PlaySoundClick();
			initialized = false;
            window.close();
        }
        else if (keyBoard.Enter()) {
            PlaySoundClick();
            isTypingName = true;
            tmp_name = "";
        }
    }
    else {
        if (keyBoard.Backspace()) {
            if (!tmp_name.empty()) {
                tmp_name.pop_back();
				PlaySoundClick();
            }
        }
        else if (keyBoard.Enter()) {
            playerName[ID] = tmp_name;
            isTypingName = false;
            tmp_name = "";
            nameConfirmed = true;
            isSelectingPicture = false;
            setting.SaveSettings();
			PlaySoundClick();
        }
        else if (keyBoard.Esc()) {
            isTypingName = false;
            tmp_name = "";
			PlaySoundClick();
        }
        else {
            bool addedChar = false;
            if (keyBoard.Shift()) {
                for (char c = 'A'; c <= 'Z'; c++)
                    if (tmp_name.length() < MAX_LENGTH_NAME &&
                        keyBoard.combineAlphabetCheck(c, true)) {
                        tmp_name += c;
                        addedChar = true;
                    }
            }
            else {
                for (char c = 'A'; c <= 'Z'; c++)
                    if (tmp_name.length() < MAX_LENGTH_NAME &&
                        keyBoard.combineAlphabetCheck(c)) {
                        tmp_name += (char)(c - 'A' + 'a');
                        addedChar = true;
                    }
            }
            if (addedChar)
                PlaySoundClick();
        }
    }
}

void drawMenuName_for_firstTime(RenderWindow& window) {
    if (nameConfirmed && !confirmedNameFirstTime) {
        drawPictureSelection(window);
        return;
    }

    // pre set up
    const float winWidth = window.getSize().x;
    const float winHeight = window.getSize().y;

    // Draw settings menu
    RectangleShape bg(Vector2f(winWidth, winHeight));
    bg.setFillColor(Color(30, 30, 30));
    window.draw(bg); // thich them background gi thi tu them vao

    RectangleShape settingsBox(Vector2f(boxWidth, boxHeight));
    settingsBox.setFillColor(Color(50, 50, 50, 240));
    settingsBox.setOutlineColor(Color::White);
    settingsBox.setOutlineThickness(winHeight * 0.003f);
    settingsBox.setPosition(winWidth / 2 - boxWidth / 2,
        winHeight / 2 - boxHeight / 2);
    window.draw(settingsBox);
    Text titleText;
    titleText.setFont(font);
    titleText.setString("Make your wonderful name!");
    titleText.setCharacterSize((int)(winHeight * 0.05f));
    titleText.setFillColor(Color::Yellow);
    FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition(window.getSize().x / 2 - titleBounds.width / 2,
        winHeight / 2 - boxHeight / 2 + winHeight * 0.03f);
    window.draw(titleText);

    // draw buttons
    drawButtons(window);
}

void drawButtons(RenderWindow& window) {
    // window size
    static float winWidth = window.getSize().x;
    static float winHeight = window.getSize().y;

    // fixed size and indentations
    static float startY = winHeight / 2 - boxHeight / 2 + winHeight * 0.12f;
    static float buttonHeight = winHeight * 0.06f;
    static float buttonWidth = boxWidth * 0.75f;
    static float buttonSpacing = winHeight * 0.12f;
    static float outlineThick = winHeight * 0.004f;
    static float outlineThickSelected = winHeight * 0.006f;

    // draw general settings buttons
    if (!isTypingName) { // fix auto enter issue
        for (int IDButton = SelectPage * 3, cntBut = 0; cntBut < numSettingsCurPage;
            ++IDButton, ++cntBut) {
            generalSettingsBox(window, IDButton, cntBut);
        }
    }
    else {
        string display_name = tmp_name;
        if (display_name.empty())
            display_name = " ";
        generalSettingsBox(window, 0, 0, "Typing: " + display_name, 1);
        for (int IDButton = SelectPage * 3 + 1, cntBut = 1; cntBut < numSettingsCurPage;
            ++IDButton, ++cntBut) {
            generalSettingsBox(window, IDButton, cntBut);
        }
    }
}

void generalSettingsBox(RenderWindow& window, int IDButton, int row, string contextString, int selectedC) {
    bool selected = false;
    if (row == 0)
        selected = true;
    // window size
    static float winWidth = window.getSize().x;
    static float winHeight = window.getSize().y;

    // fixed size and indentations
    static float startY = winHeight / 2 - boxHeight / 2 + winHeight * 0.12f;
    static float buttonHeight = winHeight * 0.06f;
    static float buttonWidth = boxWidth * 0.75f;
    static float buttonSpacing = winHeight * 0.12f;
    static float outlineThick = winHeight * 0.004f;
    static float outlineThickSelected = winHeight * 0.006f;

    // Box for each button
    RectangleShape buttonBox(Vector2f(buttonWidth, buttonHeight));
    buttonBox.setFillColor(selected ? Color(100, 150, 200)
        : Color(70, 70, 70));
    buttonBox.setOutlineColor(selected ? Color::Yellow
        : Color::White);
    buttonBox.setOutlineThickness(
        selected ? outlineThickSelected : outlineThick);
    buttonBox.setPosition(winWidth / 2 - boxWidth * 0.375f,
        startY + buttonSpacing * row);

    // Button text (centered)
    Text buttonText;
    buttonText.setFont(font);
    buttonText.setString(contextString.empty() ? contextSetButtons[IDButton] : contextString);
    buttonText.setCharacterSize((int)(winHeight * 0.04f));
    buttonText.setFillColor(selected ? Color::Yellow : Color::White);

    FloatRect buttonTextBounds = buttonText.getLocalBounds();
    buttonText.setPosition(winWidth / 2 - buttonTextBounds.width / 2,
        startY + buttonSpacing * row + buttonHeight / 2 -
        buttonTextBounds.height / 2 - winHeight * 0.01f);

    window.draw(buttonBox);
    window.draw(buttonText);
}

void drawPictureSelection(RenderWindow& window) {
    const float winWidth = window.getSize().x;
    const float winHeight = window.getSize().y;
    const float boxWidth = winWidth * 0.6f;
    const float boxHeight = winHeight * 0.7f;

    // Draw background
    RectangleShape bg(Vector2f(winWidth, winHeight));
    bg.setFillColor(Color(30, 30, 30));
    window.draw(bg);

    // Draw main box
    RectangleShape settingsBox(Vector2f(boxWidth, boxHeight));
    settingsBox.setFillColor(Color(50, 50, 50, 240));
    settingsBox.setOutlineColor(Color::White);
    settingsBox.setOutlineThickness(winHeight * 0.003f);
    settingsBox.setPosition(winWidth / 2 - boxWidth / 2, winHeight / 2 - boxHeight / 2);
    window.draw(settingsBox);

    // Draw title
    Text titleText;
    titleText.setFont(font);
    titleText.setString("Choose Your Picture!");
    titleText.setCharacterSize((int)(winHeight * 0.05f));
    titleText.setFillColor(Color::Yellow);
    FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition(winWidth / 2 - titleBounds.width / 2,
        winHeight / 2 - boxHeight / 2 + winHeight * 0.03f);
    window.draw(titleText);

    // Draw picture preview
    float previewSize = min(boxWidth * 0.4f, boxHeight * 0.4f);
    float previewX = winWidth / 2 - previewSize / 2;
    float previewY = winHeight / 2 - previewSize / 2 - winHeight * 0.05f;

    // Try to load and display the selected picture
    Texture previewTexture;
    bool loaded = false;
    if (selectedPictureIndex >= 0 && selectedPictureIndex < totalAvailablePictures) {
        if (previewTexture.loadFromFile(availablePictures[selectedPictureIndex])) {
            loaded = true;
        }
    }

    // Draw preview background
    RectangleShape previewBg(Vector2f(previewSize, previewSize));
    previewBg.setFillColor(Color(30, 30, 30));
    previewBg.setOutlineColor(Color::Yellow);
    previewBg.setOutlineThickness(3);
    previewBg.setPosition(previewX, previewY);
    window.draw(previewBg);

    // Draw the picture if loaded
    if (loaded) {
        Sprite previewSprite;
        previewSprite.setTexture(previewTexture);
        float scale = min(previewSize / previewTexture.getSize().x, previewSize / previewTexture.getSize().y);
        previewSprite.setScale(scale, scale);
        FloatRect spriteBounds = previewSprite.getLocalBounds();
        previewSprite.setOrigin(spriteBounds.width / 2.f, spriteBounds.height / 2.f);
        previewSprite.setPosition(previewX + previewSize / 2.f, previewY + previewSize / 2.f);
        window.draw(previewSprite);
    }

    // Draw navigation arrows
    Text leftArrow;
    leftArrow.setFont(font);
    leftArrow.setString("<");
    leftArrow.setCharacterSize(48);
    leftArrow.setFillColor(Color::Yellow);
    FloatRect leftBounds = leftArrow.getLocalBounds();
    leftArrow.setPosition(previewX - leftBounds.width - 20, previewY + previewSize / 2 - leftBounds.height / 2);
    window.draw(leftArrow);

    Text rightArrow;
    rightArrow.setFont(font);
    rightArrow.setString(">");
    rightArrow.setCharacterSize(48);
    rightArrow.setFillColor(Color::Yellow);
    FloatRect rightBounds = rightArrow.getLocalBounds();
    rightArrow.setPosition(previewX + previewSize + 20, previewY + previewSize / 2 - rightBounds.height / 2);
    window.draw(rightArrow);

    // Draw picture index info
    Text indexText;
    indexText.setFont(font);
    indexText.setString(to_string(selectedPictureIndex + 1) + " / " + to_string(totalAvailablePictures));
    indexText.setCharacterSize((int)(winHeight * 0.03f));
    indexText.setFillColor(Color::White);
    FloatRect indexBounds = indexText.getLocalBounds();
    indexText.setPosition(winWidth / 2 - indexBounds.width / 2,
        previewY + previewSize + winHeight * 0.03f);
    window.draw(indexText);

    // Draw instructions
    Text instructionText;
    instructionText.setFont(font);
    instructionText.setString("Use Left/Right arrows to browse, Enter to confirm");
    instructionText.setCharacterSize((int)(winHeight * 0.025f));
    instructionText.setFillColor(Color(200, 200, 200));
    FloatRect instBounds = instructionText.getLocalBounds();
    instructionText.setPosition(winWidth / 2 - instBounds.width / 2,
        winHeight / 2 + boxHeight / 2 - winHeight * 0.08f);
    window.draw(instructionText);

    // Draw esc instruction
    Text escText;
    escText.setFont(font);
    escText.setString("Press Esc to go back");
    escText.setCharacterSize((int)(winHeight * 0.025f));
    escText.setFillColor(Color(200, 200, 200));
    FloatRect escBounds = escText.getLocalBounds();
    escText.setPosition(winWidth / 2 - escBounds.width / 2,
        winHeight / 2 + boxHeight / 2 - winHeight * 0.05f);
    window.draw(escText);
}