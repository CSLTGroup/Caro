#include "../global.h"
using namespace sf;

void Button::setPosition(int posX, int posY, int widthX, int heightY) {
    x = posX;
    y = posY;
    width = widthX;
    height = heightY;
}
void Button::draw(RenderWindow& window) {

    //rectangle
    RectangleShape buttonGUI = RectangleShape(Vector2f(width, height)); // size
    buttonGUI.setPosition(Vector2f(x, y)); // position : top-left corner
    if (selected) { // hover
        if (state == 0) // menuID
            buttonGUI.setFillColor(Color::Green); // color
        else buttonGUI.setFillColor(Color(100, 100, 100));
    }
    else { // not hover
        if (state == 0)
            buttonGUI.setFillColor(Color::Yellow);
        else buttonGUI.setFillColor(Color(158, 158, 158));
    }
    window.draw(buttonGUI); // Ve len giao dien terminal window

    //text
    Text text;
    int heightText = height * 90 / 100; // 90% button's height

    text.setFont(font);
    text.setString(context); // noi dung button
	text.setCharacterSize(heightText); // character's size = chieu cao text
    FloatRect textBounds = text.getLocalBounds(); // idk <(")

    if (selected) { // hover
        if (state == 0) {
            text.setFillColor(Color::White);
        }
        else text.setFillColor(Color(185, 185, 185));
    }
    else {
        if (state == 0) {
            text.setFillColor(Color(208, 95, 147));
        }
        else text.setFillColor(Color(130, 130, 130));
    }
    text.setPosition(
        x + width / 10.f,
        y + (height / 2.f - (textBounds.top + textBounds.height / 2.f))
    );
    window.draw(text);
}

