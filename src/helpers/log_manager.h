#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <time.h>
#include <string>

class LogManager
{
public:
    static LogManager& Instance()
    {
        static LogManager manager;
        return manager;
    }

    void initLog();

    void logInFileString(std::string str);

private:
    LogManager() {};
    ~LogManager() {};

    LogManager(const LogManager&) = delete;
    LogManager& operator= (LogManager&) = delete;

    void SettingLogFileNameString();

    std::string logFileName;
};

#endif // LOG_MANAGER_H
