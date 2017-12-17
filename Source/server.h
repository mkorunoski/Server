#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>

class Server : public QObject
{
    Q_OBJECT

    using MapIterator = QMap<QString, QList<double>>::iterator;
    using ConstMapIterator = QMap<QString, QList<double>>::const_iterator;

public:
    explicit Server(int, int, const QString &, QObject *parent = nullptr);
    ~Server();

public slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();

private:
    QString getKey(const QTcpSocket *);
    bool canCalculate();
    void calculate();

    static const quint16 sMaxNumLinesPerFile;

    QString mResltsFile;

    int mMaxConnections;
    QTcpServer *mTcpServerPtr;
    QMap<QString, QList<double>> mSums;
    QVector<QTcpSocket *> mConnections;

};

#endif // SERVER_H
