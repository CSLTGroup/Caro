#pragma once
#include "../global.h"
//#ifdef _CREDIT_H
//#define CREDIT_H


struct CreditEntry {
    std::string name;
    std::string role;
};

struct Credit {
public:
    Credit(); // Constructor

    void CreditLogic(RenderWindow& window);
    void draw(RenderWindow& window);
    void reset(float windowHeight);

private:
    std::vector<CreditEntry> entries;
    float scrollY;
    Clock deltaClock;

    void initCredits();
};

//#endif
