#include "header.hpp"

int main() {
    LinkedList list;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    list.print(); // Expected output: 3 -> 2 -> 1

    list.reverse();

    list.print(); // Expected output: 1 -> 2 -> 3

    return 0;
}