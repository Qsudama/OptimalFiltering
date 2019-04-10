#include "filter_results_table.h"
#include "src/gui/font_manager.h"
#include <QAction>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>

#include "src/helpers/alert_helper.h"

FilterResultsTable::FilterResultsTable(const Core::FilterOutput &data, const std::string &label,
                                       const Math::Vector &scale, QWidget *parent)
    : QMainWindow(parent)
    , m_table(nullptr)
{
    setFont(FontManager::instance().regular(GuiConfig::FONT_SIZE_NORMAL));
    setWindowTitle(QString::fromStdString(label));

    initTable(data, scale);

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

    initMenu();
}

void FilterResultsTable::initTable(const Core::FilterOutput &data, const Math::Vector &scale)
{
    int dim = int(data[0].meanX.size());
    if (dim != scale.size()) {
        AlertHelper::showErrorAlertWithText("FilterResultsTable::initTable\ndim != scale.size()");
        return;
    }
    int rows = int(data.size());
    int cols = 1 + 4 * dim + 1;

    m_table = new QTableWidget(rows, cols);

    QFont titleFont   = FontManager::instance().mono(GuiConfig::FONT_SIZE_NORMAL);
    QFont warningFont = FontManager::instance().monoBold(GuiConfig::FONT_SIZE_NORMAL);

    QStringList labels;
    labels.append(tr("t"));
    for (int i = 0; i < dim; ++i) {
        QString strNum = QString::number(i + 1);
        labels.append(tr("Mx") + strNum);
        labels.append(tr("Me") + strNum);
        labels.append(tr("Sx") + strNum);
        labels.append(tr("Se") + strNum);
    }
    labels.append(tr("Mint"));

    m_table->setHorizontalHeaderLabels(labels);
    m_table->setFont(titleFont);

    m_table->setSortingEnabled(false);
    m_table->setWordWrap(false);

    QVector<double> arrT, arrMx, arrMe, arrSx, arrSe, arrMint;

    Core::GetTime(data, arrT);
    for (int i = 0; i < rows; ++i) {
        QTableWidgetItem *newItem = new QTableWidgetItem(QString::number(arrT[i]));
        newItem->setFlags(newItem->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
        m_table->setItem(i, 0, newItem);
    }

    for (int j = 0; j < dim; ++j) {
        Core::GetMeanX(data, j, arrMx, scale[j]);
        Core::GetMeanE(data, j, arrMe, scale[j]);
        Core::GetStdDeviationX(data, j, arrSx, scale[j]);
        Core::GetStdDeviationE(data, j, arrSe, scale[j]);
        Core::GetMeanIntegralE(data, j, arrMint, scale[j]);

        for (int i = 0; i < rows; ++i) {
            QTableWidgetItem *twItem = new QTableWidgetItem(QString::number(arrMx[i]));
            twItem->setFlags(twItem->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
            m_table->setItem(i, 1 + 4 * j, twItem);

            twItem = new QTableWidgetItem(QString::number(arrMe[i]));
            twItem->setFlags(twItem->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
            m_table->setItem(i, 2 + 4 * j, twItem);

            twItem = new QTableWidgetItem(QString::number(arrSx[i]));
            twItem->setFlags(twItem->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
            m_table->setItem(i, 3 + 4 * j, twItem);

            twItem = new QTableWidgetItem(QString::number(arrSe[i]));
            twItem->setFlags(twItem->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
            if (arrSe[i] > arrSx[i]) {
                twItem->setTextColor(Qt::red);
                twItem->setFont(warningFont);
            }
            m_table->setItem(i, 4 + 4 * j, twItem);
        }
        for (int i = 0; i < rows; ++i) {
            QTableWidgetItem *newItem = new QTableWidgetItem(QString::number(arrMint[i]));
            newItem->setFlags(newItem->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
            m_table->setItem(i, 4 + 4 * (dim - 1) + 1, newItem);
        }
    }
}

void FilterResultsTable::initMenu()
{
    QAction *action = new QAction(tr("Сохранить таблицу"), this);
    connect(action, SIGNAL(triggered()), this, SLOT(onSaveTable()));

    QMenu *menu = menuBar()->addMenu(tr("Файл"));
    menu->addAction(action);
}

void FilterResultsTable::onSaveTable()
{
    QString fileName =
        QFileDialog::getSaveFileName(this, tr("Сохранить таблицу"), QDir::homePath(), tr("Текст (*.txt)"));
    if (fileName.isEmpty()) {
        return;
    } else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Не получилось открыть файл!"), file.errorString());
            return;
        }
        QTextStream out(&file);
        writeToFile(out);
        file.close();
    }
}

void FilterResultsTable::writeToFile(QTextStream &out)
{
    // найдем самые длинные строки в каждом столбце:
    QVector<int> maxLength(m_table->columnCount());
    for (int j = 0; j < m_table->columnCount(); ++j) {
        maxLength[j] = 5;
    }

    for (int i = 0; i < m_table->rowCount(); ++i) {
        for (int j = 0; j < m_table->columnCount(); ++j) {
            int length;
            if ((length = m_table->item(i, j)->text().length()) > maxLength[j]) {
                maxLength[j] = length;
            }
        }
    }

    // сделаем линии для отчеркивания строк:
    QString line = "|";
    for (int j = 0; j < m_table->columnCount(); ++j) {
        for (int k = 0; k < maxLength[j]; ++k) {
            line += "-";
        }
        if (j != m_table->columnCount() - 1) {
            line += "--+";
        } else {
            line += "--|\n";
        }
    }

    QString simpleLine = "";
    for (int k = 0; k < line.length() - 1; ++k) {
        simpleLine += "-";
    }
    simpleLine += "\n";

    QString titleLine = "";
    titleLine += QString("| %1 |").arg("t", -maxLength[0]);
    for (int j = 1; j < maxLength.size(); j += 4) {
        int index = (j - 1) / 4 + 1;
        titleLine += QString(" %1 |").arg("Mx" + QString::number(index), -maxLength[j]);
        titleLine += QString(" %1 |").arg("Me" + QString::number(index), -maxLength[j + 1]);
        titleLine += QString(" %1 |").arg("Sx" + QString::number(index), -maxLength[j + 2]);
        titleLine += QString(" %1 |").arg("Se" + QString::number(index), -maxLength[j + 3]);
    }
    titleLine += "\n";


    // запишем в поток:
    QString str = "";
    out << simpleLine << titleLine << line;
    for (int i = 0; i < m_table->rowCount(); ++i) {
        str = "|";
        for (int j = 0; j < m_table->columnCount(); ++j) {
            str += QString(" %1 |").arg(m_table->item(i, j)->text(), -maxLength[j]);
        }
        str += "\n";
        out << str;
        if (i < m_table->rowCount() - 1) {
            out << line;
        } else {
            out << simpleLine;
        }
    }
}
