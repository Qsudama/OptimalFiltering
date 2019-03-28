#include "timer_results_table.h"
#include "src/gui/font_manager.h"
#include <QHBoxLayout>

TimerResultsTable::TimerResultsTable(const QVector<FilterTimeResult> filters_results, QWidget *parent)
    : QMainWindow(parent)
    , m_table(nullptr)
{
    setFont(FontManager::instance().regular(GuiConfig::FONT_SIZE_NORMAL));

    initTable(filters_results);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(GuiConfig::LAYOUT_MARGIN_SMALL);
    mainLayout->setSpacing(GuiConfig::LAYOUT_SPACING_SMALL);
    mainLayout->addWidget(m_table);

    if (!centralWidget()) {
        setCentralWidget(new QWidget);
    }
    centralWidget()->setLayout(mainLayout);
    centralWidget()->setMinimumHeight(250);
    centralWidget()->setMinimumWidth(500);
}

void TimerResultsTable::initTable(QVector<FilterTimeResult> filters_results)
{
//    int dim = int(data[0].meanX.size());

////    assert(dim == scale.size() && "FilterResultsTable::initTable(data, scale) : corrupt dimension of scale");
//    if (dim != scale.size()) {
//        AlertHelper::showErrorAlertWithText("FilterResultsTable::initTable\ndim != scale.size()");
//        return;
//    }

    int rows = int(filters_results.size());
    int cols = 3;
    m_table = new QTableWidget(rows, cols);

    QFont titleFont = FontManager::instance().mono(GuiConfig::FONT_SIZE_NORMAL);
//    QFont warningFont = FontManager::instance().monoBold(GuiConfig::FONT_SIZE_NORMAL);

    QStringList horisontal_labels;
    QStringList vertical_labels;

    horisontal_labels.append(tr("Время моделирования (мсек)"));
    horisontal_labels.append(tr("Время всех пауз (мсек)"));
    horisontal_labels.append(tr("Время без пауз (мсек)"));

    for (int i = 0; i < rows; ++i) {
        FilterTimeResult filter_result = filters_results[i];
        QString title = QString::fromStdString(filter_result.name);
        vertical_labels.append(title);
    }

    m_table->setVerticalHeaderLabels(vertical_labels);
    m_table->setHorizontalHeaderLabels(horisontal_labels);
    m_table->setFont(titleFont);

    for (int i = 0; i < rows; i++) {
        FilterTimeResult filter_result = filters_results[i];

        QTableWidgetItem *twItem = new QTableWidgetItem(QString::number(filter_result.all_time));
        twItem->setFlags(twItem->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
        m_table->setItem(i, 0, twItem);

        twItem = new QTableWidgetItem(QString::number(filter_result.pause_time));
        twItem->setFlags(twItem->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
        m_table->setItem(i, 1, twItem);

        twItem = new QTableWidgetItem(QString::number(filter_result.result_time));
        twItem->setFlags(twItem->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
        m_table->setItem(i, 2, twItem);

    }
}
