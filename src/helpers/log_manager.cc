#include "log_manager.h"

#include <QDebug>

#include <iostream>
#include <fstream>

using namespace std;


static string LOCAL_PATH_FILE_MACOS = "//Users//eos//Desktop//";
static string LOCAL_PATH_FILE_LINUX = "~\\Desctop\\";
static string LOCAL_PATH_FILE_WIN = "~\\Desctop\\"; //Нужно указывать руками
static string NAME_SYFIX_FILE = "Log_";
static string FORMAT_FILE = ".txt";

string localTimeForFileName() {
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char const *format = "%d_%b_%Y_%I.%M.%S";
    strftime(buffer, 80, format, timeinfo);

    string result = buffer;
    return result;
}

void LogManager::SettingLogFileNameString() {
    string pathFile;
#ifdef Q_OS_MAC
    pathFile = LOCAL_PATH_FILE_MACOS;
#endif
#ifdef Q_OS_WIN
    pathFile = LOCAL_PATH_FILE_WIN;
#endif
#ifdef Q_OS_LINUX
    pathFile = LOCAL_PATH_FILE_LINUX;
#endif
    this->logFileName = pathFile + NAME_SYFIX_FILE + localTimeForFileName() + FORMAT_FILE;
    qDebug() << "Path for log file = " << QString::fromStdString(this->logFileName);
}

void LogManager::initLog()
{
    SettingLogFileNameString();

    ofstream stream;
    stream.open(this->logFileName, ios::out);
    stream << "Log started\n";
    stream.close();
}

void LogManager::logInFileString(string str)
{
    ofstream stream;
    stream.open(this->logFileName, ios::app);
    stream << str << "\n";
    stream.close();
}

