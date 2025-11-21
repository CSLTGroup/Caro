#ifndef _SETTING_H_
#define _SETTING_H_
#include "../global.h"

struct Settings {
  void SettingsLogic(RenderWindow &window);
  void sfx();
  void draw(RenderWindow &window);
  void SettingButtons(RenderWindow &window);
  void subSoundSettingBox(RenderWindow &window);
  void generalSettingsBox(RenderWindow &window, int IDButton, int row, string contextString = "", int selectedC = -1);

  // For overlay during gameplay
  void handleSettingsOverlay(RenderWindow &window);
  void settingBoxOverlay(RenderWindow &window);
  void SettingButtonsOverlay(RenderWindow &window);
  void generalSettingsBoxOverlay(RenderWindow &window, int IDButton, int row);
};

#endif
