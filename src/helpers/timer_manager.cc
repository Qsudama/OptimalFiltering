#include "timer_manager.h"

#include <QDebug>
#include <QSysInfo>

#include <iostream>

using namespace std;

static QString LOG_HEADER = "TimerManager: ";

static QString TITLE_TIME_MODELS = "Время моделирования: ";
static QString TITLE_TIME_PAUSE = "Время пауз: ";
static QString TITLE_TIME_RESULT = "Результативное время: ";

static QString TIME_UNITS = "сек.";

clock_t start_time, end_time, start_pause, end_pause;
double all_executive_time, sum_pause, result_time;

TimerManager::TimerManager (){}
TimerManager::~TimerManager() {}

void TimerManager::start_timer()
{
    sum_pause = 0.0;
    all_executive_time = 0.0;
    result_time = 0.0;
    start_time = clock();
}

void TimerManager::stop_timer()
{
    end_time = clock();
    all_executive_time = this->calculate_per_sek_for_times(start_time, end_time);
    result_time = all_executive_time - sum_pause;
//    result_execute_time = this->convert_sek_to_msek(result_execute);
    qDebug() <<  LOG_HEADER
                 + TITLE_TIME_MODELS + QString::number(all_executive_time) + "   "
                 + TITLE_TIME_PAUSE + QString::number(sum_pause) + "   "
                 + TITLE_TIME_RESULT + QString::number(result_time);
}

void TimerManager::interrupt_timer()
{
    start_pause = clock();
}

void TimerManager::continue_timer()
{
    end_pause = clock();
    double duration_pause_sec = this->calculate_per_sek_for_times(start_pause, end_pause);
    sum_pause += duration_pause_sec;
}

double TimerManager::convert_sek_to_msek(double time)
{
    return time * 1000;
}

double TimerManager::calculate_per_sek_for_times(clock_t start_time, clock_t end_time)
{
    return double(end_time - start_time) / CLOCKS_PER_SEC;
}

FilterTimeResult TimerManager::result_execute_time(string filter_name, double coeff)
{
    FilterTimeResult filter_time;
    filter_time.name = filter_name;
    filter_time.all_time = this->convert_sek_to_msek(all_executive_time) / coeff;
    filter_time.pause_time = this->convert_sek_to_msek(sum_pause) / coeff;
    filter_time.result_time = this->convert_sek_to_msek(result_time) / coeff;
    return filter_time;
}
