#include "../global.h"

void HowToPlay::HowToPlayLogic(RenderWindow& window) {

    if (keyBoard.Esc()) {
        PlaySoundClick();
        stateMenu = 0; 
    }
}

void HowToPlay::draw(RenderWindow& window) {
    float winWidth = (float)window.getSize().x;
    float winHeight = (float)window.getSize().y;

    float boxWidth = winWidth * 0.9f;    
    float boxHeight = winHeight * 0.9f;  
    float boxX = (winWidth - boxWidth) / 2.0f;
    float boxY = (winHeight - boxHeight) / 2.0f;

    RectangleShape panel(Vector2f(boxWidth, boxHeight));
    panel.setFillColor(Color(30, 30, 30, 240));
    panel.setOutlineColor(Color::White);
    panel.setOutlineThickness(winHeight * 0.006f);
    panel.setPosition(boxX, boxY);
    window.draw(panel);

    Text mainTitle;
    mainTitle.setFont(font);
    mainTitle.setString("HOW TO PLAY");
    mainTitle.setCharacterSize((int)(winHeight * 0.1f)); 
    mainTitle.setStyle(Text::Bold);

    FloatRect titleBounds = mainTitle.getLocalBounds();
    float titleX = boxX + (boxWidth - titleBounds.width) / 2.0f;
    float titleY = boxY + winHeight * 0.02f; 

    mainTitle.setFillColor(Color(0, 0, 0, 150));
    mainTitle.setPosition(titleX + 5.0f, titleY + 5.0f);
    window.draw(mainTitle);

 
    mainTitle.setFillColor(Color::Yellow);
    mainTitle.setPosition(titleX, titleY);
    window.draw(mainTitle);

 
    float startY = titleY + winHeight * 0.14f; 
    float textX = boxX + winWidth * 0.04f;   
    float currentY = startY;

    
    auto drawLine = [&](const string& text, float sizeRatio, Color color, bool isHeader = false, bool isItalic = false) {
        Text textObj;
        textObj.setFont(font);
        textObj.setString(text);
        textObj.setCharacterSize((int)(winHeight * sizeRatio));
        textObj.setFillColor(color);

        if (isHeader) textObj.setStyle(Text::Bold);
        if (isItalic) textObj.setStyle(Text::Italic);

 
        float spacing = isHeader ? (winHeight * 0.07f) : (winHeight * 0.055f);


        textObj.setPosition(textX, currentY);
        window.draw(textObj);

  
        currentY += spacing;
        };


    float headerSize = 0.055f; // Tiêu đề mục
    float textSize = 0.042f; // Nội dung bình thường
    float noteSize = 0.038f; // Chú thích nhỏ


    drawLine("OBJECTIVE:", headerSize, Color(255, 180, 0), true);
    drawLine("Create a line of 5 pieces to win.", textSize, Color::White);
    currentY += winHeight * 0.01f; 

   
    drawLine("CONTROLS:", headerSize, Color(255, 180, 0), true);
    drawLine("> MOVE:   [W][A][S][D] ", textSize, Color::White);
    drawLine("> SELECT: [ENTER] ", textSize, Color::White);
    drawLine("> BACK:   [ESC]", textSize, Color::White);
    currentY += winHeight * 0.01f;

 
    drawLine("GAME RULES:", headerSize, Color(255, 180, 0), true);
    drawLine("1. Black (X) goes first.", textSize, Color::White);
    drawLine("2. First to 5 in a row wins.", textSize, Color::White);
    drawLine("3. Blocked 2 ends = NO WIN.", textSize, Color(255, 80, 80)); 
    drawLine("4. Full board = Draw.", textSize, Color::White);


    Text hintText;
    hintText.setFont(font);
    hintText.setString("PRESS [ESC] TO RETURN");
    hintText.setCharacterSize((int)(winHeight * 0.04f));
    hintText.setFillColor(Color(100, 200, 255));
    hintText.setStyle(Text::Bold);

    FloatRect hintBounds = hintText.getLocalBounds();
    
    hintText.setPosition((winWidth - hintBounds.width) / 2.0f, boxY + boxHeight - winHeight * 0.06f);
    window.draw(hintText);
}