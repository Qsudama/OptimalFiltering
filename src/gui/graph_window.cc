#include "graph_window.h"

#include "src/helpers/alert_helper.h"

GraphWindow::GraphWindow(QWidget *parent)
    : QMainWindow(parent) 
{
    loadFonts();
    initPlotter();
    initLayouts();
    initActions();
    initMenus();
    onClear();
}

GraphWindow::~GraphWindow()
{
    delete m_plotter;
}

void GraphWindow::loadFonts()
{
    m_plotterTitleFont         = FontManager::instance().regularBold(GuiConfig::FONT_SIZE_HUGE);
    m_plotterSubTitleFont      = FontManager::instance().regular(GuiConfig::FONT_SIZE_LARGE);
    m_plotterAxesLabelFont     = FontManager::instance().regular(GuiConfig::FONT_SIZE_LARGE);
    m_plotterLegendFont        = FontManager::instance().regular(GuiConfig::FONT_SIZE_BIG);
    m_plotterAxesTickLabelFont = FontManager::instance().regular(GuiConfig::FONT_SIZE_NORMAL);
}

void GraphWindow::initActions()
{
    m_actionSavePng = new QAction(tr("Сохранить изображение (.png)"), this);
    connect(m_actionSavePng, SIGNAL(triggered()), this, SLOT(onSavePng()));

    m_actionShowSetRangesDialog = new QAction(tr("Задать границы вручную"), this);
    connect(m_actionShowSetRangesDialog, SIGNAL(triggered()), this, SLOT(onShowSetRangesDialog()));

    m_actionSetAutoRanges = new QAction(tr("Автоопределение границ"), this);
    m_actionSetAutoRanges->setCheckable(true);
    m_actionSetAutoRanges->setChecked(true);
    connect(m_actionSetAutoRanges, SIGNAL(toggled(bool)), this, SLOT(onSetAutoRanges(bool)));

    m_actionShowStatisticSheet = new QAction(tr("Статистики"), this);
    m_actionShowStatisticSheet->setEnabled(true);
    m_actionShowStatisticSheet->setCheckable(true);
    m_actionShowStatisticSheet->setChecked(true);
    connect(m_actionShowStatisticSheet, SIGNAL(toggled(bool)), this, SLOT(onShowStatisticSheet(bool)));

    m_actionShowRealizationSheet = new QAction(tr("Реализации"), this);
    m_actionShowRealizationSheet->setEnabled(false);
    m_actionShowRealizationSheet->setCheckable(true);
    m_actionShowRealizationSheet->setChecked(false);
    connect(m_actionShowRealizationSheet, SIGNAL(toggled(bool)), this, SLOT(onShowRealizationSheet(bool)));

    m_actionShowFilterParamsSheet = new QAction(tr("Параметры"), this);
    m_actionShowFilterParamsSheet->setEnabled(false);
    m_actionShowFilterParamsSheet->setCheckable(true);
    m_actionShowFilterParamsSheet->setChecked(false);
    connect(m_actionShowFilterParamsSheet, SIGNAL(toggled(bool)), this, SLOT(onShowFilterParamsSheet(bool)));

    m_actionShowSeToSxSheet = new QAction(tr("Se / Sx"), this);
    m_actionShowSeToSxSheet->setEnabled(false);
    m_actionShowSeToSxSheet->setCheckable(true);
    m_actionShowSeToSxSheet->setChecked(false);
    connect(m_actionShowSeToSxSheet, SIGNAL(toggled(bool)), this, SLOT(onShowSeToSxSheet(bool)));
}

