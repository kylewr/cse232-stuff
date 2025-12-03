#pragma once

#include <iostream>

struct Node {
        int value {};  // stores the data
        Node* next {}; // pointer to the next node
};

class LinkedList {
    private:
        Node* head {}; // first node (nullptr if empty)
        int n {};      // number of elements
        void clear();  // helper to clear all nodes
    public:
        LinkedList();  // constructor
        ~LinkedList(); // destructor
        bool empty();
        int size();
        void push_front(int v);   // insert at the front
        bool pop_front(int& out); // remove from the front
        void print();             // print contents

        void reverse();
};
