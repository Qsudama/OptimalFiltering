#include "log_in_file_manager.h"

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

void LogInFileManager::SettingLogFileNameString() {
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

LogInFileManager::LogInFileManager () {}
LogInFileManager::~LogInFileManager() {}

void LogInFileManager::initLog()
{
    SettingLogFileNameString();

    ofstream stream;
    stream.open(this->logFileName, ios::out);
    stream << "Log started" << "\n";
    stream.close();
}

void LogInFileManager::logInFileString(string str)
{
    ofstream stream;
    stream.open(this->logFileName, ios::app);
    stream << str << "\n";
    stream.close();
}

void LogInFileManager::logInFileVector(Vector vector, string str)
{
    ofstream stream;
    stream.open(this->logFileName, ios::app);
    for (int i = 0; i < vector.rows(); i++) {
        stream << str << vector[i] << "\n";
    }
    stream.close();
}

void LogInFileManager::logInFileVector(Vector vector, string name, string str)
{
    ofstream stream;
    stream.open(this->logFileName, ios::app);
    stream << name + "\n";
    stream.close();
    logInFileVector(vector, str);
}

void LogInFileManager::logInFileArrayVectors(Array<Array<Vector>> array, int number_i, string name, string str)
{
    ofstream stream;
    stream.open(this->logFileName, ios::app);
    stream << name << "\n";
    int size = array.size();
    stream << name << str;
    for (int i = 0; i < size; i++) {
        stream << (i+1) << str << str;
    }
    stream << "\n";
    Vector first_v = array[0][0];
    for (int size_vector = 0; size_vector < first_v.rows(); size_vector++) {
        for (int i = 0; i < size; i++) {
            stream << array[i][number_i][size_vector] << str;
        }
    }
    stream.close();
}





//void LogInFileManager::logInFileMatrix(Matrix matrix, string str) {

//}



//Файл может быть открыт в одном из следующих режимов:

//ios::in — открыть файл в режиме чтения данных; режим является режимом по умолчанию для потоков ifstream;
//ios::out — открыть файл в режиме записи данных (при этом информация о существующем файле уничтожается); режим является режимом по умолчанию для потоков ofstream;
//ios::app — открыть файл в режиме записи данных в конец файла;
//ios::ate — передвинуться в конец уже открытого файла;
//ios::trunc — очистить файл, это же происходит в режиме ios::out;
//ios::nocreate — не выполнять операцию открытия файла, если он не существует;
//ios::noreplace — не открывать существующий файл.
