#ifndef TIMER_RESULTS_TABLE_H
#define TIMER_RESULTS_TABLE_H

#include "src/gui/gui_config.h"
#include "src/helpers/timer_manager.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QTextStream>


/*!
 * \brief Виджет для отображения таблиц.
 */

class TimerResultsTable : public QMainWindow
{
    Q_OBJECT

public:
    //! \brief Конструктор.
    TimerResultsTable(const QVector<FilterTimeResult> filters_results,
                       QWidget *parent = nullptr);

private slots:
//    void onSaveTable();


private:
//    //! \brief Создает таблицу и заполняет ее из filters_results.
    void initTable(QVector<FilterTimeResult> filters_results);


private:
    QTableWidget *m_table;

};

#endif // TIMER_RESULTS_TABLE_H
