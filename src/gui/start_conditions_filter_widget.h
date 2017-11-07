#ifndef START_CONDITIONS_FILTER_WIDGET_H
#define START_CONDITIONS_FILTER_WIDGET_H

#include "src/core/types_info.h"
#include <QGroupBox>
#include <QComboBox>
#include <QVBoxLayout>

using namespace Core;

/*!
 \brief Класс виджета, предаставляющего дополнительные настройки для начальных условий фильтра
*/
class StartConditionsFilterWidget : public QGroupBox
{
    Q_OBJECT
public:
    //! \brief Конструктор.
    explicit StartConditionsFilterWidget(QWidget *parent = nullptr);

    //! \brief Деструктор.
    ~StartConditionsFilterWidget();

    //! \brief Возвращает способ задания начальных данных для фильтра в данный момент.
    INITIAL_CONDITIONS initialConditionForFilter();

private:
    /*!
     \brief Загружает шрифты и устанавливает их параметры (начертание, размер и т.д.).
     \see FontManager.
    */
    void loadFonts();

    //! \brief Инициализирует управляющие элементы и связывает их сигналы с нужными слотами.
    void initControls();

    //! \brief Устанавливает расположение всех элементов на виджете.
    void initLayouts();

private:
    QComboBox *m_InitialConditionsFilter;
};

#endif // START_CONDITIONS_FILTER_WIDGET_H
