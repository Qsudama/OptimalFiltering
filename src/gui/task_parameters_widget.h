#ifndef TASK_PARAMETERS_WIDGET_H
#define TASK_PARAMETERS_WIDGET_H

#include "src/core/task.h"
#include "src/gui/gui_config.h"
#include "src/gui/matrix_widget.h"
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QHideEvent>
#include <QVBoxLayout>
#include <QWidget>


/*!
 \brief Класс виджета, предаставляющего возможность работы с параметрами задач.

 \see Core::TaskParameters
*/

class TaskParametersWidget : public QWidget
{
    Q_OBJECT

public:
    //! \brief Конструктор.
    explicit TaskParametersWidget(Core::PtrTask task, QWidget *parent = nullptr);

    //! \brief Записывает измененные параметры в task.
    void loadParamsTo(Core::PtrTask task);


private:
    //! \brief Инициализирует элементы, соответствующие общим для всех задач параметрам.
    void initMain(Core::PtrTask task, QVBoxLayout *mainLayout);

    //! \brief Инициализирует элементы для работы со специфическими параметрами (если таковые имеются).
    void initParameters(Core::PtrTask task, QVBoxLayout *mainLayout);

    //! \brief Инициализирует элементы для отображения специфических константами (если таковые имеются).
    void initConstants(Core::PtrTask task, QVBoxLayout *mainLayout);

    QHBoxLayout *createMeanVarLayout(MatrixWidget *left, MatrixWidget *right);


private:
    MatrixWidget *m_meanX;
    MatrixWidget *m_meanV;
    MatrixWidget *m_meanW;
    MatrixWidget *m_varX;
    MatrixWidget *m_varV;
    MatrixWidget *m_varW;

    QVector<QDoubleSpinBox *> m_params;
    QVector<QDoubleSpinBox *> m_consts;
};


#endif // TASK_PARAMETERS_WIDGET_H
