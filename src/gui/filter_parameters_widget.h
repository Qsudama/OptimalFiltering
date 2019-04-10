#ifndef FILTERPARAMETERSWIDGET_H
#define FILTERPARAMETERSWIDGET_H

#include "src/core/filter_parameters.h"
#include "src/gui/gui_config.h"
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


/*!
 \brief Класс виджета, предаставляющего элементы управления параметрами фильтрации.

 \see Core::FilterParameters
*/

class FilterParametersWidget : public QGroupBox
{
    Q_OBJECT

public:
    //! \brief Конструктор.
    explicit FilterParametersWidget(QWidget *parent = nullptr);

    //! \brief Деструктор.
    ~FilterParametersWidget();

    //! \brief Возвращает указатель на текущий набор параметров.
    Core::PtrFilterParameters parameters();


public slots:
    /*!
     \brief Делает часть параметров доступными или недоступными.
     \param index - номер текущей вкладки.
     \details Слот. Реакция на изменение текущей вкладки в FilterStartButtonBox.
    */
    void onFiltersFamilyChanged(int index);


private slots:
    /*!
     \brief Устанавливает новое значение времени окончания фильтрации.
     \param value - время \f$T_{max}\f$.
     \details Слот. Реакция на сигнал valueChanged(double) элемента m_dsbMaxTime.
    */
    void onMaxTimeChanged(double value);

    /*!
     \brief Устанавливает новое значение интервала между измерениями.
     \param value - величина интервала \f$\delta t\f$.
     \details Слот. Реакция на сигнал valueChanged(double) элемента m_dsbMeasurementStep.
    */
    void onMeasurementStepChanged(double value);

    /*!
     \brief Устанавливает новое значение интервала между прогнозами.
     \param value - величина интервала \f$\delta \tau\f$.
     \details Слот. Реакция на сигнал valueChanged(double) элемента m_dsbPredictionStep.
    */
    void onPredictionStepChanged(double value);

    /*!
     \brief Устанавливает новое значение интервала между прогнозами (по их количеству).
     \param value - количество прогнозов \f$L\f$ на интервале \f$\delta t\f$.
     \details Слот. Реакция на сигнал valueChanged(int) элемента m_sbPredictionCount.
    */
    void onPredictionCountChanged(int value);

    /*!
     \brief Устанавливает новое значение шага интегрирования.
     \param value - величина шага \f$\Delta t\f$.
     \details Слот. Реакция на сигнал valueChanged(double) элемента m_dsbIntegrationStep.
    */
    void onIntegrationStepChanged(double value);

    /*!
     \brief Включает / выключает возможность изменять параметры.
     \details Слот. Реакция на сигнал toggled(bool) элемента m_checkFixAll.
    */
    void onFixAllToggled(bool checked);

    /*!
     \brief Вклачает режим изменения интервала между прогнозами по его значению.
     \details Слот. Реакция на сигнал toggled(bool) элемента m_radioPredictionStep.
    */
    void onPredictionStepToggled(bool checked);

    /*!
     \brief Вклачает режим изменения интервала между прогнозами по их количеству между соседними измерениями.
     \details Слот. Реакция на сигнал toggled(bool) элемента m_radioPredictionCount.
    */
    void onPredictionCountToggled(bool checked);

    /*!
     \brief Устанавливает новое значение кратности порядка фильтра.
     \param value - новое значение кратности \f$l\f$ порядка \f$p = l\cdot dim(Y)\f$.
     \details Слот. Реакция на сигнал valueChanged(int) элемента m_sbOrderMultiplicity.
    */
    void onOrderMultChanged(int value);

    /*!
     \brief Устанавливает новое значение кол-ва аргументов фильтра.
    */
    void onArgumentsCountChanged(int value);

    /*!
     \brief Устанавливает новый размер (объём) выборок для метода Монте-Карло.
     \param value - новое значение размера выборок.
     \details Слот. Реакция на сигнал valueChanged(int) элемента m_sbSampleSize.
    */
    void onSampleSizeChanged(int value);

    /*!
     \brief Устанавливает новый номер выводимой реализации в трубке.
     \param value - новое значение выводимой реализации.
     \details Слот. Реакция на сигнал valueChanged(int) элемента m_sbSpecificRealization.
    */
    void onSpecificRealizationChanged(int value);

