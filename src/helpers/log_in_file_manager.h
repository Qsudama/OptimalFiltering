#ifndef LOG_IN_FILE_MANAGER_H
#define LOG_IN_FILE_MANAGER_H

#include <time.h>
#include <string>
#include "src/math/math.h"
#include "src/math/matrix.h"
#include "src/array.h"

using Math::Vector;
using Math::Matrix;

/*!
 \brief Модуль, логирующий что-то либо в файл формата txt.

 Для использования, файл инициализируется в main.m, а затем из любого места в коде вызываются функции с параметрами для логирования.
*/

class LogInFileManager
{
public:
    /*!
    \brief Instance метод синглтона.
    */
    static LogInFileManager& Instance()
    {
        static LogInFileManager manager;
        return manager;
    }

    //! \brief Инициализирует все необходимое для логирования в файл.
    void initLog();

    /*!
    \brief Пишет в файл строку.

    \param str - строка которую нужно залогировать.
    */
    void logInFileString(std::string str);


    /*!
    \brief Пишет в файл значение вектора в виде столбца.

    \param vector - вектор.
    \param name - строка.
    \param str - строка.
    */
    void logInFileVector(Vector vector, std::string str);
    void logInFileVector(Vector vector, std::string name, std::string str);
    void logInFileArrayVectors(Array<Array<Vector>> array, int number_i, std::string name, std::string str);

//    void logInFileMatrix(Matrix matrix, std::string str);

private:

    //! \brief Конструктор.
    LogInFileManager();

    //! \brief Деструктор.
    ~LogInFileManager();

    //! \brief Переопределение конструктора = так как работаем с синглтоном.
    LogInFileManager(const LogInFileManager&) = delete;

    //! \brief Переопределение оператора = так как работаем с синглтоном.
    LogInFileManager& operator= (LogInFileManager&) = delete;

    //! \brief Создает logFileName.
    void SettingLogFileNameString();

    //! \brief Строка содержащая путь к файлу лога.
    std::string logFileName;
};

#endif // LOG_IN_FILE_MANAGER_H
