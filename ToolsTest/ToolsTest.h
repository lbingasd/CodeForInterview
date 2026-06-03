#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

namespace ToolTest{

       std::string GetCurrentTime()
       {
              auto now = std::chrono::system_clock::now();
              auto currentTime = std::chrono::system_clock::to_time_t(now);
              std::stringstream ss;
              ss<<std::put_time(std::localtime(&currentTime), "%Y-%m-%d %H:%M:%S");
              return ss.str();
       }

       std::vector<int> SplitVersionByDot(const std::string& version, char split)
       {
              std::vector<int> versionsCode;
              std::stringstream ss(version);
              std::string vStr = "";
              while(std::getline(ss, vStr, split))
              {
                     if (vStr.empty() || std::stoi(vStr) < 0)
                            versionsCode.push_back(0);
                     else
                            versionsCode.push_back(std::stoi(vStr));
              } 
              return versionsCode;
       }

       int CompareVersion(const std::string& vCurrent, const std::string& vNew, char split)
       {
              std::vector<int> v1 = SplitVersionByDot(vCurrent, split);
              std::vector<int> v2 = SplitVersionByDot(vNew, split);

              // 取两者的最大长度
              size_t maxSize = std::max(v1.size(), v2.size());

              for (size_t i = 0; i < maxSize; ++i) 
              {
                     // 优化点：如果索引超出范围，则取 0，无需手动补齐 vector
                     int val1 = (i < v1.size()) ? v1[i] : 0;
                     int val2 = (i < v2.size()) ? v2[i] : 0;

                     if (val1 < val2) return 1;  // vNew 更大
                     if (val1 > val2) return -1; // vCurrent 更大
              }
              return 0; // 完全相等
       }

       void reverseString(char* str) 
       {
              if (!str) return;
              int left = 0;
              int right = strlen(str) - 1;
              while (left < right) 
              {
                     // 使用位运算或 std::swap 交换
                     std::swap(str[left++], str[right--]);
              }
       }

       void trim(char* str) 
       {
              if (!str) return;
              
              // 1. 处理开头空格
              char* start = str;
              while (*start && isspace(*start)) start++;
              
              // 2. 处理末尾空格
              char* end = str + strlen(str) - 1;
              while (end > start && isspace(*end)) end--;
              *(end + 1) = '\0';
              
              // 3. 将内容移到最前面
              if (start != str) 
              {
                     //目标：str
                     //源：start
                     //移动长度：end - start + 2 字节
                     std::memmove(str, start, end - start + 2); // +2 是为了包含 '\0'
              }
       }

       int my_atoi(const char* str) 
       {
              if (!str) return 0;
              
              while (isspace(*str)) str++; // 跳过空格
              
              int sign = 1;
              if (*str == '-' || *str == '+') {
                     if (*str == '-') sign = -1;
                     str++;
              }
              
              long long res = 0; // 用更长的类型检测溢出
              while (*str >= '0' && *str <= '9') 
              {
                     res = res * 10 + (*str - '0');
                     // 溢出检查
                     if (res * sign >= INT_MAX) return INT_MAX;
                     if (res * sign <= INT_MIN) return INT_MIN;
                     str++;
              }
              return (int)(res * sign);
       }

       //暴力匹配
       const char* my_strstr(const char* haystack, const char* needle) 
       {
              if (!*needle) return haystack;
              
              for (; *haystack; ++haystack) 
              {
                     const char *h = haystack;
                     const char *n = needle;
                     // 尝试匹配
                     while (*h && *n && *h == *n) 
                     {
                            h++;
                            n++;
                     }
                     if (!*n) return haystack; // 匹配成功
              }
              return nullptr;
       }
}