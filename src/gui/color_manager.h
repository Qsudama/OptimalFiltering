#ifndef COLORMANAGER_H
#define COLORMANAGER_H

#include <QColor>
#include <QVector>


class ColorManager
{

public:
    ColorManager();

    const QColor &nextColor();
    void          reset();


private:
    int             m_currentIndex;
    QVector<QColor> m_colors;
};


#endif // COLORMANAGER_H
