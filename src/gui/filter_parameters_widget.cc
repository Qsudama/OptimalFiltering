#include "filter_parameters_widget.h"
#include "src/gui/font_manager.h"


FilterParametersWidget::FilterParametersWidget(QWidget *parent)
    : QGroupBox(parent)
    , m_updateOn(true)
    , m_parameters(new Core::FilterParameters(200.0, 1.0, 1.0, 0.1, 200, 1))
    , m_currentFiltersFamily(0)
{
    setTitle(tr("Параметры фильтрации"));
    loadFonts();
    initControls();
    initLayouts();
    onFixAllToggled(m_checkFixAll->isChecked());
}

FilterParametersWidget::~FilterParametersWidget()
{
}

void FilterParametersWidget::loadFonts()
{
    m_regularFont  = FontManager::instance().regular(GuiConfig::FONT_SIZE_NORMAL);
    m_monotypeFont = FontManager::instance().mono(GuiConfig::FONT_SIZE_NORMAL);
    this->setFont(m_regularFont);
}

int FilterParametersWidget::computeMinimumWidth()
{
    QDoubleSpinBox *dsbTemp = new QDoubleSpinBox(this);
    dsbTemp->setFont(m_monotypeFont);
    int h = dsbTemp->height();
    int w1, w2, w3, w4;
    delete dsbTemp;

    w1 = h + QFontMetrics(m_regularFont).width(tr("Количество прогнозов на интервале δt"));

    w2 = QFontMetrics(m_regularFont).width("δτ");
    w2 = qMax(w2, QFontMetrics(m_regularFont).width("δt"));
    w2 = qMax(w2, QFontMetrics(m_regularFont).width("Δt"));

    w3 = QFontMetrics(m_regularFont).width("=");
    w4 = h + QFontMetrics(m_monotypeFont).width("499.9999");

    return w1 + w2 + w3 + w4;
}

void FilterParametersWidget::setRange(QDoubleSpinBox *dsb, double min, double max)
{
    dsb->setMinimum(min);
    dsb->setMaximum(max);
    dsb->setSingleStep(0.01 * (max - min));
}

void FilterParametersWidget::setRange(QSpinBox *sb, int min, int max)
{
    sb->setMinimum(min);
    sb->setMaximum(max);
    sb->setSingleStep(1);
}

