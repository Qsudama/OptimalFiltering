#ifndef GRAPHSHEET_H
#define GRAPHSHEET_H

#include <QPen>


struct GCurve {
    QVector<double> x;
    QVector<double> y;
    QString         name;
    QPen            pen;
    bool            visible;
};

struct GAxisRange {
    double xMin;
    double xMax;
    double yMin;
    double yMax;
};


class GraphSheet
{

public:
    GraphSheet();
    ~GraphSheet();

    void clear();

    const QString &        xLabel() const;
    const QString &        yLabel() const;
    const QString &        titleLabel() const;
    const QString &        subTitleLabel() const;
    const GAxisRange &     axisRange() const;
    bool                   autoCalcRanges() const;
    const QVector<GCurve> &curves() const;

    void setXLabel(const QString &label);
    void setYLabel(const QString &label);
    void setTitleLabel(const QString &label);
    void setSubTitleLabel(const QString &label);
    void setAutoCalcRanges(bool autoCalc);
    void setAxisRange(const GAxisRange &axisRange);
    void setCurveVisible(int index, bool visible);

    void addCurve(const QVector<double> &x, const QVector<double> &y, const QString &name, const QPen &pen,
                  bool visible = true);


private:
    QVector<GCurve> m_curves;
    QString         m_xLabel;
    QString         m_yLabel;
    QString         m_titleLabel;
    QString         m_subTitleLabel;
    bool            m_autoCalcRanges;
    GAxisRange      m_axisRange;
    GAxisRange      m_userAxisRange;

    void calcRanges();
};


#endif // GRAPHSHEET_H
