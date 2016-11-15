#ifndef MATRIXWIDGET_H
#define MATRIXWIDGET_H

#include "src/math/matrix.h"
#include <QDoubleSpinBox>
#include <QWidget>


class MatrixWidget : public QWidget
{
    Q_OBJECT

public:
    MatrixWidget(const Math::Matrix &matrix, bool onlyPositive = false, bool symmetric = false,
                 QWidget *parent = nullptr);
    MatrixWidget(long rows, long cols = 1, bool onlyPositive = false, bool symmetric = false,
                 QWidget *parent = nullptr);

    const Math::Matrix &matrix() const;


private slots:
    void onValueChanged();


private:
    void initControls();
    void initLayouts();


private:
    bool         m_onlyPositive;
    bool         m_symmetric;
    Math::Matrix m_matrix;

    QVector<QVector<QDoubleSpinBox *>> m_boxes;
};

#endif // MATRIXWIDGET_H
