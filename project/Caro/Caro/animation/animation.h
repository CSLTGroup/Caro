#pragma once
#include "../global.h"

struct Animation {
    Texture texture;

    int frameWidth;
    int frameHeight;
    int frameCount;
    int columns;
    int rows;

    Animation(const std::string&, int, int, int, int, int);
};

void updateAnimation(float, float, int, int&, float&);
void applyFrame(sf::Sprite&, const Animation&, int);