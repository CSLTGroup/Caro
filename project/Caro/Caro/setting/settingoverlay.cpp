#include "../global.h"

// Overlay settings variables
static int SelectSettingsOverlay = 0;
// 0: Sound 1: Save 2: Load 3: Back to Menu 4: Exit Game
static bool inSoundSubmenuOverlay = false; 

static bool initializedOverlay = false; // check if overplay settings open

// External variables from setting.cpp
extern int sound;
extern bool SoundSettings;
extern int MusicVolumeLevel;
extern int EffectVolumeLevel;
extern bool initialized;
extern bool backToMenu;
extern bool ExitGame;
extern int state;

// box size functions (shared)
inline float GetBoxWidth(RenderWindow& window) { return window.getSize().x * 0.4f; }
inline float GetBoxHeight(RenderWindow& window) { return window.getSize().y * 0.5f; }

// Overlay Settings Functions
void Settings::handleSettingsOverlay(RenderWindow& window)
{
	if (!initialized) {
		MusicVolumeLevel = (int)(GetMusicVolume() / 5.0f);
		EffectVolumeLevel = (int)(GetEffectVolume() / 5.0f);
		initialized = true;
	}
	settingBoxOverlay(window);
	// Handle sound submenu navigation
	if (inSoundSubmenuOverlay) {
		sfx();
		// Exit sound submenu on Esc
		if (keyBoard.Esc()) {
			PlaySoundClick();
			inSoundSubmenuOverlay = false;
			sound = 0;
		}
	}
	else {
		// Action in settings board
		// SelectSettingsOverlay: 0 = Sound, 1 = Save, 2 = Load, 3 = Back to Menu, 4 = Exit Game
		if (keyBoard.Up() ^ keyBoard.Down()) {
			if (keyBoard.Up()) {
				--SelectSettingsOverlay;
				if (SelectSettingsOverlay < 0)
					SelectSettingsOverlay = 4;
			}
			else {
				++SelectSettingsOverlay;
				if (SelectSettingsOverlay > 4)
					SelectSettingsOverlay = 0;
			}
			PlaySoundClick();
		}

		// Enter key action
		if (keyBoard.Enter()) {
			PlaySoundClick();
			if (SelectSettingsOverlay == 0) {
				// Enter sound settings submenu
				inSoundSubmenuOverlay = true;
				sound = 0;
			}
			else if (SelectSettingsOverlay == 1) { // Save
				// TODO: Implement save functionality
				// SaveGame();
			}
			else if (SelectSettingsOverlay == 2) { // Load
				// TODO: Implement load functionality
				// LoadGame();
			}
			else if (SelectSettingsOverlay == 3) { // back to menu
				backToMenu = true;
				SelectSettingsOverlay = 0;
				state = 0;
			}
			else if (SelectSettingsOverlay == 4) { // exit game
				ExitGame = true;
				window.close();
			}
		}

		// exit settings on Esc key
		if (keyBoard.Esc() && !initializedOverlay) {
			PlaySoundClick();
			inSoundSubmenuOverlay = false;
			SelectSettingsOverlay = 0;
			state = 0; // return to main menu
		}
	}
}

void Settings::settingBoxOverlay(RenderWindow& window)
{
	// Draw settings menu overlay 
	RectangleShape bg(Vector2f(window.getSize().x, window.getSize().y));
	bg.setFillColor(Color(0, 0, 0, 180)); 
	window.draw(bg);

	float boxWidth = GetBoxWidth(window);
	float boxHeight = GetBoxHeight(window);

	RectangleShape settingsBox(Vector2f(boxWidth, boxHeight));
	settingsBox.setFillColor(Color(50, 50, 50, 240));
	settingsBox.setOutlineColor(Color::White);
	settingsBox.setOutlineThickness(3);
	settingsBox.setPosition(
		window.getSize().x / 2 - boxWidth / 2,
		window.getSize().y / 2 - boxHeight / 2
	);
	window.draw(settingsBox);
	Text titleText;
	titleText.setFont(font);
	titleText.setString("Settings");
	titleText.setCharacterSize(48);
	titleText.setFillColor(Color::Yellow);
	FloatRect titleBounds = titleText.getLocalBounds();
	titleText.setPosition(
		window.getSize().x / 2 - titleBounds.width / 2,
		window.getSize().y / 2 - boxHeight / 2 + 30
	);
	window.draw(titleText);

	SettingButtonsOverlay(window);
}

