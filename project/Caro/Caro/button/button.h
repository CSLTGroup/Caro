#pragma once
#include "../global.h"

using namespace std;
using namespace sf;

struct Button {
    int x = 0, y = 0, width = 0, height = 0;
    int ID = 0;
    bool selected = false;
    string context = "";
    bool menuColorScheme = false;
    Button() {}
    void setPosition(int posX, int posY, int widthX, int heightY);
    void setMenuTheme(bool enable);
    void draw(RenderWindow& window);
};