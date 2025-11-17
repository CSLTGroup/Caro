#include "../global.h"

static int SelectSettings = 0;
// 0: Sound 1: Back to Menu 2: Exit Game

// Shared variables (accessible from settingoverlay.cpp via extern)
int sound = 0; // 0: mute/ unmute sound 1: adjust music volume 2: adjust effect volume
bool SoundSettings = true; // true: on, false: off
int MusicVolumeLevel = 20; // from 0 to 20 step 5% in 100% (0-100%)
int EffectVolumeLevel = 20; // from 0 to 20 step 5% in 100% (0-100%)
bool backToMenu = false; // check back to menu
// Initialize VolumeLevel to match current volume
bool initialized = false;
bool ExitGame = false; // check exit game

// Private to setting.cpp only
static bool inSoundSubmenu = false; // track if we're in the sound settings submenu
static bool justEnteredSettings = false; // track if we just entered settings menu


// box size functions 
inline float GetBoxWidth(RenderWindow& window) { return window.getSize().x * 0.4f; }
inline float GetBoxHeight(RenderWindow& window) { return window.getSize().y * 0.5f; }

void Settings::sfx() {
	if (keyBoard.Up() ^ keyBoard.Down()) {
		if (keyBoard.Up()) {
			--sound;
			if (sound < 0)
				sound = 2;
		}
		else {
			++sound;
			if (sound > 2)
				sound = 0;
		}
		PlaySoundClick();
	}
	if (keyBoard.Enter())
	{
		PlaySoundClick();
		if (sound == 0)
		{
			SoundSettings = !SoundSettings;
			SoundMute();
		}
	}
	// Adjust music volume (sound == 1)
	if (sound == 1) {
		if (keyBoard.Left() && MusicVolumeLevel > 0) {
			MusicVolumeLevel--;
			float newVolume = MusicVolumeLevel * 5.0f;
			PlaySoundClick();
			SetMusicVolume(newVolume);
		}
		else if (keyBoard.Right() && MusicVolumeLevel < 20) {
			MusicVolumeLevel++;
			float newVolume = MusicVolumeLevel * 5.0f;
			PlaySoundClick();
			SetMusicVolume(newVolume);
		}
	}
	// Adjust effect volume (sound == 2)
	if (sound == 2) {
		if (keyBoard.Left() && EffectVolumeLevel > 0) {
			EffectVolumeLevel--;
			float newVolume = EffectVolumeLevel * 5.0f;
			PlaySoundClick();
			SetEffectVolume(newVolume);
		}
		else if (keyBoard.Right() && EffectVolumeLevel < 20) {
			EffectVolumeLevel++;
			float newVolume = EffectVolumeLevel * 5.0f;
			PlaySoundClick();
			SetEffectVolume(newVolume);
		}
	}
}

void Settings::handleSettings(RenderWindow& window)
{
	if (!initialized) {
		MusicVolumeLevel = (int)(GetMusicVolume() / 5.0f);
		EffectVolumeLevel = (int)(GetEffectVolume() / 5.0f);
		initialized = true;
	}
	
	// Check if we just entered settings (first frame)
	static bool wasInSettings = false;
	if (!wasInSettings) {
		justEnteredSettings = true;
		wasInSettings = true;
		SelectSettings = 0; // Reset to Sound button
	}
	
    settingBox(window);
	// Handle sound submenu navigation
	if (inSoundSubmenu) {
		sfx();
		// Exit sound submenu on Esc
		if (keyBoard.Esc()) {
			PlaySoundClick();
			inSoundSubmenu = false;
			sound = 0;
		}
	}
	else {
		// Action in settings board
		// SelectSettings: 0 = Sound, 1 = Back to Menu, 2 = Exit Game
		if (keyBoard.Up() ^ keyBoard.Down()) {
			if (keyBoard.Up()) {
				--SelectSettings;
				if (SelectSettings < 0)
					SelectSettings = 2;
			}
			else {
				++SelectSettings;
				if (SelectSettings > 2)
					SelectSettings = 0;
			}
			PlaySoundClick();
			justEnteredSettings = false; // Clear flag on navigation
		}

		// Enter key action 
		if (keyBoard.Enter() && !justEnteredSettings) {
			PlaySoundClick();
			if (SelectSettings == 0) {
				// Enter sound settings submenu
				inSoundSubmenu = true;
				sound = 0;
			}
			else if (SelectSettings == 1) { // back to menu
				backToMenu = true;
				SelectSettings = 0;
				wasInSettings = false; // Reset flag when leaving
				state = 0;
			}
			else if (SelectSettings == 2) { // exit game
				ExitGame = true;
				window.close();
			}
		}
		else if (keyBoard.Enter() && justEnteredSettings) {
			// check if we enter the settings for the first time (ignore the enter key)
			justEnteredSettings = false;
		}

		// exit settings on Esc key
		if (keyBoard.Esc()) {
			PlaySoundClick();
			inSoundSubmenu = false;
			SelectSettings = 0;
			wasInSettings = false; // Reset flag when leaving
			state = 0; // assuming state 0 is the main menu
		}
	}
}

