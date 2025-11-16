#ifndef _SETTING_H_
#define _SETTING_H_
#include "../global.h"

struct Settings {
    void handleSettings(RenderWindow& window);
    void handleSettingsOverlay(RenderWindow& window); // For overlay during gameplay
};

#endif
