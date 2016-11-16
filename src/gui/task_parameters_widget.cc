#include "task_parameters_widget.h"
#include "src/gui/font_manager.h"
#include <QLabel>
#include <QScrollArea>


TaskParametersWidget::TaskParametersWidget(Core::PtrTask task, QWidget *parent)
    : QWidget(parent)
{
    this->setFont(FontManager::instance().regular(9));
    this->setWindowTitle(tr("Параметры задачи"));

    QWidget *    widget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(5);
    layout->setSpacing(5);

    initMain(task, layout);
    initParameters(task, layout);
    initConstants(task, layout);

    widget->setLayout(layout);
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(widget);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(scrollArea);
    this->setLayout(mainLayout);
}

void TaskParametersWidget::loadParamsTo(Core::PtrTask task)
{
    task->setMeanX0(m_meanX->matrix());
    task->setMeanV(m_meanV->matrix());
    task->setMeanW(m_meanW->matrix());
    task->setVarX0(m_varX->matrix());
    task->setVarV(m_varV->matrix());
    task->setVarW(m_varW->matrix());

    // WARNING: как-то криво написано...
    int i = 0;
    for (auto &p : *(task->params().get())) {
        task->changeParameter(p.first, m_params[i]->value());
        ++i;
    }
}

QHBoxLayout *TaskParametersWidget::createMeanVarLayout(MatrixWidget *left, MatrixWidget *right)
{
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(5);
    layout->setSpacing(5);
    layout->addWidget(new QLabel("M ="));
    layout->addWidget(left);
    layout->addWidget(new QLabel("    D ="));
    layout->addWidget(right);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));

    return layout;
}

void TaskParametersWidget::initMain(Core::PtrTask task, QVBoxLayout *mainLayout)
{
    m_meanX = new MatrixWidget(task->meanX0());
    m_meanV = new MatrixWidget(task->meanV());
    m_meanW = new MatrixWidget(task->meanW());
    m_varX  = new MatrixWidget(task->varX0(), true, true);
    m_varV  = new MatrixWidget(task->varV(), true, true);
    m_varW  = new MatrixWidget(task->varW(), true, true);

    mainLayout->addWidget(new QLabel(tr("Моментные характеристики объекта X:")));
    mainLayout->addLayout(createMeanVarLayout(m_meanX, m_varX));
    mainLayout->addWidget(new QLabel(""));
    mainLayout->addWidget(new QLabel(tr("Моментные характеристики шума V:")));
    mainLayout->addLayout(createMeanVarLayout(m_meanV, m_varV));
    mainLayout->addWidget(new QLabel(""));
    mainLayout->addWidget(new QLabel(tr("Моментные характеристики шума W:")));
    mainLayout->addLayout(createMeanVarLayout(m_meanW, m_varW));
}

void TaskParametersWidget::initParameters(Core::PtrTask task, QVBoxLayout *mainLayout)
{
    m_params.clear();

    if (task->params()->size() == 0) {
        return;
    }

    mainLayout->addWidget(new QLabel(""));
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(new QLabel("Специфические параметры:"));

    for (auto &p : *(task->params().get())) {
        QDoubleSpinBox *dsb = new QDoubleSpinBox;
        dsb->setDecimals(6);
        dsb->setMinimum(-9999999.0);
        dsb->setMaximum(9999999.0);
        dsb->setSingleStep(0.01);
        dsb->setValue(p.second);
        dsb->setAlignment(Qt::AlignRight);
        dsb->setButtonSymbols(QDoubleSpinBox::NoButtons);
        dsb->setFont(FontManager::instance().mono(9));
        m_params.push_back(dsb);

        QHBoxLayout *layout = new QHBoxLayout;
        layout->setMargin(5);
        layout->setSpacing(5);
        QLabel *lbl = new QLabel(QString::fromStdString(p.first));
        lbl->setMinimumWidth(45);
        layout->addWidget(lbl);
        layout->addWidget(new QLabel(" = "));
        layout->addWidget(m_params.last());
        layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
        mainLayout->addLayout(layout);
    }
}

void TaskParametersWidget::initConstants(Core::PtrTask task, QVBoxLayout *mainLayout)
{
    if (task->consts()->size() == 0) {
        return;
    }

    mainLayout->addWidget(new QLabel(""));
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(new QLabel("Специфические константы:"));

    for (auto &p : *(task->consts().get())) {
        QHBoxLayout *layout = new QHBoxLayout;
        layout->setMargin(5);
        layout->setSpacing(5);
        QLabel *lbl = new QLabel(QString::fromStdString(p.first));
        lbl->setMinimumWidth(45);
        layout->addWidget(lbl);
        layout->addWidget(new QLabel(" = "));
        layout->addWidget(new QLabel(QString::number(p.second)));
        layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
        mainLayout->addLayout(layout);
    }
}
