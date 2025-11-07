#include <iostream>

double Centrifuge(int& timeTicks, int& ore, double* u238) {
    int numCycles {static_cast<int>(timeTicks / 12)};
    int availableCycles {static_cast<int>(ore / 10)};

    int cyclesToRun {std::min(numCycles, availableCycles)};

    double producedU235 = cyclesToRun * 0.007;
    for (int i = 0; i < cyclesToRun; ++i) {
        ore -= 10;
        *u238 += 0.993;
        timeTicks -= 12;
    }

    return producedU235;
}

int main() {
    int timeTicks = 900;
    int ore = 45;
    double u238 = 0.0;
    double producedU235 = Centrifuge(timeTicks, ore, &u238);

    std::cout << "Produced U-235: " << producedU235 << std::endl;
    std::cout << "Remaining Ore: " << ore << std::endl;
    std::cout << "Remaining Time Ticks: " << timeTicks << std::endl;
    std::cout << "Remaining U-238: " << u238 << std::endl;

    return 0;
}