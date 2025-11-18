#ifndef _SETTING_H_
#define _SETTING_H_
#include "../global.h"

struct Settings {
    void handleSettings(RenderWindow& window);
    void sfx();
	void settingBox(RenderWindow& window);
	void SettingButtons(RenderWindow& window);
	void subSoundSettingBox(RenderWindow& window);
	void generalSettingsBox(RenderWindow& window, int IDButton, int row);

    /*void handleSettingsOverlay(RenderWindow& window);*/ // For overlay during gameplay
};


#endif
