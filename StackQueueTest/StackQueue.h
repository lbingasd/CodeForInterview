#pragma once

#include <algorithm>
#include <functional>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

namespace StackQueueTest {

/**
 * 1. 有效括号
 * 难度：简单
 * 思路：左括号入栈，遇到右括号时匹配栈顶。
 * 复杂度：时间 O(N)，空间 O(N)
 */
bool isValidParentheses(const std::string& s)
{
    // 栈中保存尚未匹配的左括号。
    std::stack<char> st;
    // 右括号映射到它期望匹配的左括号。
    std::unordered_map<char, char> pair = {
        {')', '('},
        {']', '['},
        {'}', '{'}
    };

    for (char ch : s) {
        if (ch == '(' || ch == '[' || ch == '{') {
            // 左括号先入栈，等待后续右括号匹配。
            st.push(ch);
        } else if (pair.count(ch)) {
            // 右括号必须匹配最近一个尚未匹配的左括号。
            if (st.empty() || st.top() != pair[ch]) return false;
            st.pop();
        }
    }
    // 所有左括号都被匹配后，字符串才合法。
    return st.empty();
}

/**
 * 2. 最小栈
 * 难度：简单
 * 思路：额外维护一个最小值栈，栈顶始终是当前最小值。
 * 复杂度：每个操作 O(1)，空间 O(N)
 */
class MinStack {
public:
    void Push(int value)
    {
        data.push(value);
        // 只有当 value 能成为当前最小值时，才压入最小值栈。
        if (mins.empty() || value <= mins.top()) {
            mins.push(value);
        }
    }

    void Pop()
    {
        if (data.empty()) return;
        // 如果弹出的值等于当前最小值，最小值栈也要同步弹出。
        if (data.top() == mins.top()) {
            mins.pop();
        }
        data.pop();
    }

    int Top() const
    {
        return data.top();
    }

    int GetMin() const
    {
        return mins.top();
    }

private:
    std::stack<int> data;
    std::stack<int> mins;
};

/**
 * 3. 用栈实现队列
 * 难度：简单
 * 思路：输入栈负责入队，输出栈负责出队。
 * 复杂度：均摊时间 O(1)，空间 O(N)
 */
class MyQueue {
public:
    void Push(int x)
    {
        // 新元素总是先进入输入栈。
        in.push(x);
    }

    int Pop()
    {
        // 确保输出栈栈顶是队列中最早进入的元素。
        MoveIfNeeded();
        int value = out.top();
        out.pop();
        return value;
    }

    int Peek()
    {
        MoveIfNeeded();
        return out.top();
    }

    bool Empty() const
    {
        return in.empty() && out.empty();
    }

private:
    void MoveIfNeeded()
    {
        if (!out.empty()) return;
        // 一次性搬移所有元素，反转顺序后即可暴露队首。
        while (!in.empty()) {
            out.push(in.top());
            in.pop();
        }
    }

    std::stack<int> in;
    std::stack<int> out;
};

/**
 * 4. 每日温度
 * 难度：中等
 * 思路：单调递减栈保存还没找到更高温度的下标。
 * 复杂度：时间 O(N)，空间 O(N)
 */
std::vector<int> dailyTemperatures(const std::vector<int>& temperatures)
{
    std::vector<int> result(temperatures.size(), 0);
    // 栈中保存还没有找到更暖一天的下标。
    std::stack<int> st;

    for (int i = 0; i < static_cast<int>(temperatures.size()); ++i) {
        // 当前温度可以解决栈顶所有更冷的历史日期。
        while (!st.empty() && temperatures[i] > temperatures[st.top()]) {
            int prev = st.top();
            st.pop();
            result[prev] = i - prev;
        }
        // 当前日期入栈，等待未来更高温度。
        st.push(i);
    }
    return result;
}

/**
 * 5. 前 K 个高频元素
 * 难度：中等
 * 思路：先用哈希表计数，再用小根堆保留频率最高的 K 个元素。
 * 复杂度：时间 O(N log K)，空间 O(N)
 */
std::vector<int> topKFrequent(const std::vector<int>& nums, int k)
{
    // 第一遍统计每个数字出现的次数。
    std::unordered_map<int, int> freq;
    for (int num : nums) {
        ++freq[num];
    }

    using Item = std::pair<int, int>; // {频率, 数值}
    // 小根堆只保留 K 个最大频率，其中最小的频率在堆顶。
    std::priority_queue<Item, std::vector<Item>, std::greater<Item>> heap;
    for (const auto& entry : freq) {
        heap.push({entry.second, entry.first});
        if (static_cast<int>(heap.size()) > k) {
            // 堆大小超过 K 时，移除当前频率最小的元素。
            heap.pop();
        }
    }

    std::vector<int> result;
    while (!heap.empty()) {
        // 小根堆弹出顺序是从低频到高频。
        result.push_back(heap.top().second);
        heap.pop();
    }
    // 反转后让高频元素排在前面。
    std::reverse(result.begin(), result.end());
    return result;
}

}
