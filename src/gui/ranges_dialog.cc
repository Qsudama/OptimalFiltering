#include "ranges_dialog.h"
#include "src/gui/font_manager.h"


RangesDialog::RangesDialog(const Math::Vector2 &x, const Math::Vector2 &y, QWidget *parent)
    : QDialog(parent)
{
    initControls(x, y);
    initPlotter();
    loadFonts();
    initLayouts();
}

RangesDialog::~RangesDialog()
{
}

void RangesDialog::initControls(const Math::Vector2 &x, const Math::Vector2 &y)
{
    m_dsbXMin = new QDoubleSpinBox;
    m_dsbXMin->setMinimum(-100000.0);
    m_dsbXMin->setMaximum(100000.0);
    m_dsbXMin->setDecimals(3);
    m_dsbXMin->setSingleStep(1.0);
    m_dsbXMin->setValue(x[0]);
    connect(m_dsbXMin, SIGNAL(valueChanged(double)), this, SLOT(onXMinChanged(double)));

    m_dsbXMax = new QDoubleSpinBox;
    m_dsbXMax->setMinimum(-100000.0);
    m_dsbXMax->setMaximum(100000.0);
    m_dsbXMax->setDecimals(3);
    m_dsbXMax->setSingleStep(1.0);
    m_dsbXMax->setValue(x[1]);
    connect(m_dsbXMax, SIGNAL(valueChanged(double)), this, SLOT(onXMaxChanged(double)));

    m_dsbYMin = new QDoubleSpinBox;
    m_dsbYMin->setMinimum(-100000.0);
    m_dsbYMin->setMaximum(100000.0);
    m_dsbYMin->setDecimals(3);
    m_dsbYMin->setSingleStep(1.0);
    m_dsbYMin->setValue(y[0]);
    connect(m_dsbYMin, SIGNAL(valueChanged(double)), this, SLOT(onYMinChanged(double)));

    m_dsbYMax = new QDoubleSpinBox;
    m_dsbYMax->setMinimum(-100000.0);
    m_dsbYMax->setMaximum(100000.0);
    m_dsbYMax->setDecimals(3);
    m_dsbYMax->setSingleStep(1.0);
    m_dsbYMax->setValue(y[1]);
    connect(m_dsbYMax, SIGNAL(valueChanged(double)), this, SLOT(onYMaxChanged(double)));

    m_buttonBox = new QDialogButtonBox;
    m_buttonBox->setOrientation(Qt::Horizontal);
    m_buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(onAcceptedClicked()));
    connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(onRejectedClicked()));
}

void RangesDialog::initPlotter()
{
    m_plotter = new QCustomPlot;
    m_plotter->xAxis->setLabel(tr("X"));
    m_plotter->yAxis->setLabel(tr("Y"));
    m_plotter->xAxis->setRange(m_dsbXMin->value(), m_dsbXMax->value());
    m_plotter->yAxis->setRange(m_dsbYMin->value(), m_dsbYMax->value());
    m_plotter->yAxis->setLabelPadding(m_plotter->xAxis->labelPadding() + 2);
    m_plotter->axisRect()->setupFullAxesBox();
    m_plotter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_plotter->replot();
}

void RangesDialog::loadFonts()
{
    this->setFont(FontManager::instance().regular(GuiConfig::FONT_SIZE_NORMAL));

    QFont monoFont = FontManager::instance().mono(GuiConfig::FONT_SIZE_NORMAL);

    m_dsbXMin->setFont(monoFont);
    m_dsbXMax->setFont(monoFont);
    m_dsbYMin->setFont(monoFont);
    m_dsbYMax->setFont(monoFont);
}

