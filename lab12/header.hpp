#pragma once

#ifndef CIRCULARARRAY_HPP
#define CIRCULARARRAY_HPP

#include <iostream>
#include <string>

class CircularArray {
    private:
        char* charArray;
        int capacity;
        int frontIndex;
        int backIndex;

    public:
        CircularArray(int);
        char* getArray();
        void pushBack(char);
        char popBack();
        char peekBack();
        int size();
        std::string toString();
        void pushFront(char);
        char popFront();
        char peekFront();
};
#endif // CIRCULARARRAY_HPP
