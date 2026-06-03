#include <fstream>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>
#include <mutex>
#include <ctime>
#include <sstream>
#include <thread>
#include <fstream>
#include <filesystem> 

enum LogLevel{
       DEBUG = 0,
       INFO,
       WARNING,
       ERROR
};

class Logger
{
public:
       static Logger* GetInstance()
       {
              static Logger Instance;
              return &Instance;
       }

       void SetLogFile(const std::string& filename) 
       {
              std::lock_guard<std::mutex> lock(mtx);
              
              try {
                     // 1. 获取文件所属的目录路径
                     std::filesystem::path filePath(filename);
                     std::filesystem::path parentPath = filePath.parent_path();

                     // 2. 如果目录路径不为空且不存在，则递归创建目录
                     if (!parentPath.empty() && !std::filesystem::exists(parentPath)) 
                     {
                            std::filesystem::create_directories(parentPath);
                     }

                     // 3. 打开文件（ios::app 模式下，如果文件不存在会自动创建文件）
                     if (fileStream.is_open()) {
                            fileStream.close();
                     }
                     
                     fileStream.open(filename, std::ios::app);
                     
                     if (!fileStream) {
                            std::cerr << "无法创建或打开日志文件: " << filename << std::endl;
                     }
              } catch (const std::filesystem::filesystem_error& e) {
                     std::cerr << "文件系统错误: " << e.what() << std::endl;
              }
       }

       void Log(LogLevel level, const std::string& file, int line, const std::string& message) 
       {
        std::lock_guard<std::mutex> lock(mtx);
        std::stringstream logLine;
        logLine << "[" << GetCurrentTime() << "] "
                  << "[" << LevelToString(level) << "] "
                  << "[" << std::this_thread::get_id() << "] "
                  << "[" << file << ":" << line << "] "
                  << message << std::endl;
       std::string finalLog = logLine.str();
       std::cout << finalLog;
       if (fileStream.is_open()) 
       {
            fileStream << finalLog << std::endl;
            fileStream.flush(); 
        }
    }

protected:
       std::string LevelToString(const LogLevel& level)
       {
              switch(level)
              {
                     case LogLevel::DEBUG: return "DEBUG" ;
                     case LogLevel::INFO: return "INFO" ;
                     case LogLevel::WARNING: return "WARNING" ;
                     case LogLevel::ERROR: return "ERROR" ;
                     default :  return "UNKNOWN";
              }
       }

       std::string GetCurrentTime()
       {
              auto now = std::chrono::system_clock::now();
              auto in_time_t = std::chrono::system_clock::to_time_t(now);
              std::stringstream ss;
              ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
              return ss.str();
       }
       
private:
       Logger() = default;
       ~Logger(){};

private:
       std::mutex mtx;
       std::fstream fileStream;
};

class LogMessage {
public:
    LogMessage(LogLevel level, const char* file, int line) 
        : Level(level), File(file), Line(line) {}

    ~LogMessage() {
        Logger::GetInstance()->Log(Level, File, Line, Oss.str());
    }

    template <typename T>
    LogMessage& operator << (const T& msg)
    {
       Oss << msg;
       return *this;
    }

private:
    std::string File;
    int Line;
    LogLevel Level;
    std::ostringstream Oss;
};

#define LOG_DEBUG LogMessage(LogLevel::DEBUG, __FILE__, __LINE__)
#define LOG_INFO  LogMessage(LogLevel::INFO, __FILE__, __LINE__)
#define LOG_WARN  LogMessage(LogLevel::WARN, __FILE__, __LINE__)
#define LOG_ERROR LogMessage(LogLevel::ERROR, __FILE__, __LINE__)