void GraphWindow::initMenus()
{
    if (!menuBar()) {
        setMenuBar(new QMenuBar(this));
    }

    m_menuFile = menuBar()->addMenu(tr("Файл"));
    m_menuFile->addAction(m_actionSavePng);

    m_menuView = menuBar()->addMenu(tr("Вид"));

    m_menuViewSheets = m_menuView->addMenu(tr("Отображение"));
    m_menuViewSheets->addAction(m_actionShowStatisticSheet);
    m_menuViewSheets->addAction(m_actionShowRealizationSheet);
    m_menuViewSheets->addAction(m_actionShowFilterParamsSheet);
    m_menuViewSheets->addAction(m_actionShowSeToSxSheet);
    m_menuView->addSeparator();

    m_menuSheet = m_menuView->addMenu(tr("Компонентa"));
    m_menuView->addSeparator(); 

    m_menuShow = m_menuView->addMenu(tr("Показать"));
    m_menuHide = m_menuView->addMenu(tr("Скрыть"));
    m_menuView->addSeparator();

    m_menuView->addAction(m_actionShowSetRangesDialog);
    m_menuView->addAction(m_actionSetAutoRanges);
}

void GraphWindow::initLayouts()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(GuiConfig::LAYOUT_MARGIN_SMALL);
    mainLayout->setSpacing(GuiConfig::LAYOUT_SPACING_SMALL);

    QFrame *frame = new QFrame;
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    frame->setLineWidth(1);

    QHBoxLayout *frameLayout = new QHBoxLayout;
    frameLayout->setMargin(GuiConfig::LAYOUT_MARGIN_SMALL);
    frameLayout->setSpacing(GuiConfig::LAYOUT_SPACING_SMALL);
    frameLayout->addWidget(m_plotter);
    frame->setMinimumWidth(m_plotter->minimumWidth() + 2 * frameLayout->margin());
    frame->setMinimumHeight(m_plotter->minimumHeight() + 2 * frameLayout->margin());

    frame->setLayout(frameLayout);
    mainLayout->addWidget(frame);

    if (!centralWidget()) {
        setCentralWidget(new QWidget);
    }
    centralWidget()->setLayout(mainLayout);
    centralWidget()->setMinimumWidth(frame->minimumWidth() + 2 * centralWidget()->layout()->margin());
    centralWidget()->setMinimumHeight(frame->minimumHeight() + 2 * centralWidget()->layout()->margin());

    this->setMinimumWidth(centralWidget()->minimumWidth() + 2 * this->layout()->margin());
    this->setMinimumHeight(centralWidget()->minimumHeight() + 2 * this->layout()->margin());
}

void GraphWindow::initPlotter()
{
    m_plotter = new QCustomPlot;
    m_plotter->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend |
                               QCP::iSelectPlottables);

    m_plotter->xAxis->setTickLabelFont(m_plotterAxesTickLabelFont);
    m_plotter->yAxis->setTickLabelFont(m_plotterAxesTickLabelFont);
    m_plotter->xAxis->grid()->setSubGridVisible(true);
    m_plotter->yAxis->grid()->setSubGridVisible(true);
    m_plotter->yAxis->setLabelPadding(m_plotter->xAxis->labelPadding() + 2);
    m_plotter->xAxis->setLabelFont(m_plotterAxesLabelFont);
    m_plotter->yAxis->setLabelFont(m_plotterAxesLabelFont);
    m_plotter->xAxis->setSelectedLabelFont(m_plotterAxesLabelFont);
    m_plotter->yAxis->setSelectedLabelFont(m_plotterAxesLabelFont);
    m_plotter->axisRect()->setupFullAxesBox();

    m_plotter->setFont(this->font());
    m_plotter->plotLayout()->insertRow(0);
    m_plotter->plotLayout()->addElement(0, 0, new QCPTextElement(m_plotter, ""));
    m_plotter->plotLayout()->insertRow(1);
    m_plotter->plotLayout()->addElement(1, 0, new QCPTextElement(m_plotter, ""));
    m_plotter->legend->setVisible(true);
    m_plotter->legend->setFont(m_plotterLegendFont);
    m_plotter->legend->setSelectedFont(m_plotterLegendFont);
    m_plotter->legend->setSelectableParts(QCPLegend::spItems);
    QBrush brush = m_plotter->legend->brush();
    brush.setColor(QColor::fromRgb(255, 255, 255, 215));
    m_plotter->legend->setBrush(brush);
    m_plotter->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(m_plotter, SIGNAL(selectionChangedByUser()), this, SLOT(onSelectionChanged()));
    connect(m_plotter, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(onMousePress()));
    connect(m_plotter, SIGNAL(mouseWheel(QWheelEvent *)), this, SLOT(onMouseWheel()));
    connect(m_plotter, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onMenuRequest(QPoint)));
    connect(m_plotter->xAxis, SIGNAL(rangeChanged(QCPRange)), m_plotter->xAxis2, SLOT(setRange(QCPRange)));
    connect(m_plotter->yAxis, SIGNAL(rangeChanged(QCPRange)), m_plotter->yAxis2, SLOT(setRange(QCPRange)));

    QFontMetrics *metric    = new QFontMetrics(m_plotterSubTitleFont);
    int           minWidth  = metric->width(tr("шаг интегрирования 0.0001, между измерениями 9999.9999"));
    int           minHeight = int(0.6 * minWidth);

    m_plotter->setMinimumWidth(minWidth);
    m_plotter->setMinimumHeight(minHeight);
    delete metric;
}

