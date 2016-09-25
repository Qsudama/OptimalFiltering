#ifndef RANGES_DIALOG_H
#define RANGES_DIALOG_H

#include "src/math/matrix.h"
#include "third_party/qcustomplot/qcustomplot.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFontMetrics>
#include <climits>


class RangesDialog : public QDialog
{
    Q_OBJECT

public:
    RangesDialog(const Math::Vector2 &x, const Math::Vector2 &y, QWidget *parent = nullptr);
    ~RangesDialog();


private slots:
    void onYMaxChanged(double yMax);
    void onYMinChanged(double yMin);
    void onXMaxChanged(double xMax);
    void onXMinChanged(double xMin);
    void onAcceptedClicked();
    void onRejectedClicked();


signals:
    void rangesChanged(Math::Vector2 x, Math::Vector2 y);


private:
    QDoubleSpinBox *  m_dsbXMin;
    QDoubleSpinBox *  m_dsbXMax;
    QDoubleSpinBox *  m_dsbYMin;
    QDoubleSpinBox *  m_dsbYMax;
    QCustomPlot *     m_plotter;
    QDialogButtonBox *m_buttonBox;

    void loadFonts();
    void initControls(const Math::Vector2 &x, const Math::Vector2 &y);
    void initPlotter();
    void initLayouts();
};


#endif // RANGES_DIALOG_H
