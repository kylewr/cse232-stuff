#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>

std::vector<std::vector<int>> findDistinctPairs(const std::vector<int>& nums, int target) {
    std::unordered_set<int> seen;
    std::unordered_set<std::string> usedPairs;
    std::vector<std::vector<int>> result;
    for (int num : nums) {
        int complement = target - num;
        if (seen.find(complement) != seen.end()) {
            int first = std::min(num, complement);
            int second = std::max(num, complement);
            std::string pairKey = std::to_string(first) + "," + std::to_string(second);
            if (!usedPairs.count(pairKey)) {
                result.push_back({first, second});
                usedPairs.insert(pairKey);
            }
        }
        seen.insert(num);
    }

    std::sort(result.begin(), result.end());

    return result;
}

int main() {
    std::vector<int> nums = {1, 2, 3, 4, 3, 2, 1};
    int target = 5;

    std::vector<std::vector<int>> pairs = findDistinctPairs(nums, target);

    std::cout << "Distinct pairs that sum to " << target << ":\n";
    for (const auto& pair : pairs) {
        std::cout << "[" << pair[0] << ", " << pair[1] << "]\n";
    }

    return 0;
}
