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

// UI handle
static float boxWidth = 0.0f;  // window.getSize().x * 0.6f; (shared with overlay)
static float boxHeight = 0.0f; // window.getSize().y * 0.5f; (shared with overlay)

void menuName_for_firstTimeLogic(RenderWindow& window) {
    if (!initialized) {
        SelectPage = 0;
        numSettingsCurPage = min(3, totalSetButtons - SelectPage * 3);
		boxWidth = window.getSize().x * 0.5f;
        boxHeight = window.getSize().y * 0.5f;
		initialized = true;
		isTypingName = false;
    }
    if (!isTypingName) { // exit game
        cerr << "im here" << endl;
        if (keyBoard.Esc()) {
            PlaySoundClick();
            firstTimePlaying = true;
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
            if (!tmp_name.empty())
                tmp_name.pop_back();
        }
        else if (keyBoard.Enter()) {
            playerName[ID] = tmp_name;
            isTypingName = false;
            tmp_name = "";
            confirmedNameFirstTime = true;
			stateMenu = 0; // go to main menu
        }
        else if (keyBoard.Esc()) {
            isTypingName = false;
            tmp_name = "";
        }
        else {
            if (keyBoard.Shift()) {
                for (char c = 'A'; c <= 'Z'; c++)
                    if (tmp_name.length() < MAX_LENGTH_NAME && keyBoard.combineAlphabetCheck(c, true))
                        tmp_name += c;
            }
            else {
                for (char c = 'A'; c <= 'Z'; c++)
                    if (tmp_name.length() < MAX_LENGTH_NAME && keyBoard.combineAlphabetCheck(c))
                        tmp_name += (char)(c - 'A' + 'a');
            }
        }
    }
}

void drawMenuName_for_firstTime(RenderWindow& window) {

    cerr << "im here2" << endl;
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