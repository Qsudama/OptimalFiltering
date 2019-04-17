#include "filter_parameters_widget.h"
#include "src/gui/font_manager.h"

static double const coeff = 0.5;
static double const minMesStep = 0.0001;
static double const step = 0.01;

int setting_specific_realization = 0;

enum FilterFamily {
    discrete,
    continuous,
    continuous_discrete,
    logic_dynamic
};

FilterParametersWidget::FilterParametersWidget(QWidget *parent)
    : QGroupBox(parent)
    , m_updateOn(true)
    , m_parameters(new Core::FilterParameters(50.0, 1.0, 1.0, 0.1, 200, 1, 4, 2))
    , m_currentFiltersFamily(discrete)
{
    setTitle(tr("Параметры фильтрации"));
    loadFonts();
    initControls();
    initLabels();
    connectFieldSignals();
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

void FilterParametersWidget::setRange(QDoubleSpinBox *dsb, double min, double max, double step)
{
    dsb->setMinimum(min);
    dsb->setMaximum(max);
    dsb->setSingleStep(step);
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
    // Терминальное время - maxTime
    m_dsbMaxTime = new QDoubleSpinBox;
    m_dsbMaxTime->setDecimals(2);
    setRange(m_dsbMaxTime, m_parameters->measurementStep(), 300.0, 1.0);
    m_dsbMaxTime->setValue(m_parameters->maxTime());
    m_dsbMaxTime->setFont(m_monotypeFont);
    connect(m_dsbMaxTime, SIGNAL(valueChanged(double)), this, SLOT(onMaxTimeChanged(double)));

    // Интервал между измерениями - measurementStep
    m_dsbMeasurementStep = new QDoubleSpinBox;
    m_dsbMeasurementStep->setDecimals(4);
    setRange(m_dsbMeasurementStep, minMesStep, coeff * m_parameters->maxTime(), step);
    m_dsbMeasurementStep->setValue(m_parameters->measurementStep());
    m_dsbMeasurementStep->setFont(m_monotypeFont);
    connect(m_dsbMeasurementStep, SIGNAL(valueChanged(double)), this, SLOT(onMeasurementStepChanged(double)));

    // Интервал между прогнозами (для непрерывно-дискретных фильтров) - predictionStep
    m_dsbPredictionStep = new QDoubleSpinBox;
    m_dsbPredictionStep->setDecimals(4);
    setRange(m_dsbPredictionStep, m_parameters->integrationStep(), m_parameters->measurementStep());
    m_dsbPredictionStep->setValue(m_parameters->predictionStep());
    m_dsbPredictionStep->setFont(m_monotypeFont);
    connect(m_dsbPredictionStep, SIGNAL(valueChanged(double)), this, SLOT(onPredictionStepChanged(double)));

    // Шаг интегрирования (для непрерывно-дискретных фильтров) - integrationStep
    m_dsbIntegrationStep = new QDoubleSpinBox;
    m_dsbIntegrationStep->setDecimals(4);
    setRange(m_dsbIntegrationStep, 0.0001, m_parameters->predictionStep());
    m_dsbIntegrationStep->setValue(m_parameters->integrationStep());
    m_dsbIntegrationStep->setFont(m_monotypeFont);
    connect(m_dsbIntegrationStep, SIGNAL(valueChanged(double)), this, SLOT(onIntegrationStepChanged(double)));

    // Кол-во прогнозов на интервале (для непрерывно-дискретных фильтров)
    m_sbPredictionCount = new QSpinBox;
    m_sbPredictionCount->setEnabled(false);
    setRange(m_sbPredictionCount, 0, int(m_dsbPredictionStep->maximum() / m_dsbIntegrationStep->minimum()) - 1);
    m_sbPredictionCount->setValue(int(m_parameters->predictionCount()) - 1);
    m_sbPredictionCount->setFont(m_monotypeFont);
    connect(m_sbPredictionCount, SIGNAL(valueChanged(int)), this, SLOT(onPredictionCountChanged(int)));
    // Кратность фильтра (для ФКП) - orderMult
    m_sbOrderMultiplicity = new QSpinBox;
    m_sbOrderMultiplicity->setMinimum(1);
    m_sbOrderMultiplicity->setMaximum(50);
    m_sbOrderMultiplicity->setSingleStep(1);
    m_sbOrderMultiplicity->setValue(int(m_parameters->orderMult()));
    m_sbOrderMultiplicity->setFont(m_monotypeFont);
    connect(m_sbOrderMultiplicity, SIGNAL(valueChanged(int)), this, SLOT(onOrderMultChanged(int)));

    // Количество аргументов в фильтре (дискретные фильтры) - argumentsCount
    m_argumentsCount = new QSpinBox;
    m_argumentsCount->setMinimum(2);
    m_argumentsCount->setMaximum(3);
    m_argumentsCount->setSingleStep(1);
    m_argumentsCount->setValue(int(m_parameters->argumentsCount()));
    m_argumentsCount->setFont(m_monotypeFont);
    connect(m_argumentsCount, SIGNAL(valueChanged(int)), this, SLOT(onArgumentsCountChanged(int)));

    // Размер выборки - sampleSize
    m_sbSampleSize = new QSpinBox;
    static int const singleStepSampleSize = 50;
    m_sbSampleSize->setMinimum(singleStepSampleSize);
    m_sbSampleSize->setMaximum(10000);
    m_sbSampleSize->setSingleStep(singleStepSampleSize);
    m_sbSampleSize->setValue(int(m_parameters->sampleSize()));
    m_sbSampleSize->setFont(m_monotypeFont);

    // Номер конкретной реализации - m_sbSpecificRealization

    m_sbSpecificRealization = new QSpinBox;
    m_sbSpecificRealization->setMinimum(1);
    m_sbSpecificRealization->setMaximum(10000);
    m_sbSpecificRealization->setSingleStep(1);
    m_sbSpecificRealization->setValue(1);
    m_sbSpecificRealization->setFont(m_monotypeFont);

}

void FilterParametersWidget::connectFieldSignals()
{
    connect(m_sbSampleSize, SIGNAL(valueChanged(int)), this, SLOT(onSampleSizeChanged(int)));
    connect(m_sbSpecificRealization, SIGNAL(valueChanged(int)), this, SLOT(onSpecificRealizationChanged(int)));

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

void FilterParametersWidget::initLabels()
{
    m_maxTimeLabel = new QLabel(tr("Терминальное время"));
    m_measurementStepLabel = new QLabel(tr("Интервал между измерениями"));
    m_integrationStepLabel = new QLabel(tr("Шаг интегрирования"));
    m_orderMultiplicityLabel = new QLabel(tr("Число запоминаемых измерений/оценок"));
    m_argumentsCountLabel = new QLabel(tr("Порядок ФМП (не больше размерности X)"));
    m_sampleSizeLabel = new QLabel(tr("Размер выборок"));
    m_specificRealizationLabel = new QLabel(tr("Номер выводимой реализации"));
}

void FilterParametersWidget::initLayouts()
{
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setMargin(GuiConfig::LAYOUT_MARGIN_BIG);
    mainLayout->setSpacing(GuiConfig::LAYOUT_SPACING_BIG);

    mainLayout->addWidget(m_maxTimeLabel, 0, 0);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 0, 1);
    mainLayout->addWidget(new QLabel(tr("T")), 0, 2);
    mainLayout->addWidget(new QLabel("="), 0, 3);
    mainLayout->addWidget(m_dsbMaxTime, 0, 4);

    mainLayout->addWidget(m_measurementStepLabel, 1, 0);
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

    mainLayout->addWidget(m_integrationStepLabel, 4, 0);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 4, 1);
    mainLayout->addWidget(new QLabel(tr("Δt")), 4, 2);
    mainLayout->addWidget(new QLabel("="), 4, 3);
    mainLayout->addWidget(m_dsbIntegrationStep, 4, 4);

    mainLayout->addWidget(m_orderMultiplicityLabel, 5, 0);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 5, 1);
    mainLayout->addWidget(new QLabel(tr("l")), 5, 2);
    mainLayout->addWidget(new QLabel("="), 5, 3);
    mainLayout->addWidget(m_sbOrderMultiplicity, 5, 4);

    mainLayout->addWidget(m_argumentsCountLabel, 6, 0);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 6, 1);
    mainLayout->addWidget(m_argumentsCount, 6, 4);

    mainLayout->addWidget(m_sampleSizeLabel, 7, 0);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 7, 1);
    mainLayout->addWidget(new QLabel(tr("S")), 7, 2);
    mainLayout->addWidget(new QLabel("="), 7, 3);
    mainLayout->addWidget(m_sbSampleSize, 7, 4);

    mainLayout->addWidget(m_specificRealizationLabel, 8, 0);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 8, 1);
    mainLayout->addWidget(new QLabel(tr("N")), 8, 2);
    mainLayout->addWidget(new QLabel("="), 8, 3);
    mainLayout->addWidget(m_sbSpecificRealization, 8, 4);

    mainLayout->addWidget(m_checkFixAll, 9, 0);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 9, 1);
    mainLayout->addWidget(new QLabel(" "), 9, 2);
    mainLayout->addWidget(new QLabel(" "), 9, 3);
    mainLayout->addWidget(m_btnUpdate, 9, 4);

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
    setRange(m_dsbMeasurementStep, minMesStep, coeff * m_parameters->maxTime(), step);
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
//    setRange(m_dsbMeasurementStep, m_dsbPredictionStep->value(), 0.25 * m_dsbMaxTime->value());
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

    m_argumentsCount->setEnabled(true);
    m_sbOrderMultiplicity->setEnabled(true);

    enableLabel(m_argumentsCountLabel, true);
    enableLabel(m_orderMultiplicityLabel, true);

    enableLabel(m_integrationStepLabel, false); // включено только для непрерывно-дискретных
    m_dsbIntegrationStep->setEnabled(false); // включено только для непрерывно-дискретных

    if (m_currentFiltersFamily == discrete || m_currentFiltersFamily == logic_dynamic) {
        m_dsbIntegrationStep->setEnabled(false);
        m_radioPredictionCount->setEnabled(false);
        m_radioPredictionStep->setEnabled(false);
        m_dsbPredictionStep->setEnabled(false);
        m_sbPredictionCount->setEnabled(false);

        if (m_currentFiltersFamily == logic_dynamic) {
            enableLabel(m_argumentsCountLabel, false);
            m_argumentsCount->setEnabled(false);
        }
    } else if (m_currentFiltersFamily == continuous) {
        m_radioPredictionCount->setEnabled(false);
        m_radioPredictionStep->setEnabled(false);
        m_dsbPredictionStep->setEnabled(false);
        m_sbPredictionCount->setEnabled(false);

        enableLabel(m_orderMultiplicityLabel, false);
        m_sbOrderMultiplicity->setEnabled(false);
    } else if (m_currentFiltersFamily == continuous_discrete) {
            enableLabel(m_integrationStepLabel, true);
            m_dsbIntegrationStep->setEnabled(true);
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

void FilterParametersWidget::onArgumentsCountChanged(int value)
{
    m_parameters->setArgumentsCount(Uint(value));
}

void FilterParametersWidget::onSampleSizeChanged(int value)
{
    m_parameters->setSampleSize(uint(value));
}

void FilterParametersWidget::onSpecificRealizationChanged(int value)
{
    if (setting_specific_realization > 1) { // предохранитель от внутренней рекурсии с вызовом алерта
        return;
    }
    setting_specific_realization++;
    bool setting = m_parameters->setSpecificRealization(uint(value));
    if (!setting) {
        m_sbSpecificRealization->setValue(m_parameters->sampleSize());
    }
    setting_specific_realization--;
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


// private update labels

void FilterParametersWidget::enableLabel(QLabel *label, bool enable)
{
    QPalette palette = label->palette();
    if (!enable) {
        palette.setColor(label->foregroundRole(), Qt::darkGray);
    } else {
        palette.setColor(label->foregroundRole(), Qt::black);
    }
    label->setPalette(palette);
}
