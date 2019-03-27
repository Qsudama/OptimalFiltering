#include "timer_results_table.h"
#include "src/gui/font_manager.h"
#include <QHBoxLayout>

TimerResultsTable::TimerResultsTable(const QVector<std::string> &labels, QWidget *parent)
    : QMainWindow(parent)
    , m_table(nullptr)
{
    setFont(FontManager::instance().regular(GuiConfig::FONT_SIZE_NORMAL));

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
