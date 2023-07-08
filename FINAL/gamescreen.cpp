#include "gamescreen.h"
#include "ui_gamescreen.h"
#include <QTime>
#include <QString>
#include <QStringList>
#include <unistd.h>
#include <cstdlib>
#include "prompt.h"

GameScreen::GameScreen(QWidget *parent, Client *client) :
    QMainWindow(parent),
    ui(new Ui::GameScreen),
    client(client)
{
    connect(client, &Client::roomNotFound, this, &GameScreen::roomNotFoundNotification);
    ui->setupUi(this);
    ui->GameWidget->setVisible(false);
    ui->NotiLabel->setVisible(false);
    connect(client, &Client::started, this, &GameScreen::handleStarted);
    connect(client, &Client::getAnswer, this, &GameScreen::handleGetAnswer);
    connect(client, &Client::getTopReceived, this, &GameScreen::onEndGame);
    ui->KeyWordButton->setChecked(false);
    ui->EnterButton->setEnabled(false);
    ui->LetterButton->setChecked(true);

    Point = 0;
}

GameScreen::~GameScreen()
{
    delete ui;
}


void GameScreen::handleStarted(){
    bool turn = client->isTurn();
    ui->label->setVisible(false);
    ui->GameWidget->setVisible(true);
    ui->QuizzLabel->setText(client->showQuizz());
    ui->SpinButton->setEnabled(turn);
    ui->TurnLabel->setVisible(turn);
    preTurn = turn;
}

void GameScreen::on_SpinButton_clicked()
{
    QStringList getScore;
//    getScore << "0"<< "250"<< "300"<< "400"<< "500"<< "650"<< "750"<< "Loose Point"<< "Loose Chance"<<"150"<< "100"<< "200"<< "350"<< "450"<< "550"<< "600"<< "700"<<"800";

    getScore<< "250"<< "300"<< "400"<< "500"<< "650"<< "750"<<"150"<< "100"<< "200"<< "350"<< "450"<< "550"<< "600"<< "700"<<"800";

    int randomNumber = arc4random_uniform(14);
    turnPoint = getScore[randomNumber].toInt();
//    if(randomNumber!=7 && randomNumber!=8){
//        turnPoint = getScore[randomNumber].toInt();
//    }

//    if(randomNumber==7){
//        Point = 0;
//    }
    ui -> ScoreLabel ->setText(getScore[randomNumber]);


    ui->EnterButton->setEnabled(true);
    ui->SpinButton->setEnabled(false);
}

void GameScreen::handleGetAnswer(){
    QString data = client->showQuizz();
    bool turn = client->isTurn();

    ui->SpinButton->setEnabled(preTurn==false && turn==true);
    ui->EnterButton->setEnabled(preTurn==true && turn==true);
    ui->TurnLabel->setVisible(turn);
    if(data == "LETTEREXISTED"){
        if(turn){
            QString notification = "The letter(s) " + input +" was already found!";
            ui->NotiLabel->setText(notification);
            ui->NotiLabel->setVisible(true);
        }
    }

    else if (data == "NOTFOUND"){
        if(preTurn==true && turn==false){
            QString notification = "There is no letter "+ input;
            ui->NotiLabel->setText(notification);
            ui->NotiLabel->setVisible(true);
        }
    }

    else if(data == "ENDGAME"){
        QString notification = "Game Ended!";
        ui->NotiLabel->setText(notification);
        ui->NotiLabel->setVisible(true);
        if(preTurn==true && turn==false){
            Point += 800;
            ui->LCDScore->display(Point);
        }
        client->endGame(Point);


    }
    else if(data == "WRONGANSWER"){
        QString notification = "Wrong answer";
        ui->NotiLabel->setText(notification);
        ui->NotiLabel->setVisible(true);
    }

    else{
        ui->QuizzLabel->setText(data);
        ui->NotiLabel->setVisible(false);
        if(preTurn==true && turn==false){
            Point += turnPoint;
            ui->LCDScore->display(Point);
        }
    }
    preTurn = turn;
}

void GameScreen::on_EnterButton_clicked()
{
    input = ui->LetterInput->text().toUpper();
    if(ui->LetterButton->isChecked()==true){
        if(input.size()==0){
            ui->NotiLabel->setText("Please enter the letter youu guess!");
            ui->NotiLabel->setVisible(true);
        }
        else if(input.size()>1){
            ui->NotiLabel->setText("Enter 1 letter only!");
            ui->NotiLabel->setVisible(true);
        }
        else{
            ui->NotiLabel->setVisible(false);
            client->guessLetter(input);
            ui->EnterButton->setEnabled(false);
            ui->SpinButton->setEnabled(true);
            ui->LetterInput->setText("");
        }
    }
    if(ui->KeyWordButton->isChecked()==true){
        if(input.size()==0){
            ui->NotiLabel->setText("Please enter the answer!");
            ui->NotiLabel->setVisible(true);
        }
        else{
            ui->NotiLabel->setVisible(false);
            client->guessQuizz(input);
            ui->EnterButton->setEnabled(false);
            ui->SpinButton->setEnabled(true);
            ui->LetterInput->setText("");
        }
    }

}
void GameScreen::roomNotFoundNotification(){
    QMainWindow* lobby = qobject_cast<QMainWindow*>(parent());
    close();
    if(lobby){
        lobby->show();
        Prompt *prompt = new Prompt();
        QString label="No room is available, please create a new one";
        prompt->setLabel(label);
        prompt->show();
    }
}

void GameScreen::on_KeyWordButton_clicked()
{
    ui->EnterButton->setEnabled(true);
    ui->SpinButton->setEnabled(false);
}


void GameScreen::on_LetterButton_clicked()
{
    ui->EnterButton->setEnabled(false);
    ui->SpinButton->setEnabled(true);
}
void GameScreen::onEndGame(){
    QMainWindow* lobby = qobject_cast<QMainWindow*>(parent());
    close();
    if(lobby){
        lobby->show();
        Prompt *prompt = new Prompt();
        QString label="You've finished the game with the top " + QString::number(client->getTop());
        prompt->setLabel(label);
        prompt->show();
        return;
    }
}

