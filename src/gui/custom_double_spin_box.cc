#include "custom_double_spin_box.h"
#include <limits>
#include <QtMath>
#include <QLineEdit>

QCustomDoubleSpinBox::QCustomDoubleSpinBox(QWidget *parent) :
    QDoubleSpinBox(parent),
    m_isUpperCase(false),
    m_decimals(2)
{
    QDoubleSpinBox::setRange(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
    QDoubleSpinBox::setDecimals(std::numeric_limits<double>::max_exponent10);
}

QValidator::State QCustomDoubleSpinBox::validate(QString &input, int &pos) const
{
    QString str = stripped(input);
    int p = pos - prefix().size();

    QDoubleValidator validator(minimum(), maximum(), m_decimals);
    validator.setNotation(QDoubleValidator::ScientificNotation);
    return validator.validate(str, p);
}

double QCustomDoubleSpinBox::valueFromText(const QString &text) const
{
    return locale().toDouble(stripped(text));
}

QString QCustomDoubleSpinBox::textFromValue(double val) const
{
    return locale().toString(val, m_isUpperCase ? 'E' : 'e', m_decimals);
}

void QCustomDoubleSpinBox::stepBy(int steps)
{
    enum Section { MantissaSection, ExponentSection };

    int mantissaPosition, separatorPosition, exponentPosition, suffixPosition;
    positions(mantissaPosition, separatorPosition, exponentPosition, suffixPosition);

    int section = lineEdit()->cursorPosition() < separatorPosition ? MantissaSection : ExponentSection;
    setValue(section == MantissaSection
             ? value() + singleStep() * steps
             : value() * qPow(10.0, steps));

    positions(mantissaPosition, separatorPosition, exponentPosition, suffixPosition);
    (section == MantissaSection)
            ? lineEdit()->setSelection(separatorPosition, mantissaPosition - separatorPosition)
            : lineEdit()->setSelection(suffixPosition, separatorPosition + 1 - suffixPosition);
}

void QCustomDoubleSpinBox::setUpperCase(bool upperCase)
{
    m_isUpperCase = upperCase;
    QDoubleSpinBox::setPrefix(QDoubleSpinBox::prefix());  // update
}

void QCustomDoubleSpinBox::setDecimals(int prec)
{
    m_decimals = prec;
    QDoubleSpinBox::setDecimals(QDoubleSpinBox::decimals());  // update
}

void QCustomDoubleSpinBox::positions(int &mantissaPosition, int &separatorPosition, int &exponentPosition, int &suffixPosition)
{
    QString text = lineEdit()->text();
    mantissaPosition = prefix().size();
    separatorPosition = text.indexOf('e', mantissaPosition, Qt::CaseInsensitive);
    exponentPosition = separatorPosition + 1;
    suffixPosition = text.size() - suffix().size();
}

QString QCustomDoubleSpinBox::stripped(const QString &text) const
{
    QString copy = text;
    return copy.mid(prefix().size(), copy.size() - prefix().size() - suffix().size());
}
