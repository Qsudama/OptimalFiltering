#ifndef GRAPH_WINDOW_H
#define GRAPH_WINDOW_H

#include "src/array.h"
#include "src/gui/font_manager.h"
#include "src/gui/graph_sheet.h"
#include "src/gui/ranges_dialog.h"
#include "src/math/matrix.h"
#include "third_party/qcustomplot/qcustomplot.h"
#include <QAction>
#include <QFileDialog>
#include <QFontDatabase>
#include <QFrame>
#include <QMainWindow>
#include <QMenu>
#include <QPen>


class GraphWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GraphWindow(QWidget *parent = nullptr);
    ~GraphWindow();

    int         countSheets() const;
    GraphSheet &sheet(int index);
    GraphSheet &currentSheet();

    void setCountSheets(int count);
    void updatePlotter();


public slots:
    void onClear();


private slots:
    void onSelectionChanged();
    void onMousePress();
    void onMouseWheel();
    void onMenuRequest(QPoint pos);
    void onMoveLegend();
    void onRangesChanged(Math::Vector2 x, Math::Vector2 y);
    void onSavePng();
    void onShowSetRangesDialog();
    void onSetAutoRanges(bool checked);
    void onHideCurve(QAction *action);
    void onShowCurve(QAction *action);
    void onCurrentSheetChanged(QAction *action);


private:
    QVector<GraphSheet> m_sheets;
    GraphSheet *        m_currentSheet;
    QCustomPlot *       m_plotter;

    QFont m_plotterTitleFont;
    QFont m_plotterSubTitleFont;
    QFont m_plotterLegendFont;
    QFont m_plotterAxesLabelFont;
    QFont m_plotterAxesTickLabelFont;

    QAction *m_actionSavePng;
    QAction *m_actionShowSetRangesDialog;
    QAction *m_actionSetAutoRanges;

    QMenu *m_menuFile;
    QMenu *m_menuView;
    QMenu *m_menuSheet;
    QMenu *m_menuShow;
    QMenu *m_menuHide;

    void loadFonts();
    void initActions();
    void initMenus();
    void initLayouts();
    void initPlotter();
    void updateMenu();
};


#endif // GRAPH_WINDOW_H
