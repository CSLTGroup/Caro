#include "../global.h"

// general settings variables
static int SelectSettings = 0; // 0: Sound 1: Back to Menu 2: Exit Game
static int SelectPage = 0; // 0: first 3 buttons; 1: next 3 buttons; ...
const int totalSetButtons = 3;
const int totalPage = (totalSetButtons + 2 ) / 3;
bool hoverSetButtons[totalSetButtons] = { false }; // hover button settings
bool inSetButtons[totalSetButtons] = { false }; // check in button settings
string contextSetButtons[totalSetButtons] = { "Sound", "Back to Menu", "Exit Game" };
bool initialized = false;
static bool inSettings = false;
static bool inGeneralSettings = false; // to ignore first enter key

// id buttons in settings
const int IDSFX = 0;
const int IDBACKTOMENU = 1;
const int IDEXITGAME = 2;

// sound settings variables
int IDSoundButtons = 0; // 0: mute/ unmute sound 1: adjust music volume 2: adjust effect volume
bool isNotMuted = true; // true: on, false: off
int MusicVolumeLevel = 20; // from 0 to 20 step 5% in 100% (0-100%)
int EffectVolumeLevel = 20; // from 0 to 20 step 5% in 100% (0-100%)
static bool inSoundSubmenu = false; // track if we're in the sound settings submenu

// UI handle
static float boxWidth = 0.0f; // window.getSize().x * 0.4f;
static float boxHeight = 0.0f; // window.getSize().y * 0.5f;

void Settings::sfx() {
	if (keyBoard.Up() ^ keyBoard.Down()) {
		if (keyBoard.Up()) {
			--IDSoundButtons;
			if (IDSoundButtons < 0)
				IDSoundButtons = 2;
		}
		else {
			++IDSoundButtons;
			if (IDSoundButtons == 3)
				IDSoundButtons = 0;
		}
		PlaySoundClick();
	}
	if (IDSoundButtons == 0) // Adjust mute/unmute
	{
		if (keyBoard.Enter())
		{
			PlaySoundClick();
			isNotMuted = !isNotMuted;
			SoundMute();
		}
	}
	else if (IDSoundButtons == 1) { // Adjust music volume (sound == 1)
		if (keyBoard.Left() ^ keyBoard.Right()) {
			float preMusicVolumeLevel = MusicVolumeLevel;

			if (keyBoard.Left())
				MusicVolumeLevel = max(MusicVolumeLevel - 1, 0);
			else MusicVolumeLevel = min(MusicVolumeLevel + 1, 20);

			if (preMusicVolumeLevel != MusicVolumeLevel) {
				float newVolume = MusicVolumeLevel * 5.0f;
				PlaySoundClick();
				SetMusicVolume(newVolume);
			} // else do nothing
		}
	}
	// Adjust effect volume (sound == 2)
	else if (IDSoundButtons == 2) {
		if (keyBoard.Left() ^ keyBoard.Right()) {
			float preEffectVolumeLevel = EffectVolumeLevel;

			if (keyBoard.Left())
				EffectVolumeLevel = max(EffectVolumeLevel - 1, 0);
			else EffectVolumeLevel = min(EffectVolumeLevel + 1, 20);

			if (EffectVolumeLevel != preEffectVolumeLevel) {
				float newVolume = EffectVolumeLevel * 5.0f;
				PlaySoundClick();
				SetEffectVolume(newVolume);
			}
		}
	}
}