void FilterParametersWidget::initControls()
{
    m_dsbMaxTime = new QDoubleSpinBox;
    m_dsbMaxTime->setDecimals(2);
    setRange(m_dsbMaxTime, m_parameters->measurementStep(), 500.0);
    m_dsbMaxTime->setValue(m_parameters->maxTime());
    m_dsbMaxTime->setFont(m_monotypeFont);
    connect(m_dsbMaxTime, SIGNAL(valueChanged(double)), this, SLOT(onMaxTimeChanged(double)));

    m_dsbMeasurementStep = new QDoubleSpinBox;
    m_dsbMeasurementStep->setDecimals(4);
    setRange(m_dsbMeasurementStep, m_parameters->predictionStep(), 0.25 * m_parameters->maxTime());
    m_dsbMeasurementStep->setValue(m_parameters->measurementStep());
    m_dsbMeasurementStep->setFont(m_monotypeFont);
    connect(m_dsbMeasurementStep, SIGNAL(valueChanged(double)), this, SLOT(onMeasurementStepChanged(double)));

    m_dsbPredictionStep = new QDoubleSpinBox;
    m_dsbPredictionStep->setDecimals(4);
    setRange(m_dsbPredictionStep, m_parameters->integrationStep(), m_parameters->measurementStep());
    m_dsbPredictionStep->setValue(m_parameters->predictionStep());
    m_dsbPredictionStep->setFont(m_monotypeFont);
    connect(m_dsbPredictionStep, SIGNAL(valueChanged(double)), this, SLOT(onPredictionStepChanged(double)));

    m_dsbIntegrationStep = new QDoubleSpinBox;
    m_dsbIntegrationStep->setDecimals(4);
    setRange(m_dsbIntegrationStep, 0.0001, m_parameters->predictionStep());
    m_dsbIntegrationStep->setValue(m_parameters->integrationStep());
    m_dsbIntegrationStep->setFont(m_monotypeFont);
    connect(m_dsbIntegrationStep, SIGNAL(valueChanged(double)), this, SLOT(onIntegrationStepChanged(double)));

    m_sbPredictionCount = new QSpinBox;
    m_sbPredictionCount->setEnabled(false);
    setRange(m_sbPredictionCount, 0, int(m_dsbPredictionStep->maximum() / m_dsbIntegrationStep->minimum()) - 1);
    m_sbPredictionCount->setValue(int(m_parameters->predictionCount()) - 1);
    m_sbPredictionCount->setFont(m_monotypeFont);
    connect(m_sbPredictionCount, SIGNAL(valueChanged(int)), this, SLOT(onPredictionCountChanged(int)));

    m_sbOrderMultiplicity = new QSpinBox;
    m_sbOrderMultiplicity->setMinimum(1);
    m_sbOrderMultiplicity->setMaximum(10);
    m_sbOrderMultiplicity->setSingleStep(1);
    m_sbOrderMultiplicity->setValue(int(m_parameters->orderMult()));
    m_sbOrderMultiplicity->setFont(m_monotypeFont);
    connect(m_sbOrderMultiplicity, SIGNAL(valueChanged(int)), this, SLOT(onOrderMultChanged(int)));

    m_sbSampleSize = new QSpinBox;
    m_sbSampleSize->setMinimum(50);
    m_sbSampleSize->setMaximum(5000);
    m_sbSampleSize->setSingleStep(50);
    m_sbSampleSize->setValue(int(m_parameters->sampleSize()));
    m_sbSampleSize->setFont(m_monotypeFont);
    connect(m_sbSampleSize, SIGNAL(valueChanged(int)), this, SLOT(onSampleSizeChanged(int)));

    m_radioPredictionStep = new QRadioButton(tr("Интервал между прогнозами"));
    m_radioPredictionStep->setChecked(true);
    connect(m_radioPredictionStep, SIGNAL(toggled(bool)), this, SLOT(onPredictionStepToggled(bool)));

    m_radioPredictionCount = new QRadioButton(tr("Количество прогнозов на интервале δt"));
    m_radioPredictionCount->setChecked(false);
    connect(m_radioPredictionCount, SIGNAL(toggled(bool)), this, SLOT(onPredictionCountToggled(bool)));

    m_checkFixAll = new QCheckBox(tr("Зафиксировать параметры"));
    m_checkFixAll->setChecked(false);
    connect(m_checkFixAll, SIGNAL(toggled(bool)), this, SLOT(onFixAllToggled(bool)));

    m_btnUpdate = new QPushButton(tr("Обновить"));
    connect(m_btnUpdate, SIGNAL(clicked()), this, SLOT(onUpdateValues()));
}

