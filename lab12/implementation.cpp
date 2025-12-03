#include "header.hpp"

#include <iostream>

CircularArray::CircularArray(int c) : capacity(c), frontIndex(-1), backIndex(-1) {
    charArray = new char[capacity];
}

void CircularArray::pushBack(char c) {
    if (size() >= capacity) {
        std::cout << "Character array is full!" << '\n';
        return;
    }

    if (backIndex == -1) {
        backIndex = 0;
        frontIndex = 0;
    } else {
        backIndex = (backIndex + 1) % capacity;
    }

    charArray[backIndex] = c;    
}

std::string CircularArray::toString() {
    // std::cout << "front index: " << frontIndex << ", back index: " << backIndex << std::endl;
    // std::cout << "size: " << size() << std::endl;
    if (size() == 0) {
        return "[]";
    }
    std::string result = "[";
    int n = size();
    for (int i = 0; i < n; ++i) {
        int idx = (frontIndex + i) % capacity;
        result.push_back(charArray[idx]);
        if (i != n - 1) {
            result += ", ";
        }
    }
    result += "]";
    return result;
}

char* CircularArray::getArray() {
    return charArray;
}

char CircularArray::peekBack() {
    if (size() == 0) {
        return 0;
    }
    int idx = (backIndex + capacity) % capacity;
    return charArray[idx];
}

char CircularArray::popBack() {
    if (size() == 0) {
        return 0;
    }

    if (size() == 1) {
        char c = charArray[backIndex];
        frontIndex = -1;
        backIndex = -1;
        return c;
    }

    int lastBackIndex = backIndex;

    backIndex = (backIndex - 1 + capacity) % capacity;
    return charArray[lastBackIndex];
}

int CircularArray::size() {
    if (frontIndex > backIndex) {
        return capacity - (frontIndex - backIndex - 1);
    } else if (frontIndex == -1 && backIndex == -1) {
        return 0;
    } else {
        return backIndex - frontIndex + 1;
    }
}

void CircularArray::pushFront(char c) {
    if (size() >= capacity) {
        std::cout << "Character array is full!" << '\n';
        return;
    }

    if (frontIndex == -1) {
        frontIndex = 0;
        backIndex = 0;
    } else {
        frontIndex = (frontIndex - 1 + capacity) % capacity;
    }

    charArray[frontIndex] = c;
}

char CircularArray::popFront() {
    if (size() == 0) {
        return 0;
    }

    if (size() == 1) {
        char c = charArray[frontIndex];
        frontIndex = -1;
        backIndex = -1;
        return c;
    }

    char c = charArray[frontIndex];
    frontIndex = (frontIndex + 1) % capacity;
    return c;
}

char CircularArray::peekFront() {
    if (size() == 0) {
        return 0;
    }
    return charArray[frontIndex];
}
