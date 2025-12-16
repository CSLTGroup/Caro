#include "../global.h"


Credit::Credit() {
    scrollY = 0;
}

void Credit::initCredits() {
    if (entries.empty()) {
        entries = {
            {"The Apex Penguins", ""},
            {"Thank you for playing", ""},
            {"", ""},
            {"Dang Thanh Nghia", "Group leader"},
            {"Mai Phuc Hung", "SFX & Music"},
            {"", ""},
            {"THANK YOU FOR PLAYING", ""},
            {"", ""}
        };
    }
}

void Credit::CreditLogic(RenderWindow& window) {
    if (keyBoard.Esc()) {
        stateMenu = 0;
    }
}


void Credit::reset(float windowHeight) {
    scrollY = windowHeight + 50.f;
    deltaClock.restart();
}


void Credit::draw(RenderWindow& window) {
    initCredits();

    float winWidth = (float)window.getSize().x;
    float winHeight = (float)window.getSize().y;

    float dt = deltaClock.restart().asSeconds();
    float scrollSpeed = winHeight * 0.15f;

    scrollY -= scrollSpeed * dt;

    float totalHeight = entries.size() * (winHeight * 0.06f);
    if (scrollY < -(totalHeight + winHeight * 0.5f)) {
        scrollY = winHeight;
    }

    sf::RectangleShape dim(Vector2f(winWidth, winHeight));
    dim.setFillColor(Color(10, 10, 15, 255));
    window.draw(dim);


    float fontSize = winHeight * 0.035f;
    float lineHeight = winHeight * 0.06f;
    float gapFromCenter = winWidth * 0.02f;
    float centerX = winWidth * 0.5f;

    for (size_t i = 0; i < entries.size(); ++i) {
        float yPos = scrollY + (i * lineHeight);

        if (yPos > -lineHeight && yPos < winHeight + lineHeight) {

            if (!entries[i].name.empty()) {
                Text nameText;
                nameText.setFont(font);
                nameText.setString(entries[i].name);
                nameText.setCharacterSize((unsigned int)fontSize);
                nameText.setFillColor(sf::Color::White);

                FloatRect bounds = nameText.getLocalBounds();

                // title group name and thks for playing
                if (entries[i].role.empty()) {
                    nameText.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top);
                    nameText.setPosition(centerX, yPos);
                    nameText.setStyle(sf::Text::Bold);
                    nameText.setFillColor(sf::Color::Yellow);
                }
                else {
                    nameText.setOrigin(bounds.left + bounds.width, bounds.top);
                    nameText.setPosition(centerX - gapFromCenter, yPos);
                }
                window.draw(nameText);
            }


            if (!entries[i].role.empty()) {
                Text roleText;
                roleText.setFont(font);
                roleText.setString(entries[i].role);
                roleText.setCharacterSize((unsigned int)fontSize);
                roleText.setFillColor(Color(180, 180, 180));
                FloatRect rolebounds = roleText.getLocalBounds();
                roleText.setOrigin(rolebounds.left, rolebounds.top);
                roleText.setPosition(centerX + gapFromCenter, yPos);
                window.draw(roleText);
            }
        }
    }


    Text guide;
    guide.setFont(font);
    guide.setString("ESC to Return");
    guide.setCharacterSize((unsigned int)(winHeight * 0.03f));
    guide.setFillColor(sf::Color(100, 100, 100));
    guide.setPosition(winWidth * 0.02f, winHeight * 0.95f);
    window.draw(guide);
}