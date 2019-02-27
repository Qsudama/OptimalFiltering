#include "filter_start_buttons_box.h"
#include "src/gui/font_manager.h"


using namespace Core;
using namespace Filters;


FilterStartButtonsBox::FilterStartButtonsBox(QWidget *parent)
    : QGroupBox(parent)
{
    loadFonts();
    initControls();
    initLayouts();

    this->setTitle(tr("Фильтры"));
}

FilterStartButtonsBox::~FilterStartButtonsBox()
{
}

void FilterStartButtonsBox::loadFonts()
{
    this->setFont(FontManager::instance().regular(GuiConfig::FONT_SIZE_NORMAL));
}

void FilterStartButtonsBox::initControls()
{
    // непрерывно-дискретные:

    m_btnContinuousDiscreteAof = new QPushButton(tr("АОФ"));
    connect(m_btnContinuousDiscreteAof, SIGNAL(clicked()), this, SLOT(onBtnContinuousDiscreteAofClicked()));

    m_btnContinuousDiscreteFos = new QPushButton(tr("ФМП"));
    connect(m_btnContinuousDiscreteFos, SIGNAL(clicked()), this, SLOT(onBtnContinuousDiscreteFosClicked()));

    m_btnContinuousDiscreteDfos = new QPushButton(tr("ФМПд"));
    connect(m_btnContinuousDiscreteDfos, SIGNAL(clicked()), this, SLOT(onBtnContinuousDiscreteDfosClicked()));

    m_btnContinuousDiscreteDfosbo = new QPushButton(tr("ФКП"));
    connect(m_btnContinuousDiscreteDfosbo, SIGNAL(clicked()), this, SLOT(onBtnContinuousDiscreteDfosboClicked()));

    // непрерывные:

    m_btnContinuousAof = new QPushButton(tr("АОФ"));
    connect(m_btnContinuousAof, SIGNAL(clicked()), this, SLOT(onBtnContinuousAofClicked()));

    m_btnContinuousFos = new QPushButton(tr("ФМП"));
    connect(m_btnContinuousFos, SIGNAL(clicked()), this, SLOT(onBtnContinuousFosClicked()));


    //дискретные:

    m_btnDiscreteAof = new QPushButton(tr("АОФ"));
    connect(m_btnDiscreteAof, SIGNAL(clicked()), this, SLOT(onBtnDiscreteAofClicked()));

    m_btnDiscreteFos = new QPushButton(tr("ФМП"));
    connect(m_btnDiscreteFos, SIGNAL(clicked()), this, SLOT(onBtnDiscreteFosClicked()));

    m_btnDiscreteMfos = new QPushButton(tr("MФМП"));
    connect(m_btnDiscreteMfos, SIGNAL(clicked()), this, SLOT(onBtnDiscreteMfosClicked()));

    m_btnDiscreteDuof = new QPushButton(tr("ДУОФ"));
    connect(m_btnDiscreteDuof, SIGNAL(clicked()), this, SLOT(onBtnDiscreteDuofClicked()));

    m_btnDiscreteDfmp = new QPushButton(tr("ДФМП"));
    connect(m_btnDiscreteDfmp, SIGNAL(clicked()), this, SLOT(onBtnDiscreteDfmpClicked()));

    m_btnDiscreteFkp = new QPushButton(tr("ФКП"));
    connect(m_btnDiscreteFkp, SIGNAL(clicked()), this, SLOT(onBtnDiscreteFkpClicked()));

    m_btnDiscreteDfkp = new QPushButton(tr("ДФКП"));
    connect(m_btnDiscreteDfkp, SIGNAL(clicked()), this, SLOT(onBtnDiscreteDfkpClicked()));

    m_btnDiscreteMdfmp = new QPushButton(tr("МДФМП"));
    connect(m_btnDiscreteMdfmp, SIGNAL(clicked()), this, SLOT(onBtnDiscreteMdfmpClicked()));

    // лд фильтры

    m_btnLDAof = new QPushButton(tr("лд-АОФ"));
    connect(m_btnLDAof, SIGNAL(clicked()), this, SLOT(onBtnLDAofClicked()));

    m_btnLDFos = new QPushButton(tr("лд-ФОС"));
    connect(m_btnLDFos, SIGNAL(clicked()), this, SLOT(onBtnLDFosClicked()));

    m_btnLDFkp = new QPushButton(tr("лд-ФКП"));
    connect(m_btnLDFkp, SIGNAL(clicked()), this, SLOT(onBtnLDFkpClicked()));

    m_btnLDFbp = new QPushButton(tr("лд-ФБП"));
    connect(m_btnLDFbp, SIGNAL(clicked()), this, SLOT(onBtnLDFbpClicked()));
}