void Settings::handleSettings(RenderWindow& window)
{
	if (!initialized) {

		// music part
		MusicVolumeLevel = (int)(GetMusicVolume() / 5.0f);
		EffectVolumeLevel = (int)(GetEffectVolume() / 5.0f);

		// UI part
		boxWidth = window.getSize().x * 0.4f;
		boxHeight = window.getSize().y * 0.5f;

		initialized = true;
	}
	
    settingBox(window);
	if (!inSettings) { // avoid first enter key issue

		// initialize settings part
		SelectSettings = IDSFX; // Reset to Sound button - first Button
		SelectPage = 0;
		inSettings = true;
		inGeneralSettings = true;
	}
	else if (inGeneralSettings) {

		// Action in settings board
		// SelectSettings: 0 = Sound, 1 = Back to Menu, 2 = Exit Game
		if (keyBoard.Up() ^ keyBoard.Down()) {
			hoverSetButtons[SelectSettings] = false;
			if (keyBoard.Up()) {
				--SelectSettings;
				if (SelectSettings < 0)
					SelectSettings = totalSetButtons - 1;
			}
			else {
				++SelectSettings;
				if (SelectSettings == totalSetButtons)
					SelectSettings = 0;
			}
			hoverSetButtons[SelectSettings] = true;
			PlaySoundClick();
		}

		// enter action in settings board
		if (keyBoard.Enter() || keyBoard.Esc()) {
			inGeneralSettings = false;
			if (SelectSettings == IDBACKTOMENU || keyBoard.Esc()) { // back to menu
				SelectSettings = 0;
				inSettings = false; // Reset flag when leaving
				stateMenu = 0; // return to main menu
				return;
			}

			// other buttons
			inSetButtons[SelectSettings] = true;
			if (SelectSettings == IDSFX) { // sound settings submenu
				IDSoundButtons = 0;
			}
			else if (SelectSettings == IDEXITGAME) { // exit game
				window.close();
			}
			PlaySoundClick();
		}
	}
	else // for specific submenu handling
	if (inSetButtons[IDSFX]) { // Handle sound submenu navigation
		sfx();
		// Exit sound submenu on Esc
		if (keyBoard.Esc()) {
			PlaySoundClick();
			inSetButtons[IDSFX] = false;
			IDSoundButtons = 0; // reset to mute/unmute
			inGeneralSettings = true; // back to general settings
		}
	}
}

void Settings::settingBox(RenderWindow& window)
{
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
	settingsBox.setPosition(
		winWidth / 2 - boxWidth / 2,
		winHeight / 2 - boxHeight / 2
	);
	window.draw(settingsBox);
	Text titleText;
	titleText.setFont(font);
	titleText.setString("Settings");
	titleText.setCharacterSize((int)(winHeight * 0.05f));
	titleText.setFillColor(Color::Yellow);
	FloatRect titleBounds = titleText.getLocalBounds();
	titleText.setPosition(
		window.getSize().x / 2 - titleBounds.width / 2,
		winHeight / 2 - boxHeight / 2 + winHeight * 0.03f
	);
	window.draw(titleText);

	// draw page
	Text pageText;
	pageText.setFont(font);
	pageText.setString("Page " + to_string(SelectPage + 1) + "/" + to_string(totalPage));
	pageText.setCharacterSize((int)(winHeight * 0.03f));
	pageText.setStyle(Text::Italic);
	pageText.setFillColor(Color(120, 158, 158));
	FloatRect pageBounds = pageText.getLocalBounds();
	pageText.setOrigin(pageBounds.width / 2, pageBounds.height / 2);
	pageText.setPosition(
		winWidth / 2 + boxWidth / 2 - boxWidth * 0.125 ,
		winHeight / 2 + boxHeight / 2 - boxHeight * 0.07f
	);
	window.draw(pageText);


	// draw buttons
	SettingButtons(window);
}

