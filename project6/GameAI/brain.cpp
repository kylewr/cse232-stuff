#include "brain.h"

#include <algorithm>
#include <fstream>
#include <map>
#include <set>
#include <thread>
#include <vector>

#define LOG_TO_FILE

Brain::Brain() {}

int Brain::getNextMove(GameState& gamestate) {
    // if (gamestate.stage > 5) {
    //     return STAY;
    // }

    int nextMove = STAY;
    gamestate_ = &gamestate;
    playerVisionCached_ = false;
    cachedPlayerVision_ = {-1, -1};

    playerPos.first = gamestate.pos[1];
    playerPos.second = gamestate.pos[0];

    clearFile();
    switch (gamestate.stage) {
        case 0:
        case 2:
        case 5:
            nextMove = runStageMaze();
            break;
        case 1:
            nextMove = runStageFood();
            break;
        case 3:
            nextMove = runStageFlags();
            break;
        case 4:
            nextMove = runStageEnemies();
            break;
    }

    lastPlayerPos.first = gamestate.pos[1];
    lastPlayerPos.second = gamestate.pos[0];

    return move(nextMove);
}

int Brain::move(int direction) {
    if (playerPos.first == -1 || playerPos.second == -1) {
        return direction;
    }

    switch (direction) {
        case UP:
            playerPos.first -= 1;
            break;
        case DOWN:
            playerPos.first += 1;
            break;
        case LEFT:
            playerPos.second -= 1;
            break;
        case RIGHT:
            playerPos.second += 1;
            break;
    }

    return direction;
}

POS Brain::simulate(int direction, POS startPos) {
    POS simPos = startPos;

    switch (direction) {
        case UP:
            if (simPos.first - 1 > 1) {
                simPos.first -= 1;
            }
            break;
        case DOWN:
            simPos.first += 1;
            break;
        case LEFT:
            if (simPos.second - 1 > 1) {
                simPos.second -= 1;
            }
            break;
            simPos.second -= 1;
            break;
        case RIGHT:
            simPos.second += 1;
            break;
    }

    return simPos;
}

std::string Brain::translate(int direction) {
    switch (direction) {
        case STAY:
            return "STAY";
        case UP:
            return "UP";
        case LEFT:
            return "LEFT";
        case DOWN:
            return "DOWN";
        case RIGHT:
            return "RIGHT";
        default:
            return "UNKNOWN";
    }
}

void Brain::clearFile() {
#ifdef LOG_TO_FILE
    std::ofstream ofs("out.txt", std::ios::trunc);
#endif
}

void Brain::print(const char* msg) {
#ifdef LOG_TO_FILE
    std::ofstream ofs("out.txt", std::ios::app);
    if (!ofs)
        return;
    ofs << msg << '\n';
#endif
}

void Brain::printVision() {
#ifdef LOG_TO_FILE
    if (gamestate_ == nullptr) {
        return;
    }

    VISION& v = gamestate_->vision;

    std::ofstream ofs("out.txt", std::ios::app);
    if (!ofs)
        return;

    for (size_t r = 0; r < v.size(); r++) {
        for (size_t c = 0; c < v.at(r).size(); c++) {
            ofs << v.at(r).at(c);
        }
        ofs << '\n';
    }
#endif
}

POS Brain::getPlayerInVision() {
    if (playerVisionCached_) {
        return cachedPlayerVision_;
    }

    static const std::vector<char> targets = {'>', '<', '^', 'v'};
    cachedPlayerVision_ = getSingleTargetInVision(targets);
    playerVisionCached_ = true;
    return cachedPlayerVision_;
}

POS Brain::getSingleTargetInVision(char targ) {
    return getSingleTargetInVision(std::vector<char> {targ});
}

POS Brain::getSingleTargetInVision(const std::vector<char>& targ) {
    POS outs {-1, -1};

    if (gamestate_ == nullptr) {
        return outs;
    }

    VISION& v = gamestate_->vision;

    for (size_t r = 0; r < v.size(); r++) {
        for (size_t c = 0; c < v.at(r).size(); c++) {
            char current = v.at(r).at(c);
            for (char t : targ) {
                if (current == t) {
                    outs.first = static_cast<int>(r);
                    outs.second = static_cast<int>(c);
                    return outs;
                }
            }
        }
    }

    return outs;
}

