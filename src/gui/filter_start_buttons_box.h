#ifndef FILTERSTARTBUTTONSBOX_H
#define FILTERSTARTBUTTONSBOX_H

#include "src/filters/filters_factory.h"
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QTabWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QWidget>


//! \brief Класс виджета, содержащего только кнопки для запуска того или иного фильтра.

class FilterStartButtonsBox : public QGroupBox
{
    Q_OBJECT

public:
    //! \brief Конструктор.
    explicit FilterStartButtonsBox(QWidget *parent = nullptr);

    //! \brief Деструктор.
    ~FilterStartButtonsBox();


signals:
    /*!
     \brief Сигнал. Отправляет сообщение о том, что нужно создать и запустить фильтр.

     \param ftype - идентификатор типа фильтра.
     \param atype - идентификатор метода приближения (аппроксимации).
     \param id    - идентификатор алгоритма.
    */
    void start(Core::FILTER_TYPE ftype, Core::APPROX_TYPE atype, Filters::FILTER_ID id);


private slots:
    //@{
    /*!
      Слот. Реакция на нажатие кнопки (сигнал clicked() объекта класса QButtonBox).

      Отправляет сигнал start(Core::FILTER_TYPE, Core::APPROX_TYPE, Filters::FILTER_ID)
      c параметрами, соответсвующими кнопке, на которую кликнули.
    */

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
    //@}


private:
    /*!
     \brief Загружает шрифты и устанавливает их параметры (начертание, размер и т.д.).
     \see FontManager.
    */
    void loadFonts();

    //! \brief Инициализирует управляющие элементы и связывает их сигналы с нужными слотами.
    void initControls();

    //! \brief Устанавливает расположение всех элементов на виджете.
    void initLayouts();

    /*!
     \brief Вычисляет размеры для виджетов.
     \details  Это, в частности, необходимо при изменении шрифтов, так как размеры элементов зависят от них.

     \warning В Linux и Windows всё работает как задумано, в MacOS - фигово. Косяк либо тут, либо в FontManager.
    */
    void computeSizes();


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
};


#endif // FILTERSTARTBUTTONSBOX_H
