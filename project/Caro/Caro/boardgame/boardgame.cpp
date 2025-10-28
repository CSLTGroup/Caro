#include "../global.h"

void BoardGame::setUp() {
    board.assign(size, vector<int>(size, 0));
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
    if (board[curX][curY] >= 1 || resultGame) return;
    board[curX][curY] = curPlayer;
    drawPosition(curX, curY, window);
    curPlayer = 3 - curPlayer; //swap player
}
void BoardGame::setMove(RenderWindow& window) {
    if (keyBoard.Up() && curY > 0)
        --curY;
    if (keyBoard.Down() && curY < size - 1)
        ++curY;
    if (keyBoard.Left() && curX > 0)
        --curX;
    if (keyBoard.Right() && curX < size - 1)
        ++curX;
    if (keyBoard.Enter())
        setChoice(window);
    else if (keyBoard.Esc()) {
        reset();
        state = 0;
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
int BoardGame::result() {
    if (resultGame) return resultGame;
    return checkResult();
}