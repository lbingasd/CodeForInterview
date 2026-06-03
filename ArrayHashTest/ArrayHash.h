#pragma once

#include <algorithm>
#include <climits>
#include <unordered_map>
#include <vector>

namespace ArrayHashTest {

/**
 * 1. 两数之和
 * 难度：简单
 * 思路：遍历数组时，用哈希表记录“数值 -> 下标”。
 * 复杂度：时间 O(N)，空间 O(N)
 */
std::vector<int> twoSum(const std::vector<int>& nums, int target)
{
    // 哈希表的键表示数值，值表示该数值第一次出现的下标。
    std::unordered_map<int, int> indexByValue;
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        // 如果 target - nums[i] 之前出现过，旧下标和当前下标就是答案。
        int need = target - nums[i];
        auto it = indexByValue.find(need);
        if (it != indexByValue.end()) {
            return {it->second, i};
        }
        // 先查找再记录当前数，避免同一个元素被使用两次。
        indexByValue[nums[i]] = i;
    }
    return {};
}

/**
 * 2. 三数之和
 * 难度：中等
 * 思路：先排序，再固定一个数，剩下两个数用双指针查找。
 * 复杂度：时间 O(N^2)，除结果外空间 O(1)
 */
std::vector<std::vector<int>> threeSum(std::vector<int> nums)
{
    std::vector<std::vector<int>> result;
    // 排序后才能方便去重，并使用双指针收缩区间。
    std::sort(nums.begin(), nums.end());

    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        // 跳过重复的固定数，避免生成重复三元组。
        if (i > 0 && nums[i] == nums[i - 1]) continue;
        int left = i + 1;
        int right = static_cast<int>(nums.size()) - 1;

        while (left < right) {
            int sum = nums[i] + nums[left] + nums[right];
            if (sum == 0) {
                result.push_back({nums[i], nums[left], nums[right]});
                // 记录一个合法三元组后，跳过左右两侧的重复值。
                while (left < right && nums[left] == nums[left + 1]) ++left;
                while (left < right && nums[right] == nums[right - 1]) --right;
                ++left;
                --right;
            } else if (sum < 0) {
                // 当前和偏小，需要把左指针右移到更大的值。
                ++left;
            } else {
                // 当前和偏大，需要把右指针左移到更小的值。
                --right;
            }
        }
    }

    return result;
}

/**
 * 3. 最大子数组和
 * 难度：简单
 * 思路：Kadane 动态规划，维护“必须以当前位置结尾”的最大和。
 * 复杂度：时间 O(N)，空间 O(1)
 */
int maxSubArray(const std::vector<int>& nums)
{
    // 当前和表示必须以当前下标结尾的最大子数组和。
    int current = nums.empty() ? 0 : nums[0];
    // 最优和表示遍历过程中见过的全局最大子数组和。
    int best = current;
    for (int i = 1; i < static_cast<int>(nums.size()); ++i) {
        // 要么接在前一个子数组后面，要么从 nums[i] 重新开始。
        current = std::max(nums[i], current + nums[i]);
        best = std::max(best, current);
    }
    return best;
}

/**
 * 4. 合并区间
 * 难度：中等
 * 思路：按区间起点排序，相邻区间如果重叠就合并。
 * 复杂度：时间 O(N log N)，除结果外空间 O(1)
 */
std::vector<std::vector<int>> mergeIntervals(std::vector<std::vector<int>> intervals)
{
    if (intervals.empty()) return {};
    // 按起点排序后，可能重叠的区间一定相邻。
    std::sort(intervals.begin(), intervals.end());

    std::vector<std::vector<int>> result;
    result.push_back(intervals[0]);
    for (int i = 1; i < static_cast<int>(intervals.size()); ++i) {
        if (intervals[i][0] <= result.back()[1]) {
            // 有重叠：必要时扩展最后一个合并区间的右端点。
            result.back()[1] = std::max(result.back()[1], intervals[i][1]);
        } else {
            // 无重叠：开启一个新的合并区间。
            result.push_back(intervals[i]);
        }
    }
    return result;
}

/**
 * 5. 和为 K 的子数组
 * 难度：中等
 * 思路：prefixSum[j] - prefixSum[i] == k，统计历史前缀和出现次数。
 * 复杂度：时间 O(N)，空间 O(N)
 */
int subarraySum(const std::vector<int>& nums, int k)
{
    // 前缀和计数表用于记录某个历史前缀和出现过多少次。
    std::unordered_map<int, int> countByPrefix;
    // 空前缀用于处理从下标 0 开始的子数组。
    countByPrefix[0] = 1;

    int prefix = 0;
    int result = 0;
    for (int num : nums) {
        prefix += num;
        // 需要查找是否存在历史前缀和等于 prefix - k。
        auto it = countByPrefix.find(prefix - k);
        if (it != countByPrefix.end()) {
            result += it->second;
        }
        // 当前前缀和加入统计，供后续子数组使用。
        ++countByPrefix[prefix];
    }
    return result;
}

}
