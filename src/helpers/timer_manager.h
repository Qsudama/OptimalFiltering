#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H

#include <time.h> // includes clock_t and CLOCKS_PER_SEC

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

    void start_timer();
    void stop_timer();

    void interrupt_timer();
    void continue_timer();

    double result_execute_time;

private:

    //! \brief Конструктор.
    TimerManager();

    //! \brief Деструктор.
    ~TimerManager();

    //! \brief Переопределение конструктора = так как работаем с синглтоном.
    TimerManager(const TimerManager&) = delete;

    //! \brief Переопределение оператора = так как работаем с синглтоном.
    TimerManager& operator= (TimerManager&) = delete;

    double calculate_per_sek_for_times(clock_t start_time, clock_t end_time);

    void test_log_core_name();
};

#endif // TIMER_MANAGER_H
