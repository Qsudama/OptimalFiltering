#ifndef RANGES_DIALOG_H
#define RANGES_DIALOG_H

#include "src/math/matrix.h"
#include "third_party/qcustomplot/qcustomplot.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFontMetrics>
#include <climits>


//! \brief Диалоговое окно для изменения границ осей координат.

class RangesDialog : public QDialog
{
    Q_OBJECT

public:
    /*!
     \brief Конструктор.

     \param x - пара (min, max) для оси OX.
     \param y - пара (min, max) для оси OY.
    */
    RangesDialog(const Math::Vector2 &x, const Math::Vector2 &y, QWidget *parent = nullptr);

    //! \brief Деструктор.
    ~RangesDialog();


private slots:

    //! \brief Сохраняет новое значение верхней границы оси OY.
    void onYMaxChanged(double yMax);

    //! \brief Сохраняет новое значение нижней границы оси OY.
    void onYMinChanged(double yMin);

    //! \brief Сохраняет новое значение правой границы оси OX.
    void onXMaxChanged(double xMax);

    //! \brief Сохраняет новое значение левой границы оси OX.
    void onXMinChanged(double xMin);

    //! \brief Испускает сигнал rangesChanged(Math::Vector2 x, Math::Vector2 y) и закрывает окно.
    void onAcceptedClicked();

    //! \brief Закрывает окно (все изменения отклонены).
    void onRejectedClicked();


signals:
    /*!
     \brief Сигнал. Сообщает всем связанным виджетам новые значения границ осей координат.

     \param x
     \param y
    */
    void rangesChanged(Math::Vector2 x, Math::Vector2 y);


private:
    /*!
     \brief Загружает шрифты и устанавливает их параметры (начертание, размер и т.д.).
     \see FontManager.
    */
    void loadFonts();

    /*!
     //! \brief Инициализирует управляющие элементы и связывает их сигналы с нужными слотами.

     \param x - пара (min, max) для оси OX.
     \param y - пара (min, max) для оси OY.
    */
    void initControls(const Math::Vector2 &x, const Math::Vector2 &y);

    //! \brief Устанавливает расположение всех элементов на виджете.
    void initLayouts();

    //! \brief Инициализирует поле рисовальщика QCustomPlot.
    void initPlotter();


private:
    QDoubleSpinBox *  m_dsbXMin;
    QDoubleSpinBox *  m_dsbXMax;
    QDoubleSpinBox *  m_dsbYMin;
    QDoubleSpinBox *  m_dsbYMax;
    QCustomPlot *     m_plotter;
    QDialogButtonBox *m_buttonBox;
};


#endif // RANGES_DIALOG_H
