#include "alert_helper.h"

void AlertHelper::showErrorAlertWithText(QString text)
{
    AlertHelper::showAlert("Ошибка", text, QMessageBox::Information);
}

void AlertHelper::showWarningAlertWithText(QString text)
{
    AlertHelper::showAlert("Внимание", text, QMessageBox::Warning);
}

void AlertHelper::showAlert(QString title, QString text, QMessageBox::Icon icon)
{
     QMessageBox msgBox;
     msgBox.setText(title);
     msgBox.setInformativeText(text);
     msgBox.setIcon(icon);
     msgBox.setDefaultButton(QMessageBox::Ok);
     QSpacerItem* horizontalSpacer = new QSpacerItem(300, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
     QGridLayout* layout = (QGridLayout*)msgBox.layout();
     layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
     msgBox.exec();
}
