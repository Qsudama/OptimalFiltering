#include "start_conditions_filter_widget.h"
#include "src/gui/font_manager.h"
#include "src/gui/gui_config.h"



StartConditionsFilterWidget::StartConditionsFilterWidget(QWidget *parent)
    : QGroupBox(parent)
{
    setTitle(tr("Метод задания начальных условий"));
    initControls();
    initLayouts();
    loadFonts();
}

StartConditionsFilterWidget::~StartConditionsFilterWidget()
{
}

void StartConditionsFilterWidget::loadFonts()
{
    this->setFont(FontManager::instance().regular(GuiConfig::FONT_SIZE_NORMAL));
}

void StartConditionsFilterWidget::initControls()
{
    m_InitialConditionsFilter = new QComboBox;
    m_InitialConditionsFilter->addItem(tr("Метод гауссовского приближения"));
    m_InitialConditionsFilter->addItem(tr("Метод Монте – Карло"));
    m_InitialConditionsFilter->setCurrentIndex(0);
}

void StartConditionsFilterWidget::initLayouts()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(GuiConfig::LAYOUT_MARGIN_BIG);
    mainLayout->setSpacing(GuiConfig::LAYOUT_SPACING_BIG);

    mainLayout->addWidget(m_InitialConditionsFilter);

    this->setLayout(mainLayout);
}

INITIAL_CONDITIONS StartConditionsFilterWidget::initialConditionForFilter()
{
    int index = m_InitialConditionsFilter->currentIndex();
    switch (index) {
    case 0:
            return INITIAL_CONDITIONS::GaussApproximation;
    case 1:
            return INITIAL_CONDITIONS::MonteCarlo;
    }
    return INITIAL_CONDITIONS::GaussApproximation;
}
