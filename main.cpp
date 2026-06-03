#include <iostream>
#include "SingletonTest/LazySingleton.h"
#include "ThreadTest/ProduceConsumers.h"
#include "LogTest/Logger.h"
#include "ToolsTest/ToolsTest.h"
#include "TimerTest/Timer.h"
#include "LRUCacheTest/LRUCache.h"
#include "MyStringTest/MyString.h"
#include "MyVectorTest/MyVector.h"

int main()
{
       LOG_DEBUG << "start main";
       LazySingleton::GetInstance()->Show();
       std::string currentTime = ToolTest::GetCurrentTime();
       Timer Timer;
       Timer.Start([](){
              auto now = std::chrono::system_clock::now();
              auto currentTime = std::chrono::system_clock::to_time_t(now);
              LOG_DEBUG << "Timer Loop:" << currentTime;
       });

       {
              LOG_DEBUG << "start producer  and consumer";
              // 1. 初始化生产者消费者管理对象，设置缓冲区容量为 3
              // 此时 isStop 为 false，mutex 和 condition_variable 已准备就绪
              ProduceConsumers q(3);

              /**
               * 2. 创建并启动线程
               * - std::thread 接收：函数名, 参数1, 参数2...
               * - std::ref(q)：非常关键！强制以引用方式传递对象。
               * 如果不加 ref，thread 会尝试拷贝 q，而 q 包含 mutex（不可拷贝），会导致编译报错。
               */

              // 启动两个生产者线程，分别传入不同的 ID (1 和 2)
              thread p1(ProducerTask, std::ref(q), 1);
              thread p2(ProducerTask, std::ref(q), 2);

              // 启动一个消费者线程
              thread c1(ConsumerTask, std::ref(q));

              /**
               * 3. 线程阻塞与同步 (Join)
               * - join() 会使主线程进入等待状态，直到被调用的子线程执行完毕。
               * - 如果不调用 join，主线程执行完大括号 {} 就会销毁对象 q，
               * 而此时子线程若还在运行并访问 q，会导致程序非法内存访问而崩溃（Segment Fault）。
               */
              
              p1.join(); // 等待生产者 1 结束
              p2.join(); // 等待生产者 2 结束
              c1.join(); // 等待消费者结束
       }
       

       std::cout<<"123";
       return 0;
}