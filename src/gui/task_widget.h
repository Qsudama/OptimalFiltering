#ifndef TASK_WIDGET_H
#define TASK_WIDGET_H

#include "src/gui/gui_config.h"
#include "src/gui/task_parameters_widget.h"
#include "src/tasks/tasks_factory.h"
#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>


/*!
 \brief Виджет для работы с задачами для фильтров.
*/

class TaskWidget : public QGroupBox
{
    Q_OBJECT

public:
    //! \brief Конструктор.
    explicit TaskWidget(QWidget *parent = nullptr);

    /*!
     \brief Создает экземпляр задачи.
     \param ftype - идентификатор типа фильтра (непрерывный, дискретный, ...).
     \return указатель на экземпляр задачи.
    */
    Core::PtrTask task(Core::FILTER_TYPE ftype);

    /*!
     \brief Функция для проверки существования задачи.
     \param ftype - идентификатор типа фильтра (непрерывный, дискретный, ...).
     \details true - экземпляр существует, иначе экземпляр не существует.
     \return bool.
    */
    bool taskIsNull(Core::FILTER_TYPE ftype);

    //! \brief Возвращает идентификатор выбранной в данный момент задачи.
    TASK_ID id() const;

    //! \brief Возвращает название выбранной в данный момент задачи.
    const QString name() const;


signals:
    //! \brief Сигнал. Сообщает о том, что пользователь изменил текущую задачу.
    void changed();


private slots:
    //! \brief Открывает окно работы с параметрами задачи.
    void onBtnParametersClicked();

    //! \brief Изменяет окно работы с параметрами, а так же испускает сигнал changed().
    void onCbTaskChanged(int);

public slots:
    void onFiltersFamilyChanged(Core::FILTER_TYPE index);

private:
    /*!
     \brief Настраивает окно параметров задачи.
    */
    void reloadParametersWidget(bool emitChanged);

    /*!
     \brief Загружает шрифты и устанавливает их параметры (начертание, размер и т.д.).
     \see FontManager.
    */
    void loadFonts();

    //! \brief Инициализирует управляющие элементы и связывает их сигналы с нужными слотами.
    void initControls();

    //! \brief Устанавливает расположение всех элементов на виджете.
    void initLayouts();

    bool initParametersWidget();

    Core::PtrTask tempTask(Core::FILTER_TYPE ftype);

private:
    QComboBox *           m_cbTask;
    QPushButton *         m_btnParameters;
    TaskParametersWidget *m_parametersWidget;
    Core::FILTER_TYPE     m_currentFiltersFamily = Core::Discrete;
};


#endif // TASK_WIDGET_H
