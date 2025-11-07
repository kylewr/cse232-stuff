#include "Matrix.hpp"

#include <iostream>

std::vector<std::vector<int>> create2DVector(int rows, int cols) {
    std::vector<std::vector<int>> output;

    for (int i = 0; i < rows; i++) {
        std::vector<int> newVec = std::vector<int>();
        for (int j = 0; j < cols; j++) {
            newVec.push_back(0);
        }
        output.push_back(newVec);
    }

    return output;
}

void print2DVector(const std::vector<std::vector<int>>& vec) {
    for (const auto& row : vec) {
        for (const auto& elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
}

void fillWithIndices(std::vector<std::vector<int>>& vec) {
    if (vec.empty() || vec.at(0).empty()) {
        return;
    }

    for (size_t i = 0; i < vec.size(); i++) {
        for (size_t j = 0; j < vec.at(i).size(); j++) {
            vec.at(i).at(j) = i + j;
        }
    }
}

void doubleElements(std::vector<std::vector<int>>* vec) {
    if (!vec) {
        return;
    }

    for (size_t i = 0; i < vec->size(); i++) {
        for (size_t j = 0; j < vec->at(i).size(); j++) {
            vec->at(i).at(j) *= 2;
        }
    }
}
