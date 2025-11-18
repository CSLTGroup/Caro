#include "../global.h"

// for expanding settings in the future (please add ID buttons down below AND a
// section for specific buttons' variables if needed)
const int totalSetButtonsOverlay = 5;
string contextSetButtonsOverlay[totalSetButtonsOverlay] = {
    "Sound", "Save", "Load", "Back to Menu", "Exit Game"};

// general settings variables
static int SelectSettingsOverlay =
    0; // 0: Sound 1: Save 2: Load 3: Back to Menu 4: Exit Game
static int SelectPageOverlay = 0; // 0: first 3 buttons; 1: next 3 buttons; ...
static int numSettingsCurPageOverlay = 3;
const int totalPageOverlay = (totalSetButtonsOverlay + 2) / 3;
bool hoverSetButtonsOverlay[totalSetButtonsOverlay] = {
    false}; // hover button settings
bool inSetButtonsOverlay[totalSetButtonsOverlay] = {
    false}; // check in button settings
static bool initializedOverlay = false;
static bool inSettingsOverlay = false;
static bool inGeneralSettingsOverlay = false; // to ignore first enter key

// id buttons in settings
const int IDSFXOVERLAY = 0;
const int IDSAVEOVERLAY = 1;
const int IDLOADOVERLAY = 2;
const int IDBACKTOMENUOVERLAY = 3;
const int IDEXITGAMEOVERLAY = 4;

// External variables from setting.cpp
extern int MusicVolumeLevel;
extern int EffectVolumeLevel;
extern bool initialized;
extern int IDSoundButtons; // shared with setting.cpp
extern bool isNotMuted; // shared with setting.cpp
extern float boxWidth; // shared with setting.cpp
extern float boxHeight; // shared with setting.cpp