void Settings::settingBox(RenderWindow& window)
{
	// Draw settings menu
	RectangleShape bg(Vector2f(window.getSize().x, window.getSize().y));
	bg.setFillColor(Color(30, 30, 30));
	window.draw(bg); // thich them background gi thi tu them vao

	float boxWidth = GetBoxWidth(window);
	float boxHeight = GetBoxHeight(window);

	RectangleShape settingsBox(Vector2f(boxWidth, boxHeight));
	settingsBox.setFillColor(Color(50, 50, 50, 240));
	settingsBox.setOutlineColor(Color::White);
	settingsBox.setOutlineThickness(window.getSize().y * 0.003f); 
	settingsBox.setPosition(
		window.getSize().x / 2 - boxWidth / 2,
		window.getSize().y / 2 - boxHeight / 2
	);
	window.draw(settingsBox);
	Text titleText;
	titleText.setFont(font);
	titleText.setString("Settings");
	titleText.setCharacterSize((int)(window.getSize().y * 0.05f)); 
	titleText.setFillColor(Color::Yellow);
	FloatRect titleBounds = titleText.getLocalBounds();
	titleText.setPosition(
		window.getSize().x / 2 - titleBounds.width / 2,
		window.getSize().y / 2 - boxHeight / 2 + window.getSize().y * 0.03f 
	);
	window.draw(titleText);

	SettingButtons(window);
}

