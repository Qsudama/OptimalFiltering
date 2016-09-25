#include "main_window.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_randSeed(1447528517)
{
    initControls();
    initLayouts();
    initStatusBar();
    loadFonts();

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
    this->setFont(FontManager::instance().regular(9));
    this->statusBar()->setFont(FontManager::instance().regularItalic(9));
}

void MainWindow::initControls()
{
    m_taskWidget         = new TaskWidget;
    m_filterParamsWidget = new FilterParametersWidget;
    m_filterStartWidget  = new FilterStartButtonsBox;
    m_btnClear           = new QPushButton(tr("Очистить"));

    connect(m_btnClear, SIGNAL(clicked()), this, SIGNAL(clear()));
    connect(m_taskWidget, SIGNAL(changed()), this, SIGNAL(clear()));
    connect(this, SIGNAL(clear()), this, SLOT(onClear()));
    connect(m_filterStartWidget, SIGNAL(start(Core::FILTER_TYPE, Core::APPROX_TYPE, Filters::FILTER_ID)), this,
            SLOT(onStart(Core::FILTER_TYPE, Core::APPROX_TYPE, Filters::FILTER_ID)));
}

void MainWindow::initLayouts()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(5);
    layout->setSpacing(5);
    layout->addWidget(m_taskWidget);
    layout->addWidget(m_filterParamsWidget);
    layout->addWidget(m_filterStartWidget);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout->addWidget(m_btnClear);

    if (!centralWidget()) {
        setCentralWidget(new QWidget);
    }
    centralWidget()->setLayout(layout);

    centralWidget()->setMinimumWidth(layout->margin() * 2 + m_filterParamsWidget->minimumWidth());
    this->setMinimumWidth(this->layout()->margin() * 2 + centralWidget()->minimumWidth());
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

    if (!statusBar()) {
        setStatusBar(new QStatusBar);
    }
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
}

void MainWindow::onFilterUpdatePercent(int p)
{
    m_statusProgressBar->setValue(p);
}

void MainWindow::onStart(Core::FILTER_TYPE ftype, Core::APPROX_TYPE atype, Filters::FILTER_ID id)
{
    Core::PtrTask             task   = m_taskWidget->task(ftype, atype);
    Core::PtrFilterParameters params = m_filterParamsWidget->parameters();
    // Core::PtrFilterParameters params (new Core::FilterParameters(2, 0.5, 0.1, 0.001, 100, 1));
    Core::PtrFilter filter = Filters::FilterFactory::create(ftype, id, params, task);
    connect(filter.get(), SIGNAL(updatePercent(int)), this, SLOT(onFilterUpdatePercent(int)));

    m_statusProgressBar->setEnabled(true);
    QString status = tr("Состояние: выполняется ") + QString::fromStdString(filter->info()->name());
    statusBar()->showMessage(status);

    Math::Rand::setRandSeed(uint(m_randSeed));
    filter->run(); // TODO сделать отдельный поток

    showData(filter);
    this->statusBar()->showMessage(tr("Состояние: ничего не выполняется"));
    m_statusProgressBar->setEnabled(false);
    m_statusProgressBar->setValue(0);
}

void MainWindow::showData(Core::PtrFilter filter)
{
    QColor  color = m_colorManager.nextColor();
    QString fname = QString::fromStdString(filter->info()->name());

    QPen mxPen, sxPen, sePen;
    mxPen.setWidthF(2.0);
    mxPen.setColor(Qt::darkMagenta);
    sxPen.setWidthF(1.5);
    sxPen.setColor(Qt::darkGray);
    sxPen.setStyle(Qt::DashLine);
    sePen.setWidthF(1.5);
    sePen.setColor(color);

    int dim = int(filter->result()[0].mx.size());
    if (m_graphWindow->countSheets() != dim) {
        m_graphWindow->setCountSheets(dim);
    }

    QString title = QString("Статистика <") + m_taskWidget->name() + QString(">");
    QString subTitle =
        QString("Размер выборки ") + QString::number(m_filterParamsWidget->parameters()->sampleSize()) +
        QString(", шаг интегрирования ") + QString::number(m_filterParamsWidget->parameters()->integrationStep()) +
        QString(", между измерениями ") + QString::number(m_filterParamsWidget->parameters()->measurementStep());
    for (int i = 0; i < dim; i++) {
        m_graphWindow->sheet(i).setTitleLabel(title);
        m_graphWindow->sheet(i).setSubTitleLabel(subTitle);
    }
    if (m_taskWidget->id() == Tasks::TASK_ID::Landing) {
        m_graphWindow->sheet(0).setXLabel("Время (с)");
        m_graphWindow->sheet(1).setXLabel("Время (с)");
        m_graphWindow->sheet(2).setXLabel("Время (с)");
        m_graphWindow->sheet(0).setYLabel("Скорость (м/c)");
        m_graphWindow->sheet(1).setYLabel("Угол наклона (°)");
        m_graphWindow->sheet(2).setYLabel("Высота (м)");
    }

    QVector<double> mm(dim, 1.0);
    if (m_taskWidget->id() == Tasks::TASK_ID::Landing) {
        mm[0] = mm[2] = 1000;
        mm[1]         = Math::Convert::RadToDeg(1.0);
    }
    QVector<double> x, y;
    Core::GetTime(filter->result(), x);

    for (int i = 0; i < dim; i++) {
        Core::GetMeanX(filter->result(), i, y, mm[i]);
        m_graphWindow->sheet(i).addCurve(x, y, "Mx" + QString::number(i + 1), mxPen, false);

        Core::GetStdDeviationX(filter->result(), i, y, mm[i]);
        m_graphWindow->sheet(i).addCurve(x, y, "Sx" + QString::number(i + 1), sxPen, false);

        Core::GetStdDeviationE(filter->result(), i, y, mm[i]);
        m_graphWindow->sheet(i).addCurve(x, y, "Se" + QString::number(i + 1) + " " + fname, sePen, true);
    }

    m_graphWindow->updatePlotter();
}
