#ifndef LOG_IN_CONSOLE_MANAGER_H
#define LOG_IN_CONSOLE_MANAGER_H

#include <QDebug>

#include <string>
#include "QString"

#include "src/math/math.h"
#include "src/math/matrix.h"
#include "src/array.h"

using Math::Vector;
using Math::Matrix;

/*!
 \brief Модуль, логирующий что-то либо консоль.
*/

class LogInConsoleManager
{

public:
    /*!
    \brief Instance метод синглтона.
    */
    static LogInConsoleManager& Instance()
    {
        static LogInConsoleManager manager;
        return manager;
    }

    void logQStringWithQDebug(QString str);
    void logStringWithQDebug(std::string str);

    void logDoubleValueWithQDebug(double value);
    void logSizeTValueWithQDebug(size_t value);
    // Да мне известно про существование template. Но это г...но не захотело линковаться!
//    template <class T> void logNumericValueWithQDebug(T value);

    void logArrayWithQDebug(Array<int> array);

private:

    //! \brief Конструктор.
    LogInConsoleManager();

    //! \brief Деструктор.
    ~LogInConsoleManager();

    //! \brief Переопределение конструктора = так как работаем с синглтоном.
    LogInConsoleManager(const LogInConsoleManager&) = delete;

    //! \brief Переопределение оператора = так как работаем с синглтоном.
    LogInConsoleManager& operator= (LogInConsoleManager&) = delete;
};

#endif // LOG_IN_CONSOLE_MANAGER_H
