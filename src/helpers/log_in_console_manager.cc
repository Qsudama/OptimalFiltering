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

void LogInConsoleManager::logDoubleValueWithQDebug(double value)
{
    qDebug() <<  LOG_HEADER << QString::number(value);
}

void LogInConsoleManager::logSizeTValueWithQDebug(size_t value)
{
    qDebug() <<  LOG_HEADER << QString::number(value);
}
//template <class T>
//void LogInConsoleManager::logNumericValueWithQDebug(T value)
//{
//    qDebug() <<  LOG_HEADER << QString::number(value);
//}

void LogInConsoleManager::logArrayWithQDebug(Array<int> array)
{
    QString str;
    for (int i = 0; i < int(array.size()); ++i) {
        if (i > 0) {
            str += " ";
        }
        str += QString::number(array[i]);
    }

    this->logQStringWithQDebug(str);
}
