#include "../global.h"
using namespace sf;

void Button::setPosition(int posX, int posY, int widthX, int heightY) {
    x = posX;
    y = posY;
    width = widthX;
    height = heightY;
    needUpdate = true;
}
void Button::setMenuTheme(bool enable) {
    menuColorScheme = enable;
}
void Button::draw(RenderWindow& window) {
    const float winWidth = window.getSize().x;
    const float winHeight = window.getSize().y;
    const int heightText = height * 90 / 100; // 90% button's height

    static bool initialized = false;
    static Texture focusTexture;
    static Texture unfocusTexture;
    static Texture shadowTex;
    if (!initialized) {
        initialized = true;
        focusTexture.loadFromFile("assets/image/focusButton.png");
        unfocusTexture.loadFromFile("assets/image/unfocusButton.png");
        shadowTex.loadFromFile("assets/image/shadow2.png");
    }

    // draw text
    Text text;

    text.setFont(font);
    text.setString(context); // noi dung button
	text.setCharacterSize(heightText); // character's size = chieu cao text
    FloatRect textBounds = text.getLocalBounds();

    bool useMenuColors = menuColorScheme || stateMenu == 0;

    if (selected) { // hover
        if (useMenuColors) {
            text.setFillColor(Color(254, 255, 195));
        }
        else text.setFillColor(Color(185, 185, 185));
    }
    else {
        if (useMenuColors) {
            text.setFillColor(Color(223, 252, 255));
        }
        else text.setFillColor(Color(130, 130, 130));
    }
    text.setOrigin(0, textBounds.height);
    text.setPosition(
        x + width / 10.f,
        y //+ (context == "Settings" || context == "How to Play") * (height / 10) 
        //- (context != "Settings" && context != "How to Play") * (height / 10)
        - height / 10
    );

    // draw box menu & shadow
    if (needUpdate) {

        // box menu
        if (selected)
            spriteButton.setTexture(focusTexture);
        else spriteButton.setTexture(unfocusTexture);

        spriteButton.setOrigin(spriteButton.getLocalBounds().width / 2, spriteButton.getLocalBounds().height / 2);
        float scaleX = 1.0f * width / focusTexture.getSize().x;
        float scaleY = 1.0f * height / focusTexture.getSize().y;
        spriteButton.setScale(scaleX, scaleY);
        spriteButton.setPosition(winWidth / 2, y);

        // shadow
        spriteShadow.setTexture(shadowTex);
        FloatRect bounds = spriteShadow.getLocalBounds();
        spriteShadow.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
        scaleX = 1.40f * textBounds.width / shadowTex.getSize().x;
        scaleY = 1.0f * textBounds.height / shadowTex.getSize().y;
        spriteShadow.setScale(scaleX, scaleY);
        spriteShadow.setPosition(winWidth / 2.0f - width / 2.0f + width / 10.0f + textBounds.width / 2.0f, y); // set to the left
        spriteShadow.setColor(sf::Color(255, 255, 255, 120));

        needUpdate = false;
    }


    // draw onto the screen
    window.draw(spriteButton);
    window.draw(spriteShadow);
    window.draw(text);
}