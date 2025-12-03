#include <iostream>

#include "header.hpp"

int main() {

    std::vector<std::string> words = {"eat", "tea", "tan", "ate", "nat", "bat"};

    auto groupedAnagrams = groupSortedAnagrams(words);

    for (const auto& group : groupedAnagrams) {
        std::cout << "[ ";
        for (const auto& word : group) {
            std::cout << word << " ";
        }
        std::cout << "]\n";
    }

    return 0;
}