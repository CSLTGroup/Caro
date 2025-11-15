#pragma once
#include "../global.h"

using namespace sf;
struct KeyBoardPressed {
    short mask = 0; // 001010
    const short UP = 1 << 0;
    const short RIGHT = 1 << 1;
    const short LEFT = 1 << 2;
    const short DOWN = 1 << 3;
    const short ENTER = 1 << 4;
    const short ESC = 1 << 5;
    const short ALL = (1 << 6) - 1;

    bool Up();
    bool Right();
    bool Down();
    bool Left();
    bool Enter();
    bool Esc();
    bool isAnyKeyPressed();
    void setState(RenderWindow& window);
};