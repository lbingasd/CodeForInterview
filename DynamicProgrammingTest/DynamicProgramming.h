#pragma once

#include <algorithm>
#include <climits>
#include <vector>

namespace DynamicProgrammingTest {

/**
 * 1. 爬楼梯
 * 难度：简单
 * 思路：f(n) = f(n - 1) + f(n - 2)，本质是斐波那契。
 * 复杂度：时间 O(N)，空间 O(1)
 */
int climbStairs(int n)
{
    if (n <= 2) return n;
    // 前前状态表示走到 i - 2 的方法数，前一状态表示走到 i - 1 的方法数。
    int prev2 = 1;
    int prev1 = 2;
    for (int i = 3; i <= n; ++i) {
        // 最后一步可以从 i - 1 走一步，也可以从 i - 2 走两步。
        int current = prev1 + prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}

/**
 * 2. 打家劫舍
 * 难度：中等
 * 思路：每间房都选择“偷当前房 + 前前状态”或“不偷当前房”。
 * 复杂度：时间 O(N)，空间 O(1)
 */
int rob(const std::vector<int>& nums)
{
    // 前前状态表示隔一间房之前的最优解，前一状态表示上一间房之前的最优解。
    int prev2 = 0;
    int prev1 = 0;
    for (int money : nums) {
        // 要么跳过当前房屋，要么偷当前房屋并加上 prev2。
        int current = std::max(prev1, prev2 + money);
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}

/**
 * 3. 零钱兑换
 * 难度：中等
 * 思路：dp[x] 表示凑出金额 x 所需的最少硬币数。
 * 复杂度：时间 O(amount * coinCount)，空间 O(amount)
 */
int coinChange(const std::vector<int>& coins, int amount)
{
    // 金额加一作为无穷大，因为存在 1 元硬币时最多也只需要 amount 枚。
    std::vector<int> dp(amount + 1, amount + 1);
    // 金额为 0 时需要 0 枚硬币。
    dp[0] = 0;

    for (int x = 1; x <= amount; ++x) {
        for (int coin : coins) {
            if (coin <= x) {
                // 当前使用一枚 coin，再加上凑出 x - coin 的最优解。
                dp[x] = std::min(dp[x], dp[x - coin] + 1);
            }
        }
    }
    return dp[amount] > amount ? -1 : dp[amount];
}

/**
 * 4. 最长递增子序列
 * 难度：中等
 * 思路：tails[len] 记录长度为 len + 1 的递增子序列的最小尾值。
 * 复杂度：时间 O(N log N)，空间 O(N)
 */
int lengthOfLIS(const std::vector<int>& nums)
{
    // 尾值数组保持递增，它的长度就是当前找到的最长递增子序列长度。
    std::vector<int> tails;
    for (int num : nums) {
        // 找到第一个 >= num 的尾值并替换，让同长度子序列尾值尽量小。
        auto it = std::lower_bound(tails.begin(), tails.end(), num);
        if (it == tails.end()) {
            // 当前数字可以扩展当前最长递增子序列。
            tails.push_back(num);
        } else {
            // 当前数字优化某个长度的尾值，但不改变当前最长长度。
            *it = num;
        }
    }
    return static_cast<int>(tails.size());
}

/**
 * 5. 不同路径
 * 难度：中等
 * 思路：dp[j] 表示到达当前行第 j 列的路径数。
 * 复杂度：时间 O(M * N)，空间 O(N)
 */
int uniquePaths(int m, int n)
{
    // 第一行每个格子都只有一种走法：一直向右。
    std::vector<int> dp(n, 1);
    for (int i = 1; i < m; ++i) {
        for (int j = 1; j < n; ++j) {
            // 来自上方的路径数是旧 dp[j]，来自左侧的是 dp[j - 1]。
            dp[j] += dp[j - 1];
        }
    }
    return dp[n - 1];
}

}
