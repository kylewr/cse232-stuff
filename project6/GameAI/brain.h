#ifndef BRAIN_H
#define BRAIN_H

#include "../Game/game.h"
#include <cstdlib>
#include <ctime>
#include <set>
#include <string>
#include <utility>
#include <vector>

#define STAY (int)0
#define UP (int)1
#define LEFT (int)2
#define DOWN (int)3
#define RIGHT (int)4

typedef std::vector<std::vector<char>> VISION;
typedef std::pair<int, int> POS;

class Brain {
    public:
        Brain();                               // Constructor
        int getNextMove(GameState& gamestate); // Returns the next move for the AI
        int move(int direction);
        POS simulate(int direction, POS startPos);

        std::string translate(int direction);

        void clearFile();
        void print(const char* msg);
        void printVision();

        POS getPlayerInVision();
        POS getSingleTargetInVision(char targ);
        POS getSingleTargetInVision(const std::vector<char>& targ);
        std::vector<POS> getRelativeAllInVision(char targ);

        POS relToAbs(const POS& relPos);
        std::vector<POS> relToAbs(const std::vector<POS>& relPos);

        POS getClosestTarget(const std::vector<POS>& targets);
        POS getClosestTarget(const std::set<POS>& targets);
        int moveTowards(POS target, int limit = -1);

        std::vector<int> getCardinals(int r, char targ);

        int runStageMaze();
        int runStageFood();
        int runStageFlags();

    private:
        GameState* gamestate_;

        POS playerPos {-1, -1};
        POS lastPlayerPos {-1, -1};
        POS cachedPlayerVision_ {-1, -1};
        bool playerVisionCached_ {false};
};

#endif // BRAIN_H
