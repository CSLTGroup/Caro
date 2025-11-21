#pragma once
#include "../global.h"

using namespace sf;

sf::Keyboard::Key charToKey(char c);

struct KeyBoardPressed {
    bool combineAlphabet[26] = { false };
    int mask = 0;
	// escape : 0, enter : 1, backspace : 2, shift : 3
    static const int ESC = 1 << 0;
    static const int ENTER = 1 << 1;
	static const int BACKSPACE = 1 << 2;
	static const int SHIFT = 1 << 3;

    bool combineAlphabetCheck(char c, bool uppercase = false);
    bool Up();
    bool Right();
    bool Down();
    bool Left();
    bool Enter();
    bool Esc();
	bool Backspace();
	bool Shift();
    void setState(RenderWindow& window);
};