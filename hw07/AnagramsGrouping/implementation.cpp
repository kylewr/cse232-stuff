#include "header.hpp"

#include <algorithm>
#include <unordered_map>

std::vector<std::vector<std::string>> groupSortedAnagrams(const std::vector<std::string>& words) {
    std::vector<std::vector<std::string>> out {};

    for (const auto& word : words) {
        std::string sortedWord = word;
        std::sort(sortedWord.begin(), sortedWord.end());

        auto it = std::find_if(out.begin(), out.end(),
                               [&sortedWord](const std::vector<std::string>& group) {
                                   std::string representative = group.front();
                                   std::sort(representative.begin(), representative.end());
                                   return representative == sortedWord;
                               });

        if (it != out.end()) {
            it->push_back(word);
        } else {
            out.push_back({word});
        }
    }

    for (auto& group : out) {
        std::sort(group.begin(), group.end());
    }

    std::sort(out.begin(), out.end());

    return out;
}