void GraphWindow::updateMenu()
{

    //меню - выбор страницы
    m_menuSheet->clear();
    if (m_actionShowStatisticSheet->isChecked()) {
        for (int i = 0; i < m_sheets.size(); i++) {
            QAction *action = new QAction(QString::number(i + 1), m_menuSheet);
            action->setData(i + 100);
            action->setCheckable(true);
            if (m_currentSheet == &(m_sheets[i])) {
                action->setChecked(true);
            } else {
                action->setChecked(false);
            }
            action->setAutoRepeat(false);
            m_menuSheet->addAction(action);
        }

        if (m_statisticLDFiltersSheet) {
            QAction *action = new QAction(tr("Режим I"), m_menuSheet);
            action->setData(m_sheets.size() + 100);
            action->setCheckable(true);
            action->setChecked(m_currentSheet == m_statisticLDFiltersSheet);
            action->setAutoRepeat(false);
            m_menuSheet->addAction(action);
        }
    } else if (m_actionShowRealizationSheet->isChecked()) {
        for (int i = 0; i < m_realizationFiltersSheets.size(); i++) {
            QAction *action = new QAction(QString::number(i + 1), m_menuSheet);
            action->setData(i + 100);
            action->setCheckable(true);
            if (m_currentSheet == &(m_realizationFiltersSheets[i])) {
                action->setChecked(true);
            } else {
                action->setChecked(false);
            }
            action->setAutoRepeat(false);
            m_menuSheet->addAction(action);
        }

        if (m_realizationLDFiltersSheet) {
            QAction *action = new QAction(tr("Режим I"), m_menuSheet);
            action->setData(m_realizationFiltersSheets.size() + 100);
            action->setCheckable(true);
            action->setChecked(m_currentSheet == m_realizationLDFiltersSheet);
            action->setAutoRepeat(false);
            m_menuSheet->addAction(action);
        }
    } else if (m_filterParametersSheet && m_actionShowFilterParamsSheet->isChecked()) {
        QAction *action = new QAction(tr("Параметрыыыы"), m_menuSheet);
        action->setData(99999999);
        action->setCheckable(true);
        action->setChecked(m_currentSheet == m_filterParametersSheet);
        action->setAutoRepeat(false);
        m_menuSheet->addAction(action);
    } else if (m_actionShowSeToSxSheet->isChecked()) {
        for (int i = 0; i < m_SeToSxSheets.size(); i++) {
            QAction *action = new QAction(QString::number(i + 1), m_menuSheet);
            action->setData(i + 1000);
            action->setCheckable(true);
            if (m_currentSheet == &(m_SeToSxSheets[i])) {
                action->setChecked(true);
            } else {
                action->setChecked(false);
            }
            action->setAutoRepeat(false);
            m_menuSheet->addAction(action);
        }
    }

    connect(m_menuSheet, SIGNAL(triggered(QAction *)), this, SLOT(onCurrentSheetChanged(QAction *)));

    //меню - скрыть графики
    m_menuHide->clear();
    int count = 0;
    if (m_currentSheet) {
        for (int i = 0; i < m_currentSheet->curves().size(); i++) {
            if (m_currentSheet->curves()[i].visible == true) {
                QAction *action = new QAction(m_currentSheet->curves()[i].fullName(), m_menuHide);
                action->setData(m_currentSheet->curves()[i].fullName());
                action->setAutoRepeat(false);
                m_menuHide->addAction(action);
                count++;
            }
        }
    }

    if (count == 0) {
        QAction *action = new QAction(tr("Нет показываемых графиков"), m_menuHide);
        action->setAutoRepeat(false);
        m_menuHide->addAction(action);
    }
    connect(m_menuHide, SIGNAL(triggered(QAction *)), this, SLOT(onHideCurve(QAction *)));

    //меню - показать скрытые
    m_menuShow->clear();
    count = 0;
    if (m_currentSheet) {
        for (int i = 0; i < m_currentSheet->curves().size(); i++) {
            if (m_currentSheet->curves()[i].visible == false) {
                QAction *action = new QAction(m_currentSheet->curves()[i].fullName(), m_menuShow);
                action->setData(m_currentSheet->curves()[i].fullName());
                action->setAutoRepeat(false);
                m_menuShow->addAction(action);
                count++;
            }
        }
    }
    if (count == 0) {
        QAction *action = new QAction(tr("Нет скрытых графиков"), m_menuShow);
        action->setAutoRepeat(false);
        m_menuShow->addAction(action);
    }
    connect(m_menuShow, SIGNAL(triggered(QAction *)), this, SLOT(onShowCurve(QAction *)));
}


