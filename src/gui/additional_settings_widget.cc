#include "additional_settings_widget.h"
#include "src/gui/font_manager.h"
#include "src/gui/gui_config.h"


using namespace Math::LinAlg;


AdditionalSettingsWidget::AdditionalSettingsWidget(QWidget *parent)
    : QGroupBox(parent)
{
    setTitle(tr("Дополнительные настройки"));
    initControls();
    initLayouts();
    loadFonts();
}

AdditionalSettingsWidget::~AdditionalSettingsWidget()
{
}

void AdditionalSettingsWidget::loadFonts()
{
    QFont font = FontManager::instance().regular(GuiConfig::FONT_SIZE_NORMAL);
    setFont(font);
    m_radioPinvGreville->setFont(font);
    m_radioPinvSvd->setFont(font);
}

void AdditionalSettingsWidget::initControls()
{
    m_radioPinvGreville = new QRadioButton(tr("Использовать метод SVD"));
    m_radioPinvGreville->setChecked(true);
    connect(m_radioPinvGreville, SIGNAL(toggled(bool)), this, SLOT(onPinvGrevilleToggled(bool)));

    m_radioPinvGreville = new QRadioButton(tr("Использовать метод Гревиля"));
    m_radioPinvGreville->setChecked(false);
    connect(m_radioPinvGreville, SIGNAL(toggled(bool)), this, SLOT(onPinvGrevilleToggled(bool)));
}

void AdditionalSettingsWidget::initLayouts()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(GuiConfig::LAYOUT_MARGIN_BIG);
    mainLayout->setSpacing(GuiConfig::LAYOUT_SPACING_BIG);

    mainLayout->addWidget(m_radioPinvSvd);
    mainLayout->addWidget(m_radioPinvGreville);

    this->setLayout(mainLayout);
}

void AdditionalSettingsWidget::onPinvSvdToggled(bool checked)
{
    if (checked) {
        SetPinvMethod(PseudoinverseMethods::SVD);
    }
}

void AdditionalSettingsWidget::onPinvGrevilleToggled(bool checked)
{
    if (checked) {
        SetPinvMethod(PseudoinverseMethods::GREVILLE);
    }
}
