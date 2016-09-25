#ifndef FILTERPARAMETERSWIDGET_H
#define FILTERPARAMETERSWIDGET_H

#include "src/core/filter_parameters.h"
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QSpacerItem>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>


class SingleParamHBox : public QHBoxLayout
{

public:
    SingleParamHBox(QWidget *w1, QWidget *w2, QWidget *w3, QWidget *w4, int width1, int width2, int width3, int width4);
};


class FilterParametersWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit FilterParametersWidget(QWidget *parent = nullptr);
    ~FilterParametersWidget();

    Core::PtrFilterParameters parameters();


private slots:
    void onMaxTimeChanged(double arg1);
    void onMeasurementStepChanged(double arg1);
    void onPredictionStepChanged(double arg1);
    void onPredictionCountChanged(int arg1);
    void onIntegrationStepChanged(double arg1);
    void onFixAllToggled(bool checked);
    void onPredictionStepToggled(bool checked);
    void onPredictionCountToggled(bool checked);
    void onOrderMultChanged(int arg1);
    void onSampleSizeChanged(int arg1);
    void onUpdateValues();


private:
    QDoubleSpinBox *m_dsbMaxTime;
    QDoubleSpinBox *m_dsbMeasurementStep;
    QDoubleSpinBox *m_dsbPredictionStep;
    QSpinBox *      m_sbPredictionCount;
    QDoubleSpinBox *m_dsbIntegrationStep;
    QSpinBox *      m_sbOrderMultiplicity;
    QSpinBox *      m_sbSampleSize;
    QRadioButton *  m_radioPredictionStep;
    QRadioButton *  m_radioPredictionCount;
    QCheckBox *     m_checkFixAll;
    QPushButton *   m_btnUpdate;

    bool                      m_updateOn;
    Core::PtrFilterParameters m_parameters;

    QFont m_regularFont;
    QFont m_monotypeFont;
    QFont m_labelsFont;
    QFont m_spinBoxesFont;

    void setRange(QDoubleSpinBox *dsb, double min, double max);
    void setRange(QSpinBox *sb, int min, int max);
    void loadFonts();
    void computeSizes(int &w1, int &w2, int &w3, int &w4);
    void initLayouts();
    void initControls();
};


#endif // FILTERPARAMETERSWIDGET_H
