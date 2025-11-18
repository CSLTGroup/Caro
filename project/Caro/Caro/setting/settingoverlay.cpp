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
inline float GetBoxWidth(RenderWindow &window) {
  return boxWidth;
}
inline float GetBoxHeight(RenderWindow &window) {
  return boxHeight;
}

// Overlay Settings Functions
void Settings::handleSettingsOverlay(RenderWindow &window) {
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
  } else {
    // Action in settings board
    // SelectSettingsOverlay: 0 = Sound, 1 = Save, 2 = Load, 3 = Back to Menu, 4
    // = Exit Game
    if (keyBoard.Up() ^ keyBoard.Down()) {
      if (keyBoard.Up()) {
        --SelectSettingsOverlay;
        if (SelectSettingsOverlay < 0)
          SelectSettingsOverlay = 4;
      } else {
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
      } else if (SelectSettingsOverlay == 1) { // Save
        // SaveGame();
      } else if (SelectSettingsOverlay == 2) { // Load
        // LoadGame();
      } else if (SelectSettingsOverlay == 3) { // back to menu
        backToMenu = true;
        SelectSettingsOverlay = 0;
        state = 0;
      } else if (SelectSettingsOverlay == 4) { // exit game
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

void Settings::settingBoxOverlay(RenderWindow &window) {
  // Draw settings menu overlay
  RectangleShape bg(Vector2f(window.getSize().x, window.getSize().y));
  bg.setFillColor(Color(0, 0, 0, 180));
  window.draw(bg);

  float boxWidth = GetBoxWidth(window);
  float boxHeight = GetBoxHeight(window);

  RectangleShape settingsBox(Vector2f(boxWidth, boxHeight));
  settingsBox.setFillColor(Color(50, 50, 50, 240));
  settingsBox.setOutlineColor(Color::White);
  settingsBox.setOutlineThickness(window.getSize().y * 0.003f);
  settingsBox.setPosition(window.getSize().x / 2 - boxWidth / 2,
                          window.getSize().y / 2 - boxHeight / 2);
  window.draw(settingsBox);
  Text titleText;
  titleText.setFont(font);
  titleText.setString("Settings");
  titleText.setCharacterSize((int)(window.getSize().y * 0.05f));
  titleText.setFillColor(Color::Yellow);
  FloatRect titleBounds = titleText.getLocalBounds();
  titleText.setPosition(window.getSize().x / 2 - titleBounds.width / 2,
                        window.getSize().y / 2 - boxHeight / 2 +
                            window.getSize().y * 0.03f);
  window.draw(titleText);

  SettingButtonsOverlay(window);
}

void Settings::SettingButtonsOverlay(RenderWindow &window) {
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
  soundBox.setFillColor(SelectSettingsOverlay == 0 ? Color(100, 150, 200)
                                                   : Color(70, 70, 70));
  soundBox.setOutlineColor(SelectSettingsOverlay == 0 ? Color::Yellow
                                                      : Color::White);
  soundBox.setOutlineThickness(SelectSettingsOverlay == 0 ? outlineThickSelected
                                                          : outlineThick);
  soundBox.setPosition(window.getSize().x / 2 - boxWidth * 0.375f, startY);
  window.draw(soundBox);

  // Sound button text (centered)
  Text soundText;
  soundText.setFont(font);
  soundText.setString("Sound");
  soundText.setCharacterSize((int)(winHeight * 0.04f));
  soundText.setFillColor(SelectSettingsOverlay == 0 ? Color::Yellow
                                                    : Color::White);
  FloatRect soundTextBounds = soundText.getLocalBounds();
  soundText.setPosition(window.getSize().x / 2 - soundTextBounds.width / 2,
                        startY + buttonHeight / 2 - soundTextBounds.height / 2 -
                            winHeight * 0.01f);
  window.draw(soundText);

  // Mute/Unmute indicator inside Sound box (always visible)
  std::string muteStatus = SoundSettings ? "Unmute" : "Mute";
  Text muteText;
  muteText.setFont(font);
  muteText.setString(muteStatus);
  muteText.setCharacterSize((int)(winHeight * 0.03f));
  // Highlight when in submenu and sound == 0 (mute/unmute is selected)
  if (inSoundSubmenuOverlay && sound == 0) {
    muteText.setFillColor(Color::Yellow);
  } else {
    muteText.setFillColor(SoundSettings ? Color(100, 200, 100)
                                        : Color(200, 100, 100));
  }
  FloatRect muteTextBounds = muteText.getLocalBounds();
  float muteX = window.getSize().x / 2 + boxWidth * 0.375f -
                muteTextBounds.width - winWidth * 0.005f;
  muteText.setPosition(muteX,
                       startY + buttonHeight / 2 - muteTextBounds.height / 2);
  window.draw(muteText);

  // Show main settings buttons when NOT in sound submenu
  if (!inSoundSubmenuOverlay) {
    // Save button (between Sound and Back to Menu)
    RectangleShape saveBox(Vector2f(boxWidth * 0.75f, buttonHeight));
    saveBox.setFillColor(SelectSettingsOverlay == 1 ? Color(100, 150, 200)
                                                    : Color(70, 70, 70));
    saveBox.setOutlineColor(SelectSettingsOverlay == 1 ? Color::Yellow
                                                       : Color::White);
    saveBox.setOutlineThickness(
        SelectSettingsOverlay == 1 ? outlineThickSelected : outlineThick);
    saveBox.setPosition(window.getSize().x / 2 - boxWidth * 0.375f,
                        startY + buttonSpacing);
    window.draw(saveBox);

    // Save button text
    Text saveText;
    saveText.setFont(font);
    saveText.setString("Save");
    saveText.setCharacterSize((int)(winHeight * 0.04f));
    saveText.setFillColor(SelectSettingsOverlay == 1 ? Color::Yellow
                                                     : Color::White);
    FloatRect saveTextBounds = saveText.getLocalBounds();
    saveText.setPosition(window.getSize().x / 2 - saveTextBounds.width / 2,
                         startY + buttonSpacing + buttonHeight / 2 -
                             saveTextBounds.height / 2 - winHeight * 0.01f);
    window.draw(saveText);

    // Load button (between Save and Back to Menu)
    RectangleShape loadBox(Vector2f(boxWidth * 0.75f, buttonHeight));
    loadBox.setFillColor(SelectSettingsOverlay == 2 ? Color(100, 150, 200)
                                                    : Color(70, 70, 70));
    loadBox.setOutlineColor(SelectSettingsOverlay == 2 ? Color::Yellow
                                                       : Color::White);
    loadBox.setOutlineThickness(
        SelectSettingsOverlay == 2 ? outlineThickSelected : outlineThick);
    loadBox.setPosition(window.getSize().x / 2 - boxWidth * 0.375f,
                        startY + buttonSpacing * 2);
    window.draw(loadBox);

    // Load button text
    Text loadText;
    loadText.setFont(font);
    loadText.setString("Load");
    loadText.setCharacterSize((int)(winHeight * 0.04f));
    loadText.setFillColor(SelectSettingsOverlay == 2 ? Color::Yellow
                                                     : Color::White);
    FloatRect loadTextBounds = loadText.getLocalBounds();
    loadText.setPosition(window.getSize().x / 2 - loadTextBounds.width / 2,
                         startY + buttonSpacing * 2 + buttonHeight / 2 -
                             loadTextBounds.height / 2 - winHeight * 0.01f);
    window.draw(loadText);

    // Back to Menu button
    RectangleShape backBox(Vector2f(boxWidth * 0.75f, buttonHeight));
    backBox.setFillColor(SelectSettingsOverlay == 3 ? Color(100, 150, 200)
                                                    : Color(70, 70, 70));
    backBox.setOutlineColor(SelectSettingsOverlay == 3 ? Color::Yellow
                                                       : Color::White);
    backBox.setOutlineThickness(
        SelectSettingsOverlay == 3 ? outlineThickSelected : outlineThick);
    backBox.setPosition(window.getSize().x / 2 - boxWidth * 0.375f,
                        startY + buttonSpacing * 3);
    window.draw(backBox);

    // Back to Menu button text
    Text backText;
    backText.setFont(font);
    backText.setString("Back to Menu");
    backText.setCharacterSize((int)(winHeight * 0.04f));
    backText.setFillColor(SelectSettingsOverlay == 3 ? Color::Yellow
                                                     : Color::White);
    FloatRect backTextBounds = backText.getLocalBounds();
    backText.setPosition(window.getSize().x / 2 - backTextBounds.width / 2,
                         startY + buttonSpacing * 3 + buttonHeight / 2 -
                             backTextBounds.height / 2 - winHeight * 0.01f);
    window.draw(backText);

    // Exit Game button
    RectangleShape exitBox(Vector2f(boxWidth * 0.75f, buttonHeight));
    exitBox.setFillColor(SelectSettingsOverlay == 4 ? Color(100, 150, 200)
                                                    : Color(70, 70, 70));
    exitBox.setOutlineColor(SelectSettingsOverlay == 4 ? Color::Yellow
                                                       : Color::White);
    exitBox.setOutlineThickness(
        SelectSettingsOverlay == 4 ? outlineThickSelected : outlineThick);
    exitBox.setPosition(window.getSize().x / 2 - boxWidth * 0.375f,
                        startY + buttonSpacing * 4);
    window.draw(exitBox);

    // Exit Game button text
    Text exitText;
    exitText.setFont(font);
    exitText.setString("Exit Game");
    exitText.setCharacterSize((int)(winHeight * 0.04f));
    exitText.setFillColor(SelectSettingsOverlay == 4 ? Color::Yellow
                                                     : Color::White);
    FloatRect exitTextBounds = exitText.getLocalBounds();
    exitText.setPosition(window.getSize().x / 2 - exitTextBounds.width / 2,
                         startY + buttonSpacing * 4 + buttonHeight / 2 -
                             exitTextBounds.height / 2 - winHeight * 0.01f);
    window.draw(exitText);
  } else {
    subSoundSettingBox(window);
  }
}
