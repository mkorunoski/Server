#include <QApplication>
#include <QSettings>
#include <QDir>

#include "server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSettings settings(QDir::currentPath().append("/config.ini"), QSettings::IniFormat);
    settings.beginGroup("Configuration parameters");
        int port            = settings.value("port").toInt();
        int maxConnections  = settings.value("maxConnections") .toInt();
        QString resultsFile = settings.value("resultsFile").toString();        
    settings.endGroup();

    Server server(port, maxConnections, resultsFile);

    return a.exec();
}
