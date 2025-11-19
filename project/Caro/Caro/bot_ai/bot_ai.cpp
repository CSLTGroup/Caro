#include "bot_ai.h"
#include <algorithm>
#include <cmath>
#include <limits>

namespace {

constexpr int directions[4][2] = {
    {1, 0},
    {0, 1},
    {1, 1},
    {1, -1}
};

bool createsFive(const std::vector<std::vector<int>>& board, int player, int x, int y) {
    int size = static_cast<int>(board.size());

    for (auto& dir : directions) {
        int count = 1;
        int dx = dir[0];
        int dy = dir[1];

        for (int step = 1; step < 5; ++step) {
            int nx = x + dx * step;
            int ny = y + dy * step;
            if (nx < 0 || ny < 0 || nx >= size || ny >= size)
                break;
            if (board[nx][ny] == player)
                ++count;
            else
                break;
        }

        for (int step = 1; step < 5; ++step) {
            int nx = x - dx * step;
            int ny = y - dy * step;
            if (nx < 0 || ny < 0 || nx >= size || ny >= size)
                break;
            if (board[nx][ny] == player)
                ++count;
            else
                break;
        }

        if (count >= 5)
            return true;
    }

    return false;
}

int neighborScore(const std::vector<std::vector<int>>& board, int aiPlayer, int x, int y) {
    int score = 0;
    int size = static_cast<int>(board.size());
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0)
                continue;
            int nx = x + dx;
            int ny = y + dy;
            if (nx < 0 || ny < 0 || nx >= size || ny >= size)
                continue;
            if (board[nx][ny] == aiPlayer)
                score += 4;
            else if (board[nx][ny] == 0)
                score += 1;
            else
                score += 2;
        }
    }
    return score;
}

} // namespace

std::pair<int, int> CalculateBotMove(const std::vector<std::vector<int>>& board, int aiPlayer) {
    if (board.empty())
        return { -1, -1 };

    int size = static_cast<int>(board.size());
    int opponent = 3 - aiPlayer;

    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            if (board[x][y] != 0)
                continue;
            if (createsFive(board, aiPlayer, x, y))
                return { x, y };
        }
    }

    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            if (board[x][y] != 0)
                continue;
            if (createsFive(board, opponent, x, y))
                return { x, y };
        }
    }

    int bestScore = std::numeric_limits<int>::min();
    std::pair<int, int> bestMove = { -1, -1 };
    int center = size / 2;

    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            if (board[x][y] != 0)
                continue;

            int distanceScore = size - (std::abs(x - center) + std::abs(y - center));
            int surroundScore = neighborScore(board, aiPlayer, x, y);
            int totalScore = distanceScore + surroundScore;

            if (totalScore > bestScore) {
                bestScore = totalScore;
                bestMove = { x, y };
            }
        }
    }

    return bestMove;
}

