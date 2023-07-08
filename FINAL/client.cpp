#include "client.h"
#include <QDebug>
#include <QString>
#include <QTextStream>
#include <QThread>
#include <QEventLoop>
Client::Client(QObject *parent) : QObject(parent)
{
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &Client::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
            this, &Client::onError);
    QuizzString ="";
    Turn = false;
    top = 0;
    host = false;
}

void Client::connectToServer()
{
    m_socket->connectToHost("127.0.0.1", 5500); // Thay đổi localhost và cổng kết nối nếu cần thiết
}
void Client::disconnectFromServer()
{
    m_socket->disconnectFromHost();
}
void Client::onConnected()
{
    qDebug() << "Connected to server";
    qDebug() << "Client port:" << m_socket->localPort();

     m_socket->write("ok");
}

void Client::onDisconnected()
{
    qDebug() << "Disconnected from server";
}

void Client::onReadyRead()
{
    QByteArray data = m_socket->readAll();
     qDebug() << "Received data from server:" << data;
    QString receivedData = QString::fromUtf8(data);
    QStringList stringList = receivedData.split(" "); // Tách các chuỗi bằng dấu cách

    if(stringList[0]=="LOGIN"){
        stringList.removeFirst();
        bool isNumber = false;
        rank = stringList.at(2).toInt(&isNumber);

        if(isNumber){
            stringList.removeAt(2);
        }

        loginNotification="";
        foreach (const QString& str, stringList) {
            loginNotification = loginNotification + str+ " ";
        }
        stringList.removeFirst();
        stringList.removeFirst();

        emit loginResponseReceived();

        if(loginNotification.contains("Logged in")){
            int  k = -1;
            QString roomInfor;
            for(int i= 0; i < stringList.size();i++){

                if(stringList[i]=="ROOM"){

                    k = 1;
                    roomInfor="ID: ";
                    continue;
                }
                else if(k ==1){
                    roomInfor=roomInfor+stringList[i]+"\t\t Room of: ";
                    k++;
                }
                else if(k == 2){
                    roomInfor=roomInfor+stringList[i]+"\t\t Rank: ";
                    k++;
                }
                else if(k == 3){
                    roomInfor=roomInfor+stringList[i];
                    k=0;
                    if(!roomList.contains(roomInfor)){
                        roomList.append(roomInfor);
                    }
                }
            }
            qDebug()<<roomList;

        }

    }
    if(stringList[0]=="SIGNUP"){
        stringList.removeFirst();
        signUpNotification="";
        foreach (const QString& str, stringList) {
            signUpNotification = signUpNotification + str + " ";
        }
        emit signUpResponseReceived();
    }
    if(stringList.last()=="TURN"){

        Turn = true;
    }
    if(stringList.last()!="TURN"){

        Turn = false;
    }
    if(stringList[0]=="PLAYRANDOM"){
        if(stringList[1]=="NOTFOUND"){
            emit roomNotFound();
        }
        else{
            QuizzString = stringList[1];
            roomId = stringList[2].toInt();
            emit started();
        }
    }
    if(stringList[0]=="CREATEROOM"){
        roomId = stringList[1].toInt();
    }
    if(stringList[0]=="NEWROOM"){
        QString newRoom = "ID: " + stringList[1]+"\t\t Room of: "+ stringList[3]+"\t\t Rank: "+ stringList[2];
        roomList.append(newRoom);
       emit updateRoomList();
    }
    if(stringList[0]=="GUESSLETTER"){
        QuizzString = stringList[1];
        host = true;
        emit getAnswer();
    }
    if(stringList[0]=="GUESSQUIZZ"){
        QuizzString = stringList[1];
        emit getAnswer();
    }
    if(stringList[0]=="FINISH"){
        if(stringList[1] == "FIRST") top = 1;
        if(stringList[1] == "SECOND") top = 2;
        if(stringList[1] == "THIRD") top = 3;
        emit getTopReceived();
    }

}

void Client::onError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Socket error:" << socketError;
}

void Client::readInput(QString data)
{

    m_socket->write(data.toUtf8());

}

QString Client::login(QString username,QString password){
    QString data = "LOGIN " + username +" "+ password;
    m_socket->write(data.toUtf8());

    QEventLoop loop;
    connect(this, &Client::loginResponseReceived, &loop, &QEventLoop::quit);
    loop.exec();
    if(loginNotification.contains("Logged in")){
        c_username = username;
        c_password = password;
    }
    return loginNotification;
}

QString Client::signup(QString username,QString password){
    QString data = "SIGNUP " + username +" "+ password;
    m_socket->write(data.toUtf8());

    QEventLoop loop;
    connect(this, &Client::signUpResponseReceived, &loop, &QEventLoop::quit);
    loop.exec();

    return signUpNotification;
}
void Client::playRandom(){
    QString data = "PLAYRANDOM "+QString::number(rank);
    m_socket->write(data.toUtf8());
}
void Client::playByRoom(QString roomid){
    QString data = "PLAYBYROOM "+roomid;
    m_socket->write(data.toUtf8());
}
void Client::createRoom(){
    QString data = "CREATEROOM " + QString::number(rank) +" "+c_username;
    m_socket->write(data.toUtf8());
}
QString Client::showQuizz(){
    return QuizzString;
}

void Client::guessLetter(QString letter){
    QString data = "GUESSLETTER " + letter +" "+QString::number(roomId);
    m_socket->write(data.toUtf8());
}

void Client::guessQuizz(QString answer){
    QString data = "GUESSQUIZZ " + answer  +" "+QString::number(roomId);
    m_socket->write(data.toUtf8());
}

bool Client::isTurn(){
    return Turn;
}
QString Client::getUsername(){
    return c_username;
}
void Client::endGame(int point){

    QString data = "ENDGAME "+ QString::number(roomId) +" "+ c_username+" "+ QString::number(point);
    m_socket->write(data.toUtf8());
    roomId=-1;
    QuizzString ="";
    Turn = false;
    top = 0;
    host = false;
}
int Client::getTop(){
    return top;
}
int Client::getRank(){
    return rank;
}
QStringList Client::getRoomList(){
    return roomList;
}

