#ifndef FILTERSTARTBUTTONSBOX_H
#define FILTERSTARTBUTTONSBOX_H

#include "src/filters/filters_factory.h"
#include "src/gui/gui_config.h"
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
    void start(Core::FILTER_TYPE ftype, Core::APPROX_TYPE atype, FILTER_ID id);

    /*!
     \brief Сигнал. Отправляет сообщение о том, что текущая вкладка с кнопками изменилась.
     \param index - номер текущей вкладки.
    */

    void filtersFamilyChanged(Core::FILTER_TYPE);

private slots:
    //@{
    /*!
      Слот. Реакция на нажатие кнопки (сигнал clicked() объекта класса QButtonBox).

      Отправляет сигнал start(Core::FILTER_TYPE, Core::APPROX_TYPE, Filters::FILTER_ID)
      c параметрами, соответсвующими кнопке, на которую кликнули.
    */

    void onBtnContinuousDiscreteAofClicked();
    void onBtnContinuousDiscreteFosClicked();
    void onBtnContinuousDiscreteDfosClicked();
    void onBtnContinuousDiscreteDfosboClicked();

    void onBtnContinuousAofClicked();
    void onBtnContinuousFosClicked();

    void onBtnDiscreteAofClicked();
    void onBtnDiscreteFosClicked();
    void onBtnDiscreteMfosClicked();
    void onBtnDiscreteDuofClicked();
    void onBtnDiscreteDfmpClicked();
    void onBtnDiscreteFkpClicked();
    void onBtnDiscreteDfkpClicked();
    void onBtnDiscreteMdfmpClicked();

    void onBtnLDAofClicked();
    void onBtnLDFosClicked();
    void onBtnLDFkpClicked();
    void onBtnLDFbpClicked();
    //@}

    void tabWidgetCurrentChanged(int);

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


private:
    QPushButton *m_btnContinuousDiscreteAof;
    QPushButton *m_btnContinuousDiscreteFos;
    QPushButton *m_btnContinuousDiscreteDfos;
    QPushButton *m_btnContinuousDiscreteDfosbo;

    QPushButton *m_btnContinuousAof;
    QPushButton *m_btnContinuousFos;

    QPushButton *m_btnDiscreteAof;
    QPushButton *m_btnDiscreteFos;
    QPushButton *m_btnDiscreteMfos;
    QPushButton *m_btnDiscreteDuof;
    QPushButton *m_btnDiscreteDfmp;
    QPushButton *m_btnDiscreteFkp;
    QPushButton *m_btnDiscreteDfkp;
    QPushButton *m_btnDiscreteMdfmp;

    QPushButton *m_btnLDAof;
    QPushButton *m_btnLDFos;
    QPushButton *m_btnLDFkp;
    QPushButton *m_btnLDFbp;
};


#endif // FILTERSTARTBUTTONSBOX_H
