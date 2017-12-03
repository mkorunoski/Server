#include "server.h"

#include <QDebug>
#include <QByteArray>

#include "files.h"

const quint16 Server::sMaxNumLinesPerFile = 512;

Server::Server(int port, int maxConnections, const QString &resltsFile, QObject *parent)
    : QObject(parent),
      mResltsFile(resltsFile)
{
    Q_UNUSED(maxConnections);

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
}

void Server::onReadyRead()
{
    QTcpSocket *senderPtr = qobject_cast<QTcpSocket *>(sender());
    QByteArray filename = senderPtr->readAll();

    mSums[getKey(senderPtr)].append(Files::sumFileContent(QString::fromUtf8(filename).toStdString()));

    if (canCalculate())
    {
        auto sum = sumFirst() / (mSums.size() * sMaxNumLinesPerFile);
        Files::write(sum, mResltsFile.toStdString());
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
    for (ConstMapIterator it = mSums.constBegin(); it != mSums.constEnd(); ++it)
    {
        if (0 == it->size())
            return false;
    }

    return true;
}

double Server::sumFirst()
{
    double sum = 0.0;
    for (MapIterator it = mSums.begin(); it != mSums.end(); ++it)
    {
        sum += it->first();
        it->removeFirst();
    }
    return sum;
}
