#include "matrix_widget.h"
#include "src/gui/font_manager.h"
#include <QHBoxLayout>
#include <QVBoxLayout>


MatrixWidget::MatrixWidget(const Math::Matrix &matrix, bool onlyPositive, bool symmetric, QWidget *parent)
    : QWidget(parent)
    , m_onlyPositive(onlyPositive)
    , m_symmetric(symmetric)
    , m_matrix(matrix)
{
    initControls();
    initLayouts();
}

MatrixWidget::MatrixWidget(long rows, long cols, bool onlyPositive, bool symmetric, QWidget *parent)
    : QWidget(parent)
    , m_onlyPositive(onlyPositive)
    , m_symmetric(symmetric)
    , m_matrix(Math::Matrix::Zero(rows, cols))
{
    initControls();
    initLayouts();
}

MatrixWidget::~MatrixWidget()
{
    for (int i = 0; i < m_boxes.size(); ++i) {
        for (int j = 0; j < m_boxes[0].size(); ++j) {
            if (m_boxes[i][j] != nullptr) {
                delete m_boxes[i][j];
                m_boxes[i][j] = nullptr;
            }
        }
        m_boxes[i].clear();
    }
    m_boxes.clear();
}

void MatrixWidget::initControls()
{
    int rows = int(m_matrix.rows());
    int cols = int(m_matrix.cols());

    QFont           font    = FontManager::instance().mono(GuiConfig::FONT_SIZE_NORMAL);
    QDoubleSpinBox *dsbTemp = new QDoubleSpinBox(this);
    dsbTemp->setFont(font);
    int height = dsbTemp->height();
    int width  = int(QFontMetricsF(font).width("-000000.000000") + height);
    delete dsbTemp;

    m_boxes.resize(rows);
    for (int i = 0; i < rows; ++i) {
        m_boxes[i].resize(cols);
        for (int j = 0; j < cols; ++j) {
            if (m_boxes[i][j] != nullptr) {
                delete m_boxes[i][j];
                m_boxes[i][j] = nullptr;
            }
            m_boxes[i][j] = new QDoubleSpinBox;
            m_boxes[i][j]->setDecimals(6);
            m_boxes[i][j]->setMinimum(m_onlyPositive ? 0.0 : -999999.0);
            m_boxes[i][j]->setMaximum(999999.0);
            m_boxes[i][j]->setSingleStep(0.01);
            m_boxes[i][j]->setValue(m_matrix(i, j));
            m_boxes[i][j]->setAlignment(Qt::AlignRight);
            m_boxes[i][j]->setButtonSymbols(QDoubleSpinBox::NoButtons);
            m_boxes[i][j]->setFont(font);
            m_boxes[i][j]->setMinimumWidth(width);
            m_boxes[i][j]->setMaximumWidth(width);
            connect(m_boxes[i][j], SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
        }
    }

    if (m_symmetric) {
        int min = m_boxes.size() < m_boxes[0].size() ? m_boxes.size() : m_boxes[0].size();
        for (int i = 0; i < min; ++i) {
            for (int j = i + 1; j < min; ++j) {
                //значения меняются симметрично:
                QDoubleSpinBox::connect(m_boxes[i][j], SIGNAL(valueChanged(double)), m_boxes[j][i],
                                        SLOT(setValue(double)));
                connect(m_boxes[j][i], SIGNAL(valueChanged(double)), m_boxes[i][j], SLOT(setValue(double)));
            }
        }
    }
}

void MatrixWidget::initLayouts()
{
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setMargin(GuiConfig::LAYOUT_MARGIN_SMALL);
    mainLayout->setSpacing(GuiConfig::LAYOUT_SPACING_NORMAL);

    for (int i = 0; i < m_boxes.size(); ++i) {
        for (int j = 0; j < m_boxes[0].size(); ++j) {
            mainLayout->addWidget(m_boxes[i][j], i, j);
        }
    }

    this->setLayout(mainLayout);
    this->setMinimumWidth(mainLayout->margin() * 2 + mainLayout->spacing() * (m_boxes[0].size() - 1) +
                          m_boxes[0].size() * m_boxes[0][0]->width());
    this->setMaximumWidth(this->minimumWidth());
}

void MatrixWidget::onValueChanged()
{
    for (int i = 0; i < m_boxes.size(); ++i) {
        for (int j = 0; j < m_boxes[0].size(); ++j) {
            m_matrix(i, j) = m_boxes[i][j]->value();
        }
    }
}

const Math::Matrix &MatrixWidget::matrix() const
{
    return m_matrix;
}
