#pragma once
#include <SFML/Graphics.hpp>
#include "button/button.h"
#include "keyboard/keyboard.h"
#include "boardgame/boardgame.h"
#include "menu/menu.h"
#include <vector>
#include <iostream>

using namespace sf;

extern int state;
extern Font font;
extern Menu menuGUI;
extern KeyBoardPressed keyBoard;
extern BoardGame boardGame;
