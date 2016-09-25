#include "graph_sheet.h"


GraphSheet::GraphSheet()
{
    clear();
}


GraphSheet::~GraphSheet()
{
}


void GraphSheet::clear()
{
    m_xLabel         = "";
    m_yLabel         = "";
    m_titleLabel     = "";
    m_subTitleLabel  = "";
    m_autoCalcRanges = true;
    m_curves.clear();

    m_axisRange.xMin = m_axisRange.yMin = 0;
    m_axisRange.xMax = m_axisRange.yMax = 1;
    m_userAxisRange                     = m_axisRange;
}


// getters -------------------------------------------------------------------//

const QString &GraphSheet::xLabel() const
{
    return m_xLabel;
}

const QString &GraphSheet::yLabel() const
{
    return m_yLabel;
}

const QString &GraphSheet::titleLabel() const
{
    return m_titleLabel;
}

const QString &GraphSheet::subTitleLabel() const
{
    return m_subTitleLabel;
}

const GAxisRange &GraphSheet::axisRange() const
{
    return m_axisRange;
}

bool GraphSheet::autoCalcRanges() const
{
    return m_autoCalcRanges;
}

const QVector<GCurve> &GraphSheet::curves() const
{
    return m_curves;
}


// setters -------------------------------------------------------------------//

void GraphSheet::setXLabel(const QString &label)
{
    m_xLabel = label;
}

void GraphSheet::setYLabel(const QString &label)
{
    m_yLabel = label;
}

void GraphSheet::setTitleLabel(const QString &label)
{
    m_titleLabel = label;
}

void GraphSheet::setSubTitleLabel(const QString &label)
{
    m_subTitleLabel = label;
}

void GraphSheet::setAutoCalcRanges(bool autoCalc)
{
    m_autoCalcRanges = autoCalc;
    calcRanges();
}

void GraphSheet::setAxisRange(const GAxisRange &axisRange)
{
    m_userAxisRange = axisRange;
}

void GraphSheet::setCurveVisible(int index, bool visible)
{
    if (index < 0 || index > m_curves.size() - 1) {
        return;
    }
    if (m_curves[index].visible == visible) {
        return;
    }
    m_curves[index].visible = visible;
    calcRanges();
}


// ---------------------------------------------------------------------------//

void GraphSheet::addCurve(const QVector<double> &x, const QVector<double> &y, const QString &name, const QPen &pen,
                          bool visible)
{
    //запрещаем добавление кривых с одинаковыми названиями (name):
    for (int i = 0; i < m_curves.size(); i++) {
        if (m_curves[i].name == name) {
            return;
        }
    }

    m_curves.resize(m_curves.size() + 1);
    m_curves.last().name    = name;
    m_curves.last().pen     = pen;
    m_curves.last().visible = visible;
    m_curves.last().x       = x;
    m_curves.last().y       = y;

    calcRanges();
}


void GraphSheet::calcRanges()
{
    if (m_autoCalcRanges) {
        if (m_curves.size() == 0) {
            m_axisRange.xMin = m_axisRange.yMin = 0;
            m_axisRange.xMax = m_axisRange.yMax = 1;
            return;
        }

        double xmin = std::numeric_limits<double>::max();
        double ymin = std::numeric_limits<double>::max();
        double xmax = std::numeric_limits<double>::min();
        double ymax = std::numeric_limits<double>::min();

        for (int i = 0; i < m_curves.size(); i++) {
            if (m_curves[i].visible == false) {
                continue;
            }
            for (int j = 0; j < m_curves[i].x.size(); j++) {
                if (m_curves[i].x[j] > xmax) {
                    xmax = m_curves[i].x[j];
                }
                if (m_curves[i].y[j] > ymax) {
                    ymax = m_curves[i].y[j];
                }
                if (m_curves[i].x[j] < xmin) {
                    xmin = m_curves[i].x[j];
                }
                if (m_curves[i].y[j] < ymin) {
                    ymin = m_curves[i].y[j];
                }
            }
        }
        double xd = (xmax - xmin) * 0.005;
        double yd = (xmax - xmin) * 0.005;

        m_axisRange.xMin = xmin - xd;
        m_axisRange.xMax = xmax + xd;
        m_axisRange.yMin = ymin - yd;
        m_axisRange.yMax = ymax + yd;
    } else {
        m_axisRange = m_userAxisRange;
    }
}