void Settings::SettingButtons(RenderWindow& window)
{
	// Use same dimensions as settings box
	float boxWidth = GetBoxWidth(window);
	float boxHeight = GetBoxHeight(window);

	// window size
	float winWidth = window.getSize().x;
	float winHeight = window.getSize().y;

	float startY = window.getSize().y / 2 - boxHeight / 2 + winHeight * 0.12f; 
	float buttonHeight = winHeight * 0.06f; 
	float buttonSpacing = winHeight * 0.12f; 
	float outlineThick = winHeight * 0.004f; 
	float outlineThickSelected = winHeight * 0.006f; 
    
	// Sound button
	RectangleShape soundBox(Vector2f(boxWidth * 0.75f, buttonHeight));
	soundBox.setFillColor(SelectSettings == 0 ? Color(100, 150, 200) : Color(70, 70, 70));
	soundBox.setOutlineColor(SelectSettings == 0 ? Color::Yellow : Color::White);
	soundBox.setOutlineThickness(SelectSettings == 0 ? outlineThickSelected : outlineThick);
	soundBox.setPosition(
		window.getSize().x / 2 - boxWidth * 0.375f,
		startY 
	);
	window.draw(soundBox);

	// Sound button text (centered)
	Text soundText;
	soundText.setFont(font);
	soundText.setString("Sound");
	soundText.setCharacterSize((int)(winHeight * 0.04f)); 
	soundText.setFillColor(SelectSettings == 0 ? Color::Yellow : Color::White);
	FloatRect soundTextBounds = soundText.getLocalBounds();
	soundText.setPosition(
		window.getSize().x / 2 - soundTextBounds.width / 2,
		startY + buttonHeight / 2 - soundTextBounds.height / 2 - winHeight*0.01f
	);
	window.draw(soundText);

	// Mute/Unmute indicator inside Sound box (always visible)
	std::string muteStatus = SoundSettings ? "Unmute" : "Mute"; 
	Text muteText;
	muteText.setFont(font);
	muteText.setString(muteStatus);
	muteText.setCharacterSize((int)(winHeight * 0.03f)); 
	if (inSoundSubmenu && sound == 0) {
		muteText.setFillColor(Color::Yellow);
	}
	else {
		muteText.setFillColor(SoundSettings ? Color(100, 200, 100) : Color(200, 100, 100));
	}
	FloatRect muteTextBounds = muteText.getLocalBounds();
	float muteX = window.getSize().x / 2 + boxWidth * 0.375f - muteTextBounds.width - winWidth * 0.005f;
	muteText.setPosition(
		muteX,
		startY + buttonHeight / 2 - muteTextBounds.height / 2 
	);
	window.draw(muteText);
	
	// Show main settings buttons when NOT in sound submenu
	if (!inSoundSubmenu) {
		// Back to Menu button
		RectangleShape backBox(Vector2f(boxWidth * 0.75f, buttonHeight));
		backBox.setFillColor(SelectSettings == 1 ? Color(100, 150, 200) : Color(70, 70, 70));
		backBox.setOutlineColor(SelectSettings == 1 ? Color::Yellow : Color::White);
		backBox.setOutlineThickness(SelectSettings == 1 ? outlineThickSelected : outlineThick);
		backBox.setPosition(
			window.getSize().x / 2 - boxWidth * 0.375f,
			startY + buttonSpacing 
		);
		window.draw(backBox);
		
		// Back to Menu button text
		Text backText;
		backText.setFont(font);
		backText.setString("Back to Menu");
		backText.setCharacterSize((int)(winHeight * 0.04f)); 
		backText.setFillColor(SelectSettings == 1 ? Color::Yellow : Color::White);
		FloatRect backTextBounds = backText.getLocalBounds();
		backText.setPosition(
			window.getSize().x / 2 - backTextBounds.width / 2,
			startY + buttonSpacing + buttonHeight / 2 - backTextBounds.height / 2 - winHeight * 0.01f
		);
		window.draw(backText);

		// Exit Game button
		RectangleShape exitBox(Vector2f(boxWidth * 0.75f, buttonHeight));
		exitBox.setFillColor(SelectSettings == 2 ? Color(100, 150, 200) : Color(70, 70, 70));
		exitBox.setOutlineColor(SelectSettings == 2 ? Color::Yellow : Color::White);
		exitBox.setOutlineThickness(SelectSettings == 2 ? outlineThickSelected : outlineThick);
		exitBox.setPosition(
			window.getSize().x / 2 - boxWidth * 0.375f,
			startY + buttonSpacing * 2
		);
		window.draw(exitBox);
		
		// Exit Game button text
		Text exitText;
		exitText.setFont(font);
		exitText.setString("Exit Game");
		exitText.setCharacterSize((int)(winHeight * 0.04f)); // 3.7% of height
		exitText.setFillColor(SelectSettings == 2 ? Color::Yellow : Color::White);
		FloatRect exitTextBounds = exitText.getLocalBounds();
		exitText.setPosition(
			window.getSize().x / 2 - exitTextBounds.width / 2,
			startY + buttonSpacing * 2 + buttonHeight / 2 - exitTextBounds.height / 2 - winHeight * 0.01f
		);
		window.draw(exitText);
	}
	else {
		// Show sound submenu when in sound submenu
		subSoundSettingBox(window);
	}
}

