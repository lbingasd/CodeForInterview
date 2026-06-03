#include <ctime>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class Timer
{
public:
       Timer(int msec):interval(msec){}
       Timer(){}
       ~Timer()
       {
              Stop();
       }

       void Start(std::function<void()> task, int msec = 1000, bool periodic = false)
       {
              Stop();
              isStop = false;
              interval = msec;
              worker = std::thread([this, task, periodic]()
              {
                     while (true)
                     {
                            {
                                   // wait_for 会阻塞直到：
                                   //1. 超时(msec)；
                                   //2. 被 cv.notify_all() 唤醒
                                   // 如果返回 false，说明是超时了，该执行任务了
                                   std::unique_lock<std::mutex> lc(mtx);
                                   if (cv.wait_for(lc, std::chrono::milliseconds(interval), [this] { return isStop;})) 
                                   {
                                          // 如果是因为 isStop 变为 true 被唤醒，直接退出
                                          break;
                                   }
                            }
                            if(task) task();
                            if (!periodic) break;
                     }
              });
       }

       void Stop()
       {
              {
                     std::lock_guard<std::mutex> lc(mtx);
                     isStop = true;
              }
              cv.notify_all();
              if (worker.joinable())
              {
                     worker.join();
              }
              
       }
private:
       int interval = 1000;
       std::thread worker;
       std::mutex mtx;
       bool isStop = false;
       std::condition_variable cv;
};
