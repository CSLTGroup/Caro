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
    bool combineAlphabetNXT[26] = { false }; // input mới từ bàn phím
    bool combineAlphabetpreNXT[26] = { false }; // trạng thái được sử dụng để truyền xuống các hàm xử 
    // combineAlphabet và mask: dữ liệu bàn phím lưu trước đây
	int maskINP = 0; // quy ước bit 0: escape, bit 1: enter, bit 2: backspace, bit 3: shift
    bool anyKeyPressed = false;
	// nhập input vào combineAlphabetNXT và maskINP
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

	if (!anyKeyPressed) { // reset về 0 nếu không có phím nào được nhấn
        mask = 0;
		memset(combineAlphabet, 0, sizeof(combineAlphabet));
		menuGUI.updateState(window);
    }
	else { // cập nhật trạng thái mới
        for (int c = 0; c < 26; c++) {
            if (!(combineAlphabet[c]) && combineAlphabetNXT[c])
				combineAlphabetpreNXT[c] = true;
        }
		int maskpreNXT = 0;
        for (int i = 0; i <= 3; i++) {
            if (!(mask & (1 << i)) && (maskINP & (1 << i)))
                maskpreNXT |= (1 << i); // set premask bit
		}

		// đẩy premask và precombineAlphabet xuống menuGUI.updateState trước khi cập nhật trạng thái mới
		mask = maskpreNXT;
		memcpy(combineAlphabet, combineAlphabetpreNXT, sizeof(combineAlphabet));
        menuGUI.updateState(window);
        // trạng thái lưu trữ mới = input bàn phím
		mask = maskINP;
		memcpy(combineAlphabet, combineAlphabetNXT, sizeof(combineAlphabet));
    }
}