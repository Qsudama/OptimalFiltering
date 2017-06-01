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

    m_btnContinuousDiscreteGaussAof = new QPushButton(tr("Г-АОФ"));
    connect(m_btnContinuousDiscreteGaussAof, SIGNAL(clicked()), this, SLOT(onBtnContinuousDiscreteGaussAofClicked()));

    m_btnContinuousDiscreteLinearAof = new QPushButton(tr("Л-АОФ"));
    connect(m_btnContinuousDiscreteLinearAof, SIGNAL(clicked()), this, SLOT(onBtnContinuousDiscreteLinearAofClicked()));

    m_btnContinuousDiscreteGaussFos = new QPushButton(tr("Г-ФМП"));
    connect(m_btnContinuousDiscreteGaussFos, SIGNAL(clicked()), this, SLOT(onBtnContinuousDiscreteGaussFosClicked()));

    m_btnContinuousDiscreteLinearFos = new QPushButton(tr("Л-ФМП"));
    connect(m_btnContinuousDiscreteLinearFos, SIGNAL(clicked()), this, SLOT(onBtnContinuousDiscreteLinearFosClicked()));

    m_btnContinuousDiscreteGaussDfos = new QPushButton(tr("Г-ФМПд"));
    connect(m_btnContinuousDiscreteGaussDfos, SIGNAL(clicked()), this, SLOT(onBtnContinuousDiscreteGaussDfosClicked()));

    m_btnContinuousDiscreteLinearDfos = new QPushButton(tr("Л-ФМПд"));
    connect(m_btnContinuousDiscreteLinearDfos, SIGNAL(clicked()), this,
            SLOT(onBtnContinuousDiscreteLinearDfosClicked()));

    m_btnContinuousDiscreteGaussDfosbo = new QPushButton(tr("Г-ФКП"));
    connect(m_btnContinuousDiscreteGaussDfosbo, SIGNAL(clicked()), this,
            SLOT(onBtnContinuousDiscreteGaussDfosboClicked()));

    m_btnContinuousDiscreteLinearDfosbo = new QPushButton(tr("Л-ФКП"));
    connect(m_btnContinuousDiscreteLinearDfosbo, SIGNAL(clicked()), this,
            SLOT(onBtnContinuousDiscreteLinearDfosboClicked()));

    // непрерывные:

    m_btnContinuousGaussAof = new QPushButton(tr("Г-АОФ"));
    connect(m_btnContinuousGaussAof, SIGNAL(clicked()), this, SLOT(onBtnContinuousGaussAofClicked()));

    m_btnContinuousLinearAof = new QPushButton(tr("Л-АОФ"));
    connect(m_btnContinuousLinearAof, SIGNAL(clicked()), this, SLOT(onBtnContinuousLinearAofClicked()));

    m_btnContinuousGaussFos = new QPushButton(tr("Г-ФМП"));
    connect(m_btnContinuousGaussFos, SIGNAL(clicked()), this, SLOT(onBtnContinuousGaussFosClicked()));

    m_btnContinuousLinearFos = new QPushButton(tr("Л-ФМП"));
    connect(m_btnContinuousLinearFos, SIGNAL(clicked()), this, SLOT(onBtnContinuousLinearFosClicked()));


    //дискретные:

    m_btnDiscreteLinearAof = new QPushButton(tr("Л-АОФ"));
    connect(m_btnDiscreteLinearAof, SIGNAL(clicked()), this, SLOT(onBtnDiscreteLinearAofClicked()));

    m_btnDiscreteGaussAof = new QPushButton(tr("Г-АОФ"));
    connect(m_btnDiscreteGaussAof, SIGNAL(clicked()), this, SLOT(onBtnDiscreteGaussAofClicked()));

    m_btnDiscreteLinearFos = new QPushButton(tr("Л-ФМП"));
    connect(m_btnDiscreteLinearFos, SIGNAL(clicked()), this, SLOT(onBtnDiscreteLinearFosClicked()));

    m_btnDiscreteGaussFos = new QPushButton(tr("Г-ФМП"));
    connect(m_btnDiscreteGaussFos, SIGNAL(clicked()), this, SLOT(onBtnDiscreteGaussFosClicked()));

    m_btnDiscreteLinearMfos = new QPushButton(tr("Л-MФМП"));
    connect(m_btnDiscreteLinearMfos, SIGNAL(clicked()), this, SLOT(onBtnDiscreteLinearMfosClicked()));

    m_btnDiscreteGaussMfos = new QPushButton(tr("Г-MФМП"));
    connect(m_btnDiscreteGaussMfos, SIGNAL(clicked()), this, SLOT(onBtnDiscreteGaussMfosClicked()));

    // лд фильтры

    m_btnLDAof = new QPushButton(tr("лд-АОФ"));
    connect(m_btnLDAof, SIGNAL(clicked()), this, SLOT(onBtnLDAofClicked()));

    m_btnLDFos = new QPushButton(tr("лд-ФОС"));
    connect(m_btnLDFos, SIGNAL(clicked()), this, SLOT(onBtnLDFosClicked()));

    m_btnLDFkp = new QPushButton(tr("лд-ФКП"));
    connect(m_btnLDFkp, SIGNAL(clicked()), this, SLOT(onBtnLDFkpClicked()));
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

    tab1Layout->addWidget(m_btnDiscreteGaussAof, 0, 0);
    tab1Layout->addWidget(m_btnDiscreteLinearAof, 1, 0);
    tab1Layout->addWidget(m_btnDiscreteGaussFos, 0, 1);
    tab1Layout->addWidget(m_btnDiscreteLinearFos, 1, 1);
    tab1Layout->addWidget(m_btnDiscreteGaussMfos, 0, 2);
    tab1Layout->addWidget(m_btnDiscreteLinearMfos, 1, 2);


    // непрерывные фильтры:
    QGridLayout *tab2Layout = new QGridLayout;
    tab2Layout->setMargin(GuiConfig::LAYOUT_MARGIN_NORMAL);
    tab2Layout->setSpacing(GuiConfig::LAYOUT_SPACING_NORMAL);

    tab2Layout->addWidget(m_btnContinuousGaussAof, 0, 0);
    tab2Layout->addWidget(m_btnContinuousLinearAof, 1, 0);
    tab2Layout->addWidget(m_btnContinuousGaussFos, 0, 1);
    tab2Layout->addWidget(m_btnContinuousLinearFos, 1, 1);


    // непрерывно-дискретные фильтры:
    QGridLayout *tab3Layout = new QGridLayout;
    tab3Layout->setMargin(GuiConfig::LAYOUT_MARGIN_NORMAL);
    tab3Layout->setSpacing(GuiConfig::LAYOUT_SPACING_NORMAL);

    tab3Layout->addWidget(m_btnContinuousDiscreteGaussAof, 0, 0);
    tab3Layout->addWidget(m_btnContinuousDiscreteLinearAof, 1, 0);
    tab3Layout->addWidget(m_btnContinuousDiscreteGaussFos, 0, 1);
    tab3Layout->addWidget(m_btnContinuousDiscreteLinearFos, 1, 1);
    tab3Layout->addWidget(m_btnContinuousDiscreteGaussDfos, 0, 2);
    tab3Layout->addWidget(m_btnContinuousDiscreteLinearDfos, 1, 2);
    tab3Layout->addWidget(m_btnContinuousDiscreteGaussDfosbo, 0, 3);
    tab3Layout->addWidget(m_btnContinuousDiscreteLinearDfosbo, 1, 3);

    // лд фильтры:
    QGridLayout *tab4Layout = new QGridLayout;
    tab4Layout->setMargin(GuiConfig::LAYOUT_MARGIN_NORMAL);
    tab4Layout->setSpacing(GuiConfig::LAYOUT_SPACING_NORMAL);

    tab4Layout->addWidget(m_btnLDAof, 0, 0);
    tab4Layout->addWidget(m_btnLDFos, 0, 1);
    tab4Layout->addWidget(m_btnLDFkp, 0, 2);

    tab1->setLayout(tab1Layout);
    tab2->setLayout(tab2Layout);
    tab3->setLayout(tab3Layout);
    tab4->setLayout(tab4Layout);
}


