#include "../global.h"

sf::Keyboard::Key charToKey(char c) {
    c = toupper(c);
    if (c >= 'A' && c <= 'Z')
        return static_cast<sf::Keyboard::Key>(sf::Keyboard::A + (c - 'A'));
    return sf::Keyboard::Unknown;
}

bool KeyBoardPressed::Up() {
	return combineAlphabet['W' - 'A'];
}
bool KeyBoardPressed::Right() {
	return combineAlphabet['D' - 'A'];
}
bool KeyBoardPressed::Down() {
	return combineAlphabet['S' - 'A'];
}
bool KeyBoardPressed::Left() {
	return combineAlphabet['A' - 'A'];
}
bool KeyBoardPressed::Enter() {
    return mask & ENTER;
}
bool KeyBoardPressed::Esc() {
    return mask & ESC;
}
bool KeyBoardPressed::Backspace() {
    return mask & BACKSPACE;
}
bool KeyBoardPressed::Shift() {
    return mask & SHIFT;
}
bool KeyBoardPressed::combineAlphabetCheck(char c, bool uppercase) {
	return combineAlphabet[c - 'A'] && (!uppercase || Shift());
}
void KeyBoardPressed::setState(RenderWindow& window) {
    bool combineAlphabetNXT[26] = { false };
    bool combineAlphabetpreNXT[26] = { false };
    int maskINP = 0;
    bool anyKeyPressed = false;
    for (char c = 'A'; c <= 'Z'; c++) {
        combineAlphabetNXT[c - 'A'] = Keyboard::isKeyPressed(charToKey(c));
        if (combineAlphabetNXT[c - 'A'])
			anyKeyPressed = true;
	}
    if (Keyboard::isKeyPressed(Keyboard::Escape))
		maskINP |= ESC;
    if (Keyboard::isKeyPressed(Keyboard::Enter))
        maskINP |= ENTER;
	if (Keyboard::isKeyPressed(Keyboard::Backspace))
		maskINP |= BACKSPACE;
	if (Keyboard::isKeyPressed(Keyboard::LShift) || Keyboard::isKeyPressed(Keyboard::RShift))
		maskINP |= SHIFT;
    if (Keyboard::isKeyPressed(Keyboard::Up))
        combineAlphabetNXT['W' - 'A'] = true, anyKeyPressed = true;
    if (Keyboard::isKeyPressed(Keyboard::Down))
        combineAlphabetNXT['S' - 'A'] = true, anyKeyPressed = true;
    if (Keyboard::isKeyPressed(Keyboard::Left))
        combineAlphabetNXT['A' - 'A'] = true, anyKeyPressed = true;
    if (Keyboard::isKeyPressed(Keyboard::Right))
        combineAlphabetNXT['D' - 'A'] = true, anyKeyPressed = true;
    if (maskINP) 
        anyKeyPressed = true;
	
    mask = maskINP;
    if (!anyKeyPressed) {
		memset(combineAlphabet, 0, sizeof(combineAlphabet));
		menuGUI.updateState(window);
    }
    else {
        for (int c = 0; c < 26; c++) {
            if (!(combineAlphabet[c]))
                if (combineAlphabetNXT[c])
					combineAlphabetpreNXT[c] = true;
        }

        // premask -> nxtmask
		memcpy(combineAlphabet, combineAlphabetpreNXT, sizeof(combineAlphabet));
        menuGUI.updateState(window);
		memcpy(combineAlphabet, combineAlphabetNXT, sizeof(combineAlphabet));
    }

}