std::vector<POS> Brain::getRelativeAllInVision(char targ) {
    std::vector<POS> outs;

    VISION& v = gamestate_->vision;

    for (size_t r = 0; r < v.size(); r++) {
        for (size_t c = 0; c < v.at(r).size(); c++) {
            char current = v.at(r).at(c);
            if (current == targ) {
                POS pos;
                pos.first = static_cast<int>(c);
                pos.second = static_cast<int>(r);
                outs.push_back(pos);
            }
        }
    }
    return outs;
}

POS Brain::relToAbs(const POS& relPos) {
    POS absP;

    POS playerInVision = getPlayerInVision();

    if (relPos.first == -1 || relPos.second == -1) {
        absP.first = -1;
        absP.second = -1;
        return absP;
    }

    absP.first = relPos.first - playerInVision.second + playerPos.first;
    absP.second = relPos.second - playerInVision.first + playerPos.second;

    return absP;
}

std::vector<POS> Brain::relToAbs(const std::vector<POS>& relPos) {
    std::vector<POS> absPos;

    POS playerInVision = getPlayerInVision();

    for (const POS& p : relPos) {
        if (p.first == -1 || p.second == -1) {
            POS invalid {-1, -1};
            absPos.push_back(invalid);
            continue;
        }

        POS absP;
        absP.first = p.first - playerInVision.second + playerPos.first;
        absP.second = p.second - playerInVision.first + playerPos.second;
        absPos.push_back(absP);
    }

    return absPos;
}

POS Brain::getClosestTarget(const std::vector<POS>& targets) {
    POS closest {-1, -1};
    int minDist = 300000;

    for (const POS& p : targets) {
        int dist = std::abs(p.first - playerPos.first) + std::abs(p.second - playerPos.second);
        if (dist < minDist) {
            minDist = dist;
            closest = p;
        }
    }

    return closest;
}

POS Brain::getClosestTarget(const std::set<POS>& targets) {
    POS closest {-1, -1};
    int minDist = 300000;

    for (const POS& p : targets) {
        int dist = std::abs(p.first - playerPos.first) + std::abs(p.second - playerPos.second);
        if (dist < minDist) {
            minDist = dist;
            closest = p;
        }
    }

    return closest;
}

int Brain::moveTowards(POS target, int limit) {
    int dc = target.first - playerPos.first;
    int dr = target.second - playerPos.second;

    int move = STAY;

    if (std::abs(dr) >= std::abs(dc)) {
        // Move vertically
        if (dr < 0) {
            if (limit != UP) {
                move = UP;
            }
        } else if (dr > 0) {
            if (limit != DOWN) {
                move = DOWN;
            }
        }
    }

    if (move == STAY) {
        // Move horizontally
        if (dc < 0) {
            if (limit != LEFT) {
                move = LEFT;
            }
        } else if (dc > 0) {
            if (limit != RIGHT) {
                move = RIGHT;
            }
        }
    }

    return move;
}

std::vector<int> Brain::getCardinals(int r, char targ) {
    std::vector<int> outs;

    if (gamestate_ == nullptr) {
        return outs;
    }

    VISION& v = gamestate_->vision;

    int rows = static_cast<int>(v.size());
    if (rows == 0) {
        return outs;
    }
    int cols = static_cast<int>(v.at(0).size());

    POS p = getPlayerInVision();
    int pr = p.first;
    int pc = p.second;

    // UP
    if (pr - r >= 0) {
        if (v.at(pr - r).at(pc) == targ) {
            outs.push_back(UP);
        }
    }
    // DOWN
    if (pr + r < rows) {
        if (v.at(pr + r).at(pc) == targ) {
            outs.push_back(DOWN);
        }
    }
    // LEFT
    if (pc - r >= 0) {
        if (v.at(pr).at(pc - r) == targ) {
            outs.push_back(LEFT);
        }
    }

    // RIGHT
    if (pc + r < cols) {
        if (v.at(pr).at(pc + r) == targ) {
            outs.push_back(RIGHT);
        }
    }
    return outs;
}

