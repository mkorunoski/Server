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
    explicit Server(int, const QString &, QObject *parent = nullptr);
    ~Server();

public slots:
    void onNewConnection();
    void onReadyRead();

private:
    QString getKey(const QTcpSocket *);
    bool canCalculate();
    double sumFirst();

    static const quint16 sMaxNumLinesPerFile;

    QString mResltsFile;

    QTcpServer *mTcpServerPtr;
    QMap<QString, QList<double>> mSums;
    QVector<QTcpSocket *> mConnections;

};

#endif // SERVER_H