    /*!
     \brief Обновляет значения во всех элементах.
     \note Параметры хранятся в экземпляре класса Core::FilterParameters.
            При изменении какого-либо параметра его значение предварительно корректируется
            внутри Core::FilterParameters. Поэтому реально используемые в фильтре значения могут отличаться
            от тех, что пользователь видит на экране.

            При клике на кнопку "Обновить" во все элементы загружаются откорректированные актуальные данные.

      \details Слот. Реакция на сигнал clicked() элемента m_btnUpdate.
    */
    void onUpdateValues();


private:
    /*!
     \brief Загружает шрифты и устанавливает их параметры (начертание, размер и т.д.).
     \see FontManager.
    */
    void loadFonts();

    //! \brief Инициализирует управляющие элементы.
    void initControls();

    //! \brief Связывает управляющие элементы c их сигналами с нужными слотами.
    void connectFieldSignals();

    //! \brief Устанавливает расположение всех элементов на виджете.
    void initLayouts();

    void initLabels();
    /*!
     \brief Изменяет диапозон значений QDoubleSpinBox.

     \param dsb - указатель на элемент.
     \param min - минимальное значение.
     \param max - максимальное значение.
     \param step - шаг.
    */
    void setRange(QDoubleSpinBox *dsb, double min, double max, double step);

    /*!
     \brief Изменяет диапозон значений QDoubleSpinBox.

     \param dsb - указатель на элемент.
     \param min - минимальное значение.
     \param max - максимальное значение.
    */
    void setRange(QDoubleSpinBox *dsb, double min, double max);

    /*!
     \brief Изменяет диапозон значений QSpinBox.

     \param dsb - указатель на элемент.
     \param min - минимальное значение.
     \param max - максимальное значение.
    */
    void setRange(QSpinBox *sb, int min, int max);

    /*!
     \brief Вычисляет минимальную ширину виджета.
     \details Все элементы расположены на сетке по 4 в одной строке.

     Чтобы всё было "ровно" вычисляются 4 значения, каждое из которых соответствуют
     самому длинному элементу в соответсвующей колонке.

     Это, в частности, необходимо при изменении шрифтов, так как размеры элементов зависят от них.
    */
    int computeMinimumWidth();


    void enableLabel(QLabel *label, bool enable);

private:
    QDoubleSpinBox *m_dsbMaxTime;
    QDoubleSpinBox *m_dsbMeasurementStep;
    QDoubleSpinBox *m_dsbPredictionStep;
    QSpinBox *m_sbPredictionCount;
    QDoubleSpinBox *m_dsbIntegrationStep;
    QSpinBox *m_sbOrderMultiplicity;
    QSpinBox *m_argumentsCount;
    QSpinBox *m_sbSampleSize;
    QSpinBox *m_sbSpecificRealization;
    QRadioButton *m_radioPredictionStep;
    QRadioButton *m_radioPredictionCount;
    QCheckBox *m_checkFixAll;
    QPushButton *m_btnUpdate;

    QLabel *m_maxTimeLabel;
    QLabel *m_measurementStepLabel;
    QLabel *m_integrationStepLabel;
    QLabel *m_orderMultiplicityLabel;
    QLabel *m_argumentsCountLabel;
    QLabel *m_sampleSizeLabel;
    QLabel *m_specificRealizationLabel;

    /*!
     * \details Если true, то запрещает виджетам изменяться.
     *
     * Меньшие интервалы не могут превышать размером бОльшие. Чтобы всегда выполнялось
     * \f$T_{max} \geq \delta t \geq \delta \tau \geq \Delta t\f$ и пользователь не мог ввести
     * явно ошибочное значение, после каждого изменения перевычисляются диапозоны для всех элементов.
     *
     * Это может привести к изменению значения элемента, что вызовет соответствующий слот в ненужное время.
     * Чтобы разрулить это и нужна эта переменная.
     */
    bool                      m_updateOn;
    Core::PtrFilterParameters m_parameters;
    int                       m_currentFiltersFamily;

    QFont m_regularFont;
    QFont m_monotypeFont;
    QFont m_labelsFont;
    QFont m_spinBoxesFont;
};


#endif // FILTERPARAMETERSWIDGET_H