void RangesDialog::initLayouts()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(GuiConfig::LAYOUT_MARGIN_BIG);
    mainLayout->setSpacing(GuiConfig::LAYOUT_SPACING_BIG);

    QFrame *frame = new QFrame;
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    frame->setLineWidth(1);

    QHBoxLayout *frameLayout = new QHBoxLayout;
    frameLayout->setMargin(GuiConfig::LAYOUT_MARGIN_SMALL);
    frameLayout->setSpacing(GuiConfig::LAYOUT_SPACING_SMALL);
    frameLayout->addWidget(m_plotter);

    frame->setLayout(frameLayout);
    mainLayout->addWidget(frame);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setMargin(GuiConfig::LAYOUT_MARGIN_SMALL);
    gridLayout->setSpacing(GuiConfig::LAYOUT_SPACING_NORMAL);

    gridLayout->addWidget(new QLabel(tr(" Ось X:")), 0, 0);
    gridLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 0, 1);
    gridLayout->addWidget(new QLabel(tr("от")), 0, 2);
    gridLayout->addWidget(m_dsbXMin, 0, 3);
    gridLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 0, 4);
    gridLayout->addWidget(new QLabel(tr("до")), 0, 5);
    gridLayout->addWidget(m_dsbXMax, 0, 6);

    gridLayout->addWidget(new QLabel(tr(" Ось Y:")), 1, 0);
    gridLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding), 1, 1);
    gridLayout->addWidget(new QLabel(tr("от")), 1, 2);
    gridLayout->addWidget(m_dsbYMin, 1, 3);
    gridLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding), 1, 4);
    gridLayout->addWidget(new QLabel(tr("до")), 1, 5);
    gridLayout->addWidget(m_dsbYMax, 1, 6);

    mainLayout->addLayout(gridLayout);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setMargin(GuiConfig::LAYOUT_MARGIN_SMALL);
    btnLayout->setSpacing(GuiConfig::LAYOUT_SPACING_NORMAL);

    btnLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
    btnLayout->addWidget(m_buttonBox);

    mainLayout->addLayout(btnLayout);

    this->setLayout(mainLayout);

    int dsbWidth    = QFontMetrics(m_dsbXMin->font()).width("-100000,000") + 5;
    int labelsWidth = QFontMetrics(m_dsbXMin->font()).width(tr("Ось Y: от до"));
    int minWidth    = 2 * dsbWidth + labelsWidth + 2 * GuiConfig::LAYOUT_MARGIN_BIG + 5 * GuiConfig::LAYOUT_SPACING_BIG;

    this->setMinimumWidth(minWidth);
    this->setMinimumHeight(minWidth);
}

void RangesDialog::onYMaxChanged(double yMax)
{
    m_dsbYMin->setMinimum(yMax - 150000.1);
    m_dsbYMin->setMaximum(yMax - 0.1);

    if (m_dsbYMin->value() >= yMax) {
        m_dsbYMin->setValue(yMax - 0.1);
    }
    m_plotter->yAxis->setRange(m_dsbYMin->value(), m_dsbYMax->value());
    m_plotter->replot();
}

void RangesDialog::onYMinChanged(double yMin)
{
    m_dsbYMax->setMaximum(yMin + 150000.1);
    m_dsbYMax->setMinimum(yMin + 0.1);

    if (m_dsbYMax->value() <= yMin) {
        m_dsbYMax->setValue(yMin + 0.1);
    }
    m_plotter->yAxis->setRange(m_dsbYMin->value(), m_dsbYMax->value());
    m_plotter->replot();
}

void RangesDialog::onXMaxChanged(double xMax)
{
    m_dsbXMin->setMinimum(xMax - 150000.1);
    m_dsbXMin->setMaximum(xMax - 0.1);

    if (m_dsbXMin->value() >= xMax) {
        m_dsbXMin->setValue(xMax - 0.1);
    }
    m_plotter->xAxis->setRange(m_dsbXMin->value(), m_dsbXMax->value());
    m_plotter->replot();
}

void RangesDialog::onXMinChanged(double xMin)
{
    m_dsbXMax->setMaximum(xMin + 150000.1);
    m_dsbXMax->setMinimum(xMin + 0.1);

    if (m_dsbXMax->value() <= xMin) {
        m_dsbXMax->setValue(xMin + 0.1);
    }
    m_plotter->xAxis->setRange(m_dsbXMin->value(), m_dsbXMax->value());
    m_plotter->replot();
}

void RangesDialog::onAcceptedClicked()
{
    Math::Vector2 x, y;

    x[0] = m_dsbXMin->value();
    x[1] = m_dsbXMax->value();
    y[0] = m_dsbYMin->value();
    y[1] = m_dsbYMax->value();

    emit rangesChanged(x, y);

    close();
}

void RangesDialog::onRejectedClicked()
{
    close();
}