void Settings::handleSettingsOverlay(RenderWindow &window) {

	if (!initializedOverlay) {
    // music part
		MusicVolumeLevel = (int)(GetMusicVolume() / 5.0f);
		EffectVolumeLevel = (int)(GetEffectVolume() / 5.0f);

    // UI part - use shared boxWidth and boxHeight
		boxWidth = window.getSize().x * 0.4f;
		boxHeight = window.getSize().y * 0.5f;
		initializedOverlay = true;
	}

	settingBoxOverlay(window);

	if (!inSettingsOverlay) { // avoid first enter key issue

    // initialize settings part

		SelectSettingsOverlay = IDSFXOVERLAY; // Reset to Sound button - first Button
		SelectPageOverlay = 0;
		numSettingsCurPageOverlay = min(3, totalSetButtonsOverlay - SelectPageOverlay * 3);
		inSettingsOverlay = true;
		inGeneralSettingsOverlay = true;

	} else if (inGeneralSettingsOverlay) {

    // Action in settings board
    // SelectSettingsOverlay: 0 = Sound, 1 = Save, 2 = Load, 3 = Back to Menu, 4= Exit Game

		if (keyBoard.Up() ^ keyBoard.Down()) {


			int preSelectSettingsOverlay = SelectSettingsOverlay;
			if (keyBoard.Up()) {
				--SelectSettingsOverlay;
				if (SelectSettingsOverlay < SelectPageOverlay * 3)
                SelectSettingsOverlay += numSettingsCurPageOverlay;
			} else {
				++SelectSettingsOverlay;

				if (SelectSettingsOverlay ==SelectPageOverlay * 3 + numSettingsCurPageOverlay)
                SelectSettingsOverlay = SelectPageOverlay * 3;
			}

			if (preSelectSettingsOverlay != SelectSettingsOverlay) {
				hoverSetButtonsOverlay[preSelectSettingsOverlay] = false;
				hoverSetButtonsOverlay[SelectSettingsOverlay] = true;
				PlaySoundClick();
			}
		}
    // page change
    if (keyBoard.Left() ^ keyBoard.Right()) {
  		int preSelectPageOverlay = SelectPageOverlay;
  		if (keyBoard.Left()) {
  			SelectPageOverlay = max(SelectPageOverlay - 1, 0);
  		} else {
  			SelectPageOverlay = min(SelectPageOverlay + 1, totalPageOverlay - 1);
  		}
  		if (preSelectPageOverlay != SelectPageOverlay) {
  			PlaySoundClick();
  			SelectSettingsOverlay =
            SelectPageOverlay * 3; // reset to first button of the page
  			numSettingsCurPageOverlay = min(3, totalSetButtonsOverlay - SelectPageOverlay * 3);
  		}
  	}

    // enter action in settings board
    if (keyBoard.Enter() || keyBoard.Esc()) {
    	inGeneralSettingsOverlay = false;

    	if (SelectSettingsOverlay == IDBACKTOMENUOVERLAY ||
            keyBoard.Esc()) { // back to menu
    		SelectSettingsOverlay = 0;
    		SelectPageOverlay = 0;
    		numSettingsCurPageOverlay = 3;
    		inSettingsOverlay = false; // Reset flag when leaving
    		inGeneralSettingsOverlay = false;
    		inSetButtonsOverlay[IDSFXOVERLAY] = false;
    		IDSoundButtons = 0;
    		stateMenu = 0; // return to main menu
    		return;
            }
    	// other buttons
    	if (SelectSettingsOverlay == IDSFXOVERLAY) { // sound settings submenu
    		inSetButtonsOverlay[SelectSettingsOverlay] = true;
    		IDSoundButtons = 0;

    	} else if (SelectSettingsOverlay == IDSAVEOVERLAY) { // save
        // SaveGame();
    		inGeneralSettingsOverlay = true; // return to general settings

    	} else if (SelectSettingsOverlay == IDLOADOVERLAY) { // load
        // LoadGame();
    		inGeneralSettingsOverlay = true; // return to general settings
    		} else if (SelectSettingsOverlay == IDEXITGAMEOVERLAY) { // exit game
    			window.close();
    			return;
    		}
    	PlaySoundClick();
    }

	} else // for specific submenu handling
        if (inSetButtonsOverlay[IDSFXOVERLAY]) { // Handle sound submenu navigation
        	sfx(); // use shared sfx() from setting.cpp
      // Exit sound submenu on Esc
        	if (keyBoard.Esc()) {
        		PlaySoundClick();
        		inSetButtonsOverlay[IDSFXOVERLAY] = false;
        		IDSoundButtons = 0; // reset to mute/unmute
        		inGeneralSettingsOverlay = true; // back to general settings
        	}
        }
}

void Settings::settingBoxOverlay(RenderWindow &window) {
  // pre set up
	const float winWidth = window.getSize().x;
	const float winHeight = window.getSize().y;

  // Draw settings menu overlay

	RectangleShape bg(Vector2f(winWidth, winHeight));
	bg.setFillColor(Color(0, 0, 0, 180));
	window.draw(bg);

	RectangleShape settingsBox(Vector2f(boxWidth, boxHeight));
	settingsBox.setFillColor(Color(50, 50, 50, 240));
	settingsBox.setOutlineColor(Color::White);
	settingsBox.setOutlineThickness(winHeight * 0.003f);
	settingsBox.setPosition(winWidth / 2 - boxWidth / 2,
                          winHeight / 2 - boxHeight / 2);
	window.draw(settingsBox);
	Text titleText;
	titleText.setFont(font);
	titleText.setString("Settings");
	titleText.setCharacterSize((int)(winHeight * 0.05f));
	titleText.setFillColor(Color::Yellow);
	FloatRect titleBounds = titleText.getLocalBounds();
	titleText.setPosition(window.getSize().x / 2 - titleBounds.width / 2,
                        winHeight / 2 - boxHeight / 2 +
                            winHeight * 0.03f);
	window.draw(titleText);

  // draw page
	Text pageText;
	pageText.setFont(font);
	pageText.setString("Page " + to_string(SelectPageOverlay + 1) + "/" +
                     to_string(totalPageOverlay));
	pageText.setCharacterSize((int)(winHeight * 0.03f));
	pageText.setStyle(Text::Italic);
	pageText.setFillColor(Color(120, 158, 158));
	FloatRect pageBounds = pageText.getLocalBounds();
	pageText.setOrigin(pageBounds.width / 2, pageBounds.height / 2);
	pageText.setPosition(
      winWidth / 2 + boxWidth / 2 - boxWidth * 0.125,
      winHeight / 2 + boxHeight / 2 - boxHeight * 0.07f);
	window.draw(pageText);

  // draw buttons
	SettingButtonsOverlay(window);
}