// slots:

void GraphWindow::onSelectionChanged()
{
    bool test = m_plotter->xAxis->selectedParts().testFlag(QCPAxis::spAxis) ||
                m_plotter->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
                m_plotter->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) ||
                m_plotter->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels);
    if (test) {
        m_plotter->xAxis2->setSelectedParts(QCPAxis::spAxis | QCPAxis::spTickLabels);
        m_plotter->xAxis->setSelectedParts(QCPAxis::spAxis | QCPAxis::spTickLabels);
    }

    test = m_plotter->yAxis->selectedParts().testFlag(QCPAxis::spAxis) ||
           m_plotter->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
           m_plotter->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) ||
           m_plotter->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels);
    if (test) {
        m_plotter->yAxis2->setSelectedParts(QCPAxis::spAxis | QCPAxis::spTickLabels);
        m_plotter->yAxis->setSelectedParts(QCPAxis::spAxis | QCPAxis::spTickLabels);
    }

    for (int i = 0; i < m_plotter->graphCount(); ++i) {
        QCPGraph *graph = m_plotter->graph(i);
        QCPPlottableLegendItem *item = m_plotter->legend->itemWithPlottable(graph);
        if (item->selected() || graph->selected()) {
            item->setSelected(true);
            graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
            if (i + 2 < m_plotter->graphCount()) {
                if (graph->tag() == m_plotter->graph(i+1)->tag() && graph->tag() == m_plotter->graph(i+2)->tag()) {
                    QCPGraph *graph_up = m_plotter->graph(i+1);
                    QCPPlottableLegendItem *item_up = m_plotter->legend->itemWithPlottable(graph_up);
                    QCPGraph *graph_down = m_plotter->graph(i+2);
                    QCPPlottableLegendItem *item_down = m_plotter->legend->itemWithPlottable(graph_down);
                    item_up->setSelected(true);
                    graph_up->setSelection(QCPDataSelection(graph_up->data()->dataRange()));
                    item_down->setSelected(true);
                    graph_down->setSelection(QCPDataSelection(graph_down->data()->dataRange()));
                }
            }
        }
    }
}

void GraphWindow::onMousePress()
{
    if (m_plotter->xAxis->selectedParts().testFlag(QCPAxis::spAxis)) {
        m_plotter->axisRect()->setRangeDrag(m_plotter->xAxis->orientation());
    } else if (m_plotter->yAxis->selectedParts().testFlag(QCPAxis::spAxis)) {
        m_plotter->axisRect()->setRangeDrag(m_plotter->yAxis->orientation());
    } else {
        m_plotter->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    }
}

