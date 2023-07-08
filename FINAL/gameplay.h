#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <QObject>
#include <QString>
#include "database.h"
#include <QtSql>
#include <QTcpSocket>

class GamePlay : public QObject
{
    Q_OBJECT
public:
    explicit GamePlay(QObject *parent = nullptr);

public slots:
    QString getQuizz(int roomId);
    QString guessLetter(QString letter,int roomId);
    QString guessQuizz(QString answers,int roomId);
    void result(QString name, int point,int roomId, QTcpSocket *socket = nullptr);
private:
    QVector<QString> Quizz;
    QVector<QString> QuizzMask;
    Database db;
    QVector<int> clientCount;
    QVector<QVector<QString>> nameList;
    QVector<QVector<int>> pointList;
    QVector<QVector<QTcpSocket*>> socketList;
};

#endif // GAMEPLAY_H