void Settings::SettingButtonsOverlay(RenderWindow &window) {
  // window size
	static float winWidth = window.getSize().x;
	static float winHeight = window.getSize().y;

  // fixed size and indentations
	static float startY =
		winHeight / 2 - boxHeight / 2 + winHeight * 0.12f;
	static float buttonHeight = winHeight * 0.06f;
	static float buttonWidth = boxWidth * 0.75f;
	static float buttonSpacing = winHeight * 0.12f;
	static float outlineThick = winHeight * 0.004f;
	static float outlineThickSelected = winHeight * 0.006f;

	// draw general settings buttons
	if (inGeneralSettingsOverlay || !inSettingsOverlay) { // fix auto enter issue
		for (int IDButton = SelectPageOverlay * 3, cntBut = 0;
            cntBut < numSettingsCurPageOverlay; ++IDButton, ++cntBut) {
			generalSettingsBoxOverlay(window, IDButton, cntBut);
            }
	} else // draw specific settings
		if (inSetButtonsOverlay[IDSFXOVERLAY]) {
			// draw sound setting
			generalSettingsBoxOverlay(window, IDSFXOVERLAY,0); // highlight Sound button
			subSoundSettingBox(window);
		}

	// sound Mute/Unmute indicator
	if (SelectPageOverlay == 0) { // First Page contains Sound button
		// Mute/Unmute indicator inside Sound box (always visible)
		std::string muteStatus = isNotMuted ? "Unmute" : "Mute";
		Text muteText;
		muteText.setFont(font);
		muteText.setString(muteStatus);
		muteText.setCharacterSize((int)(winHeight * 0.03f));
		if (inSetButtonsOverlay[IDSFXOVERLAY] && IDSoundButtons == 0) {
			muteText.setFillColor(Color::Yellow);
		} else {
			muteText.setFillColor(isNotMuted ? Color(100, 200, 100): Color(200, 100, 100));
		}
		FloatRect muteTextBounds = muteText.getLocalBounds();
		float muteX = winWidth / 2 + boxWidth * 0.375f - muteTextBounds.width - winWidth * 0.005f;
		muteText.setPosition(muteX,
                         startY + buttonHeight / 2 - muteTextBounds.height / 2);
		window.draw(muteText);
	}
}

void Settings::generalSettingsBoxOverlay(RenderWindow &window, int IDButton, int row) {
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
	buttonBox.setFillColor(SelectSettingsOverlay == IDButton ? Color(100, 150, 200) : Color(70, 70, 70));
	buttonBox.setOutlineColor(SelectSettingsOverlay == IDButton ? Color::Yellow : Color::White);
	buttonBox.setOutlineThickness(SelectSettingsOverlay == IDButton ? outlineThickSelected : outlineThick);
	buttonBox.setPosition(
		winWidth / 2 - boxWidth * 0.375f,
		startY + buttonSpacing * row
	);

	// Button text (centered)
	Text buttonText;
	buttonText.setFont(font);
	buttonText.setString(contextSetButtonsOverlay[IDButton]);
	buttonText.setCharacterSize((int)(winHeight * 0.04f));
	buttonText.setFillColor(SelectSettingsOverlay == IDButton ? Color::Yellow : Color::White);
	FloatRect buttonTextBounds = buttonText.getLocalBounds();
	buttonText.setPosition(
		winWidth / 2 - buttonTextBounds.width / 2,
		startY + buttonSpacing * row + buttonHeight / 2 - buttonTextBounds.height / 2 - winHeight * 0.01f
	);

	window.draw(buttonBox);
	window.draw(buttonText);
}
