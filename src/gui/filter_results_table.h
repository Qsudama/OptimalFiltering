#ifndef FILTERRESULTSTABLE_H
#define FILTERRESULTSTABLE_H

#include "src/core/filter_output.h"
#include "src/gui/gui_config.h"
#include <QTableWidget>
#include <QWidget>


/*!
 * \brief Виджет для отображения таблиц.
 */

class FilterResultsTable : public QWidget
{
    Q_OBJECT

public:
    //! \brief Конструктор.
    FilterResultsTable(const Core::FilterOutput &data, const std::string &label, const Math::Vector &scale,
                       QWidget *parent = nullptr);


private:
    //! \brief Создает таблицу и заполняет ее из data.
    void initTable(const Core::FilterOutput &data, const Math::Vector &scale);


private:
    QTableWidget *m_table;
};

#endif // FILTERRESULTSTABLE_H
