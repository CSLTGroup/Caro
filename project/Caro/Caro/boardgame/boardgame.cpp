#include "../global.h"
#include "../bot_ai/bot_ai.h"

void BoardGame::setUp() {
    board.assign(size, vector<int>(size, 0));
    ensurePlayerAssets();
}
void BoardGame::reset() {
    board.assign(size, vector<int>(size, 0));
    curX = curY = 0;
    curPlayer = 1;
    resultGame = 0;
}
void BoardGame::drawTable(RenderWindow& window) {

    // set up size
    spacingTop = window.getSize().y * 20.f / 100;
    spacingLeft = window.getSize().y * 10.f / 100;
    spacingBoardBetween = window.getSize().y * 5.f / 100; // 1 nua khoang cach giua board va ty so
    widthBoard = window.getSize().x / 2.f - spacingLeft - spacingBoardBetween;
    heightBoard = window.getSize().y - 2.f * spacingTop;
    spacingCellX = widthBoard * 2.f / 100;
    spacingCellY = heightBoard * 2.f / 100;
    cellLenX = (widthBoard - spacingCellX * (size + 1)) / size;
    cellLenY = (heightBoard - spacingCellY * (size + 1)) / size;

    // bg
    RectangleShape boardBG = RectangleShape(Vector2f(widthBoard, heightBoard));
    boardBG.setPosition(Vector2f(spacingLeft, spacingTop));
    boardBG.setFillColor(Color(206, 201, 194));
    window.draw(boardBG);

    // cell
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            drawPosition(i, j, window);
        }
    }

    if (showPlayerPanel) {
        drawPlayerInfoPanel(window);
    }
    
    // ve man hinh thang
    if (resultGame == 1 || resultGame == 2) {
        drawWinnerMessage(window);
        PlaySoundWin();
    }
    else if (resultGame == 3) {
        drawWinnerMessage(window);
        PlaySoundDraw();
    }
}
void BoardGame::drawPosition(int x, int y, RenderWindow& window) {

    // shape
    RectangleShape cell = RectangleShape(Vector2f(cellLenX, cellLenY));
    cell.setPosition(
        spacingLeft + spacingCellX + x * (cellLenX + spacingCellX),
        spacingTop + spacingCellY + y * (cellLenY + spacingCellY)
    );
    if (x == curX && y == curY)
        cell.setFillColor(Color::Cyan);
    window.draw(cell);

    // text 
    int heightText = min(cellLenX, cellLenY) * 80.f / 100;
    Text text;
    text.setFont(font);
    text.setCharacterSize(heightText);
    FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(
        textBounds.width / 2.f,
        textBounds.height / 2.f
    );
    text.setPosition(
        spacingLeft + spacingCellX + x * (cellLenX + spacingCellX) + cellLenX / 4.f,
        spacingTop + spacingCellY + y * (cellLenY + spacingCellY)
    );
    if (board[x][y] == 1) {
        text.setFillColor(Color::Blue);
        text.setString("O");
    }
    else if (board[x][y] == 2) {
        text.setFillColor(Color::Red);
        text.setString("X");
    }
    window.draw(text);
}
void BoardGame::setChoice(RenderWindow& window) {
    if (board[curX][curY] >= 1 || resultGame) 
        return;
    board[curX][curY] = curPlayer;
    curPlayer = 3 - curPlayer; //swap player
    // ktra
    result();

    if (isPVCMode() && curPlayer == aiPlayer && resultGame == 0) {
        makeBotMove();
    }
}
void BoardGame::setMove(RenderWindow& window) {
    if (resultGame) {
        if (keyBoard.Enter()) {
            reset();
            if (mode == GameMode::PVC)
                curPlayer = 1;
            return;
        }
        else if (keyBoard.Esc()) {
            reset();
            stateMenu = 0;
        }
        return;
    }

    if (isPVCMode() && curPlayer == aiPlayer) {
        makeBotMove();
        return;
    }
    
    if (keyBoard.Up() && curY > 0) {
        --curY;
        PlaySoundClick(); // Play click sound when moving up
    }
    if (keyBoard.Down() && curY < size - 1) {
        ++curY;
        PlaySoundClick(); // Play click sound when moving down
    }
    if (keyBoard.Left() && curX > 0) {
        --curX;
        PlaySoundClick(); // Play click sound when moving left
    }
    if (keyBoard.Right() && curX < size - 1) {
        ++curX;
        PlaySoundClick(); // Play click sound when moving right
    }
    if (keyBoard.Enter())
        setChoice(window);
    else if (keyBoard.Esc()) {
        reset();
        stateMenu = 0;
    }
}
bool BoardGame::checkTheSame(vector<int> listCheck) {
    // Null
    if (listCheck[0] == 0) // chua ai chon
        return false;

    // Check the same
    for (int i = 0; i < listCheck.size() - 1; i++)
        if (listCheck[i] != listCheck[i + 1])
            return false;

    return true;
}
int BoardGame::checkResult() {
    // column
    for (int i = 0; i < size - 5 + 1; i++)
        for (int j = 0; j < size; j++)
            if (checkTheSame({ board[i][j], board[i + 1][j], board[i + 2][j], board[i + 3][j], board[i + 4][j] })) {
                return resultGame = board[i][j];
            }

    // row
    for (int j = 0; j < size - 5 + 1; j++)
        for (int i = 0; i < size; i++)
            if (checkTheSame({ board[i][j], board[i][j + 1], board[i][j + 2], board[i][j + 3], board[i][j + 4] })) {
                return resultGame = board[i][j];
            }

    // main cross
    for (int j = 0; j < size - 5 + 1; j++)
        for (int i = 0; i < size - 5 + 1; i++)
            if (checkTheSame({ board[i][j], board[i + 1][j + 1], board[i + 2][j + 2], board[i + 3][j + 3], board[i + 4][j + 4] })) {
                return resultGame = board[i][j];
            }

    // side cross
    for (int j = 0; j < size - 5 + 1; j++)
        for (int i = size - 1; i >= 4; i--)
            if (checkTheSame({ board[i][j], board[i - 1][j + 1], board[i - 2][j + 2], board[i - 3][j + 3], board[i - 4][j + 4] })) {
                return resultGame = board[i][j];
            }

    // not draw
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == 0)
                return resultGame = 0;

    // draw
    return resultGame = 3;
}

