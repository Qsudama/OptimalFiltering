#include "timer_results_table.h"
#include "src/gui/font_manager.h"
#include <QHBoxLayout>
#include <QHeaderView>

TimerResultsTable::TimerResultsTable(const QVector<FilterTimeResult> filters_results, QWidget *parent)
    : QMainWindow(parent)
    , m_table(nullptr)
{
    setFont(FontManager::instance().regular(GuiConfig::FONT_SIZE_NORMAL));
    setWindowTitle(QString::fromStdString("Время моделирования одного такта"));

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
    centralWidget()->setMinimumWidth(700);
}

void TimerResultsTable::initTable(QVector<FilterTimeResult> filters_results)
{

    int rows = int(filters_results.size());
    int cols = 3;
    m_table = new QTableWidget(rows, cols);

    QFont titleFont = FontManager::instance().mono(GuiConfig::FONT_SIZE_NORMAL);
//    QFont warningFont = FontManager::instance().monoBold(GuiConfig::FONT_SIZE_NORMAL);

    QStringList horisontal_labels;
    QStringList vertical_labels;

    horisontal_labels.append(tr("Моделирование (мсек)"));
    horisontal_labels.append(tr("На статистики (мсек)"));
    horisontal_labels.append(tr("На реализацию (мсек)"));

    for (int i = 0; i < rows; ++i) {
        FilterTimeResult filter_result = filters_results[i];
        QString title = QString::fromStdString(filter_result.name);
        vertical_labels.append(title);
    }

    m_table->setVerticalHeaderLabels(vertical_labels);
    m_table->setHorizontalHeaderLabels(horisontal_labels);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    m_table->setFont(titleFont);

    for (int i = 0; i < rows; i++) {
        FilterTimeResult filter_result = filters_results[i];

        QString all_time = QString("%1").arg(filter_result.all_time, 0, 'f', 3);
        QString pause_time = QString("%1").arg(filter_result.pause_time, 0, 'f', 3);
        QString result_time = QString("%1").arg(filter_result.result_time, 0, 'f', 3);

        QTableWidgetItem *twItem = new QTableWidgetItem(all_time);
        twItem->setFlags(twItem->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
        twItem->setTextAlignment(Qt::AlignCenter);
        m_table->setItem(i, 0, twItem);

        twItem = new QTableWidgetItem(pause_time);
        twItem->setFlags(twItem->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
        twItem->setTextAlignment(Qt::AlignCenter);
        m_table->setItem(i, 1, twItem);

        twItem = new QTableWidgetItem(result_time);
        twItem->setFlags(twItem->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
        twItem->setTextAlignment(Qt::AlignCenter);
        m_table->setItem(i, 2, twItem);

    }
}
