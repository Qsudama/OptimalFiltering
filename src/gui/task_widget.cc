#include "task_widget.h"
#include "font_manager.h"
#include <QApplication>
#include <QDesktopWidget>


TaskWidget::TaskWidget(QWidget *parent)
    : QGroupBox(parent)
    , m_parametersWidget(nullptr)
{
    setTitle(tr("Решаемая задача"));
    loadFonts();
    initControls();
    initLayouts();
    onCbTaskChanged(0);
}

void TaskWidget::loadFonts()
{
    this->setFont(FontManager::instance().regular(GuiConfig::FONT_SIZE_NORMAL));
}

void TaskWidget::initControls()
{
    m_cbTask = new QComboBox;
    // Нужно отслеживать соответсвие названия задачи и сопостовляемого таска в TaskWidget::id()
    m_cbTask->addItem(tr("3-мерный спуск ЛА на планету (линеар)"));                              // 0
    m_cbTask->addItem(tr("3-мерный спуск ЛА на планету (гаусс)"));                               // 1
    m_cbTask->addItem(tr("Осциллятор Ван-дер-Поля (линеар)"));                                   // 2
    m_cbTask->addItem(tr("Осциллятор Ван-дер-Поля (гаусс)"));                                    // 3
    m_cbTask->addItem(tr("Скалярный пример (линеар)"));                                          // 4
    m_cbTask->addItem(tr("Скалярный пример (гаусс)"));                                           // 5
    m_cbTask->addItem(tr("Дискретный осциллятор Ван-дер-Поля (линеар)"));                        // 6
    m_cbTask->addItem(tr("Дискретный осциллятор Ван-дер-Поля (гаусс)"));                         // 7
    m_cbTask->addItem(tr("Линейный скалярный пример с выбросами измерений"));                    // 8
    m_cbTask->addItem(tr("3-мерный спуск ЛА со сбоями 2-х датчиков (линеар)"));                  // 9
    m_cbTask->addItem(tr("6-мерный спуск ЛА со сбоями 2-х датчиков (линеар)"));                  // 10
    m_cbTask->addItem(tr("Осциллятор Ван-дер-Поля с выбросами измерений"));                      // 11
    m_cbTask->setCurrentIndex(0);
    connect(m_cbTask, SIGNAL(currentIndexChanged(int)), this, SLOT(onCbTaskChanged(int)));

    m_btnParameters = new QPushButton(tr("Показать"));
    connect(m_btnParameters, SIGNAL(clicked()), this, SLOT(onBtnParametersClicked()));
}

void TaskWidget::initLayouts()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(GuiConfig::LAYOUT_MARGIN_BIG);
    mainLayout->setSpacing(GuiConfig::LAYOUT_SPACING_BIG);

    mainLayout->addWidget(m_cbTask);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(GuiConfig::LAYOUT_MARGIN_SMALL);
    layout->setSpacing(GuiConfig::LAYOUT_SPACING_SMALL);
    QLabel *lbl = new QLabel(tr("Параметры задачи"));
    lbl->setMinimumWidth(QFontMetrics(this->font()).width(lbl->text()));
    layout->addWidget(lbl);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
    m_btnParameters->setMinimumWidth(
        QFontMetrics(this->font()).width("    " + m_btnParameters->text()));
    layout->addWidget(m_btnParameters);

    mainLayout->addLayout(layout);

    this->setLayout(mainLayout);
}

void TaskWidget::onBtnParametersClicked()
{
    setupParametersWindow();

    if (m_parametersWidget->isHidden()) {
        m_parametersWidget->show();
    }
}

void TaskWidget::setupParametersWindow() {
    TASK_ID taskId = id();
    Core::PtrTask  tmpTask;
    if (taskId == TASK_ID::LDScalarRejectionGauss || taskId == TASK_ID::LDLandingRejection3DLinear || taskId == TASK_ID::LDLandingRejection6DLinear || taskId == TASK_ID::LDVanDerPolRejectionLinear) {
        tmpTask = Tasks::TaskFactory::create(Core::FILTER_TYPE::LogicDynamic, taskId);
    } else if(taskId == TASK_ID::ScalarLinear || taskId == TASK_ID::ScalarGauss) {
        tmpTask = Tasks::TaskFactory::create(Core::FILTER_TYPE::Discrete, taskId);
    } else if (taskId == TASK_ID::ScalarGauss) {
        tmpTask = Tasks::TaskFactory::create(Core::FILTER_TYPE::Discrete, taskId);
    } else if (taskId == TASK_ID::DVanDerPolLinear || taskId == TASK_ID::DVanDerPolGauss) {
        tmpTask = Tasks::TaskFactory::create(Core::FILTER_TYPE::Discrete, taskId);
    } else {
        tmpTask = Tasks::TaskFactory::create(Core::FILTER_TYPE::Continuous, taskId);
    }

    bool hidden = true;
    if (m_parametersWidget) {
        if (!m_parametersWidget->isHidden()) {
            hidden = false;
            m_parametersWidget->hide();
        }
        delete m_parametersWidget;
    }

    m_parametersWidget = new TaskParametersWidget(tmpTask);
    m_parametersWidget->setWindowTitle(tr("Параметры задачи"));
    QRect screenRect = QApplication::desktop()->availableGeometry();
    int   x          = screenRect.width() / 2 - 832 / 2;
    int   y          = 193; // this is magic! :D
    int   w          = 832;
    int   h          = screenRect.height() - y - 65;
    m_parametersWidget->setGeometry(x, y, w, h);
    m_parametersWidget->setHidden(hidden);

    emit changed();
}

void TaskWidget::onCbTaskChanged(int)
{
//    if (m_parametersWidget) {
//        if (!m_parametersWidget->isHidden()) {
//            m_parametersWidget->hide();
//        }
//        delete m_parametersWidget;
//        m_parametersWidget = nullptr;
//    }
}

Core::PtrTask TaskWidget::task(Core::FILTER_TYPE ftype)
{
    Core::PtrTask tmpTask = Tasks::TaskFactory::create(ftype, id());
    if (!m_parametersWidget) {
        setupParametersWindow();
    }
    m_parametersWidget->loadParamsTo(tmpTask);
    return tmpTask;
}

bool TaskWidget::taskIsNull(Core::FILTER_TYPE ftype)
{
    Core::PtrTask tmpTask = Tasks::TaskFactory::create(ftype, id());
    if (tmpTask == nullptr) {
        return false;
    }
    return true;
}


TASK_ID TaskWidget::id() const
{
    switch (m_cbTask->currentIndex()) {
    case 0:
        return TASK_ID::LandingLinear;
    case 1:
        return TASK_ID::LandingGauss;
    case 2:
        return TASK_ID::VanDerPolLinear;
    case 3:
        return TASK_ID::VanDerPolGauss;
    case 4:
        return TASK_ID::ScalarLinear;
    case 5:
        return TASK_ID::ScalarGauss;
    case 6:
        return TASK_ID::DVanDerPolLinear;
    case 7:
        return TASK_ID::DVanDerPolGauss;
    case 8:
        return TASK_ID::LDScalarRejectionGauss;
    case 9:
        return TASK_ID::LDLandingRejection3DLinear;
    case 10:
        return TASK_ID::LDLandingRejection6DLinear;
    case 11:
        return TASK_ID::LDVanDerPolRejectionLinear;
    default:
        return TASK_ID::LandingLinear;
    }
}

const QString TaskWidget::name() const
{
    return m_cbTask->currentText();
}