void Settings::SettingButtons(RenderWindow& window)
{

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
	if (inGeneralSettings || !inSettings) { // fix auto enter issue
		for (int IDButton = SelectPage / 3, cntBut = 1; IDButton < totalSetButtons && cntBut <= 3; ++IDButton, ++cntBut) {
			// Box for each button
			RectangleShape buttonBox(Vector2f(buttonWidth, buttonHeight));
			buttonBox.setFillColor(SelectSettings == IDButton ? Color(100, 150, 200) : Color(70, 70, 70));
			buttonBox.setOutlineColor(SelectSettings == IDButton ? Color::Yellow : Color::White);
			buttonBox.setOutlineThickness(SelectSettings == IDButton ? outlineThickSelected : outlineThick);
			buttonBox.setPosition(
				winWidth / 2 - boxWidth * 0.375f,
				startY + buttonSpacing * IDButton
			);

			// Button text (centered)
			Text buttonText;
			buttonText.setFont(font);
			buttonText.setString(contextSetButtons[IDButton]);
			buttonText.setCharacterSize((int)(winHeight * 0.04f));
			buttonText.setFillColor(SelectSettings == IDButton ? Color::Yellow : Color::White);
			FloatRect buttonTextBounds = buttonText.getLocalBounds();
			buttonText.setPosition(
				winWidth / 2 - buttonTextBounds.width / 2,
				startY + buttonSpacing * IDButton + buttonHeight / 2 - buttonTextBounds.height / 2 - winHeight * 0.01f
			);

			window.draw(buttonBox);
			window.draw(buttonText);
		}
		// For specific buttons
		if (SelectPage == 0) {// First Page contains Sound button
			// Mute/Unmute indicator inside Sound box (always visible)
			std::string muteStatus = isNotMuted ? "Unmute" : "Mute";
			Text muteText;
			muteText.setFont(font);
			muteText.setString(muteStatus);
			muteText.setCharacterSize((int)(winHeight * 0.03f));
			if (inSoundSubmenu && IDSoundButtons == 0) {
				muteText.setFillColor(Color::Yellow);
			}
			else {
				muteText.setFillColor(isNotMuted ? Color(100, 200, 100) : Color(200, 100, 100));
			}
			FloatRect muteTextBounds = muteText.getLocalBounds();
			float muteX = winWidth / 2 + boxWidth * 0.375f - muteTextBounds.width - winWidth * 0.005f;
			muteText.setPosition(
				muteX,
				startY + buttonHeight / 2 - muteTextBounds.height / 2
			);
			window.draw(muteText);
		}
	}
	else if (inSetButtons[IDSFX]) {
		// draw sound setting

		subSoundSettingBox(window);
	}
}

void Settings::subSoundSettingBox(RenderWindow& window)
{
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
	MusicSettingBox.setFillColor((IDSoundButtons == IDSFX) ? Color(100, 150, 200) : Color(70, 70, 70));
	MusicSettingBox.setOutlineColor((IDSoundButtons == 1) ? Color::Yellow : Color::White);
	float outlineThick = winHeight * 0.003f; // line while not selected
	float outlineThickSelected = winHeight * 0.005f; // line when selected
	MusicSettingBox.setOutlineThickness((IDSoundButtons == 1) ? outlineThickSelected : outlineThick);
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
	musicText.setFillColor((IDSoundButtons == 1) ? Color::Yellow : Color::White);
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
	musicBarFill.setFillColor((IDSoundButtons == 1) ? Color(100, 200, 100) : Color(50, 150, 50));
	musicBarFill.setPosition(barX, barY);
	window.draw(musicBarFill);

	// Effect Volume Box (right under Music Volume)
	float effectBoxY = volBoxY + volBoxHeight + spacing;
	RectangleShape EffectSettingBox(Vector2f(volBoxwidth, volBoxHeight));
	EffectSettingBox.setFillColor((IDSoundButtons == 2) ? Color(100, 150, 200) : Color(70, 70, 70));
	EffectSettingBox.setOutlineColor((IDSoundButtons == 2) ? Color::Yellow : Color::White);
	EffectSettingBox.setOutlineThickness((IDSoundButtons == 2) ? outlineThickSelected : outlineThick);
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
	effectText.setFillColor((IDSoundButtons == 2) ? Color::Yellow : Color::White);
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
	effectBarFill.setFillColor((IDSoundButtons == 2) ? Color(100, 200, 100) : Color(50, 150, 50));
	effectBarFill.setPosition(barX, effectBoxY + winHeight * 0.035f); 
	window.draw(effectBarFill);

}

