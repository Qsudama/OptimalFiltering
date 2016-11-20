#ifndef FILTERRESULTSTABLE_H
#define FILTERRESULTSTABLE_H

#include "src/core/filter_output.h"
#include "src/gui/gui_config.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QTextStream>


/*!
 * \brief Виджет для отображения таблиц.
 */

class FilterResultsTable : public QMainWindow
{
    Q_OBJECT

public:
    //! \brief Конструктор.
    FilterResultsTable(const Core::FilterOutput &data, const std::string &label, const Math::Vector &scale,
                       QWidget *parent = nullptr);


private slots:
    void onSaveTable();


private:
    //! \brief Создает таблицу и заполняет ее из data.
    void initTable(const Core::FilterOutput &data, const Math::Vector &scale);

    //! \brief Инициализирует меню.
    void initMenu();

    //! \brief Форматирует текст и записывает в out.
    void writeToFile(QTextStream &out);


private:
    QTableWidget *m_table;
};

#endif // FILTERRESULTSTABLE_H
