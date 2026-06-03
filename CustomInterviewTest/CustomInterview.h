#pragma once

#include <algorithm>
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

namespace CustomInterviewTest {

/**
 * 1. 固定窗口限流器
 * 难度：简单
 * 需求：每个用户在一个时间窗口内最多请求 limit 次。
 * 思路：为每个用户保存 {窗口开始时间, 当前窗口请求次数}。
 * 复杂度：时间 O(1)，空间 O(用户数)
 */
class FixedWindowRateLimiter {
public:
    FixedWindowRateLimiter(int windowSeconds, int limit)
        : windowSeconds(windowSeconds), limit(limit) {}

    bool Allow(const std::string& userId, int timestampSeconds)
    {
        // 下标访问会为新用户自动创建一个空窗口。
        Window& window = windows[userId];
        // 如果当前请求已经超出旧窗口，则开启一个新窗口。
        if (timestampSeconds - window.start >= windowSeconds) {
            window.start = timestampSeconds;
            window.count = 0;
        }

        // 当前窗口请求次数达到上限时，拒绝本次请求。
        if (window.count >= limit) {
            return false;
        }

        // 接受本次请求，并记录到当前窗口中。
        ++window.count;
        return true;
    }

private:
    struct Window {
        int start = 0;
        int count = 0;
    };

    int windowSeconds;
    int limit;
    std::unordered_map<std::string, Window> windows;
};

/**
 * 2. 数据流滑动平均值
 * 难度：简单
 * 需求：返回最近 size 个数字的平均值。
 * 思路：队列维护当前窗口，sum 支持 O(1) 计算平均值。
 * 复杂度：时间 O(1)，空间 O(size)
 */
class MovingAverage {
public:
    MovingAverage(int size) : size(size), sum(0) {}

    double Next(int value)
    {
        // 将最新值加入窗口尾部。
        q.push_back(value);
        sum += value;
        if (static_cast<int>(q.size()) > size) {
            // 窗口超过指定大小时，移除最旧的值。
            sum -= q.front();
            q.pop_front();
        }
        // 初始阶段队列长度可能小于 size，因此用实际长度计算。
        return static_cast<double>(sum) / q.size();
    }

private:
    int size;
    long long sum;
    std::deque<int> q;
};

/**
 * 3. 最近请求计数器
 * 难度：简单
 * 需求：统计 [t - 3000, t] 区间内的请求数量。
 * 思路：队列保存时间戳，从队首弹出过期请求。
 * 复杂度：均摊时间 O(1)，空间 O(N)
 */
class RecentCounter {
public:
    int Ping(int t)
    {
        // 保存当前请求时间戳。
        requests.push_back(t);
        // 移除早于 [t - 3000, t] 范围的过期时间戳。
        while (!requests.empty() && requests.front() < t - 3000) {
            requests.pop_front();
        }
        // 队列中剩余的时间戳就是最近请求。
        return static_cast<int>(requests.size());
    }

private:
    std::deque<int> requests;
};

/**
 * 4. 循环队列
 * 难度：中等
 * 需求：不使用 STL queue，实现固定容量队列。
 * 思路：使用数组、队首下标和当前元素数量模拟环形缓冲区。
 * 复杂度：时间 O(1)，空间 O(K)
 */
class CircularQueue {
public:
    CircularQueue(int k) : data(k), head(0), count(0) {}

    bool EnQueue(int value)
    {
        if (IsFull()) return false;
        // 队尾位置由 head + count 计算，并对容量取模形成环。
        int tail = (head + count) % data.size();
        data[tail] = value;
        ++count;
        return true;
    }

    bool DeQueue()
    {
        if (IsEmpty()) return false;
        // 队首向前移动，到数组末尾后自动绕回开头。
        head = (head + 1) % data.size();
        --count;
        return true;
    }

    int Front() const
    {
        return IsEmpty() ? -1 : data[head];
    }

    int Rear() const
    {
        if (IsEmpty()) return -1;
        // 最后一个有效元素位于下一次插入位置的前一格。
        int tail = (head + count - 1) % data.size();
        return data[tail];
    }

    bool IsEmpty() const
    {
        return count == 0;
    }

    bool IsFull() const
    {
        return count == static_cast<int>(data.size());
    }

private:
    std::vector<int> data;
    int head;
    int count;
};

/**
 * 5. 规范化文件路径
 * 难度：中等
 * 需求：处理 Unix 风格路径中的 "."、".." 和重复斜杠。
 * 思路：按斜杠切分路径，用 vector 模拟栈保存有效目录。
 * 复杂度：时间 O(N)，空间 O(N)
 */
std::string simplifyPath(const std::string& path)
{
    // 路径片段数组按顺序保存有效目录。
    std::vector<std::string> stack;
    std::string part;

    for (int i = 0; i <= static_cast<int>(path.size()); ++i) {
        // 把字符串末尾当作一个斜杠，便于处理最后一个片段。
        char ch = (i == static_cast<int>(path.size())) ? '/' : path[i];
        if (ch == '/') {
            if (part == "..") {
                // ".." 表示返回上一级目录，如果存在则弹出上一个有效目录。
                if (!stack.empty()) stack.pop_back();
            } else if (!part.empty() && part != ".") {
                // 普通目录名保留；空片段和 "." 直接忽略。
                stack.push_back(part);
            }
            part.clear();
        } else {
            // 逐字符构建当前路径片段。
            part.push_back(ch);
        }
    }

    // 重新拼接规范化后的绝对路径。
    std::string result = "/";
    for (int i = 0; i < static_cast<int>(stack.size()); ++i) {
        if (i > 0) result += "/";
        result += stack[i];
    }
    return result;
}

}