void FilterStartButtonsBox::initLayouts()
{
    QTabWidget *tabWidget = new QTabWidget;

    QWidget *tab1 = new QWidget;
    QWidget *tab2 = new QWidget;
    QWidget *tab3 = new QWidget;
    QWidget *tab4 = new QWidget;

    tabWidget->addTab(tab1, tr("Дискретные"));
    tabWidget->addTab(tab2, tr("Непрерывные"));
    tabWidget->addTab(tab3, tr("Непрерывно-дискретные"));
    tabWidget->addTab(tab4, tr("Логико-Динамические"));

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SIGNAL(filtersFamilyChanged(int)));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(GuiConfig::LAYOUT_MARGIN_BIG);
    mainLayout->setSpacing(GuiConfig::LAYOUT_SPACING_BIG);
    mainLayout->addWidget(tabWidget);
    this->setLayout(mainLayout);


    // дискретные фильтры:
    QGridLayout *tab1Layout = new QGridLayout;
    tab1Layout->setMargin(GuiConfig::LAYOUT_MARGIN_NORMAL);
    tab1Layout->setSpacing(GuiConfig::LAYOUT_SPACING_NORMAL);

    tab1Layout->addWidget(m_btnDiscreteAof, 0, 0);
    tab1Layout->addWidget(m_btnDiscreteFos, 0, 1);
    tab1Layout->addWidget(m_btnDiscreteMfos, 0, 2);
    tab1Layout->addWidget(m_btnDiscreteFkp, 0, 3);

    tab1Layout->addWidget(m_btnDiscreteDuof, 1, 0);
    tab1Layout->addWidget(m_btnDiscreteDfmp, 1, 1);
    tab1Layout->addWidget(m_btnDiscreteMdfmp, 1, 2);
    tab1Layout->addWidget(m_btnDiscreteDfkp, 1, 3);


    // непрерывные фильтры:
    QGridLayout *tab2Layout = new QGridLayout;
    tab2Layout->setMargin(GuiConfig::LAYOUT_MARGIN_NORMAL);
    tab2Layout->setSpacing(GuiConfig::LAYOUT_SPACING_NORMAL);

    tab2Layout->addWidget(m_btnContinuousAof, 0, 0);
    tab2Layout->addWidget(m_btnContinuousFos, 0, 1);


    // непрерывно-дискретные фильтры:
    QGridLayout *tab3Layout = new QGridLayout;
    tab3Layout->setMargin(GuiConfig::LAYOUT_MARGIN_NORMAL);
    tab3Layout->setSpacing(GuiConfig::LAYOUT_SPACING_NORMAL);

    tab3Layout->addWidget(m_btnContinuousDiscreteAof, 0, 0);
    tab3Layout->addWidget(m_btnContinuousDiscreteFos, 0, 1);
    tab3Layout->addWidget(m_btnContinuousDiscreteDfos, 0, 2);
    tab3Layout->addWidget(m_btnContinuousDiscreteDfosbo, 0, 3);

    // лд фильтры:
    QGridLayout *tab4Layout = new QGridLayout;
    tab4Layout->setMargin(GuiConfig::LAYOUT_MARGIN_NORMAL);
    tab4Layout->setSpacing(GuiConfig::LAYOUT_SPACING_NORMAL);

    tab4Layout->addWidget(m_btnLDAof, 0, 0);
    tab4Layout->addWidget(m_btnLDFos, 0, 1);
    tab4Layout->addWidget(m_btnLDFkp, 0, 2);
    tab4Layout->addWidget(m_btnLDFbp, 0, 3);

    tab1->setLayout(tab1Layout);
    tab2->setLayout(tab2Layout);
    tab3->setLayout(tab3Layout);
    tab4->setLayout(tab4Layout);
}


