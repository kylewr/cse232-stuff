#include "brain.h"

#include <algorithm>
#include <fstream>
#include <set>
#include <vector>

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

    switch (gamestate.stage) {
        case 0:
            nextMove = runStageMaze();
            break;
        case 1:
            nextMove = runStageFood();
            break;
        case 2:
            nextMove = runStageMaze();
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
    std::ofstream ofs("out.txt", std::ios::trunc);
}

void Brain::print(const char* msg) {
    std::ofstream ofs("out.txt", std::ios::app);
    if (!ofs)
        return;
    ofs << msg << '\n';
}

void Brain::printVision() {
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

int Brain::runStageMaze() {
    /*
    this is a wall hugger
    */

    static int lastMove = RIGHT;
    int move = lastMove;

    clearFile();
    print("STAGE: MAZE");

    // print("Player Position:");
    // print(std::to_string(playerPos.first).c_str());
    // print(std::to_string(playerPos.second).c_str());

    std::vector<int> walls = getCardinals(1, '+'); // r=1
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

        print(("{" + std::to_string(playerPos.first) + "," + std::to_string(playerPos.second) +
        "}")
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
    clearFile();
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
        std::vector<POS> doors = relToAbs(getRelativeAllInVision('D'));
        if (!doors.empty()) {
            POS door = doors.front();
            if (door.first != -1 && door.second != -1) {
                if (door.first != -1 && door.second != -1) {
                    doorPos = door;
                }
            }
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
    for (POS emptySpace : relToAbs(getRelativeAllInVision(' '))) {
        foodMemory.erase(emptySpace);
    }
    foodMemory.erase(playerPos); // Remove food at current position

    print("Chosen Move:");
    print(translate(move).c_str());
    printVision();

    return move;
}

int Brain::runStageFlags() {
    clearFile();
    print("STAGE: FLAGS");

    static int lastMove = RIGHT;
    int move = 0;

    static POS flagAPos {-1, -1};
    static POS flagBPos {-1, -1};

    if (flagAPos.first == -1 && flagAPos.second == -1) {
        POS flagA = getSingleTargetInVision('A');
        if (flagA.first != -1 && flagA.second != -1) {
            flagAPos = relToAbs(std::vector<POS> {flagA}).front();
        }
    }

    return move;
}