// непрерывно-дискретные:

void FilterStartButtonsBox::onBtnContinuousDiscreteGaussAofClicked()
{
    emit start(FILTER_TYPE::ContinuousDiscrete, APPROX_TYPE::Gauss, FILTER_ID::AOF);
}

void FilterStartButtonsBox::onBtnContinuousDiscreteLinearAofClicked()
{
    emit start(FILTER_TYPE::ContinuousDiscrete, APPROX_TYPE::Linear, FILTER_ID::AOF);
}

void FilterStartButtonsBox::onBtnContinuousDiscreteGaussFosClicked()
{
    emit start(FILTER_TYPE::ContinuousDiscrete, APPROX_TYPE::Gauss, FILTER_ID::FOS);
}

void FilterStartButtonsBox::onBtnContinuousDiscreteLinearFosClicked()
{
    emit start(FILTER_TYPE::ContinuousDiscrete, APPROX_TYPE::Linear, FILTER_ID::FOS);
}

void FilterStartButtonsBox::onBtnContinuousDiscreteGaussDfosClicked()
{
    emit start(FILTER_TYPE::ContinuousDiscrete, APPROX_TYPE::Gauss, FILTER_ID::DFOS);
}

void FilterStartButtonsBox::onBtnContinuousDiscreteLinearDfosClicked()
{
    emit start(FILTER_TYPE::ContinuousDiscrete, APPROX_TYPE::Linear, FILTER_ID::DFOS);
}

