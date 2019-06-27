#include "color_manager.h"


ColorManager::ColorManager()
    : m_currentIndex(0)
    , m_realizationECollorsCurrentIndex(0)
    , m_realizationZCollorsCurrentIndex(0)
{
    initColors();
}

void ColorManager::initColors()
{
    m_colors.resize(10);
    m_colors[0]  = QColor::fromRgb(255, 0, 4);
    m_colors[1]  = QColor::fromRgb(0, 255, 251);
    m_colors[2]  = QColor::fromRgb(0, 4, 255);
    m_colors[3]  = QColor::fromRgb(251, 0, 255);
    m_colors[4]  = QColor::fromRgb(0, 255, 123);
    m_colors[5]  = QColor::fromRgb(255, 123, 0);
    m_colors[6]  = QColor::fromRgb(0, 132, 255);
    m_colors[7]  = QColor::fromRgb(132, 255, 0);
    m_colors[8]  = QColor::fromRgb(123, 0, 255);
    m_colors[9]  = QColor::fromRgb(255, 0, 132);

    m_realization_e_colors.resize(10);
    m_realization_e_colors[0]  = QColor::fromRgb(0, 170, 169);
    m_realization_e_colors[1]  = QColor::fromRgb(150, 0, 4);
    m_realization_e_colors[2]  = QColor::fromRgb(142, 0, 141);
    m_realization_e_colors[3]  = QColor::fromRgb(0, 4, 151);
    m_realization_e_colors[4]  = QColor::fromRgb(188, 103, 0);
    m_realization_e_colors[5]  = QColor::fromRgb(0, 173, 123);
    m_realization_e_colors[6]  = QColor::fromRgb(132, 172, 0);
    m_realization_e_colors[7]  = QColor::fromRgb(0, 100, 188);
    m_realization_e_colors[8]  = QColor::fromRgb(187, 0, 120);
    m_realization_e_colors[9]  = QColor::fromRgb(90, 0, 181);

    m_realization_z_colors.resize(10);
    m_realization_z_colors[0]  = QColor::fromRgb(150, 0, 4);
    m_realization_z_colors[1]  = QColor::fromRgb(0, 170, 169);
    m_realization_z_colors[2]  = QColor::fromRgb(0, 4, 151);
    m_realization_z_colors[3]  = QColor::fromRgb(142, 0, 141);
    m_realization_z_colors[4]  = QColor::fromRgb(0, 173, 123);
    m_realization_z_colors[5]  = QColor::fromRgb(188, 103, 0);
    m_realization_z_colors[6]  = QColor::fromRgb(0, 100, 188);
    m_realization_z_colors[7]  = QColor::fromRgb(132, 172, 0);
    m_realization_z_colors[8]  = QColor::fromRgb(90, 0, 181);
    m_realization_z_colors[9]  = QColor::fromRgb(187, 0, 120);

}

const int &ColorManager::nextColorIndex()
{
    int index = m_currentIndex;
    ++m_currentIndex;

    return index % m_colors.size();
}

const QColor &ColorManager::nextColorAtIndex(int index)
{
    if (index < m_colors.count()) {
        return m_colors[index];
    } else {
        return m_colors[0];
    }

}

const QColor &ColorManager::nextColorRealizationE()
{
    int index = m_realizationECollorsCurrentIndex;
    ++m_realizationECollorsCurrentIndex;
    if (m_realizationECollorsCurrentIndex >= m_realization_e_colors.size()) {
        m_realizationECollorsCurrentIndex = 0;
    }
    return m_realization_e_colors[index];
}

const QColor &ColorManager::nextColorRealizationZ()
{
    int index = m_realizationZCollorsCurrentIndex;
    ++m_realizationZCollorsCurrentIndex;
    if (m_realizationZCollorsCurrentIndex >= m_realization_z_colors.size()) {
        m_realizationZCollorsCurrentIndex = 0;
    }
    return m_realization_z_colors[index];
}

void ColorManager::reset()
{
    m_currentIndex = 0;
    m_realizationECollorsCurrentIndex = 0;
    m_realizationZCollorsCurrentIndex = 0;
}
