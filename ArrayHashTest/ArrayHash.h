#pragma once

#include <algorithm>
#include <array>
#include <climits>
#include <deque>
#include <string>
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
 * 思路：pre[i] 为 [0..i] 里所有数的和,pre[i]−pre[j−1]==k，统计历史前缀和出现次数。
 * 复杂度：时间 O(N)，空间 O(N)
 */
int subarraySum(std::vector<int>& nums, int k) 
{
        std::unordered_map<int, int> mp;
        mp[0] = 1;
        int count = 0, pre = 0;
        for (auto& x:nums) 
        {
            pre += x;
            if (mp.find(pre - k) != mp.end()) 
            {
                count += mp[pre - k];
            }
            mp[pre]++;
        }
    return count;
}

/**
 * 6. 无重复字符的最长子串
 * 难度：中等
 * 思路：滑动窗口记录每个字符最后出现的位置；遇到窗口内的重复字符时，
 *       左边界直接移动到该字符上次出现位置的下一位。
 * 复杂度：时间 O(N)，空间 O(字符集大小)
 * 注意：按 std::string 的字节处理字符；unsigned char 转换可避免负下标。
 */
int lengthOfLongestSubstring(const std::string& s)
{
    // lastSeen[ch] 表示字符 ch 最近一次出现的下标，-1 表示尚未出现。
    std::array<int, 256> lastSeen;
    lastSeen.fill(-1);

    int left = 0;
    int best = 0;
    for (int right = 0; right < static_cast<int>(s.size()); ++right) {
        unsigned char ch = static_cast<unsigned char>(s[right]);
        // 只有重复字符位于当前窗口 [left, right) 内时，才需要移动左边界。
        if (lastSeen[ch] >= left) {
            left = lastSeen[ch] + 1;
        }
        lastSeen[ch] = right;
        best = std::max(best, right - left + 1);
    }
    return best;
}

/**
 * 7. 长度最小的子数组
 * 依据：LeetCode Sliding Window 题单中的正数数组可变窗口题。
 * 思路：和达到 target 后不断收缩左边界，保留最短合法窗口。
 * 注意：nums 必须为正整数，才能保证窗口单调收缩。
 */
int minSubArrayLen(int target, const std::vector<int>& nums)
{
    int left = 0;
    long long sum = 0;
    int best = INT_MAX;
    for (int right = 0; right < static_cast<int>(nums.size()); ++right) {
        sum += nums[right];
        while (sum >= target) {
            best = std::min(best, right - left + 1);
            sum -= nums[left++];
        }
    }
    return best == INT_MAX ? 0 : best;
}

/**
 * 8. 找到字符串中所有字母异位词
 * 依据：LeetCode Sliding Window 题单中的固定长度频次窗口题。
 * 思路：维护长度为 p.size() 的窗口，并比较 26 个小写字母的计数。
 */
std::vector<int> findAnagrams(const std::string& s, const std::string& p)
{
    if (p.empty() || p.size() > s.size()) return {};
    std::vector<int> need(26), window(26), result;
    for (char ch : p) ++need[ch - 'a'];
    for (int right = 0; right < static_cast<int>(s.size()); ++right) {
        ++window[s[right] - 'a'];
        if (right >= static_cast<int>(p.size())) {
            --window[s[right - p.size()] - 'a'];
        }
        if (window == need) {
            result.push_back(right - static_cast<int>(p.size()) + 1);
        }
    }
    return result;
}

/**
 * 9. 接雨水 (LeetCode 42) — 双指针解法
 * 难度：困难
 * 思路：双指针从两端向中间收缩。对于每个位置，能接的水量由左右两侧
 *       较矮的一侧决定。维护 leftMax 和 rightMax，哪边矮就移动哪边。
 * 复杂度：时间 O(N)，空间 O(1)
 */
int trap(const std::vector<int>& height)
{
    int left = 0, right = static_cast<int>(height.size()) - 1;
    // leftMax：左侧已遍历区域的最高柱子；rightMax：右侧已遍历区域的最高柱子。
    int leftMax = 0, rightMax = 0;
    int water = 0;

    while (left < right) {
        // 哪边矮就处理哪边 —— 因为当前位置的水量由较矮一侧决定。
        if (height[left] < height[right]) {
            if (height[left] >= leftMax) {
                // 当前柱子比左侧最高还高，更新 leftMax，此处无法接水。
                leftMax = height[left];
            } else {
                // 当前柱子低于 leftMax，可以接水。
                water += leftMax - height[left];
            }
            ++left;
        } else {
            if (height[right] >= rightMax) {
                rightMax = height[right];
            } else {
                water += rightMax - height[right];
            }
            --right;
        }
    }
    return water;
}

/**
 * 10. 滑动窗口最大值
 * 依据：LeetCode 239，题目明确要求滑动窗口，并使用单调队列优化。
 * 思路：deque 保存可能成为最大值的下标，队列值保持单调递减。
 * 复杂度：时间 O(N)，空间 O(K)
 */
std::vector<int> maxSlidingWindow(const std::vector<int>& nums, int k)
{
    if (k <= 0 || k > static_cast<int>(nums.size())) return {};
    std::deque<int> candidates;
    std::vector<int> result;
    for (int right = 0; right < static_cast<int>(nums.size()); ++right) {
        while (!candidates.empty() && candidates.front() <= right - k) {
            candidates.pop_front();
        }
        while (!candidates.empty() && nums[candidates.back()] <= nums[right]) {
            candidates.pop_back();
        }
        candidates.push_back(right);
        if (right >= k - 1) result.push_back(nums[candidates.front()]);
    }
    return result;
}
}
