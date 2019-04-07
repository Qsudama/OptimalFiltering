#ifndef QCUSTOMDOUBLESPINBOX_H
#define QCUSTOMDOUBLESPINBOX_H

#include <QDoubleSpinBox>
#include <QDoubleValidator>

class QCustomDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
    Q_PROPERTY(bool upperCase READ isUpperCase WRITE setUpperCase)

public:
    explicit QCustomDoubleSpinBox(QWidget *parent = 0);

    inline bool isUpperCase() const;
    inline int decimals() const;

    QValidator::State validate(QString &input, int &pos) const;
    double valueFromText(const QString &text) const;
    QString textFromValue(double val) const;
    void stepBy(int steps);

public slots:
    void setUpperCase(bool upperCase);
    void setDecimals(int prec);

private:
    void positions(int &mantissaPosition, int &separatorPosition, int &exponentPosition, int &suffixPosition);
    QString stripped(const QString &text) const;

private:
    bool m_isUpperCase;
    int  m_decimals;
};

bool QCustomDoubleSpinBox::isUpperCase() const
{ return m_isUpperCase; }

int QCustomDoubleSpinBox::decimals() const
{ return m_decimals; }


#endif // QCUSTOMDOUBLESPINBOX_H
