#include "main_window.h"
#include "src/gui/filter_results_table.h"
#include <QTabWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_tablesIsVisible(false)
{
    loadFonts();
    initControls();
    initLayouts();
    initStatusBar();

    QRect screenRect = QApplication::desktop()->availableGeometry();
    setGeometry(screenRect.x() + 25, screenRect.y() + 50, this->minimumWidth(), screenRect.height() - 75);

    initGraphWindow();
}

MainWindow::~MainWindow()
{
    delete m_graphWindow;
}

void MainWindow::loadFonts()
{
    this->setFont(FontManager::instance().regular(GuiConfig::FONT_SIZE_NORMAL));

    if (!statusBar()) {
        setStatusBar(new QStatusBar);
    }
    this->statusBar()->setFont(FontManager::instance().regularItalic(GuiConfig::FONT_SIZE_SMALL));
}

void MainWindow::initControls()
{
    m_taskWidget               = new TaskWidget;
    m_filterParamsWidget       = new FilterParametersWidget;
    m_filterStartWidget        = new FilterStartButtonsBox;
    m_additionalSettingsWidget = new AdditionalSettingsWidget;
    m_btnClear                 = new QPushButton(tr("Очистить"));
    m_btnShowHideTables        = new QPushButton(tr("Показать таблицы"));

    m_btnShowHideTables->setEnabled(false);

    connect(m_btnClear, SIGNAL(clicked()), this, SIGNAL(clear()));
    connect(m_btnShowHideTables, SIGNAL(clicked()), this, SLOT(onShowHideTables()));
    connect(m_taskWidget, SIGNAL(changed()), this, SIGNAL(clear()));
    connect(this, SIGNAL(clear()), this, SLOT(onClear()));
    connect(m_filterStartWidget, SIGNAL(start(Core::FILTER_TYPE, Core::APPROX_TYPE, Filters::FILTER_ID)), this,
            SLOT(onStart(Core::FILTER_TYPE, Core::APPROX_TYPE, Filters::FILTER_ID)));
    connect(m_filterStartWidget, SIGNAL(filtersFamilyChanged(int)), m_filterParamsWidget,
            SLOT(onFiltersFamilyChanged(int)));
}

void MainWindow::initLayouts()
{
    // page1:

    QVBoxLayout *page1Layout = new QVBoxLayout;
    page1Layout->setMargin(GuiConfig::LAYOUT_MARGIN_BIG);
    page1Layout->setSpacing(GuiConfig::LAYOUT_SPACING_BIG);
    page1Layout->addWidget(m_taskWidget);
    page1Layout->addWidget(m_filterParamsWidget);
    page1Layout->addWidget(m_filterStartWidget);
    page1Layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setMargin(GuiConfig::LAYOUT_MARGIN_NORMAL);
    btnLayout->setSpacing(GuiConfig::LAYOUT_SPACING_NORMAL);
    btnLayout->addWidget(m_btnClear);
    btnLayout->addWidget(m_btnShowHideTables);

    page1Layout->addLayout(btnLayout);

    QWidget *page1 = new QWidget;
    page1->setLayout(page1Layout);
    page1->setMinimumWidth(page1Layout->margin() * 2 + m_filterParamsWidget->minimumWidth());

    // page2:

    QVBoxLayout *page2Layout = new QVBoxLayout;
    page2Layout->setMargin(GuiConfig::LAYOUT_MARGIN_BIG);
    page2Layout->setSpacing(GuiConfig::LAYOUT_SPACING_BIG);
    page2Layout->addWidget(m_additionalSettingsWidget);
    page2Layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    QWidget *page2 = new QWidget;
    page2->setLayout(page2Layout);


    QTabWidget *tabs = new QTabWidget;
    tabs->addTab(page1, tr("Основное"));
    tabs->addTab(page2, tr("Дополнительные настройки"));

    QWidget *    mainWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(GuiConfig::LAYOUT_MARGIN_SMALL);
    mainLayout->setSpacing(GuiConfig::LAYOUT_SPACING_SMALL);
    mainLayout->addWidget(tabs);
    mainWidget->setLayout(mainLayout);

    setCentralWidget(mainWidget);

    centralWidget()->setMinimumWidth(2 * GuiConfig::LAYOUT_MARGIN_BIG + page1->minimumWidth());
    setMinimumWidth(this->layout()->margin() * 2 + centralWidget()->minimumWidth());
}

