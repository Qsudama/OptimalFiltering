#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "src/gui/color_manager.h"
#include "src/gui/filter_parameters_widget.h"
#include "src/gui/filter_start_buttons_box.h"
#include "src/gui/font_manager.h"
#include "src/gui/graph_window.h"
#include "src/gui/task_widget.h"
#include "src/math/convert.h"
#include "src/math/random.h"
#include <QApplication>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QProgressBar>
#include <QStatusBar>
#include <QVector>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


signals:
    void clear();


private slots:
    void onClear();
    void onFilterUpdatePercent(int p);
    void onStart(Core::FILTER_TYPE ftype, Core::APPROX_TYPE atype, Filters::FILTER_ID id);


protected:
    void closeEvent(QCloseEvent *);


private:
    void loadFonts();
    void initControls();
    void initLayouts();

    void initStatusBar();
    void initGraphWindow();

    void showData(Core::PtrFilter filter);


private:
    ColorManager m_colorManager;
    int          m_randSeed;

    QProgressBar *          m_statusProgressBar;
    GraphWindow *           m_graphWindow;
    TaskWidget *            m_taskWidget;
    FilterParametersWidget *m_filterParamsWidget;
    FilterStartButtonsBox * m_filterStartWidget;
    QPushButton *           m_btnClear;
};


#endif // MAINWINDOW_H
