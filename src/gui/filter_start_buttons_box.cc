#include "filter_start_buttons_box.h"
#include "src/gui/font_manager.h"


using namespace Core;
using namespace Filters;


FilterStartButtonsBox::FilterStartButtonsBox(QWidget *parent)
    : QGroupBox(parent)
{
    loadFonts();
    initControls();
    computeSizes();
    initLayouts();

    this->setTitle(tr("Фильтры"));
}

FilterStartButtonsBox::~FilterStartButtonsBox()
{
}

void FilterStartButtonsBox::loadFonts()
{
    this->setFont(FontManager::instance().regular(9));
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

    m_btnDiscreteLinearFos = new QPushButton(tr("Л-ФКП"));
    connect(m_btnDiscreteLinearFos, SIGNAL(clicked()), this, SLOT(onBtnDiscreteLinearFosClicked()));

    m_btnDiscreteGaussFos = new QPushButton(tr("Г-ФКП"));
    connect(m_btnDiscreteGaussFos, SIGNAL(clicked()), this, SLOT(onBtnDiscreteGaussFosClicked()));

    m_btnDiscreteLinearMfos = new QPushButton(tr("Л-MФКП"));
    connect(m_btnDiscreteLinearMfos, SIGNAL(clicked()), this, SLOT(onBtnDiscreteLinearMfosClicked()));

    m_btnDiscreteGaussMfos = new QPushButton(tr("Г-MФКП"));
    connect(m_btnDiscreteGaussMfos, SIGNAL(clicked()), this, SLOT(onBtnDiscreteGaussMfosClicked()));
}

void FilterStartButtonsBox::computeSizes()
{
}

void FilterStartButtonsBox::initLayouts()
{
    QTabWidget *tabWidget = new QTabWidget;

    QWidget *tab1 = new QWidget;
    QWidget *tab2 = new QWidget;
    QWidget *tab3 = new QWidget;

    tabWidget->addTab(tab1, tr("Непрерывно-дискретные"));
    tabWidget->addTab(tab2, tr("Непрерывные"));
    tabWidget->addTab(tab3, tr("Дискретные"));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(5);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(tabWidget);
    this->setLayout(mainLayout);

    // tab1:
    QHBoxLayout *tab1MainLayout = new QHBoxLayout;
    tab1MainLayout->setMargin(5);
    tab1MainLayout->setSpacing(5);

    // tab3:
    QHBoxLayout *tab3MainLayout = new QHBoxLayout;
    tab3MainLayout->setMargin(5);
    tab3MainLayout->setSpacing(5);

    // непрерывно-дискретные фильтры
    QVBoxLayout *layoutCDAof = new QVBoxLayout;
    layoutCDAof->setMargin(0);
    layoutCDAof->setSpacing(5);
    layoutCDAof->addWidget(m_btnContinuousDiscreteGaussAof);
    layoutCDAof->addWidget(m_btnContinuousDiscreteLinearAof);
    tab1MainLayout->addLayout(layoutCDAof);

    QVBoxLayout *layoutCDFos = new QVBoxLayout;
    layoutCDFos->setMargin(0);
    layoutCDFos->setSpacing(5);
    layoutCDFos->addWidget(m_btnContinuousDiscreteGaussFos);
    layoutCDFos->addWidget(m_btnContinuousDiscreteLinearFos);
    tab1MainLayout->addLayout(layoutCDFos);

    QVBoxLayout *layoutCDDfos = new QVBoxLayout;
    layoutCDDfos->setMargin(0);
    layoutCDDfos->setSpacing(5);
    layoutCDDfos->addWidget(m_btnContinuousDiscreteGaussDfos);
    layoutCDDfos->addWidget(m_btnContinuousDiscreteLinearDfos);
    tab1MainLayout->addLayout(layoutCDDfos);

    QVBoxLayout *layoutCDDfosbo = new QVBoxLayout;
    layoutCDDfosbo->setMargin(0);
    layoutCDDfosbo->setSpacing(5);
    layoutCDDfosbo->addWidget(m_btnContinuousDiscreteGaussDfosbo);
    layoutCDDfosbo->addWidget(m_btnContinuousDiscreteLinearDfosbo);
    tab1MainLayout->addLayout(layoutCDDfosbo);


    // дискретные фильтры
    QVBoxLayout *layoutDaof = new QVBoxLayout;
    layoutDaof->setMargin(0);
    layoutDaof->setSpacing(5);
    layoutDaof->addWidget(m_btnDiscreteGaussAof);
    layoutDaof->addWidget(m_btnDiscreteLinearAof);
    tab3MainLayout->addLayout(layoutDaof);

    QVBoxLayout *layoutDfos = new QVBoxLayout;
    layoutDfos->setMargin(0);
    layoutDfos->setSpacing(5);
    layoutDfos->addWidget(m_btnDiscreteGaussFos);
    layoutDfos->addWidget(m_btnDiscreteLinearFos);
    tab3MainLayout->addLayout(layoutDfos);

    QVBoxLayout *layoutDmfos = new QVBoxLayout;
    layoutDmfos->setMargin(0);
    layoutDmfos->setSpacing(5);
    layoutDmfos->addWidget(m_btnDiscreteGaussMfos);
    layoutDmfos->addWidget(m_btnDiscreteLinearMfos);
    tab3MainLayout->addLayout(layoutDmfos);

    tab1->setLayout(tab1MainLayout);


    // tab2:
    QHBoxLayout *tab2MainLayout = new QHBoxLayout;
    tab2MainLayout->setMargin(5);
    tab2MainLayout->setSpacing(5);

    QVBoxLayout *layoutCAof = new QVBoxLayout;
    layoutCAof->setMargin(0);
    layoutCAof->setSpacing(5);
    layoutCAof->addWidget(m_btnContinuousGaussAof);
    layoutCAof->addWidget(m_btnContinuousLinearAof);
    tab2MainLayout->addLayout(layoutCAof);

    QVBoxLayout *layoutCFos = new QVBoxLayout;
    layoutCFos->setMargin(0);
    layoutCFos->setSpacing(5);
    layoutCFos->addWidget(m_btnContinuousGaussFos);
    layoutCFos->addWidget(m_btnContinuousLinearFos);
    tab2MainLayout->addLayout(layoutCFos);

    tab2->setLayout(tab2MainLayout);

    tab3->setLayout(tab3MainLayout);
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
