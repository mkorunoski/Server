#include "server.h"

#include <ctime>

#include <QApplication>
#include <QDebug>
#include <QByteArray>
#include <QStringRef>

#include "files.h"
#include "test.h"

const quint16 Server::sMaxNumLinesPerFile = 512;

Server::Server(int port, int maxConnections, const QString &resltsFile, QObject *parent)
    : QObject(parent),
      mResltsFile(resltsFile),
      mMaxConnections(maxConnections)
{
    mTcpServerPtr = new QTcpServer(this);

    connect(mTcpServerPtr, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

    if (!mTcpServerPtr->listen(QHostAddress::Any, port))
    {
        qCritical() << "Server could not start!";
    }
    else
    {
        qInfo() << "Server started!";
    }
}

Server::~Server()
{
    delete mTcpServerPtr;
    qDeleteAll(mConnections);
}

void Server::onNewConnection()
{
    mConnections.append(mTcpServerPtr->nextPendingConnection());

    if (!mConnections.last())
    {
        return;
    }

    connect(mConnections.last(), SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(mConnections.last(), SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}

void Server::onReadyRead()
{
    QTcpSocket *senderPtr = qobject_cast<QTcpSocket *>(sender());
    QByteArray filename = senderPtr->readAll();

    mSums[getKey(senderPtr)].append(Files::sumFileContent(QString::fromUtf8(filename).toStdString()));

    if (canCalculate())
    {
        calculate();
    }
}

void Server::onDisconnected()
{
    QTcpSocket *socketPtr = qobject_cast<QTcpSocket *>(sender());
    auto ok = mConnections.removeOne(socketPtr);

    if (!ok)
        return;

    qDebug() << "Client successfully disconnected.";

    if (0 == mConnections.size())
    {
        while (canCalculate())
            calculate();

        Test::instance()->saveResults(QStringRef(&mResltsFile, 0, mResltsFile.lastIndexOf("/")).toString());
        QApplication::exit();
    }
}

QString Server::getKey(const QTcpSocket *socket)
{
    return QString("%1 %2")
            .arg(socket->peerAddress().toString())
            .arg(socket->peerPort());
}

bool Server::canCalculate()
{
    if (mSums.size() != mMaxConnections)
        return false;

    for (auto it = mSums.constBegin(); it != mSums.constEnd(); ++it)
    {
        if (0 == it->size())
            return false;
    }

    return true;
}

void Server::calculate()
{
    auto start = std::clock();
    double sum = 0.0;
    for (MapIterator it = mSums.begin(); it != mSums.end(); ++it)
    {
        sum += it->first();
        it->removeFirst();
    }
    auto stop = std::clock();

    auto execTime = (stop - start) / (double) CLOCKS_PER_SEC * 1000;
    Test::instance()->newExecTime(execTime);

    sum /= (mSums.size() * sMaxNumLinesPerFile);

    Files::write(sum, mResltsFile.toStdString());
}
