#include "Matrix.hpp"
#include <iostream>
#include <vector>

int main() {
    int rows {};
    int cols {};

    std::cout << "Enter the number of rows: ";
    std::cin >> rows;
    std::cout << '\n' << "Enter the number of columns: ";
    std::cin >> cols;

    std::vector<std::vector<int>> matrix = create2DVector(rows, cols);

    std::cout << "\n\n" << "Matrix after initial creation:" << '\n';
    print2DVector(matrix);

    fillWithIndices(matrix);

    std::cout << '\n' << "Matrix after filling with the sum of indices:" << '\n';
    print2DVector(matrix);

    doubleElements(&matrix);

    std::cout << '\n' << "Matrix after doubling each element:" << '\n';
    print2DVector(matrix);

    return 0;
}