std::vector<int> Brain::getCardinals(int r, const std::vector<char>& targ) {
    std::vector<int> outs;

    if (gamestate_ == nullptr) {
        return outs;
    }

    VISION& v = gamestate_->vision;

    int rows = static_cast<int>(v.size());
    if (rows == 0) {
        return outs;
    }
    int cols = static_cast<int>(v.at(0).size());

    POS p = getPlayerInVision();
    int pr = p.first;
    int pc = p.second;

    // UP
    if (pr - r >= 0) {
        char current = v.at(pr - r).at(pc);
        for (char t : targ) {
            if (current == t) {
                outs.push_back(UP);
                break;
            }
        }
    }
    // DOWN
    if (pr + r < rows) {
        char current = v.at(pr + r).at(pc);
        for (char t : targ) {
            if (current == t) {
                outs.push_back(DOWN);
                break;
            }
        }
    }
    // LEFT
    if (pc - r >= 0) {
        char current = v.at(pr).at(pc - r);
        for (char t : targ) {
            if (current == t) {
                outs.push_back(LEFT);
                break;
            }
        }
    }

    // RIGHT
    if (pc + r < cols) {
        char current = v.at(pr).at(pc + r);
        for (char t : targ) {
            if (current == t) {
                outs.push_back(RIGHT);
                break;
            }
        }
    }
    return outs;
}

int Brain::runStageMaze() {
    /*
    this is a wall hugger
    */

    print("----------------");
    print("STAGE: MAZE");

    static const std::vector<char> WALL_TARGS {'+', 'D', 'T'};

    static int lastMove = RIGHT;
    int move = lastMove;

    // print("Player Position:");
    // print(std::to_string(playerPos.first).c_str());
    // print(std::to_string(playerPos.second).c_str());

    std::vector<int> walls = getCardinals(1, WALL_TARGS); // r=1
    for (int w : walls) {
        print(("Wall at: " + translate(w)).c_str());
    }
    static std::vector<int> previousWalls {};

    if (walls.empty()) {
        if (previousWalls.empty()) {
            move = DOWN;
        } else if (previousWalls.size() == 1) {
            move = previousWalls.front();
        } else {
            move = lastMove;
        }

    } else if (walls.size() == 1) {
        print("last player pos:");

        print(("{" + std::to_string(lastPlayerPos.first) + "," +
               std::to_string(lastPlayerPos.second) + "}")
                  .c_str());

        print("current player pos:");

        print(("{" + std::to_string(playerPos.first) + "," + std::to_string(playerPos.second) + "}")
                  .c_str());

        if (playerPos == lastPlayerPos) {
            move = RIGHT;
        } else {
            move = lastMove;
        }
    } else if (walls.size() == 2) {
        // turn to the direction without a wall and hat we werent coming from
        int chosen = STAY;
        if (lastMove == UP || lastMove == DOWN) {
            if (std::find(walls.begin(), walls.end(), LEFT) == walls.end())
                chosen = LEFT;
            else if (std::find(walls.begin(), walls.end(), RIGHT) == walls.end())
                chosen = RIGHT;
        } else if (lastMove == LEFT || lastMove == RIGHT) {
            if (std::find(walls.begin(), walls.end(), UP) == walls.end())
                chosen = UP;
            else if (std::find(walls.begin(), walls.end(), DOWN) == walls.end())
                chosen = DOWN;
        }
        if (chosen != STAY) {
            move = chosen;
        }
    }
    previousWalls = walls;

    print("Chosen Move:");
    print(translate(move).c_str());

    // printVision();
    lastMove = move;
    return move;
}