void Settings::subSoundSettingBox(RenderWindow& window)
{
	float boxWidth = GetBoxWidth(window);
	float boxHeight = GetBoxHeight(window);
	float winHeight = window.getSize().y;
	float winWidth = window.getSize().x;

	float volBoxwidth = boxWidth * 0.75f;
	float volBoxHeight = winHeight * 0.06f;
	float startY = window.getSize().y / 2 - boxHeight / 2 + winHeight * 0.12f;
	float buttonHeight = winHeight * 0.06f;
	float spacing = winHeight * 0.05f;

	// Position music volume box right under Sound button
	float volBoxY = startY + buttonHeight + spacing;

	// Music Volume Box
	RectangleShape MusicSettingBox(Vector2f(volBoxwidth, volBoxHeight));
	MusicSettingBox.setFillColor((sound == 1) ? Color(100, 150, 200) : Color(70, 70, 70));
	MusicSettingBox.setOutlineColor((sound == 1) ? Color::Yellow : Color::White);
	float outlineThick = winHeight * 0.003f; // line while not selected
	float outlineThickSelected = winHeight * 0.005f; // line when selected
	MusicSettingBox.setOutlineThickness((sound == 1) ? outlineThickSelected : outlineThick);
	MusicSettingBox.setPosition(
		window.getSize().x / 2 - boxWidth * 0.375f,
		volBoxY
	);
	window.draw(MusicSettingBox);

	// Music Volume Text
	Text musicText;
	musicText.setFont(font);
	musicText.setString("Music Volume: " + std::to_string(MusicVolumeLevel * 5) + "%");
	musicText.setCharacterSize((int)(winHeight * 0.02f));
	musicText.setFillColor((sound == 1) ? Color::Yellow : Color::White);
	FloatRect musicTextBounds = musicText.getLocalBounds();
	musicText.setPosition(
		window.getSize().x / 2 - musicTextBounds.width / 2,
		volBoxY + winHeight * 0.0065f
	);
	window.draw(musicText);

	// Music Volume Bar Background
	float barWidth = volBoxwidth * 0.8f;
	float barHeight = winHeight * 0.008f;
	float barX = window.getSize().x / 2 - barWidth / 2;
	float barY = volBoxY + winHeight * 0.036f;

	RectangleShape musicBarBg(Vector2f(barWidth, barHeight));
	musicBarBg.setFillColor(Color(30, 30, 30));
	musicBarBg.setOutlineColor(Color::White);
	musicBarBg.setOutlineThickness(winHeight * 0.0008f); // 0.08% of height
	musicBarBg.setPosition(barX, barY);
	window.draw(musicBarBg);

	// Music Volume Bar Fill
	float musicFillWidth = barWidth * (MusicVolumeLevel / 20.0f);
	RectangleShape musicBarFill(Vector2f(musicFillWidth, barHeight));
	musicBarFill.setFillColor((sound == 1) ? Color(100, 200, 100) : Color(50, 150, 50));
	musicBarFill.setPosition(barX, barY);
	window.draw(musicBarFill);

	// Effect Volume Box (right under Music Volume)
	float effectBoxY = volBoxY + volBoxHeight + spacing;
	RectangleShape EffectSettingBox(Vector2f(volBoxwidth, volBoxHeight));
	EffectSettingBox.setFillColor((sound == 2) ? Color(100, 150, 200) : Color(70, 70, 70));
	EffectSettingBox.setOutlineColor((sound == 2) ? Color::Yellow : Color::White);
	EffectSettingBox.setOutlineThickness((sound == 2) ? outlineThickSelected : outlineThick);
	EffectSettingBox.setPosition(
		window.getSize().x / 2 - boxWidth * 0.375f,
		effectBoxY
	);
	window.draw(EffectSettingBox);

	// Effect Volume Text
	Text effectText;
	effectText.setFont(font);
	effectText.setString("Effect Volume: " + std::to_string(EffectVolumeLevel * 5) + "%");
	effectText.setCharacterSize((int)(winHeight * 0.02f)); // 2.9% of height
	effectText.setFillColor((sound == 2) ? Color::Yellow : Color::White);
	FloatRect effectTextBounds = effectText.getLocalBounds();
	effectText.setPosition(
		window.getSize().x / 2 - effectTextBounds.width / 2,
		effectBoxY + winHeight * 0.0065f
	);
	window.draw(effectText);

	// Effect Volume Bar Background
	RectangleShape effectBarBg(Vector2f(barWidth, barHeight));
	effectBarBg.setFillColor(Color(30, 30, 30));
	effectBarBg.setOutlineColor(Color::White);
	effectBarBg.setOutlineThickness(winHeight * 0.0008f); 
	effectBarBg.setPosition(barX, effectBoxY + winHeight * 0.035f);
	window.draw(effectBarBg);

	// Effect Volume Bar Fill
	float effectFillWidth = barWidth * (EffectVolumeLevel / 20.0f);
	RectangleShape effectBarFill(Vector2f(effectFillWidth, barHeight));
	effectBarFill.setFillColor((sound == 2) ? Color(100, 200, 100) : Color(50, 150, 50));
	effectBarFill.setPosition(barX, effectBoxY + winHeight * 0.035f); 
	window.draw(effectBarFill);

}

