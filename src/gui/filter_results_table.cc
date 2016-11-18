#include "filter_results_table.h"
#include "src/gui/font_manager.h"
#include <QHBoxLayout>


FilterResultsTable::FilterResultsTable(const Core::FilterOutput &data, const std::string &label,
                                       const Math::Vector &scale, QWidget *parent)
    : QWidget(parent)
    , m_table(nullptr)
{
    setFont(FontManager::instance().regular(9));
    setMinimumHeight(250);
    setMinimumWidth(500);

    setWindowTitle(QString::fromStdString(label));

    initTable(data, scale);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(m_table);
    this->setLayout(layout);
}

void FilterResultsTable::initTable(const Core::FilterOutput &data, const Math::Vector &scale)
{
    int dim = int(data[0].meanX.size());
    assert(dim == scale.size());

    int rows = int(data.size());
    int cols = 1 + 3 * dim; // | time | Mx0 | ... | MxN | Sx0 | ... | SxN | Se0 | ... | SeN  |

    m_table = new QTableWidget(rows, cols);

    QFont titleFont   = FontManager::instance().mono(9);
    QFont warningFont = FontManager::instance().monoBold(9);

    QStringList labels;
    labels.append(tr("t"));
    for (int i = 0; i < dim; ++i) {
        QString strNum = QString::number(i + 1);
        labels.append(tr("Mx") + strNum);
        labels.append(tr("Sx") + strNum);
        labels.append(tr("Se") + strNum);
    }

    m_table->setHorizontalHeaderLabels(labels);
    m_table->setFont(titleFont);
    m_table->setSortingEnabled(false);
    m_table->setWordWrap(false);

    QVector<double> arrT, arrMx, arrSx, arrSe;

    Core::GetTime(data, arrT);
    for (int i = 0; i < rows; ++i) {
        QTableWidgetItem *newItem = new QTableWidgetItem(QString::number(arrT[i]));
        m_table->setItem(i, 0, newItem);
    }

    for (int j = 0; j < dim; ++j) {
        Core::GetMeanX(data, j, arrMx, scale[j]);
        Core::GetStdDeviationX(data, j, arrSx, scale[j]);
        Core::GetStdDeviationE(data, j, arrSe, scale[j]);

        for (int i = 0; i < rows; ++i) {
            QTableWidgetItem *twItem = new QTableWidgetItem(QString::number(arrMx[i]));
            m_table->setItem(i, 1 + 3 * j, twItem);

            twItem = new QTableWidgetItem(QString::number(arrSx[i]));
            m_table->setItem(i, 2 + 3 * j, twItem);

            twItem = new QTableWidgetItem(QString::number(arrSe[i]));
            if (arrSe[i] > arrSx[i]) {
                twItem->setTextColor(Qt::red);
                twItem->setFont(warningFont);
            }
            m_table->setItem(i, 3 + 3 * j, twItem);
        }
    }
}