int Brain::runStageFood() {
    print("----------------");
    print("STAGE: FOOD");

    int move = 0;

    static POS doorPos {-1, -1};
    static std::set<POS> foodMemory {};

    /*
    remember any food we start to see
    */
    for (POS food : relToAbs(getRelativeAllInVision('0'))) {
        if (std::find(foodMemory.begin(), foodMemory.end(), food) == foodMemory.end()) {
            foodMemory.insert(food);
        }
    }
    /*
    remember the door if we see it
    */
    if (doorPos.first == -1 && doorPos.second == -1) {
        POS door = relToAbs(getSingleTargetInVision('D'));
        if (door.first != -1 && door.second != -1) {
            doorPos = door;
        }
    }

    if (!foodMemory.empty()) {
        POS closest = getClosestTarget(foodMemory);
        move = moveTowards(closest);
        print(("Moving towards food at: {" + std::to_string(closest.first) + "," +
               std::to_string(closest.second) + "}")
                  .c_str());
    } else {
        if (doorPos.first != -1 && doorPos.second != -1) {
            move = moveTowards(doorPos);
        } else {
            move = RIGHT;
        }
    }

    /*
    here is if we are stuck doing something!
    */
    if (lastPlayerPos == playerPos) {
        POS targetPos;

        if (foodMemory.empty()) {
            targetPos = doorPos;
        } else {
            targetPos = getClosestTarget(foodMemory);
        }

        auto walls = getCardinals(1, '+');
        for (int w : walls) {
            print(("Wall at: " + translate(w)).c_str());
        }

        move = moveTowards(targetPos, walls.empty() ? STAY : walls.front());
    }

    // Remove food from memory if we see empty space where we thought food was
    if (!foodMemory.empty()) {
        for (POS emptySpace : relToAbs(getRelativeAllInVision(' '))) {
            foodMemory.erase(emptySpace);
        }
        foodMemory.erase(playerPos); // Remove food at current position
    }

    print("Chosen Move:");
    print(translate(move).c_str());
    // printVision();

    return move;
}

int Brain::runStageFlags() {
    print("----------------");
    print("STAGE: FLAGS");

    int move = 0;

    static POS flagAPos {-1, -1};
    static POS flagBPos {-1, -1};
    static POS doorPos {-1, -1};
    static bool collectedA {false};
    static bool collectedB {false};

    /*
    be on the lookout for flags and doors
    */
    if (flagAPos.first == -1 && flagAPos.second == -1) {
        POS flagA = getSingleTargetInVision('A');
        /*
        RANT TIME!!!
        okay so i have literally NO CLUE why but the x and y coordinates
        are somehow flipped here and if i change the helper method to return
        normal values everything else breaks!!!!
        oopsies i love programming but its okay
        */
        int temp = flagA.first;
        flagA.first = flagA.second;
        flagA.second = temp;
        if (flagA.first != -1 && flagA.second != -1) {
            flagAPos = relToAbs(flagA);
        }
    }
    if (flagBPos.first == -1 && flagBPos.second == -1) {
        POS flagB = getSingleTargetInVision('B');
        int temp = flagB.first;
        flagB.first = flagB.second;
        flagB.second = temp;
        if (flagB.first != -1 && flagB.second != -1) {
            flagBPos = relToAbs(flagB);
        }
    }
    if (doorPos.first == -1 && doorPos.second == -1) {
        POS door = getSingleTargetInVision('D');
        int temp = door.first;
        door.first = door.second;
        door.second = temp;
        if (door.first != -1 && door.second != -1) {
            doorPos = relToAbs(door);
        }
    }

    // print("last player pos:");
    // print(("{" + std::to_string(lastPlayerPos.first) + "," + std::to_string(lastPlayerPos.second)
    // +
    //        "}")
    //           .c_str());
    // print("current player pos:");
    // print(("{" + std::to_string(playerPos.first) + "," + std::to_string(playerPos.second) + "}")
    //           .c_str());
    // print("flag a:");
    // print(("{" + std::to_string(flagAPos.first) + "," + std::to_string(flagAPos.second) + "}")
    //           .c_str());
    // print("flag b:");
    // print(("{" + std::to_string(flagBPos.first) + "," + std::to_string(flagBPos.second) + "}")
    //           .c_str());
    // print("door:");
    // print(
    //     ("{" + std::to_string(doorPos.first) + "," + std::to_string(doorPos.second) +
    //     "}").c_str());
    // print("COLLECTED A? : ");
    // print((collectedA ? "YES" : "NO"));
    // print("COLLECTED B? : ");
    // print((collectedB ? "YES" : "NO"));

    /*
    do something silly now
    run the maze algorithm until we have the A flag
    */
    if (!collectedA) {
        move = runStageMaze();
    } else if (!collectedB) {
        move = runStageMaze();

        auto Bwalls = getCardinals(1, 'B');
        for (int w : Bwalls) {
            print(("Wall at: " + translate(w)).c_str());
        }
        if (!Bwalls.empty()) {
            move = Bwalls.front();
        }
    } else {
        move = moveTowards(doorPos);
    }

    if (!collectedA && flagAPos.first != -1 && flagAPos.second != -1) {
        if (playerPos.first == flagAPos.first && playerPos.second == flagAPos.second) {
            collectedA = true;
        }
    }
    if (collectedA && !collectedB && flagBPos.first != -1 && flagBPos.second != -1) {
        if (playerPos.first == flagBPos.first && playerPos.second == flagBPos.second) {
            collectedB = true;
        }
    }

    print("Chosen Move:");
    print(translate(move).c_str());

    // printVision();
    return move;
}

