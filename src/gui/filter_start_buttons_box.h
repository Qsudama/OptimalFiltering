#ifndef FILTERSTARTBUTTONSBOX_H
#define FILTERSTARTBUTTONSBOX_H

#include "src/filters/filters.h"
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QTabWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QWidget>


class FilterStartButtonsBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit FilterStartButtonsBox(QWidget *parent = nullptr);
    ~FilterStartButtonsBox();


signals:
    void start(Core::FILTER_TYPE ftype, Core::APPROX_TYPE atype, Filters::FILTER_ID id);


private slots:
    void onBtnContinuousDiscreteGaussAofClicked();
    void onBtnContinuousDiscreteLinearAofClicked();
    void onBtnContinuousDiscreteGaussFosClicked();
    void onBtnContinuousDiscreteLinearFosClicked();
    void onBtnContinuousDiscreteGaussDfosClicked();
    void onBtnContinuousDiscreteLinearDfosClicked();
    void onBtnContinuousDiscreteGaussDfosboClicked();
    void onBtnContinuousDiscreteLinearDfosboClicked();
    void onBtnContinuousGaussAofClicked();
    void onBtnContinuousLinearAofClicked();
    void onBtnContinuousGaussFosClicked();
    void onBtnContinuousLinearFosClicked();
    void onBtnDiscreteGaussAofClicked();
    void onBtnDiscreteLinearAofClicked();
    void onBtnDiscreteLinearFosClicked();
    void onBtnDiscreteGaussFosClicked();
    void onBtnDiscreteLinearMfosClicked();
    void onBtnDiscreteGaussMfosClicked();


private:
    QPushButton *m_btnContinuousDiscreteGaussAof;
    QPushButton *m_btnContinuousDiscreteLinearAof;
    QPushButton *m_btnContinuousDiscreteGaussFos;
    QPushButton *m_btnContinuousDiscreteLinearFos;
    QPushButton *m_btnContinuousDiscreteGaussDfos;
    QPushButton *m_btnContinuousDiscreteLinearDfos;
    QPushButton *m_btnContinuousDiscreteGaussDfosbo;
    QPushButton *m_btnContinuousDiscreteLinearDfosbo;
    QPushButton *m_btnContinuousGaussAof;
    QPushButton *m_btnContinuousLinearAof;
    QPushButton *m_btnContinuousGaussFos;
    QPushButton *m_btnContinuousLinearFos;
    QPushButton *m_btnDiscreteGaussAof;
    QPushButton *m_btnDiscreteLinearAof;
    QPushButton *m_btnDiscreteLinearFos;
    QPushButton *m_btnDiscreteGaussFos;
    QPushButton *m_btnDiscreteLinearMfos;
    QPushButton *m_btnDiscreteGaussMfos;

    void loadFonts();
    void initLayouts();
    void initControls();
    void computeSizes();
};


#endif // FILTERSTARTBUTTONSBOX_H
