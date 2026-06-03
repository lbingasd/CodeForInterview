#include <chrono>
#include <string>
#include <sstream>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iomanip> // 必须包含此头文件以使用 std::put_time
#include <sstream>

namespace HFrequency
{
      void run() 
      {
            // 1. 修复：获取并格式化当前时间
            auto now = std::chrono::system_clock::now();
            auto currentTime = std::chrono::system_clock::to_time_t(now);
            std::stringstream time_ss;
            time_ss << std::put_time(std::localtime(&currentTime), "%Y-%m-%d %H:%M:%S");
            std::string currentTimeStr = time_ss.str();

            //字符串分割
            std::stringstream ss("1,2,3,4,5"); 
            std::string vStr = "";
            while(std::getline(ss, vStr, ',')) 
            {
                  // 处理分割后的字符串 vStr
            }

            {
                  std::mutex mtx;
                  std::condition_variable cv;
                  bool ready = false;
                  bool isStop = false;
                  //线程处理
                  std::function<void()> task = [&](){
                        std::unique_lock<std::mutex> lock(mtx);
                        // 2. 阻塞等待：直到 线程池停止 或 队列中有新任务
                        cv.wait(lock, [&]() {
                              return isStop;
                        });
                        // 该函数会等待 1 秒，或者直到 ready 变为 true
                        cv.wait_for(lock, std::chrono::seconds(1), [&](){ return ready; });
                        if (ready) {
                              // 处理 ready 为 true 的情况
                        } else {
                              // 处理超时的情况
                        }
                  };
                  
                  //休眠std::this_thread::sleep_for
                  std::chrono::milliseconds(1000); // 1 秒
                  std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 休眠 1 秒
                  std::this_thread::sleep_for(std::chrono::seconds(1));
                  //创建并启动线程
                  std::thread t([task](){
                        task();
                  });
                  //通知线程
                  {
                        std::lock_guard<std::mutex> lock(mtx);
                        ready = true;
                        cv.notify_one();
                  }
                  
                  t.join(); 
            }

            char str[] = "Hello, World!";
            size_t len = strlen(str);
            size_t left = 0;
            size_t right = len - 1;
            std::memmove(str, str + left, right - left + 1);
            str[right - left + 1] = '\0';

            char str1[] = "Hello"; 
            // "Hello" 实际在内存中是 {'H', 'e', 'l', 'l', 'o', '\0'}
            printf("strlen(str1) = %zu\n", strlen(str1)); // 输出: 5
            printf("sizeof(str1) = %zu\n", sizeof(str1)); // 输出: 6

            {
                  //include <sstream>
                  //template <typename T>
                  //LogMessage& operator << (const T& msg)
                  //{
                  //      Oss << msg;
                  //      return *this;
                  //}
                  std::ostringstream oss;
                  oss << "Hello, " << "World!" << " The answer is ";
                  std::string str2 = oss.str();
            }
      }
}