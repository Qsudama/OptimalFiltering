#ifndef TASK_PARAMETERS_WIDGET_H
#define TASK_PARAMETERS_WIDGET_H

#include "src/core/task.h"
#include "src/gui/matrix_widget.h"
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QHideEvent>
#include <QVBoxLayout>
#include <QWidget>


class TaskParametersWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TaskParametersWidget(Core::PtrTask task, QWidget *parent = nullptr);

    void loadParamsTo(Core::PtrTask task);

private:
    MatrixWidget *m_meanX;
    MatrixWidget *m_meanV;
    MatrixWidget *m_meanW;
    MatrixWidget *m_varX;
    MatrixWidget *m_varV;
    MatrixWidget *m_varW;

    QVector<QDoubleSpinBox *> m_params;
    QVector<QDoubleSpinBox *> m_consts;

    void initMain(Core::PtrTask task, QVBoxLayout *mainLayout);
    void initParameters(Core::PtrTask task, QVBoxLayout *mainLayout);
    void initConstants(Core::PtrTask task, QVBoxLayout *mainLayout);

    QHBoxLayout *createMeanVarLayout(MatrixWidget *left, MatrixWidget *right);
};


#endif // TASK_PARAMETERS_WIDGET_H
