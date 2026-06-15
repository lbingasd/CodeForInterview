#include <climits>
using namespace std;

class Solution {
public:
    int divide(int dividend, int divisor) {
        // 特殊溢出场景：
        // INT_MIN = -2147483648
        // INT_MIN / -1 = 2147483648
        // 但 int 最大只能表示 2147483647，所以需要返回 INT_MAX
        if (dividend == INT_MIN && divisor == -1) {
            return INT_MAX;
        }

        // 判断最终结果符号。
        bool negative = (dividend < 0) ^ (divisor < 0);

        // 使用 long long 是为了在中间计算时扩大范围，降低溢出风险。
        // 题目输入是 int，但倍增时会出现 value + value，
        // 用 long long 更安全。
        long long a = dividend;
        long long b = divisor;

        // 统一转成负数处理。
        //
        // 为什么不统一转正数？
        // 因为 int 范围是：
        // [-2147483648, 2147483647]
        //
        // INT_MIN 取反会得到 2147483648，
        // 这个值超过 int 最大值，会溢出。
        //
        // 但所有正数都可以安全转成负数：
        // 2147483647 -> -2147483647
        // 100 -> -100
        //
        // 所以统一转负数更安全。
        if (a > 0) a = -a;
        if (b > 0) b = -b;

        int ans = 0;
        while (a <= b) {
            // value 表示当前这一轮要减掉的除数倍数。
            // 初始是 b，也就是 divisor 的 1 倍。
            long long value = b;

            int count = 1;

            // 不断把 value 翻倍，寻找当前 a 能减掉的最大 divisor 的 2^k 倍。
            // 防止 value + value 小于 LLONG_MIN，导致 long long 下溢。
            //
            // LLONG_MIN 是 long long 能表示的最小值：
            // -9223372036854775808
            //
            // 如果 value 已经小于 LLONG_MIN / 2，
            // 再执行 value + value 就会越界。
            // 判断翻倍后是否仍然没有超过当前 a 的绝对值。
            //
            // 因为都是负数，所以：
            // a <= value + value
            //
            // 例如：
            // a = -43, value = -24
            // value + value = -48
            //
            // -43 <= -48 是 false，
            // 说明 -48 的绝对值 48 已经超过 43，不能继续翻倍。
            while (value >= (LLONG_MIN >> 1) && a <= value + value) {
                value += value;
                count += count;
            }
            a -= value;
            ans += count;
        }

        // 根据最开始判断的符号返回结果。
        return negative ? -ans : ans;
    }
};