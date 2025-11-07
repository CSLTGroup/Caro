#include "../global.h"

bool KeyBoardPressed::Up() {
    return mask & UP;
}
bool KeyBoardPressed::Right() {
    return mask & RIGHT;
}
bool KeyBoardPressed::Down() {
    return mask & DOWN;
}
bool KeyBoardPressed::Left() {
    return mask & LEFT;
}
bool KeyBoardPressed::Enter() {
    return mask & ENTER;
}
bool KeyBoardPressed::Esc() {
    return mask & ESC;
}
bool KeyBoardPressed::isAnyKeyPressed() {
    return mask;
}
void KeyBoardPressed::setState(RenderWindow& window) {
    short maskINP = 0;
    if (Keyboard::isKeyPressed(Keyboard::Key::W))
        maskINP |= UP;
    if (Keyboard::isKeyPressed(Keyboard::Key::A))
        maskINP |= LEFT;
    if (Keyboard::isKeyPressed(Keyboard::Key::S))
        maskINP |= DOWN;
    if (Keyboard::isKeyPressed(Keyboard::Key::D))
        maskINP |= RIGHT;
    if (Keyboard::isKeyPressed(Keyboard::Key::Enter))
        maskINP |= ENTER;
    if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
        maskINP |= ESC;

    if (maskINP == 0) {
        mask = 0;
    }
    else {
        short preMask = 0;
        for (int i = 0; i < 6; i++)
            if (!(mask & (1 << i)))
                if (maskINP & (1 << i))
                    preMask |= (1 << i);

        // premask -> nxtmask
        mask = preMask;
        int nxtMask = maskINP;
        menuGUI.updateState(window);
        mask = maskINP;
    }

}