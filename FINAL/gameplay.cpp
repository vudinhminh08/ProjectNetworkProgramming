#include "gameplay.h"
#include <QObject>
#include <QChar>
#include <QSql>

GamePlay::GamePlay(QObject *parent): QObject(parent)
{


    clientCount.resize(10);
    nameList.resize(10);
    pointList.resize(10);
    socketList.resize(10);
    for(int i = 0 ; i < 10; i++){
        clientCount[i]=0;
        nameList[i].resize(3);
        pointList[i].resize(3);
        socketList[i].resize(3);

    }
    Quizz.resize(10);
    QuizzMask.resize(10);

}

QString GamePlay::getQuizz(int roomId){
    Quizz[roomId] = db.getQuizz();
    for(int i = 0; i < Quizz[roomId].size();i++){
        QuizzMask[roomId] = QuizzMask[roomId] + "*";
    }
    return QuizzMask[roomId];
}


QString GamePlay::guessLetter(QString letter,int roomId){
    QChar guessedletter = letter.at(0);
    if(Quizz[roomId].indexOf(guessedletter) == -1){
        return "NOTFOUND";
    }
    if(QuizzMask[roomId].indexOf(guessedletter) != -1){
        return "LETTEREXISTED";
    }
    for(int i = 0 ; i < Quizz[roomId].size() ; i++){
        if(Quizz[roomId].at(i)==guessedletter){
            QuizzMask[roomId].replace(i,1, guessedletter);
        }
    }
    if(Quizz[roomId] ==QuizzMask[roomId]){
        return "ENDGAME";
    }
    return QuizzMask[roomId];
}

QString GamePlay::guessQuizz(QString answer,int roomId){
    if(answer == Quizz[roomId]) return "ENDGAME";
    return "WRONGANSWER";
}

void GamePlay::result(QString name, int point, int roomId, QTcpSocket *socket){
    int first, second, third;
    nameList[roomId][clientCount[roomId]]=name;
    pointList[roomId][clientCount[roomId]]=point;
    socketList[roomId][clientCount[roomId]]=socket;
    clientCount[roomId]++;

    if(clientCount[roomId] == 3){
        QVector<int> sortList(pointList[roomId]);

        std::sort(sortList.begin(),sortList.end());
        first = pointList[roomId].indexOf(sortList.at(2));
        second = pointList[roomId].indexOf(sortList.at(1));
        third = pointList[roomId].indexOf(sortList.at(0));

        qDebug()<<nameList[roomId][first]<<nameList[roomId][second]<<nameList[roomId][third];

//        db.setPoint(nameList.at(first), 10);
//        db.setPoint(nameList.at(second), 0);
//        db.setPoint(nameList.at(third), -10);

        socketList[roomId][first]->write("FINISH FIRST");
        socketList[roomId][second]->write("FINISH SECOND");
        socketList[roomId][third]->write("FINISH THIRD");

        clientCount[roomId]=0;
        nameList[roomId].clear();
        pointList[roomId].clear();
        socketList[roomId].clear();
        nameList[roomId].resize(3);
        pointList[roomId].resize(3);
        socketList[roomId].resize(3);
        Quizz[roomId] ="";
        QuizzMask[roomId] = "";
    }
}

