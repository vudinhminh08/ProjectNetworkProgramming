#include "server.h"
#include <QDebug>
#include <pthread.h>
#include<algorithm>
#include<QThread>

Server::Server(QObject *parent) : QObject(parent)
{
    m_server = new QTcpServer(this);

    connect(m_server, &QTcpServer::newConnection, this, &Server::onNewConnection);

    turn = 0;
    clients_in_room.resize(10);
    clientCount.resize(10);
    roomRank.resize(10);
    roomHostName.resize(10);
    for (int i = 0; i < clients_in_room.size(); ++i) {
        clients_in_room[i].resize(3);
        clientCount[i]= 0;
    }

}


void Server::startServer()
{
    if (!m_server->listen(QHostAddress::Any, 5500))
    {
        qDebug() << "Could not start server";
        return;
    }

    qDebug() << "Server started";
}

void Server::onNewConnection()
{

    QTcpSocket *clientSocket = m_server->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::onDisconnected);
    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);

    m_clients.append(clientSocket);


    qDebug() << "New connection:"<<m_clients.size();

    clientSocket->write("connection accepted");
}

void Server::onDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    qDebug() << "Disconnected:" << clientSocket->peerAddress().toString();

    m_clients.removeOne(clientSocket);

    clientSocket->deleteLater();
}
int Server::roomIdGenerator(int rank){
    int smallestNumber;

    for (int i = 0; i < 10; ++i) {
        if (!availableRoom.contains(i)) {
            smallestNumber = i;
            break; // Exit the loop when the first missing number is found
        }
    }
    availableRoom.append(smallestNumber);
    roomRank[smallestNumber] = rank;
    return smallestNumber;
}
int Server::roomIdAutoFind(int rank){
    int roomId = -1;
    foreach (int i, availableRoom) {
        if(clientCount[i] < 3 && (roomRank[i]-1<= rank) && (roomRank[i]+1 >= rank)){
            roomId=i;
            break;
        }
    }
    return roomId;
}

void Server::onReadyRead()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    QByteArray data = clientSocket->readAll();
    QString receivedData = QString::fromUtf8(data);
    QStringList stringList = receivedData.split(" "); // Tách các chuỗi bằng dấu cách

    if(stringList[0]=="LOGIN"){
        QString result =db.checkLogin(stringList[1],stringList[2]);
        QString loginStatus = "LOGIN "+ result+" ";

        clientSocket->write(loginStatus.toUtf8());

        if(result.contains("Logged in")){
            foreach (int id, availableRoom) {
                QString mes = "ROOM " + QString::number(id) +" "+ roomHostName[id] +" "+QString::number(roomRank[id])+" ";
                clientSocket->write(mes.toUtf8());
            }
        }
    }
    if(stringList[0]=="SIGNUP"){
        QString result =db.checkSignUp(stringList[1],stringList[2]);
        QString signupStatus = "SIGNUP "+ result;
        clientSocket->write(signupStatus.toUtf8());
    }

    if(stringList[0]=="PLAYRANDOM"){
        int rank = stringList[1].toInt();
        int roomId = roomIdAutoFind(rank);
        if(roomId ==-1){
            qDebug()<<"No room is available";
            QString signupStatus = "PLAYRANDOM NOTFOUND";
            clientSocket->write(signupStatus.toUtf8());
        }
        else{
            clients_in_room[roomId][clientCount[roomId]] = clientSocket;
            clientCount[roomId] = clientCount[roomId]+1;

            if(clientCount[roomId]== 3){
                QString Quizz = gamePlay.getQuizz(roomId);
                QString Ans = "PLAYRANDOM " + Quizz +" "+QString::number(roomId);
                foreach (QTcpSocket* socket, clients_in_room[roomId]) {
                    socket->write(Ans.toUtf8());
                }
                clients_in_room[roomId][turn]->write(" TURN");

            }
        }
    }
    if(stringList[0]=="PLAYBYROOM"){

        int roomId = stringList[1].toInt();

        clients_in_room[roomId][clientCount[roomId]] = clientSocket;
        clientCount[roomId] = clientCount[roomId]+1;
        if(clientCount[roomId]== 3){
            QString Quizz = gamePlay.getQuizz(roomId);
            QString Ans = "PLAYRANDOM " + Quizz +" "+QString::number(roomId);
            foreach (QTcpSocket* socket, clients_in_room[roomId]) {
                socket->write(Ans.toUtf8());
            }
            clients_in_room[roomId][turn]->write(" TURN");

        }
    }
    if(stringList[0]=="CREATEROOM"){

        int rank = stringList[1].toInt();
        int roomId = roomIdGenerator(rank);
        QString name = stringList[2];
        clients_in_room[roomId][clientCount[roomId]] = clientSocket;
        clientCount[roomId] = clientCount[roomId]+1;
        roomHostName[roomId] = name;
        QString Ans = "CREATEROOM " + QString::number(roomId);
        clientSocket->write(Ans.toUtf8());

        foreach(QTcpSocket *socket,m_clients){
            if(socket != clientSocket){
                Ans = "NEWROOM " + QString::number(roomId) +" "+ QString::number(rank) +" "+ name;
                socket->write(Ans.toUtf8());
            }
        }
    }
    if(stringList[0]=="GUESSLETTER"){

        int roomId = stringList[2].toInt();
        QString Quizz = gamePlay.guessLetter(stringList[1],roomId);
        QString Ans = "GUESSLETTER " + Quizz;
        foreach (QTcpSocket* socket, clients_in_room[roomId]) {
            socket->write(Ans.toUtf8());
        }
        if(Quizz !="LETTEREXISTED"){
            if (turn ==2 ) turn = 0;
            else turn ++;
        }
        clients_in_room[roomId][turn]->write(" TURN");

    }

    if(stringList[0]=="GUESSQUIZZ"){

        int roomId = stringList[2].toInt();
        QString Quizz = gamePlay.guessQuizz(stringList[1],roomId);
        QString Ans = "GUESSQUIZZ " + Quizz;
        foreach (QTcpSocket* socket, clients_in_room[roomId]) {
            socket->write(Ans.toUtf8());
        }
        if(Quizz !="LETTEREXISTED"){
            if (turn ==2 ) turn = 0;
            else turn ++;
        }
        clients_in_room[roomId][turn]->write(" TURN");
    }
    if(stringList[0]=="ENDGAME"){

        int roomId =stringList[1].toInt();
        QString name = stringList[2];
        int point = stringList[3].toInt();

        gamePlay.result(name,point,roomId,clientSocket);

        clients_in_room[roomId].clear();
        clients_in_room[roomId].resize(3);
        clientCount[roomId]= 0;
        availableRoom.removeOne(roomId);
        roomRank[roomId]= 0;
        roomHostName[roomId]="";
    }

}
