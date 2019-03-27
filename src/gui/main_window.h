#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "src/gui/color_manager.h"
#include "src/gui/filter_parameters_widget.h"
#include "src/gui/filter_results_table.h"
#include "src/gui/filter_start_buttons_box.h"
#include "src/gui/font_manager.h"
#include "src/gui/graph_window.h"
#include "src/gui/gui_config.h"
#include "src/gui/task_widget.h"
#include "src/gui/pinv_method_setting_widget.h"
#include "src/gui/start_conditions_filter_widget.h"
#include "src/math/convert.h"
#include "src/math/random.h"
#include <QApplication>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QProgressBar>
#include <QStatusBar>
#include <QVector>


/*!
 \brief Класс для главного окна приложения.
*/

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //! Конструктор.
    explicit MainWindow(QWidget *parent = nullptr);

    //! Деструктор.
    ~MainWindow();


signals:
    //! Сигнал. Вызывает слоты onClear() всех связанных с ним виджетов.
    void clear();

private slots:
    //! Удаляет все пользовательские данные и возвращает значения по-умолчанию.
    void onClear();

    //! Показывает / скрывает таблицы результатов.
    void onShowHideTables();

    //! Показывает таблицу результатов работы таймера.
    void onShowTableTimer();

    //! Отображает прогресс выполнеия алгоритма (связывается с Filter).
    void onFilterUpdatePercent(int p);

    /*!
     \brief Создает экземпляр фильтра и запускает выполнение алгоритма фильтрации.

     \param ftype - идентификатор типа фильтра.
     \param atype - идентификатор метода приближения (аппроксимации).
     \param id    - идентификатор алгоритма.
    */
    void onStart(Core::FILTER_TYPE ftype, Core::APPROX_TYPE atype, FILTER_ID id);


protected:
    //! \brief Закрывает все второстепенные окна, затем себя.
    void closeEvent(QCloseEvent *);


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

    //! \brief Инициализирует поле статуса внизу окна.
    void initStatusBar();

    //! \brief Создает, настраивает и показывает окно графиков GraphWindow
    void initGraphWindow();

    //! \brief Преобразует результаты работы фильтра в нужный формат и отправляет в окно графиков.
    void showData(Core::PtrFilter filter, Core::FILTER_TYPE ftype, Core::PtrTask task);

    QString subtitleForParametrs(Core::FILTER_TYPE ftype, Core::PtrTask task);

    //! \brief Добавляет новую таблицу и открывает ее окно.
    void addTable(const Core::FilterOutput &data, const std::string &label, const Vector &scale);


    void showErrorMessage (void);

private:
    ColorManager m_colorManager;
    bool         m_tablesIsVisible;

    QProgressBar *            m_statusProgressBar;
    GraphWindow *             m_graphWindow;
    TaskWidget *              m_taskWidget;
    FilterParametersWidget *  m_filterParamsWidget;
    FilterStartButtonsBox *   m_filterStartWidget;
    PinvMethodSettingWidget *m_pinvMethodSettingsWidget;
    StartConditionsFilterWidget *m_startConditionsFilterWidget;
    QPushButton *             m_btnClear;
    QPushButton *             m_btnShowHideTables;
    QPushButton *             m_btnShowTimes;

    QVector<FilterResultsTable *> m_tables;
    QVector<std::string> m_filter_labels;
};


#endif // MAINWINDOW_H