// непрерывно-дискретные:

void FilterStartButtonsBox::onBtnContinuousDiscreteAofClicked()
{
    emit start(FILTER_TYPE::ContinuousDiscrete, APPROX_TYPE::Gauss, FILTER_ID::AOF);
}

void FilterStartButtonsBox::onBtnContinuousDiscreteFosClicked()
{
    emit start(FILTER_TYPE::ContinuousDiscrete, APPROX_TYPE::Gauss, FILTER_ID::FOS);
}

void FilterStartButtonsBox::onBtnContinuousDiscreteDfosClicked()
{
    emit start(FILTER_TYPE::ContinuousDiscrete, APPROX_TYPE::Gauss, FILTER_ID::DFOS);
}

void FilterStartButtonsBox::onBtnContinuousDiscreteDfosboClicked()
{
    emit start(FILTER_TYPE::ContinuousDiscrete, APPROX_TYPE::Gauss, FILTER_ID::DFOSBO);
}


//  непрерывные:

void FilterStartButtonsBox::onBtnContinuousAofClicked()
{
    emit start(FILTER_TYPE::Continuous, APPROX_TYPE::Gauss, FILTER_ID::AOF);
}

void FilterStartButtonsBox::onBtnContinuousFosClicked()
{
    emit start(FILTER_TYPE::Continuous, APPROX_TYPE::Gauss, FILTER_ID::FOS);
}


// дискретные:

void FilterStartButtonsBox::onBtnDiscreteAofClicked()
{
    emit start(FILTER_TYPE::Discrete, APPROX_TYPE::Gauss, FILTER_ID::AOF);
}

void FilterStartButtonsBox::onBtnDiscreteFosClicked()
{
    emit start(FILTER_TYPE::Discrete, APPROX_TYPE::Gauss, FILTER_ID::FOS);
}

void FilterStartButtonsBox::onBtnDiscreteMfosClicked()
{
    emit start(FILTER_TYPE::Discrete, APPROX_TYPE::Gauss, FILTER_ID::MDFOS);
}

void FilterStartButtonsBox::onBtnDiscreteDuofClicked()
{
    emit start(FILTER_TYPE::Discrete, APPROX_TYPE::Gauss, FILTER_ID::DUOF);
}

// NEW
void FilterStartButtonsBox::onBtnDiscreteDfmpClicked()
{
    emit start(FILTER_TYPE::Discrete, APPROX_TYPE::Gauss, FILTER_ID::DFMP);
}

void FilterStartButtonsBox::onBtnDiscreteFkpClicked()
{
    emit start(FILTER_TYPE::Discrete, APPROX_TYPE::Gauss, FILTER_ID::FKP);
}

void FilterStartButtonsBox::onBtnDiscreteDfkpClicked()
{
    emit start(FILTER_TYPE::Discrete, APPROX_TYPE::Gauss, FILTER_ID::DFKP);
}

void FilterStartButtonsBox::onBtnDiscreteMdfmpClicked()
{
    emit start(FILTER_TYPE::Discrete, APPROX_TYPE::Gauss, FILTER_ID::MDFMP);
}

// логико-динамические фильтры

void FilterStartButtonsBox::onBtnLDAofClicked()
{
    emit start(FILTER_TYPE::LogicDynamic, APPROX_TYPE::Linear, FILTER_ID::AOF);
}

void FilterStartButtonsBox::onBtnLDFosClicked()
{
    emit start(FILTER_TYPE::LogicDynamic, APPROX_TYPE::Linear, FILTER_ID::FOS);
}

void FilterStartButtonsBox::onBtnLDFkpClicked()
{
    emit start(FILTER_TYPE::LogicDynamic, APPROX_TYPE::Linear, FILTER_ID::LDFKP);
}

void FilterStartButtonsBox::onBtnLDFbpClicked()
{
    emit start(FILTER_TYPE::LogicDynamic, APPROX_TYPE::Linear, FILTER_ID::LDFBP);
}
