#include "src/config.h"
#include "src/gui/main_window.h"
#include <QApplication>

#include "src/helpers/log_in_file_manager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(QObject::tr("Оптимальная фильтрация"));
    a.setApplicationVersion("3.0.0");
    a.setApplicationDisplayName(a.applicationName() + "  " + a.applicationVersion());

    // По умолчанию закоментированно
//    LogManager& logInstance = LogManager::Instance();
//    logInstance.initLog();

    MainWindow window;
    window.show();

    return a.exec();
}