int Brain::runStageEnemies() {
    print("----------------");
    print("STAGE: ENEMIES");

    print("last player pos:");
    print(("{" + std::to_string(lastPlayerPos.first) + "," + std::to_string(lastPlayerPos.second) +
           "}")
              .c_str());
    print("current player pos:");
    print(("{" + std::to_string(playerPos.first) + "," + std::to_string(playerPos.second) + "}")
              .c_str());

    printVision();

    int move = STAY;

    static std::set<POS> lastEnemiesPos {};
    std::set<POS> enemyMemory {};

    // int1: COLUMN int2: DIRECTION
    std::map<int, int> enemyDirections {};
    // int1: COLUMN int2: DISTANCE
    std::map<int, int> enemyDistances {};

    /*
    remember any enemies we start to see
    */
    for (POS e : getRelativeAllInVision('X')) {
        POS enemy;
        enemy = relToAbs(e);
        if (std::find(enemyMemory.begin(), enemyMemory.end(), enemy) == enemyMemory.end()) {
            int dist =
                std::abs(enemy.first - playerPos.first) + std::abs(enemy.second - playerPos.second);
            enemyMemory.insert(enemy);
            enemyDirections[enemy.first] = 0;
            enemyDistances[enemy.first] = 0;
        }
    }

    if (!lastEnemiesPos.empty() && !enemyMemory.empty()) {
        auto itLast = lastEnemiesPos.begin();
        auto itNow = enemyMemory.begin();
        for (size_t i = 0; i < lastEnemiesPos.size(); i++, ++itLast, ++itNow) {
            int diff = itLast->first - itNow->first;
            if (diff < 0) {
                enemyDirections[itLast->first] = DOWN;
            } else {
                enemyDirections[itLast->first] = UP;
            }
        }
    }

    if (!enemyMemory.empty()) {
        move = RIGHT;

        auto distIt = enemyDistances.begin();
        auto enemIt = enemyMemory.begin();
        auto dirIt = enemyDirections.begin();

        for (int i = 0; i < enemyMemory.size(); i++, ++distIt, ++enemIt, ++dirIt) {
            int dist = std::abs(enemIt->first - playerPos.first) +

                       std::abs(enemIt->second - playerPos.second);
            distIt->second = dist;
            print(("Enemy at: {" + std::to_string(enemIt->first) + "," +
                   std::to_string(enemIt->second) + "} Direction: " + translate(dirIt->second) +
                   " Distance: " + std::to_string(dist))
                      .c_str());
        }
    }

    // this isnt needed since we arent static enemymemory
    // if (!enemyMemory.empty()) {
    //     for (POS emptySpace : relToAbs(getRelativeAllInVision(' '))) {
    //         enemyMemory.erase(emptySpace);
    //     }
    //     enemyMemory.erase(playerPos); // Remove enemy at current position
    // }
    lastEnemiesPos = enemyMemory;

#ifdef LOG_TO_FILE
    std::this_thread::sleep_for(std::chrono::milliseconds(1280));
#endif
    return move;
}