void FilterParametersWidget::initLayouts()
{
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setMargin(GuiConfig::LAYOUT_MARGIN_BIG);
    mainLayout->setSpacing(GuiConfig::LAYOUT_SPACING_BIG);

    mainLayout->addWidget(new QLabel(tr("Терминальное время")), 0, 0);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 0, 1);
    mainLayout->addWidget(new QLabel(tr("T")), 0, 2);
    mainLayout->addWidget(new QLabel("="), 0, 3);
    mainLayout->addWidget(m_dsbMaxTime, 0, 4);

    mainLayout->addWidget(new QLabel(tr("Интервал между измерениями")), 1, 0);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 1, 1);
    mainLayout->addWidget(new QLabel(tr("δt")), 1, 2);
    mainLayout->addWidget(new QLabel("="), 1, 3);
    mainLayout->addWidget(m_dsbMeasurementStep, 1, 4);

    mainLayout->addWidget(m_radioPredictionStep, 2, 0);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 2, 1);
    mainLayout->addWidget(new QLabel(tr("δτ")), 2, 2);
    mainLayout->addWidget(new QLabel("="), 2, 3);
    mainLayout->addWidget(m_dsbPredictionStep, 2, 4);

    mainLayout->addWidget(m_radioPredictionCount, 3, 0);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 3, 1);
    mainLayout->addWidget(new QLabel(tr("L")), 3, 2);
    mainLayout->addWidget(new QLabel("="), 3, 3);
    mainLayout->addWidget(m_sbPredictionCount, 3, 4);

    mainLayout->addWidget(new QLabel(tr("Шаг интегрирования")), 4, 0);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 4, 1);
    mainLayout->addWidget(new QLabel(tr("Δt")), 4, 2);
    mainLayout->addWidget(new QLabel("="), 4, 3);
    mainLayout->addWidget(m_dsbIntegrationStep, 4, 4);

    mainLayout->addWidget(new QLabel(tr("Кратность порядка фильтра (для ФПП)")), 5, 0);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 5, 1);
    mainLayout->addWidget(new QLabel(tr("l")), 5, 2);
    mainLayout->addWidget(new QLabel("="), 5, 3);
    mainLayout->addWidget(m_sbOrderMultiplicity, 5, 4);

    mainLayout->addWidget(new QLabel(tr("Размер выборок")), 6, 0);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 6, 1);
    mainLayout->addWidget(new QLabel(tr("S")), 6, 2);
    mainLayout->addWidget(new QLabel("="), 6, 3);
    mainLayout->addWidget(m_sbSampleSize, 6, 4);

    mainLayout->addWidget(m_checkFixAll, 7, 0);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 7, 1);
    mainLayout->addWidget(new QLabel(" "), 7, 2);
    mainLayout->addWidget(new QLabel(" "), 7, 3);
    mainLayout->addWidget(m_btnUpdate, 7, 4);

    this->setLayout(mainLayout);

    int minWidth = computeMinimumWidth();
    minWidth += 2 * GuiConfig::LAYOUT_MARGIN_BIG + 3 * GuiConfig::LAYOUT_SPACING_BIG;
    this->setMinimumWidth(minWidth);
}

void FilterParametersWidget::onFiltersFamilyChanged(int index)
{
    m_currentFiltersFamily = index;
    onFixAllToggled(m_checkFixAll->isChecked());
}

Core::PtrFilterParameters FilterParametersWidget::parameters()
{
    return m_parameters;
}

void FilterParametersWidget::onMaxTimeChanged(double value)
{
    if (!m_updateOn) {
        return;
    }
    m_parameters->setMaxTime(value);
    setRange(m_dsbMeasurementStep, m_dsbPredictionStep->value(), 0.25 * m_dsbMaxTime->value());
}

void FilterParametersWidget::onMeasurementStepChanged(double value)
{
    if (!m_updateOn) {
        return;
    }
    m_parameters->setMeasurementStep(value);
    setRange(m_dsbPredictionStep, m_dsbIntegrationStep->value(), m_dsbMeasurementStep->value());
    setRange(m_sbPredictionCount, 0, int(m_dsbPredictionStep->maximum() / m_dsbIntegrationStep->minimum()) - 1);

    m_updateOn = false;
    //если L = const :
    if (m_radioPredictionCount->isChecked()) {
        int    L    = m_sbPredictionCount->value();
        double dtau = value / double(L + 1);
        m_parameters->setPredictionStep(dtau);
        m_dsbPredictionStep->setValue(dtau);
    } else {
        double dtau = m_dsbPredictionStep->value();
        int    L    = int(value / dtau) - 1;
        if (value - (L + 1) * dtau >= 0.5 * dtau) {
            ++L;
        }
        m_sbPredictionCount->setValue(L);
    }
    m_updateOn = true;
}

