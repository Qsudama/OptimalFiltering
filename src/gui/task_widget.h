#ifndef TASK_WIDGET_H
#define TASK_WIDGET_H

#include "src/gui/task_parameters_widget.h"
#include "src/tasks/tasks_factory.h"
#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>


class TaskWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit TaskWidget(QWidget *parent = nullptr);

    Core::PtrTask task(Core::FILTER_TYPE ftype, Core::APPROX_TYPE atype);
    Tasks::TASK_ID id() const;
    const QString  name() const;


signals:
    void changed();


private slots:
    void onBtnParametersClicked();
    void onCbTaskChanged(int);


private:
    void loadFonts();
    void initControls();
    void initLayouts();


private:
    QComboBox *           m_cbTask;
    QPushButton *         m_btnParameters;
    TaskParametersWidget *m_parametersWidget;
};


#endif // TASK_WIDGET_H
