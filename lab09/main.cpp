#include <iostream>


int fib(int inp) {
    if (inp <= 1)
        return inp;

    int prev = 0;
    int curr = 1;

    for (int i = 2; i <= inp; i++) {
        int next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}

int main() {
    std::cout << fib(9) << std::endl;

    return 0;
}