void MainWindow::initStatusBar()
{
    int w = centralWidget()->minimumWidth() / 3;
    int h = QFontMetrics(statusBar()->font()).height();

    m_statusProgressBar = new QProgressBar;
    m_statusProgressBar->setMinimumWidth(w);
    m_statusProgressBar->setMaximumWidth(w);
    m_statusProgressBar->setMinimumHeight(h);
    m_statusProgressBar->setMaximumHeight(h);
    m_statusProgressBar->setEnabled(false);
    m_statusProgressBar->setValue(0);

    statusBar()->addPermanentWidget(m_statusProgressBar);
    statusBar()->layout()->setMargin(centralWidget()->layout()->margin());
    statusBar()->layout()->setSpacing(centralWidget()->layout()->spacing());
    statusBar()->showMessage(tr("Состояние: ничего не выполняется"));
}

void MainWindow::initGraphWindow()
{
    m_graphWindow = new GraphWindow;
    m_graphWindow->setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint |
                                  Qt::CustomizeWindowHint);
    m_graphWindow->setWindowTitle(tr("Графики"));
    QRect screenRect = QApplication::desktop()->availableGeometry();
    m_graphWindow->setGeometry(this->x() + this->width() + 25, screenRect.y() + 50,
                               screenRect.width() - this->width() - 75, screenRect.height() - 75);

    connect(this, SIGNAL(clear()), m_graphWindow, SLOT(onClear()));

    m_graphWindow->setCountSheets(1);
    m_graphWindow->updatePlotter();
    m_graphWindow->show();
}

void MainWindow::closeEvent(QCloseEvent *)
{
    QApplication::closeAllWindows();
}


// slots:

void MainWindow::onClear()
{
    m_colorManager.reset();

    for (int i = 0; i < m_tables.size(); ++i) {
        m_tables[i]->close();
        delete m_tables[i];
    }
    m_tables.clear();

    m_tablesIsVisible = false;
    m_btnShowHideTables->setText(tr("Показать таблицы"));
    m_btnShowHideTables->setEnabled(false);
}

void MainWindow::onShowHideTables()
{
    if (m_tables.size() == 0) {
        return;
    }

    if (m_tablesIsVisible) {
        for (int i = 0; i < m_tables.size(); ++i) {
            m_tables[i]->hide();
        }
        m_tablesIsVisible = false;
        m_btnShowHideTables->setText(tr("Показать таблицы"));
    } else {
        for (int i = 0; i < m_tables.size(); ++i) {
            m_tables[i]->show();
        }
        m_tablesIsVisible = true;
        m_btnShowHideTables->setText(tr("Скрыть таблицы"));
    }
}

void MainWindow::onFilterUpdatePercent(int p)
{
    m_statusProgressBar->setValue(p);
}

void MainWindow::onStart(Core::FILTER_TYPE ftype, Core::APPROX_TYPE atype, Filters::FILTER_ID id)
{
    Core::PtrTask             task   = m_taskWidget->task(ftype, atype);
    Core::PtrFilterParameters params = m_filterParamsWidget->parameters();
    Core::PtrFilter           filter = Filters::FilterFactory::create(ftype, id, params, task);
    connect(filter.get(), SIGNAL(updatePercent(int)), this, SLOT(onFilterUpdatePercent(int)));

    m_statusProgressBar->setEnabled(true);
    QString status = tr("Состояние: выполняется ") + QString::fromStdString(filter->info()->name());
    statusBar()->showMessage(status);

    filter->run(); // TODO сделать отдельный поток

    showData(filter, ftype);
    this->statusBar()->showMessage(tr("Состояние: ничего не выполняется"));
    m_statusProgressBar->setEnabled(false);
    m_statusProgressBar->setValue(0);
}