void Settings::SettingButtonsOverlay(RenderWindow& window)
{
	// Use same dimensions as settings box
	float boxWidth = GetBoxWidth(window);
	float boxHeight = GetBoxHeight(window);
	float startY = window.getSize().y / 2 - boxHeight / 2 + 170;
	float buttonSpacing = 150.f;
	float volBoxHeight = 90.0f;
    
	// Sound button
	RectangleShape soundBox(Vector2f(boxWidth * 0.75f, 90));
	soundBox.setFillColor(SelectSettingsOverlay == 0 ? Color(100, 150, 200) : Color(70, 70, 70));
	soundBox.setOutlineColor(SelectSettingsOverlay == 0 ? Color::Yellow : Color::White);
	soundBox.setOutlineThickness(SelectSettingsOverlay == 0 ? 3 : 2);
	soundBox.setPosition(
		window.getSize().x / 2 - boxWidth * 0.375f,
		startY
	);
	window.draw(soundBox);

	// Sound button text (centered)
	Text soundText;
	soundText.setFont(font);
	soundText.setString("Sound");
	soundText.setCharacterSize(36);
	soundText.setFillColor(SelectSettingsOverlay == 0 ? Color::Yellow : Color::White);
	FloatRect soundTextBounds = soundText.getLocalBounds();
	soundText.setPosition(
		window.getSize().x / 2 - soundTextBounds.width / 2,
		startY + 45 - soundTextBounds.height / 2
	);
	window.draw(soundText);

	// Mute/Unmute indicator inside Sound box (always visible)
	std::string muteStatus = SoundSettings ? "ON" : "OFF";
	Text muteText;
	muteText.setFont(font);
	muteText.setString(muteStatus);
	muteText.setCharacterSize(28);
	// Highlight when in submenu and sound == 0 (mute/unmute is selected)
	if (inSoundSubmenuOverlay && sound == 0) {
		muteText.setFillColor(Color::Yellow);
	}
	else {
		muteText.setFillColor(SoundSettings ? Color(100, 200, 100) : Color(200, 100, 100));
	}
	FloatRect muteTextBounds = muteText.getLocalBounds();
	float muteX = window.getSize().x / 2 + boxWidth * 0.375f - muteTextBounds.width - 20;
	muteText.setPosition(
		muteX,
		startY + 45 - muteTextBounds.height / 2
	);
	window.draw(muteText);
	
	// Show main settings buttons when NOT in sound submenu
	if (!inSoundSubmenuOverlay) {
		// Save button (between Sound and Back to Menu)
		RectangleShape saveBox(Vector2f(boxWidth * 0.75f, 90));
		saveBox.setFillColor(SelectSettingsOverlay == 1 ? Color(100, 150, 200) : Color(70, 70, 70));
		saveBox.setOutlineColor(SelectSettingsOverlay == 1 ? Color::Yellow : Color::White);
		saveBox.setOutlineThickness(SelectSettingsOverlay == 1 ? 3 : 2);
		saveBox.setPosition(
			window.getSize().x / 2 - boxWidth * 0.375f,
			startY + buttonSpacing
		);
		window.draw(saveBox);
		
		// Save button text
		Text saveText;
		saveText.setFont(font);
		saveText.setString("Save");
		saveText.setCharacterSize(36);
		saveText.setFillColor(SelectSettingsOverlay == 1 ? Color::Yellow : Color::White);
		FloatRect saveTextBounds = saveText.getLocalBounds();
		saveText.setPosition(
			window.getSize().x / 2 - saveTextBounds.width / 2,
			startY + buttonSpacing + 45 - saveTextBounds.height / 2
		);
		window.draw(saveText);

		// Load button (between Save and Back to Menu)
		RectangleShape loadBox(Vector2f(boxWidth * 0.75f, 90));
		loadBox.setFillColor(SelectSettingsOverlay == 2 ? Color(100, 150, 200) : Color(70, 70, 70));
		loadBox.setOutlineColor(SelectSettingsOverlay == 2 ? Color::Yellow : Color::White);
		loadBox.setOutlineThickness(SelectSettingsOverlay == 2 ? 3 : 2);
		loadBox.setPosition(
			window.getSize().x / 2 - boxWidth * 0.375f,
			startY + buttonSpacing * 2
		);
		window.draw(loadBox);
		
		// Load button text
		Text loadText;
		loadText.setFont(font);
		loadText.setString("Load");
		loadText.setCharacterSize(36);
		loadText.setFillColor(SelectSettingsOverlay == 2 ? Color::Yellow : Color::White);
		FloatRect loadTextBounds = loadText.getLocalBounds();
		loadText.setPosition(
			window.getSize().x / 2 - loadTextBounds.width / 2,
			startY + buttonSpacing * 2 + 45 - loadTextBounds.height / 2
		);
		window.draw(loadText);

		// Back to Menu button
		RectangleShape backBox(Vector2f(boxWidth * 0.75f, 90));
		backBox.setFillColor(SelectSettingsOverlay == 3 ? Color(100, 150, 200) : Color(70, 70, 70));
		backBox.setOutlineColor(SelectSettingsOverlay == 3 ? Color::Yellow : Color::White);
		backBox.setOutlineThickness(SelectSettingsOverlay == 3 ? 3 : 2);
		backBox.setPosition(
			window.getSize().x / 2 - boxWidth * 0.375f,
			startY + buttonSpacing * 3
		);
		window.draw(backBox);
		
		// Back to Menu button text
		Text backText;
		backText.setFont(font);
		backText.setString("Back to Menu");
		backText.setCharacterSize(36);
		backText.setFillColor(SelectSettingsOverlay == 3 ? Color::Yellow : Color::White);
		FloatRect backTextBounds = backText.getLocalBounds();
		backText.setPosition(
			window.getSize().x / 2 - backTextBounds.width / 2,
			startY + buttonSpacing * 3 + 45 - backTextBounds.height / 2
		);
		window.draw(backText);

		// Exit Game button
		RectangleShape exitBox(Vector2f(boxWidth * 0.75f, 90));
		exitBox.setFillColor(SelectSettingsOverlay == 4 ? Color(100, 150, 200) : Color(70, 70, 70));
		exitBox.setOutlineColor(SelectSettingsOverlay == 4 ? Color::Yellow : Color::White);
		exitBox.setOutlineThickness(SelectSettingsOverlay == 4 ? 3 : 2);
		exitBox.setPosition(
			window.getSize().x / 2 - boxWidth * 0.375f,
			startY + buttonSpacing * 4
		);
		window.draw(exitBox);
		
		// Exit Game button text
		Text exitText;
		exitText.setFont(font);
		exitText.setString("Exit Game");
		exitText.setCharacterSize(36);
		exitText.setFillColor(SelectSettingsOverlay == 4 ? Color::Yellow : Color::White);
		FloatRect exitTextBounds = exitText.getLocalBounds();
		exitText.setPosition(
			window.getSize().x / 2 - exitTextBounds.width / 2,
			startY + buttonSpacing * 4 + 45 - exitTextBounds.height / 2
		);
		window.draw(exitText);
	}
	else {
		subSoundSettingBox(window);
	}
}

