#ifndef TIMER_RESULTS_TABLE_H
#define TIMER_RESULTS_TABLE_H

#include "src/gui/gui_config.h"
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
    TimerResultsTable(const QVector<std::string> &labels,
                       QWidget *parent = nullptr);

private slots:
//    void onSaveTable();


private:
//    //! \brief Создает таблицу и заполняет ее из data.
//    void initTable(const Core::FilterOutput &data, const Math::Vector &scale);

//    //! \brief Инициализирует меню.
//    void initMenu();

//    //! \brief Форматирует текст и записывает в out.
//    void writeToFile(QTextStream &out);


private:
    QTableWidget *m_table;

};

#endif // TIMER_RESULTS_TABLE_H
