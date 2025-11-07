#include <iostream>
#include <vector>

#define ROWS 10
#define COLS 20

void printSeatRankings(const std::vector<std::vector<int>>& rankings) {
    for (const auto& row : rankings) {
        for (const auto& seat : row) {
            std::cout << " ";
            if (seat < 100 && seat >= 0) {
                std::cout << " ";
            }
            if (abs(seat) < 10) {
                std::cout << " ";
            }

            if (seat <= -100000) {
                std::cout << "  *,";
                continue;
            }
            std::cout << seat << ",";
        }
        std::cout << std::endl;
    }
}

void emplaceSuperFan(std::vector<std::vector<int>>& rankings, int col, int row) {
    if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
        int reversedRow = ROWS - 1 - row;
        rankings.at(reversedRow).at(col) = -100000;

        for (int distRow = -2; distRow <= 2; distRow++) {
            for (int distCol = -2; distCol <= 2; distCol++) {
                if (distRow == 0 && distCol == 0)
                    continue;

                int distTotal = abs(distRow) + abs(distCol);
                if (distTotal > 2)
                    continue;

                int subtractValue = 0;
                if (distTotal == 1) {
                    subtractValue = 40;
                } else if (distTotal == 2) {
                    subtractValue = 20;
                }

                try {
                    rankings.at(reversedRow + distRow).at(col + distCol) -= subtractValue;
                } catch (const std::out_of_range&) {
                }
            }
        }
    }
}

int main() {
    std::vector<std::vector<int>> seatRankings = {};

    int value = 55;
    for (int i = ROWS; i > 0; i--) {
        std::vector<int> row(COLS, value);
        seatRankings.push_back(row);
        value += 5;
    }

    std::vector<std::pair<int, int>> superFans = {};

    std::string in;

    while (std::getline(std::cin, in)) {
        if (in.empty()) {
            break;
        }

        int commaPos = in.find(',');
        int col = std::stoi(in.substr(1, commaPos));
        int row = std::stoi(in.substr(commaPos + 1));
        superFans.emplace_back(col, row);
    }

    for (const auto& [col, row] : superFans) {
        emplaceSuperFan(seatRankings, col, row);
    }

    printSeatRankings(seatRankings);

    return 0;
}
