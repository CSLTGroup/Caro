#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

#include "credit/credit.h"
#include "animation/animation.h"
#include "button/button.h"
#include "keyboard/keyboard.h"
#include "boardgame/boardgame.h"
#include "savegame/savegame.h"
#include "menu/menu.h"
#include "startgame/startgame.h"
#include "loadingscreen/loadingscreen.h"
#include "sfx/sfx.h"
#include "setting/setting.h"
#include "Howtoplay/Howtoplay.h"
#include "firsttimeplaying/firsttimeplaying.h"
#include "others/others.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <windows.h>


extern int stateMenu;
extern Font font;
extern Menu menuGUI;
extern KeyBoardPressed keyBoard;
extern BoardGame boardGame;
extern Settings setting;
extern HowToPlay howToPlay;
extern string playerName[2];
extern bool confirmedSettingsFirstTime;
extern int idWindowSize;
extern Credit credit;
extern vector<pair<int, int> > listWindowSize;
extern int lastState;