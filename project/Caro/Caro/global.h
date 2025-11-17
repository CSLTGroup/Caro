#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "button/button.h"
#include "keyboard/keyboard.h"
#include "boardgame/boardgame.h"
#include "menu/menu.h"
#include "startgame/startgame.h"
#include "loadingscreen/loadingscreen.h"
#include "sfx/sfx.h"
#include "setting/setting.h"
#include <vector>
#include <iostream>
#include <string>

using namespace sf;

extern int state;
extern Font font;
extern Menu menuGUI;
extern KeyBoardPressed keyBoard;
extern BoardGame boardGame;
extern Settings setting;
