#include "header.hpp"
#include <iostream>

int main() {
    // CircularArray ca(4);

    // ca.pushBack('A');
    // ca.pushBack('B');
    // ca.pushBack('C');

    // std::cout << ca.toString() << std::endl; // Expected output: [A, B, C]

    // ca.pushFront('D');
    // // ca.pushBack('E'); // Should indicate that the array is full

    // // ca.pushFront('C');

    // std::cout << ca.toString() << std::endl;

    CircularArray deque(4);
    deque.pushBack('A');
    deque.pushBack('B');
    deque.pushBack('C');
    deque.pushBack('D');
    std::cout << "Deque after pushing back A, B, C, D: " << deque.toString() << std::endl;
    std::cout << "Peek front: " << deque.peekFront() << std::endl;
    char aChar = deque.popBack();
    std::cout << "Popped character: " << aChar << std::endl;

    std::cout << "Deque after popping back: " << deque.toString() << std::endl;
    std::cout << "Peek frot: " << deque.peekFront() << std::endl;


    return 0;
}
