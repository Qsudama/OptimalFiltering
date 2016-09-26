#include "color_manager.h"


ColorManager::ColorManager()
    : m_currentIndex(0)
{
    m_colors.resize(12);
    m_colors[0]  = QColor::fromHsv(359, 255, 255);
    m_colors[1]  = QColor::fromHsv(179, 230, 230);
    m_colors[2]  = QColor::fromHsv(239, 255, 255);
    m_colors[3]  = QColor::fromHsv(299, 255, 255);
    m_colors[4]  = QColor::fromHsv(149, 204, 204);
    m_colors[5]  = QColor::fromHsv(29, 204, 204);
    m_colors[6]  = QColor::fromHsv(209, 204, 204);
    m_colors[7]  = QColor::fromHsv(89, 204, 204);
    m_colors[8]  = QColor::fromHsv(269, 204, 204);
    m_colors[9]  = QColor::fromHsv(329, 255, 153);
    m_colors[10] = QColor::fromHsv(59, 255, 255);
    m_colors[11] = QColor::fromHsv(119, 255, 255);
}

const QColor &ColorManager::nextColor()
{
    int index = m_currentIndex;
    ++m_currentIndex;
    if (m_currentIndex >= m_colors.size()) {
        m_currentIndex = 0;
    }
    return m_colors[index];
}

void ColorManager::reset()
{
    m_currentIndex = 0;
}