void MainWindow::showData(Core::PtrFilter filter, Core::FILTER_TYPE ftype)
{
    QColor  color = m_colorManager.nextColor();
    QString fname = QString::fromStdString(filter->info()->name());

    QPen mxPen, mePen, sxPen, sePen;
    mxPen.setWidthF(2.0);
    mxPen.setColor(Qt::darkMagenta);
    mePen.setWidthF(2.0);
    mePen.setColor(color);
    mePen.setStyle(Qt::DashDotLine);
    sxPen.setWidthF(1.5);
    sxPen.setColor(Qt::darkGray);
    sxPen.setStyle(Qt::DashLine);
    sePen.setWidthF(1.5);
    sePen.setColor(color);

    int dim = int(filter->result()[0].meanX.size());
    if (m_graphWindow->countSheets() != dim) {
        m_graphWindow->setCountSheets(dim);
    }

    QString title = tr("Статистика <") + m_taskWidget->name() + QString(">");
    QString subTitle =
        tr("Размер выборки ") + QString::number(m_filterParamsWidget->parameters()->sampleSize()) +
        tr(", шаг интегрирования ") + QString::number(m_filterParamsWidget->parameters()->integrationStep()) +
        tr(", шаг между измерениями ") + QString::number(m_filterParamsWidget->parameters()->measurementStep());

    if (m_taskWidget->id() == Tasks::TASK_ID::Landing || m_taskWidget->id() == Tasks::TASK_ID::LandingRejection) {
        m_graphWindow->sheet(0).setXLabel(tr("Время (с)"));
        m_graphWindow->sheet(1).setXLabel(tr("Время (с)"));
        m_graphWindow->sheet(2).setXLabel(tr("Время (с)"));
        m_graphWindow->sheet(0).setYLabel(tr("Скорость (м/c)"));
        m_graphWindow->sheet(1).setYLabel(tr("Угол наклона (°)"));
        m_graphWindow->sheet(2).setYLabel(tr("Высота (м)"));
    }
    if (m_taskWidget->id() == Tasks::TASK_ID::LandingTest) {
        m_graphWindow->sheet(0).setXLabel(tr("Время (с)"));
    }
    if (m_taskWidget->id() == Tasks::TASK_ID::LandingRejection) {
        m_graphWindow->sheet(3).setXLabel(tr("Время (с)"));
        m_graphWindow->sheet(4).setXLabel(tr("Время (с)"));
        m_graphWindow->sheet(5).setXLabel(tr("Время (с)"));
        m_graphWindow->sheet(3).setYLabel(tr("Квазиплотность (1/км)"));
        m_graphWindow->sheet(4).setYLabel(tr("Качество"));
        m_graphWindow->sheet(5).setYLabel(tr("Ошибка гировертикали (°)"));
    }
    if (ftype == Core::FILTER_TYPE::Discrete || ftype == Core::FILTER_TYPE::LogicDynamic) {
        subTitle =
                tr("Размер выборки ") + QString::number(m_filterParamsWidget->parameters()->sampleSize()) +
                tr(", шаг между измерениями ") + QString::number(m_filterParamsWidget->parameters()->measurementStep());
    }
    for (int i = 0; i < dim; i++) {
        m_graphWindow->sheet(i).setTitleLabel(title);
        m_graphWindow->sheet(i).setSubTitleLabel(subTitle);
    }
    Math::Vector scale(dim);
    if (m_taskWidget->id() == Tasks::TASK_ID::Landing || m_taskWidget->id() == Tasks::TASK_ID::LandingRejection) {
        scale[0] = scale[2] = 1;// Было 1000.0;
        scale[1] = scale[5] = Math::Convert::RadToDeg(1.0);
        scale[3]            = 1;// Было 1000.0;
        scale[4]            = 1.0;
    } else {
        for (int i = 0; i < dim; ++i) {
            scale[i] = 1.0;
        }
    }

    QVector<double> x, y;
    Core::GetTime(filter->result(), x);

    for (int i = 0; i < dim; i++) {
        Core::GetMeanX(filter->result(), i, y, scale[i]);
        m_graphWindow->sheet(i).addCurve(x, y, "Mx" + QString::number(i + 1), mxPen, false);

        Core::GetStdDeviationX(filter->result(), i, y, scale[i]);
        m_graphWindow->sheet(i).addCurve(x, y, "Sx" + QString::number(i + 1), sxPen, false);

        Core::GetMeanE(filter->result(), i, y, scale[i]);
        m_graphWindow->sheet(i).addCurve(x, y, "Me" + QString::number(i + 1) + " " + fname, mePen, false);

        Core::GetStdDeviationE(filter->result(), i, y, scale[i]);
        m_graphWindow->sheet(i).addCurve(x, y, "Se" + QString::number(i + 1) + " " + fname, sePen, true);
    }

    m_graphWindow->updatePlotter();

    addTable(filter->result(), filter->info()->name(), scale);
}

void MainWindow::addTable(const Core::FilterOutput &data, const std::string &label, const Math::Vector &scale)
{
    m_tables.append(new FilterResultsTable(data, label, scale));
    m_tables.last()->setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint |
                                    Qt::CustomizeWindowHint);

    QRect screenRect = QApplication::desktop()->availableGeometry();
    int   width      = m_tables.last()->minimumWidth();
    int   height     = m_tables.last()->minimumHeight();

    for (int i = 0; i < m_tables.size(); ++i) {
        m_tables[i]->setGeometry(screenRect.x() + 75 + i * 50, screenRect.y() + 90 + i * 40, width, height);
    }

    if (m_tablesIsVisible) {
        m_tables.last()->show();
    }
    m_btnShowHideTables->setEnabled(true);
}
