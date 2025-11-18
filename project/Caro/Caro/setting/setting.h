#ifndef _SETTING_H_
#define _SETTING_H_
#include "../global.h"

struct Settings {
  void handleSettings(RenderWindow &window);
  void sfx();
  void settingBox(RenderWindow &window);
  void SettingButtons(RenderWindow &window);
  void subSoundSettingBox(RenderWindow &window);
  void generalSettingsBox(RenderWindow &window, int IDButton, int row);

  // For overlay during gameplay
  void handleSettingsOverlay(RenderWindow &window);
  void settingBoxOverlay(RenderWindow &window);
  void SettingButtonsOverlay(RenderWindow &window);
  void generalSettingsBoxOverlay(RenderWindow &window, int IDButton, int row);
};

#endif
