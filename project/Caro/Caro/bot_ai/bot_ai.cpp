#include "bot_ai.h"
#include <vector>
#include <algorithm>
#include <limits>
#include <iostream>


using namespace std;

namespace {

    // Điểm số cho các thế cờ
    const long long SCORE_WIN = 100000000;      // 5 ô
    const long long SCORE_OPEN_4 = 10000000;    // 4 ô mở 2 đầu (Thắng chắc chắn)
    const long long SCORE_CLOSED_4 = 1000000;   // 4 ô bị chặn 1 đầu (Nguy hiểm)
    const long long SCORE_OPEN_3 = 1000000;     // 3 ô mở 2 đầu (Sắp thành Open 4)
    const long long SCORE_CLOSED_3 = 10000;     // 3 ô bị chặn 1 đầu
    const long long SCORE_OPEN_2 = 1000;        // 2 ô mở
    const long long SCORE_CLOSED_2 = 100;       // 2 ô bị chặn

    // 4 hướng: Ngang, Dọc, Chéo chính, Chéo phụ
    const int directions[4][2] = { {1, 0}, {0, 1}, { 1, 1 }, {1, -1} };

    // Hàm kiểm tra biên
    bool isValid(int x, int y, int size) {
        return x >= 0 && x < size && y >= 0 && y < size;
    }

    // Hàm tính điểm cho một hướng cụ thể tại vị trí (x, y) nếu đặt quân của 'player'
    long long getScoreForDirection(const vector<vector<int>>& board, int player, int x, int y, int dx, int dy) {
        int size = static_cast<int>(board.size());

        int count = 1;          // Số quân liên tiếp của mình (tính cả quân định đặt tại x,y)
        int block = 0;          // Số đầu bị chặn (bởi biên hoặc quân địch)

        // Duyệt hướng dương (tới)
        int i = 1;
        while (true) {
            int nx = x + dx * i;
            int ny = y + dy * i;
            if (!isValid(nx, ny, size)) {
                block++;
                break;
            }
            if (board[nx][ny] == player) {
                count++;
            }
            else if (board[nx][ny] == 0) {
                break; // Gặp ô trống thì dừng đếm chuỗi
            }
            else {
                block++; // Gặp quân địch
                break;
            }
            i++;
        }

        // Duyệt hướng âm (lùi)
        i = 1;
        while (true) {
            int nx = x - dx * i;
            int ny = y - dy * i;
            if (!isValid(nx, ny, size)) {
                block++;
                break;
            }
            if (board[nx][ny] == player) {
                count++;
            }
            else if (board[nx][ny] == 0) {
                break;
            }
            else {
                block++;
                break;
            }
            i++;
        }

        // --- TÍNH ĐIỂM DỰA TRÊN SỐ QUÂN VÀ SỐ ĐẦU BỊ CHẶN ---

        if (count >= 5) return SCORE_WIN;

        if (block >= 2) return 0; // Bị chặn 2 đầu thì chuỗi này vô dụng

        if (count == 4) {
            if (block == 0) return SCORE_OPEN_4; // .XXXX. -> Thắng chắc
            return SCORE_CLOSED_4;               // OXXXX. -> Phải chặn
        }

        if (count == 3) {
            if (block == 0) return SCORE_OPEN_3; // .XXX.
            return SCORE_CLOSED_3;               // OXXX..
        }

        if (count == 2) {
            if (block == 0) return SCORE_OPEN_2;
            return SCORE_CLOSED_2;
        }

        return 1; // Điểm khuyến khích
    }

    // Tính tổng điểm tấn công hoặc phòng thủ tại ô (x, y)
    long long evaluatePoint(const vector<vector<int>>& board, int player, int x, int y) {
        long long totalScore = 0;
        for (int i = 0; i < 4; i++) {
            totalScore += getScoreForDirection(board, player, x, y, directions[i][0], directions[i][1]);
        }
        return totalScore;
    }

} // namespace

// Hàm chính để Bot tính nước đi
pair<int, int> CalculateBotMove(const vector<vector<int>>& board, int aiPlayer) {
    if (board.empty()) return { -1, -1 };
    int size = static_cast<int>(board.size());

    // Nếu bàn cờ trống, đánh vào giữa (tốt nhất)
    bool isEmpty = true;
    for (const auto& row : board) {
        for (int cell : row) if (cell != 0) { isEmpty = false; break; }
    }
    if (isEmpty) return { size / 2, size / 2 };

    int humanPlayer = 3 - aiPlayer;
    long long maxScore = -1;
    pair<int, int> bestMove = { -1, -1 };

    // Duyệt qua tất cả các ô trống
    // TỐI ƯU: Chỉ nên duyệt các ô trống LÂN CẬN các ô đã có quân (để chạy nhanh hơn)
    // Nhưng với bàn cờ nhỏ (10-20), duyệt hết vẫn đủ nhanh.

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == 0) {
                // 1. Điểm Tấn Công: Nếu Bot đánh vào đây, Bot được lợi gì?
                long long attackScore = evaluatePoint(board, aiPlayer, i, j);

                // 2. Điểm Phòng Thủ: Nếu Người đánh vào đây, Người được lợi gì? (Bot cần chặn)
                long long defenseScore = evaluatePoint(board, humanPlayer, i, j);
				cerr << "(" << i << ", " << j << "): " << attackScore << " ";
				cerr << defenseScore << " ";
                cerr << endl;

                // Tổng hợp điểm
                // Bot ưu tiên tấn công hơn một chút nếu ngang nhau, 
                // nhưng nếu người chơi có nước nguy hiểm (Open 4), defenseScore sẽ rất lớn.
                long long currentScore = attackScore + defenseScore;

                // Điều chỉnh ưu tiên: Nếu có cơ hội thắng ngay (Win), ưu tiên tuyệt đối
                if (attackScore >= SCORE_WIN) currentScore = SCORE_WIN * 10;
                // Nếu đối phương sắp thắng (Win), phải chặn ngay
                else if (defenseScore >= SCORE_WIN) currentScore = SCORE_WIN * 5;

                if (currentScore > maxScore) {
                    maxScore = currentScore;
                    bestMove = { i, j };
                }
            }
        }
    }
	cerr << bestMove.first << ", " << bestMove.second << " with score " << maxScore << endl;
    return bestMove;
}