void GraphWindow::onMouseWheel()
{
    if (m_plotter->xAxis->selectedParts().testFlag(QCPAxis::spAxis)) {
        m_plotter->axisRect()->setRangeZoom(m_plotter->xAxis->orientation());
    } else if (m_plotter->yAxis->selectedParts().testFlag(QCPAxis::spAxis)) {
        m_plotter->axisRect()->setRangeZoom(m_plotter->yAxis->orientation());
    } else {
        m_plotter->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    }
}

void GraphWindow::onMenuRequest(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    if (m_plotter->legend->selectTest(pos, false) >= 0) {
        menu->addAction(tr("В левый верхний угол"), this, SLOT(onMoveLegend()))
            ->setData((int(Qt::AlignTop | Qt::AlignLeft)));
        menu->addAction(tr("В правый верхний угол"), this, SLOT(onMoveLegend()))
            ->setData((int(Qt::AlignTop | Qt::AlignRight)));
        menu->addAction(tr("В правый нижний угол"), this, SLOT(onMoveLegend()))
            ->setData((int(Qt::AlignBottom | Qt::AlignRight)));
        menu->addAction(tr("В левый нижний угол"), this, SLOT(onMoveLegend()))
            ->setData((int(Qt::AlignBottom | Qt::AlignLeft)));
    } else if (m_plotter->selectedGraphs().size() > 0) {
        menu->addAction(tr("Cкрыть"), this, SLOT(onHideCurveFromContextMenu()));
    }
    menu->popup(m_plotter->mapToGlobal(pos));
}

void GraphWindow::onMoveLegend()
{
    if (QAction *contextAction = qobject_cast<QAction *>(sender())) {
        bool ok;
        int  dataInt = contextAction->data().toInt(&ok);
        if (ok) {
            m_plotter->axisRect()->insetLayout()->setInsetAlignment(0, Qt::Alignment(dataInt));
            m_plotter->replot();
        }
    }
}

void GraphWindow::onClear()
{
    m_sheets.clear();
    m_realizationFiltersSheets.clear();
    m_SeToSxSheets.clear();

    if (m_statisticLDFiltersSheet) {
        delete m_statisticLDFiltersSheet;
        m_statisticLDFiltersSheet = nullptr;
    }

    if (m_realizationLDFiltersSheet) {
        delete m_realizationLDFiltersSheet;
        m_realizationLDFiltersSheet = nullptr;
    }

    if (m_filterParametersSheet) {
        delete m_filterParametersSheet;
        m_filterParametersSheet = nullptr;
    }

    if (m_currentSheet) {
        m_currentSheet = nullptr;
        delete m_currentSheet;
    }

    m_actionShowStatisticSheet->setChecked(true);
    m_actionShowRealizationSheet->setEnabled(false);
    m_actionShowFilterParamsSheet->setEnabled(false);
    m_actionShowSeToSxSheet->setEnabled(false);
    updatePlotter();
}

void GraphWindow::onRangesChanged(Math::Vector2 x, Math::Vector2 y)
{
    GAxisRange range;
    range.xMin = x[0];
    range.xMax = x[1];
    range.yMin = y[0];
    range.yMax = y[1];
    m_currentSheet->setAxisRange(range);
    m_currentSheet->setAutoCalcRanges(false);
    m_actionSetAutoRanges->setChecked(false);
    updatePlotter();
}

void GraphWindow::onSavePng()
{
    QString fileName =
        QFileDialog::getSaveFileName(this, tr("Сохранить изображение"), QDir::homePath(), tr("Изображение (*.png)"));
    m_plotter->savePng(fileName);
}

void GraphWindow::onShowStatisticSheet(bool checked)
{
    if (checked) {
        m_actionShowRealizationSheet->setChecked(false);
        m_actionShowFilterParamsSheet->setChecked(false);
        m_actionShowSeToSxSheet->setChecked(false);
        if (m_sheets.count() > 0) {
            m_currentSheet = &(m_sheets[0]);
            updatePlotter();
        }
    }
}

