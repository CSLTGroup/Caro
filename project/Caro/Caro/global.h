#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "button/button.h"
#include "keyboard/keyboard.h"
#include "boardgame/boardgame.h"
#include "savegame/savegame.h" // load struct RecordData before calling menu
#include "menu/menu.h"
#include "startgame/startgame.h"
#include "loadingscreen/loadingscreen.h"
#include "sfx/sfx.h"
#include "setting/setting.h"
#include "firsttimeplaying/firsttimeplaying.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace sf;
//using namespace std;

extern int stateMenu;
extern Font font;
extern Menu menuGUI;
extern KeyBoardPressed keyBoard;
extern BoardGame boardGame;
extern Settings setting;
extern string playerName[2];
extern bool confirmedNameFirstTime;
