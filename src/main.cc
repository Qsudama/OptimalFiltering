#include "src/config.h"
#include "src/gui/main_window.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(QObject::tr("Оптимальная фильтрация"));
    a.setApplicationVersion("2.6.1");
    a.setApplicationDisplayName(a.applicationName() + "  " + a.applicationVersion());

    MainWindow window;
    window.show();

    return a.exec();
}
