#include "pinv_method_setting_widget.h"
#include "src/gui/font_manager.h"
#include "src/gui/gui_config.h"

using namespace Math::LinAlg;

PinvMethodSettingWidget::PinvMethodSettingWidget(QWidget *parent)
    : QGroupBox(parent)
{
    setTitle(tr("Псевдообращение матриц"));
    initControls();
    initLayouts();
    loadFonts();
}

PinvMethodSettingWidget::~PinvMethodSettingWidget()
{
}

void PinvMethodSettingWidget::loadFonts()
{
    QFont font = FontManager::instance().regular(GuiConfig::FONT_SIZE_NORMAL);
    setFont(font);
    m_radioPinvGreville->setFont(font);
    m_radioPinvSvd->setFont(font);
}

void PinvMethodSettingWidget::initControls()
{
    m_radioPinvSvd = new QRadioButton(tr("Метод SVD"));
    m_radioPinvSvd->setChecked(true);
    connect(m_radioPinvSvd, SIGNAL(toggled(bool)), this, SLOT(onPinvSvdToggled(bool)));

    m_radioPinvGreville = new QRadioButton(tr("Метод Гревиля"));
    m_radioPinvGreville->setChecked(false);
    connect(m_radioPinvGreville, SIGNAL(toggled(bool)), this, SLOT(onPinvGrevilleToggled(bool)));
}

void PinvMethodSettingWidget::initLayouts()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(GuiConfig::LAYOUT_MARGIN_BIG);
    mainLayout->setSpacing(GuiConfig::LAYOUT_SPACING_BIG);

    mainLayout->addWidget(m_radioPinvSvd);
    mainLayout->addWidget(m_radioPinvGreville);

    this->setLayout(mainLayout);
}

void PinvMethodSettingWidget::onPinvSvdToggled(bool checked)
{
    if (checked) {
        CurrentPinvMethod::instance().set(PseudoinverseMethods::SVD);
    }
}

void PinvMethodSettingWidget::onPinvGrevilleToggled(bool checked)
{
    if (checked) {
        CurrentPinvMethod::instance().set(PseudoinverseMethods::GREVILLE);
    }
}