void GraphWindow::onShowRealizationSheet(bool checked)
{
    if (checked) {
        m_actionShowStatisticSheet->setChecked(false);
        m_actionShowFilterParamsSheet->setChecked(false);
        m_actionShowSeToSxSheet->setChecked(false);
        if (m_realizationFiltersSheets.count() > 0) {
            m_currentSheet = &(m_realizationFiltersSheets[0]);
            updatePlotter();
        }
    }
}

void GraphWindow::onShowFilterParamsSheet(bool checked)
{
    if (checked) {
        m_actionShowRealizationSheet->setChecked(false);
        m_actionShowStatisticSheet->setChecked(false);
        m_actionShowSeToSxSheet->setChecked(false);
        if (m_filterParametersSheet) {
            m_currentSheet = m_filterParametersSheet;
            updatePlotter();
        }
    }
}

void GraphWindow::onShowSeToSxSheet(bool checked)
{
    if (checked) {
        m_actionShowStatisticSheet->setChecked(false);
        m_actionShowRealizationSheet->setChecked(false);
        m_actionShowFilterParamsSheet->setChecked(false);
        if (m_SeToSxSheets.count() > 0) {
            m_currentSheet = &(m_SeToSxSheets[0]);
            updatePlotter();
        }
    }
}


void GraphWindow::onShowSetRangesDialog()
{
    Math::Vector x(2), y(2);
    x[0] = m_currentSheet->axisRange().xMin;
    x[1] = m_currentSheet->axisRange().xMax;
    y[0] = m_currentSheet->axisRange().yMin;
    y[1] = m_currentSheet->axisRange().yMax;

    RangesDialog *dialog = new RangesDialog(x, y);
    dialog->setGeometry(this->x() + int(this->width() * 0.5 - dialog->minimumWidth() * 0.5),
                        this->y() + int(this->height() * 0.5 - dialog->minimumHeight() * 0.5), dialog->minimumWidth(),
                        dialog->minimumHeight());
    dialog->setWindowTitle(tr("Границы графиков"));
    connect(dialog, SIGNAL(rangesChanged(Math::Vector2, Math::Vector2)), this,
            SLOT(onRangesChanged(Math::Vector2, Math::Vector2)));
    dialog->setModal(true);
    dialog->show();
}

void GraphWindow::onSetAutoRanges(bool checked)
{
    m_currentSheet->setAutoCalcRanges(checked);
    updatePlotter();
}

void GraphWindow::onCurrentSheetChanged(QAction *action)
{
    int snum = action->data().toInt() - 100;
    if (m_actionShowStatisticSheet->isChecked()) {
        if (snum < 0 || snum >= m_sheets.size() + 1) {
            return;
        }
        if (snum < m_sheets.size()) {
            if (m_currentSheet == &(m_sheets[snum])) {
                return;
            }
            m_currentSheet = &(m_sheets[snum]);
        } else {
            m_currentSheet = m_statisticLDFiltersSheet;
        }
    } else if (m_actionShowRealizationSheet->isChecked())  {
        if (snum < 0 || snum >= m_realizationFiltersSheets.size() + 1) {
            return;
        }
        if (snum < m_sheets.size()) {
            if (m_currentSheet == &(m_realizationFiltersSheets[snum])) {
                return;
            }
            m_currentSheet = &(m_realizationFiltersSheets[snum]);
        } else {
            m_currentSheet = m_realizationLDFiltersSheet;
        }
    } else if (m_actionShowFilterParamsSheet->isChecked()) {
        m_currentSheet = m_filterParametersSheet;
    } else if (m_actionShowSeToSxSheet->isChecked()) {
        int snum = action->data().toInt() - 1000;
        if (snum < 0 || snum >= m_SeToSxSheets.size() + 1) {
            return;
        }
        if (snum < m_SeToSxSheets.size()) {
            if (m_currentSheet == &(m_SeToSxSheets[snum])) {
                return;
            }
            m_currentSheet = &(m_SeToSxSheets[snum]);
        } else {
            m_currentSheet = m_statisticLDFiltersSheet;
        }
    }

    m_menuSheet->removeAction(action);
    m_actionSetAutoRanges->setChecked(m_currentSheet->autoCalcRanges());
    updatePlotter();
}

