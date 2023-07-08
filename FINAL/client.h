#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QString>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
signals:
    void loginResponseReceived();
    void signUpResponseReceived();
    void started();
    void getAnswer();
    void getTopReceived();
    void roomNotFound();
    void updateRoomList();
public slots:
    void connectToServer();
    void disconnectFromServer();
    void readInput(QString data);
    QString login(QString username,QString password);
    QString signup(QString username,QString password);
    void playRandom();
    void createRoom();
    QString showQuizz();
    void guessLetter(QString letter);
    void guessQuizz(QString answer);
    bool isTurn();
    QString getUsername();
    void endGame(int point);
    int getTop();
    int getRank();
    QStringList getRoomList();
    void playByRoom(QString roomid);
private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *m_socket;
    int m_ccount;
    QString loginNotification;
    QString signUpNotification;
    QString QuizzString;
    bool Turn;
    QString c_username;
    QString c_password;
    int top;
    int roomId;
    bool host;
    int point;
    int rank;
    QStringList roomList;
};

#endif // CLIENT_H
