#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H

#include <time.h> // includes clock_t and CLOCKS_PER_SEC
#include <iostream>

using namespace std;
//! \brief Структура хранящая в себе всю инфрормацию о времени выполнения конкретного фильтра.
struct FilterTimeResult
{
    //! \brief Имя фильтра.
    string name;
    //! \brief Все время работы фильтра (в миллисекундах).
    double all_time;
    //! \brief Суммарное время пауз (в миллисекундах).
    double pause_time;
    //! \brief Результативное время: all_time - pause_time (в миллисекундах).
    double result_time;
};

class TimerManager
{
public:
    /*!
    \brief Instance метод синглтона.
    */
    static TimerManager& Instance()
    {
        static TimerManager manager;
        return manager;
    }

    //! \brief Функция старта подсчета времени. Обнуляет все необходимые переменные.
    void start_timer();

    /*!
     * \brief Функция остановки подсчета времени.
     * Автоматически подсчитывает результат и записывает его в переменную result_execute_time.
    */
    void stop_timer();

    //! \brief Функция старта паузы таймера.
    void interrupt_timer();

    //! \brief Функция остановки паузы таймера.
    void continue_timer();

    //! \brief Функция конвертирования секунд в миллисекунды
    double convert_sek_to_msek(double time);

    /*!
    \brief Возвращает структуру времени выполнения фильтра.

    \param filter_name - имя фильтра.
    */
    FilterTimeResult result_execute_time(string filter_name, double coeff = 1.0);

private:

    //! \brief Конструктор.
    TimerManager();

    //! \brief Деструктор.
    ~TimerManager();

    //! \brief Переопределение конструктора = так как работаем с синглтоном.
    TimerManager(const TimerManager&) = delete;

    //! \brief Переопределение оператора = так как работаем с синглтоном.
    TimerManager& operator= (TimerManager&) = delete;

    //! \brief Расчитывает результативное время выполнения из переменных времени в double.
    double calculate_per_sek_for_times(clock_t start_time, clock_t end_time);
};

#endif // TIMER_MANAGER_H