void GraphWindow::onHideCurveFromContextMenu()
{
    if (m_plotter->selectedGraphs().size() == 0) {
        return;
    }

    QString name = m_plotter->selectedGraphs().at(0)->name();
    int index = -1;
    for (int j = 0; j < m_currentSheet->curves().size(); j++) {
        if (m_currentSheet->curves()[j].fullName() == name && m_currentSheet->curves()[j].visible == true) {
            m_currentSheet->setCurveVisible(j, false);
            index = j;
            j = m_currentSheet->curves().size() + 1;
        }
    }
    if (index > -1) {
        updatePlotter();
    }
}

void GraphWindow::onHideCurve(QAction *action)
{
    if (action == 0) {
        return;
    }

    QString name = action->data().toString();
    int index = -1;

    for (int j = 0; j < m_currentSheet->curves().size(); j++) {
        if (m_currentSheet->curves()[j].fullName() == name && m_currentSheet->curves()[j].visible == true) {
            m_menuHide->removeAction(action);
            m_currentSheet->setCurveVisible(j, false);
            index = j;
            j = m_currentSheet->curves().size() + 1;
        }
    }
    if (index > -1) {
        updatePlotter();
    }
}

void GraphWindow::onShowCurve(QAction *action)
{
    if (action == 0) {
        return;
    }

    QString name = action->data().toString();
    int index = -1;

    for (int j = 0; j < m_currentSheet->curves().size(); j++) {
        if (m_currentSheet->curves()[j].fullName() == name && m_currentSheet->curves()[j].visible == false) {

            m_menuShow->removeAction(action);
            m_currentSheet->setCurveVisible(j, true);
            index = j;
            j = m_currentSheet->curves().size() + 1;
        }
    }
    if (index > -1) {
        updatePlotter();
    }
}

int GraphWindow::countSheets() const
{
    return m_sheets.size();
}

GraphSheet &GraphWindow::sheetAtIndex(int index)
{
    if (index <= 0 && index >= m_sheets.size()) {
        AlertHelper::showErrorAlertWithText("GraphWindow::sheet\nВыход за пределы!");
        return m_sheets[0];
    }
    return m_sheets[index];
}

GraphSheet &GraphWindow::realizationSheetAtIndex(int index)
{
    if (index <= 0 && index >= m_realizationFiltersSheets.size()) {
        AlertHelper::showErrorAlertWithText("GraphWindow::realizationSheetAtIndex\nВыход за пределы!");
        return m_realizationFiltersSheets[0];
    }
    return m_realizationFiltersSheets[index];
}

GraphSheet &GraphWindow::SeToSxSheetAtIndex(int index)
{
    if (index <= 0 && index >= m_SeToSxSheets.size()) {
        AlertHelper::showErrorAlertWithText("GraphWindow::realizationSheetAtIndex\nВыход за пределы!");
        return m_SeToSxSheets[0];
    }
    return m_SeToSxSheets[index];
}

GraphSheet &GraphWindow::statisticLDSheet()
{
    return *m_statisticLDFiltersSheet;
}

GraphSheet &GraphWindow::realizationLDSheet()
{
    return *m_realizationLDFiltersSheet;
}

GraphSheet &GraphWindow::parametersSheet()
{
    return *m_filterParametersSheet;
}

GraphSheet &GraphWindow::currentSheet()
{
    return *m_currentSheet;
}

void GraphWindow::setCountSheets(int count)
{
    m_sheets.resize(count);
}

void GraphWindow::setCountRealizationSheets(int count)
{
    if (count > 0) {
        m_actionShowRealizationSheet->setEnabled(true);
    }
    m_realizationFiltersSheets.resize(count);
}

void GraphWindow::setCountSeToSxSheets(int count)
{
    if (count > 0) {
        m_actionShowSeToSxSheet->setEnabled(true);
    }
    m_SeToSxSheets.resize(count);
}

bool GraphWindow::reloadStatisticSheet()
{
    if (!m_statisticLDFiltersSheet) {
        m_statisticLDFiltersSheet = new GraphSheet;
        return true;
    }
    return false;
}

