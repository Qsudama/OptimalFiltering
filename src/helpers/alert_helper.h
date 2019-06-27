#ifndef ALERT_HELPER_H
#define ALERT_HELPER_H
//#include <QDebug>

#include <string>
#include "QString"

#include <QMessageBox>
#include <QSpacerItem>
#include <QGridLayout>

class AlertHelper
{

public:

    static void showErrorAlertWithText(QString text);
    static void showWarningAlertWithText(QString text);

private:

    static void showAlert(QString title, QString text, QMessageBox::Icon icon);

};

#endif // ALERT_HELPER_H