void FilterParametersWidget::onPredictionStepChanged(double value)
{
    if (!m_updateOn) {
        return;
    }
    m_parameters->setPredictionStep(value);
    setRange(m_dsbIntegrationStep, 0.0001, m_dsbPredictionStep->value());
    setRange(m_dsbPredictionStep, m_dsbIntegrationStep->value(), m_dsbMeasurementStep->value());
    setRange(m_dsbMeasurementStep, m_dsbPredictionStep->value(), 0.25 * m_dsbMaxTime->value());
    setRange(m_sbPredictionCount, 0, int(m_dsbPredictionStep->maximum() / m_dsbIntegrationStep->minimum()) - 1);

    //вместе с stepPrediction корректируется maxTime:
    m_updateOn = false;
    int L      = int(m_dsbMeasurementStep->value() / value) - 1;
    if (m_dsbMeasurementStep->value() - (L + 1) * value >= 0.5 * value) {
        ++L;
    }
    m_sbPredictionCount->setValue(L);
    m_updateOn = true;
}

void FilterParametersWidget::onPredictionCountChanged(int value)
{
    if (m_updateOn) {
        m_dsbPredictionStep->setValue(m_dsbMeasurementStep->value() / double(value + 1));
    }
}

void FilterParametersWidget::onIntegrationStepChanged(double value)
{
    m_parameters->setIntegrationStep(value);
    setRange(m_dsbPredictionStep, m_dsbIntegrationStep->value(), m_dsbMeasurementStep->value());
    setRange(m_sbPredictionCount, 0, int(m_dsbPredictionStep->maximum() / m_dsbIntegrationStep->minimum()) - 1);
}

void FilterParametersWidget::onFixAllToggled(bool checked)
{
    m_dsbMaxTime->setEnabled(!checked);
    m_dsbMeasurementStep->setEnabled(!checked);
    m_dsbIntegrationStep->setEnabled(!checked);
    m_sbOrderMultiplicity->setEnabled(!checked);
    m_sbSampleSize->setEnabled(!checked);
    m_radioPredictionCount->setEnabled(!checked);
    m_radioPredictionStep->setEnabled(!checked);
    m_dsbPredictionStep->setEnabled(!checked && m_radioPredictionStep->isChecked());
    m_sbPredictionCount->setEnabled(!checked && m_radioPredictionCount->isChecked());

    if (m_currentFiltersFamily == 0) { // дискретные
        m_dsbIntegrationStep->setEnabled(false);
        m_radioPredictionCount->setEnabled(false);
        m_radioPredictionStep->setEnabled(false);
        m_dsbPredictionStep->setEnabled(false);
        m_sbPredictionCount->setEnabled(false);
    } else if (m_currentFiltersFamily == 1) { // непрерывные
        m_radioPredictionCount->setEnabled(false);
        m_radioPredictionStep->setEnabled(false);
        m_dsbPredictionStep->setEnabled(false);
        m_sbPredictionCount->setEnabled(false);
    }
}
void FilterParametersWidget::onPredictionStepToggled(bool checked)
{
    m_sbPredictionCount->setEnabled(!checked);
}

void FilterParametersWidget::onPredictionCountToggled(bool checked)
{
    m_dsbPredictionStep->setEnabled(!checked);
}
void FilterParametersWidget::onOrderMultChanged(int value)
{
    m_parameters->setOrderMult(Uint(value));
}

void FilterParametersWidget::onSampleSizeChanged(int value)
{
    m_parameters->setSampleSize(uint(value));
}

void FilterParametersWidget::onUpdateValues()
{
    m_updateOn = false;
    m_dsbMaxTime->setValue(m_parameters->maxTime());
    m_dsbMeasurementStep->setValue(m_parameters->measurementStep());
    m_dsbPredictionStep->setValue(m_parameters->predictionStep());
    m_dsbIntegrationStep->setValue(m_parameters->integrationStep());
    m_sbPredictionCount->setValue(int(m_parameters->predictionCount()) - 1);
    m_updateOn = true;
}
