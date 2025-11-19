#pragma once
#include <utility>
#include <vector>

// Returns x,y coordinate for AI move (or {-1,-1} if no moves left)
std::pair<int, int> CalculateBotMove(const std::vector<std::vector<int>>& board, int aiPlayer);
