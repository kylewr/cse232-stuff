#include "header.hpp"

#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include <algorithm>

Queue::Queue() : head(nullptr), tail(nullptr), n(0) {}

void Queue::clear() {
    while (head) {
        Node* tmp {head};
        head = head->next;
        delete tmp;
    }
    tail = nullptr;
    n = 0;
}

Queue::~Queue() {
    clear();
}

bool Queue::empty() {
    return head == nullptr;
}
int Queue::size() {
    return n;
}

void Queue::enqueue(int v) {
    Node* newNode {new Node {v, nullptr}};
    if (empty()) {
        tail = newNode;
        head = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    n++;
}

bool Queue::dequeue(int& out) {
    if (empty())
        return false;
    Node* temp {head};
    out = temp->value;
    head = head->next;
    if (head == nullptr)
        tail = nullptr;
    delete temp;
    n--;
    return true;
}

void Queue::print() {
    Node* p {head};
    while (p) {
        std::cout << p->value;
        if (p->next)
            std::cout << " -> ";
        p = p->next;
    }
    std::cout << '\n';
}


void Queue::pairedRoot(int v) {
    enqueue(v);

    std::string s = std::to_string(v);
    std::vector<int> roots;
    size_t pos = 0;

    if (s.size() % 2 != 0) {
        roots.push_back(s[0] - '0');
        pos = 1;
    }

    for (size_t i = pos; i + 1 < s.size(); i += 2) {
        int val = (s.at(i) - '0') * 10 + (s.at(i + 1) - '0');
        roots.push_back(val);
    }

    int sum = 0;
    for (int num : roots) {
        sum += num;
    }

    if (sum >= 100) {
        pairedRoot(sum);
    } else {
        if (tail && sum < tail->value) {
            enqueue(sum);
        }
    }
    
}
