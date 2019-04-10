#ifndef MATRIXWIDGET_H
#define MATRIXWIDGET_H

#include "src/gui/gui_config.h"
#include "src/math/matrix.h"
#include "src/gui/custom_double_spin_box.h"
#include <QDoubleSpinBox>
#include <QWidget>


//! \brief Класс для отображения / изменения матриц (или векторов).

class MatrixWidget : public QWidget
{
    Q_OBJECT

public:
    /*!
     \brief Конструктор.

     \param matrix - матрица, по которой инициализируются элементы.
     \param onlyPositive - запрещает ввод отрицательных значений.
     \param symmetric - заставляет дублировать значение в симметричной ячейке.
    */
    MatrixWidget(const Math::Matrix &matrix, bool onlyPositive = false, bool symmetric = false,
                 QWidget *parent = nullptr);
    /*!
     \brief  Конструктор.

     \param rows - количество строк матрицы.
     \param cols - количество столбцов матрицы.
     \param matrix - матрица, по которой инициализируются элементы.
     \param onlyPositive - запрещает ввод отрицательных значений.
     \param symmetric - заставляет дублировать значение в симметричной ячейке.
    */
    MatrixWidget(long rows, long cols = 1, bool onlyPositive = false, bool symmetric = false,
                 QWidget *parent = nullptr);

    //! \brief Деструктор.
    ~MatrixWidget();

    //! \brief Возвращает матрицу, соответствующую текущему состоянию элементов виджета.
    const Math::Matrix &matrix() const;


private slots:
    //! \brief Записывает изменения в матрицу.
    void onValueChanged();


private:
    //! \brief Инициализирует управляющие элементы и связывает их сигналы с нужными слотами.
    void initControls();

    //! \brief Устанавливает расположение всех элементов на виджете.
    void initLayouts();


private:
    bool         m_onlyPositive;
    bool         m_symmetric;
    Math::Matrix m_matrix;

    QVector<QVector<QCustomDoubleSpinBox *>> m_boxes;
};

#endif // MATRIXWIDGET_H