void FilterStartButtonsBox::onBtnContinuousDiscreteGaussDfosboClicked()
{
    emit start(FILTER_TYPE::ContinuousDiscrete, APPROX_TYPE::Gauss, FILTER_ID::DFOSBO);
}

void FilterStartButtonsBox::onBtnContinuousDiscreteLinearDfosboClicked()
{
    emit start(FILTER_TYPE::ContinuousDiscrete, APPROX_TYPE::Linear, FILTER_ID::DFOSBO);
}


//  непрерывные:

void FilterStartButtonsBox::onBtnContinuousGaussAofClicked()
{
    emit start(FILTER_TYPE::Continuous, APPROX_TYPE::Gauss, FILTER_ID::AOF);
}

void FilterStartButtonsBox::onBtnContinuousLinearAofClicked()
{
    emit start(FILTER_TYPE::Continuous, APPROX_TYPE::Linear, FILTER_ID::AOF);
}

void FilterStartButtonsBox::onBtnContinuousGaussFosClicked()
{
    emit start(FILTER_TYPE::Continuous, APPROX_TYPE::Gauss, FILTER_ID::FOS);
}

void FilterStartButtonsBox::onBtnContinuousLinearFosClicked()
{
    emit start(FILTER_TYPE::Continuous, APPROX_TYPE::Linear, FILTER_ID::FOS);
}


// дискретные:

void FilterStartButtonsBox::onBtnDiscreteGaussAofClicked()
{
    emit start(FILTER_TYPE::Discrete, APPROX_TYPE::Gauss, FILTER_ID::AOF);
}

void FilterStartButtonsBox::onBtnDiscreteLinearAofClicked()
{
    emit start(FILTER_TYPE::Discrete, APPROX_TYPE::Linear, FILTER_ID::AOF);
}

void FilterStartButtonsBox::onBtnDiscreteGaussFosClicked()
{
    emit start(FILTER_TYPE::Discrete, APPROX_TYPE::Gauss, FILTER_ID::FOS);
}

void FilterStartButtonsBox::onBtnDiscreteLinearFosClicked()
{
    emit start(FILTER_TYPE::Discrete, APPROX_TYPE::Linear, FILTER_ID::FOS);
}

void FilterStartButtonsBox::onBtnDiscreteLinearMfosClicked()
{
    emit start(FILTER_TYPE::Discrete, APPROX_TYPE::Linear, FILTER_ID::MDFOS);
}

void FilterStartButtonsBox::onBtnDiscreteGaussMfosClicked()
{
    emit start(FILTER_TYPE::Discrete, APPROX_TYPE::Gauss, FILTER_ID::MDFOS);
}


// лд фильтры

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
//    emit start(FILTER_TYPE::LogicDynamic, APPROX_TYPE::Linear, FILTER_ID::MDFOS);
}