void BoardGame::drawWinnerMessage(RenderWindow& window) {
  
    RectangleShape overlay(Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(Color(0, 0, 0, 180)); 
    window.draw(overlay);
    
    // tao chu
    Text winnerText;
    winnerText.setFont(font);
    winnerText.setCharacterSize(80);
    
  
    std::string message = "";
    Color textColor;
    
    if (resultGame == 1) {
        message = "Player 1 Wins!";
        textColor = Color::Blue;
    }
    else if (resultGame == 2) {
        message = "Player 2 Wins!";
        textColor = Color::Red;
    }
    else if (resultGame == 3) {
        message = "It's a Draw!";
        textColor = Color::Yellow;
    }
    else {
        // ko ve
        return;
    }
    
    winnerText.setString(message);
    winnerText.setFillColor(textColor);
    
    // can giua
    FloatRect textBounds = winnerText.getLocalBounds();
    winnerText.setOrigin(
        textBounds.width / 2.f,
        textBounds.height / 2.f
    );
    winnerText.setPosition(
        window.getSize().x / 2.f,
        window.getSize().y / 2.f - 50
    );
    
    // Add outline for better visibility
    winnerText.setOutlineColor(Color::White);
    winnerText.setOutlineThickness(3);
    
    window.draw(winnerText);
    
    // Add instruction text
    Text instructionText;
    instructionText.setFont(font);
    instructionText.setCharacterSize(30);
    instructionText.setString("Press ENTER to play again  |  ESC to return");
    instructionText.setFillColor(Color::White);
    
    FloatRect instructionBounds = instructionText.getLocalBounds();
    instructionText.setOrigin(
        instructionBounds.width / 2.f,
        instructionBounds.height / 2.f
    );
    instructionText.setPosition(
        window.getSize().x / 2.f,
        window.getSize().y / 2.f + 80
    );
    
    window.draw(instructionText);
}


int BoardGame::result() {
    if (resultGame) return resultGame;
    int prev = resultGame;
    int res = checkResult();
    if (showPlayerPanel && prev == 0) {
        if (res == 1) ++player1Score;
        else if (res == 2) ++player2Score;
    }
    return res;
}

void BoardGame::setMode(GameMode newMode) {
    mode = newMode;
    reset();
    player1Score = 0;
    player2Score = 0;

    if (mode == GameMode::PVC) {
        curPlayer = 1; // human starts
        player1Name = "Player";
        player2Name = "Computer";
    }
    else if (mode == GameMode::PVP) {
        player1Name = "Player 1";
        player2Name = "Player 2";
    }

    showPlayerPanel = (mode == GameMode::PVP || mode == GameMode::PVC);
    if (showPlayerPanel) {
        ensurePlayerAssets();
    }
}

void BoardGame::makeBotMove() {
    if (!isPVCMode() || curPlayer != aiPlayer || resultGame)
        return;

    auto move = CalculateBotMove(board, aiPlayer);
    if (move.first < 0 || move.second < 0)
        return;

    if (board[move.first][move.second] != 0)
        return;

    curX = move.first;
    curY = move.second;
    board[move.first][move.second] = aiPlayer;
    curPlayer = 3 - curPlayer;
    result();
}

void BoardGame::ensurePlayerAssets() {
    if (!player1PhotoLoaded) {
        if (player1Photo.loadFromFile("assets/image/penguinGOGO.png")) {
            player1PhotoLoaded = true;
        }
    }
    if (!player2PhotoLoaded) {
        if (player2Photo.loadFromFile("assets/image/penguinGOGO.png")) {
            player2PhotoLoaded = true;
        }
    }
}

void BoardGame::drawPlayerInfoPanel(RenderWindow& window) {
    float panelX = spacingLeft + widthBoard + spacingBoardBetween;
    float panelWidth = window.getSize().x - panelX - spacingBoardBetween;
    float panelY = spacingTop;
    float panelHeight = heightBoard;
    if (panelWidth <= 0) return;

    RectangleShape panel(Vector2f(panelWidth, panelHeight));
    panel.setPosition(panelX, panelY);
    panel.setFillColor(Color(255, 241, 118)); // menu yellow
    panel.setOutlineColor(Color(50, 150, 50)); // menu green accent
    panel.setOutlineThickness(4);
    window.draw(panel);

    Text title;
    title.setFont(font);
    title.setString("CARO");
    title.setCharacterSize(64);
    title.setFillColor(Color::Black);
    FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin(titleBounds.left + titleBounds.width / 2.f, titleBounds.top + titleBounds.height / 2.f);
    title.setPosition(panelX + panelWidth / 2.f, panelY + 60);
    window.draw(title);

    auto drawPlayerSection = [&](int index, float topY, const string& name, int score, bool isActive, const Texture& photo, bool photoLoaded, Color accentColor) {
        float sectionHeight = (panelHeight - 160) / 2.f;
        RectangleShape section(Vector2f(panelWidth - 40, sectionHeight));
        section.setPosition(panelX + 20, topY);
        section.setFillColor(isActive ? Color(181, 255, 199) : Color(255, 252, 232));
        section.setOutlineColor(accentColor);
        section.setOutlineThickness(3);
        window.draw(section);

        float photoSize = min(sectionHeight - 40, 120.f);
        float photoX = section.getPosition().x + 20;
        float photoY = topY + sectionHeight / 2.f - photoSize / 2.f;
        if (photoLoaded) {
            Sprite sprite;
            sprite.setTexture(photo);
            float scale = min(photoSize / photo.getSize().x, photoSize / photo.getSize().y);
            sprite.setScale(scale, scale);
            FloatRect spriteBounds = sprite.getLocalBounds();
            sprite.setOrigin(spriteBounds.width / 2.f, spriteBounds.height / 2.f);
            sprite.setPosition(photoX + photoSize / 2.f, photoY + photoSize / 2.f);
            window.draw(sprite);
        }
        else {
            CircleShape placeholder(photoSize / 2.f);
            placeholder.setFillColor(Color(200, 200, 200));
            placeholder.setPosition(photoX, photoY);
            window.draw(placeholder);
        }

        Text nameText;
        nameText.setFont(font);
        nameText.setString(name);
        nameText.setCharacterSize(32);
        nameText.setFillColor(Color::Black);
        nameText.setStyle(Text::Bold);
        nameText.setPosition(photoX + photoSize + 20, topY + 25);
        window.draw(nameText);

        Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Score: " + to_string(score));
        scoreText.setCharacterSize(28);
        scoreText.setFillColor(Color(80, 80, 80));
        scoreText.setPosition(photoX + photoSize + 20, topY + 70);
        window.draw(scoreText);
    };

    float firstSectionTop = panelY + 120;
    float secondSectionTop = firstSectionTop + (panelHeight - 160) / 2.f + 40;

    drawPlayerSection(1, firstSectionTop, player1Name, player1Score, curPlayer == 1, player1Photo, player1PhotoLoaded, Color(50, 150, 50));
    drawPlayerSection(2, secondSectionTop, player2Name, player2Score, curPlayer == 2, player2Photo, player2PhotoLoaded, Color(200, 70, 70));
}