#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "database.h"
#include "gameplay.h"
#define CAPACITY 3

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

public slots:
    void startServer();

private slots:
    void onNewConnection();
    void onDisconnected();
    void onReadyRead();
    int roomIdGenerator(int rank);
    int roomIdAutoFind(int rank);

private:
    QTcpServer *m_server;
    QList<QTcpSocket *> m_clients;
    QVector<QVector<QTcpSocket *>> clients_in_room;

    Database db;
    QVector<int> clientCount;
    GamePlay gamePlay;
    int turn;
    QList<int> availableRoom;
    QVector<int> roomRank;
    QVector<QString>roomHostName;
};

#endif // SERVER_H
