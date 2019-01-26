#include "log_in_console_manager.h"

#include <iostream>

using namespace std;

static QString LOG_HEADER = "LogInConsoleManager: ";

LogInConsoleManager::LogInConsoleManager () {}
LogInConsoleManager::~LogInConsoleManager() {}

void LogInConsoleManager::logQStringWithQDebug(QString str)
{
    qDebug() <<  LOG_HEADER << str;
}

void LogInConsoleManager::logStringWithQDebug(string str)
{
    qDebug() <<  LOG_HEADER << QString::fromStdString(str);
}

void LogInConsoleManager::logArrayWithQDebug(Array<int> array)
{
    QString str;
    for (int i = 0; i < array.size(); ++i) {
        if (i > 0) {
            str += " ";
        }
        str += QString::number(array[i]);
    }

    this->logQStringWithQDebug(str);
}