bool GraphWindow::reloadRealizationSheet()
{
    if (!m_realizationLDFiltersSheet) {
        m_realizationLDFiltersSheet = new GraphSheet;
        return true;
    }
    return false;
}

bool GraphWindow::reloadFilterParamsSheet()
{
    if (!m_filterParametersSheet) {
        m_filterParametersSheet = new GraphSheet;
        m_actionShowFilterParamsSheet->setEnabled(true);
        return true;
    }
    return false;
}

void GraphWindow::updateDefaultSheet()
{
    if (!m_currentSheet) {
        m_currentSheet = &(m_sheets[0]);
    }
}

void GraphWindow::updatePlotter()
{
    m_plotter->clearGraphs();

    if (m_currentSheet) {
        GAxisRange range = m_currentSheet->axisRange();
        m_plotter->xAxis->setRange(range.xMin, range.xMax);
        m_plotter->yAxis->setRange(range.yMin, range.yMax);

        QString title = m_currentSheet->titleLabel();
        if (title.length() == 0) {
            title = tr("Статистика <...>");
        }
        QString subTitle = m_currentSheet->subTitleLabel();
        if (subTitle.length() == 0) {
    //        subTitle = tr("размер выборки <...>, шаг интегрирования <...>, между измерениями <...>");
            subTitle = tr("Шаг интегрирования <...>, между измерениями <...>");
        }

        QCPTextElement *plotTitle = new QCPTextElement(m_plotter, title);
        plotTitle->setFont(m_plotterTitleFont);
        m_plotter->plotLayout()->remove(m_plotter->plotLayout()->element(0, 0));
        m_plotter->plotLayout()->addElement(0, 0, plotTitle);
        m_plotter->plotLayout()->element(0, 0)->setAntialiased(true);

        QCPTextElement *plotSubTitle = new QCPTextElement(m_plotter, subTitle);
        plotSubTitle->setFont(m_plotterSubTitleFont);
        m_plotter->plotLayout()->remove(m_plotter->plotLayout()->element(1, 0));
        m_plotter->plotLayout()->addElement(1, 0, plotSubTitle);
        m_plotter->plotLayout()->element(1, 0)->setAntialiased(true);

        m_plotter->xAxis->setLabel(m_currentSheet->xLabel());
        m_plotter->yAxis->setLabel(m_currentSheet->yLabel());

        for (int i = 0; i < m_currentSheet->curves().size(); i++) {
            if (m_currentSheet->curves()[i].visible == false) {
                continue;
            }
            int j = m_plotter->graphCount();
            m_plotter->addGraph();
            m_plotter->graph(j)->setTag(i);
            m_plotter->graph(j)->setData(m_currentSheet->curves()[i].x, m_currentSheet->curves()[i].y);
            m_plotter->graph(j)->setPen(m_currentSheet->curves()[i].pen);
            m_plotter->graph(j)->setName(m_currentSheet->curves()[i].fullName());
            m_plotter->graph(j)->setAntialiased(true);
            m_plotter->graph(j)->setAdaptiveSampling(true);
            if (m_currentSheet->curves()[i].with_side) {
                m_plotter->addGraph();
                m_plotter->addGraph();
                m_plotter->graph(j+1)->setTag(i);
                m_plotter->graph(j+2)->setTag(i);
                m_plotter->graph(j+1)->setData(m_currentSheet->curves()[i].x, m_currentSheet->curves()[i].y_up);
                m_plotter->graph(j+1)->setPen(m_currentSheet->curves()[i].up_down_pen);
                m_plotter->graph(j+1)->setName("Up " + m_currentSheet->curves()[i].name);
                m_plotter->graph(j+2)->setData(m_currentSheet->curves()[i].x, m_currentSheet->curves()[i].y_down);
                m_plotter->graph(j+2)->setName("Down " + m_currentSheet->curves()[i].name);
                m_plotter->graph(j+2)->setPen(m_currentSheet->curves()[i].up_down_pen);
            }
        }
    }

    m_plotter->replot();
    updateMenu();
}
