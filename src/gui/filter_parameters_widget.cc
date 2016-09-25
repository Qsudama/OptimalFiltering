#include "filter_parameters_widget.h"
#include "src/gui/font_manager.h"


SingleParamHBox::SingleParamHBox(QWidget *w1, QWidget *w2, QWidget *w3, QWidget *w4, int width1, int width2, int width3,
                                 int width4)
{
    setSpacing(5);
    setMargin(0);

    w1->setMinimumWidth(width1);
    w1->setMaximumWidth(width1);

    w2->setMinimumWidth(width2);
    w2->setMaximumWidth(width2);

    w3->setMinimumWidth(width3);
    w3->setMaximumWidth(width3);

    w4->setMinimumWidth(width4);
    w4->setMaximumWidth(width4);

    addWidget(w1);
    addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
    addWidget(w2);
    addWidget(w3);
    addWidget(w4);
}


FilterParametersWidget::FilterParametersWidget(QWidget *parent)
    : QGroupBox(parent)
    , m_updateOn(true)
    , m_parameters(new Core::FilterParameters(100.0, 1.0, 0.5, 0.1, 200, 1))
{
    setTitle(tr("Параметры фильтрации"));
    loadFonts();
    initControls();
    initLayouts();
    // onUpdateValues();
}

FilterParametersWidget::~FilterParametersWidget()
{
}

void FilterParametersWidget::loadFonts()
{
    m_regularFont  = FontManager::instance().regular(9);
    m_monotypeFont = FontManager::instance().mono(9);
    this->setFont(m_regularFont);
}

void FilterParametersWidget::computeSizes(int &w1, int &w2, int &w3, int &w4)
{
    QDoubleSpinBox *dsbTemp = new QDoubleSpinBox(this);
    dsbTemp->setFont(m_monotypeFont);
    int h = dsbTemp->height();
    delete dsbTemp;

    w1 = h + QFontMetrics(m_regularFont).width(tr("Количество прогнозов на интервале δt"));

    w2 = QFontMetrics(m_regularFont).width("δτ");
    w2 = qMax(w2, QFontMetrics(m_regularFont).width("δt"));
    w2 = qMax(w2, QFontMetrics(m_regularFont).width("Δt"));

    w3 = QFontMetrics(m_regularFont).width("=");
    w4 = h + QFontMetrics(m_monotypeFont).width("499.9999");
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
    int w1, w2, w3, w4;
    computeSizes(w1, w2, w3, w4);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(5);
    mainLayout->setSpacing(5);

    mainLayout->addLayout(new SingleParamHBox(new QLabel(tr("Терминальное время")), new QLabel(tr("T")),
                                              new QLabel("="), m_dsbMaxTime, w1, w2, w3, w4));

    mainLayout->addLayout(new SingleParamHBox(new QLabel(tr("Интервал между измерениями")), new QLabel(tr("δt")),
                                              new QLabel("="), m_dsbMeasurementStep, w1, w2, w3, w4));

    mainLayout->addLayout(new SingleParamHBox(m_radioPredictionStep, new QLabel(tr("δτ")), new QLabel("="),
                                              m_dsbPredictionStep, w1, w2, w3, w4));

    mainLayout->addLayout(new SingleParamHBox(m_radioPredictionCount, new QLabel(tr("L")), new QLabel("="),
                                              m_sbPredictionCount, w1, w2, w3, w4));

    mainLayout->addLayout(new SingleParamHBox(new QLabel(tr("Шаг интегрирования")), new QLabel(tr("Δt")),
                                              new QLabel("="), m_dsbIntegrationStep, w1, w2, w3, w4));

    mainLayout->addLayout(new SingleParamHBox(new QLabel(tr("Кратность порядка фильтра (для ФПП)")),
                                              new QLabel(tr("l")), new QLabel("="), m_sbOrderMultiplicity, w1, w2, w3,
                                              w4));

    mainLayout->addLayout(new SingleParamHBox(new QLabel(tr("Размер выборок")), new QLabel(tr("S")), new QLabel("="),
                                              m_sbSampleSize, w1, w2, w3, w4));

    mainLayout->addLayout(
        new SingleParamHBox(m_checkFixAll, new QLabel(" "), new QLabel(" "), m_btnUpdate, w1, w2, w3, w4));

    this->setLayout(mainLayout);
    this->setMinimumWidth(this->layout()->margin() * 2 + 15 + w1 + w2 + w3 + w4);
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
    m_parameters->setOrderMult(ulong(value));
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
