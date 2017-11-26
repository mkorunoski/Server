#include <QCoreApplication>
#include <QSettings>
#include <QDir>

#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSettings settings(QDir::currentPath().append("/config.ini"), QSettings::IniFormat);
    settings.beginGroup("Configuration parameters");
        int port            = settings.value("port").toInt();
        QString resultsFile = settings.value("resultsFile").toString();
    settings.endGroup();

    Server server(port, resultsFile);

    return a.exec();
}
