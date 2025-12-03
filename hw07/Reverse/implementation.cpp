#include "header.hpp"

LinkedList::LinkedList() : head(nullptr), n(0) {}

void LinkedList::clear() {
    while (head) {
        Node* temp {head};
        head = head->next;
        delete temp;
    }
    n = 0;
}

LinkedList::~LinkedList() {
    clear();
}

bool LinkedList::empty() {
    return head == nullptr;
}
int LinkedList::size() {
    return n;
}

void LinkedList::push_front(int v) {
    Node* newNode {new Node {v, head}};
    head = newNode;
    n++;
}

bool LinkedList::pop_front(int& out) {
    if (empty())
        return false;
    Node* temp {head};
    out = temp->value;
    head = head->next;
    delete temp; // free memory
    n--;
    return true;
}

void LinkedList::print() {
    Node* p {head};
    while (p != nullptr) {
        std::cout << p->value;
        if (p->next != nullptr)
            std::cout << " -> ";
        p = p->next;
    }
    std::cout << "\n";
}

void LinkedList::reverse() {
    Node* prev = nullptr;
    Node* current = head;
    Node* next = nullptr;

    while (current != nullptr) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    head = prev;
}
