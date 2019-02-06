#include "timer_manager.h"

#include <QDebug>
#include <QSysInfo>

using namespace std;

static QString LOG_HEADER = "TimerManager: ";

static QString TITLE_TIME_MODELS = "Время моделирования: ";
static QString TITLE_TIME_PAUSE = "Время пауз: ";
static QString TITLE_TIME_RESULT = "Результативное время: ";

static QString TIME_UNITS = "сек.";

clock_t start_time, end_time, start_pause, end_pause;
double sum_pause;

TimerManager::TimerManager (){}
TimerManager::~TimerManager() {}

void TimerManager::start_timer()
{
    sum_pause = 0.0;
    result_execute_time = 0.0;
    start_time = clock();
}

void TimerManager::stop_timer()
{
    end_time = clock();
    double duration_sec = this->calculate_per_sek_for_times(start_time, end_time);
    result_execute_time = duration_sec - sum_pause;
    qDebug() <<  LOG_HEADER << TITLE_TIME_RESULT << result_execute_time;
//    test_log_core_name();
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

string TimerManager::execute_time_string()
{
    return " " + to_string(result_execute_time) + " c";
}

void TimerManager::test_log_core_name()
{
    QSysInfo *info = new QSysInfo();
    qDebug() <<  LOG_HEADER << "buildAbi" << info->buildAbi();
    qDebug() <<  LOG_HEADER << "buildCpuArchitecture" << info->buildCpuArchitecture();
    qDebug() <<  LOG_HEADER << "currentCpuArchitecture" << info->currentCpuArchitecture();
    qDebug() <<  LOG_HEADER << "kernelType" << info->kernelType();
    qDebug() <<  LOG_HEADER << "kernelVersion" << info->kernelVersion();
    qDebug() <<  LOG_HEADER << "machineHostName" << info->machineHostName();
    qDebug() <<  LOG_HEADER << "prettyProductName" << info->prettyProductName();
    qDebug() <<  LOG_HEADER << "productType" << info->productType();
    qDebug() <<  LOG_HEADER << "productVersion" << info->productVersion();
}

double TimerManager::calculate_per_sek_for_times(clock_t start_time, clock_t end_time)
{
    return double(end_time - start_time) / CLOCKS_PER_SEC;
}
