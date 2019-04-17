#ifndef COLORMANAGER_H
#define COLORMANAGER_H

#include <QColor>
#include <QVector>


//! \brief Класс для управления набором цветов.

class ColorManager
{

public:
    /*! \brief Конструктор.

     Генерирует набор цветов и сохраняет его в массив.

     Генерация происходит таким образом, чтобы соседние цвета были максимально "непохожими".
    */
    ColorManager();


    //! \brief Достает цвет из массива, сдвигает (циклически) индекс.
    const QColor &nextColor();

    //! \brief Достает цвет из массива, сдвигает (циклически) индекс.
    const QColor &nextColorRealizationE();

    //! \brief Достает цвет из массива, сдвигает (циклически) индекс.
    const QColor &nextColorRealizationZ();

    //! \brief Сбрасывает индекс последнего извлеченного цвета до нулевого.
    void reset();


private:

    void initColors();

    int             m_currentIndex;                                     /*!< Индекс последнего цвета, котороый "взяли". */
    int             m_realizationECollorsCurrentIndex;                  /*!< Индекс последнего цвета, котороый "взяли". */
    int             m_realizationZCollorsCurrentIndex;                  /*!< Индекс последнего цвета, котороый "взяли". */
    QVector<QColor> m_colors;                                           /*!< Массив под набор цветов. */
    QVector<QColor> m_realization_e_colors;                             /*!< Массив под набор цветов для конкретных реализаций. */
    QVector<QColor> m_realization_z_colors;                             /*!< Массив под набор цветов для конкретных реализаций. */
};


#endif // COLORMANAGER_H
