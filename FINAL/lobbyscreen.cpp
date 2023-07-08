#include <QMainWindow>
#include "lobbyscreen.h"
#include "gamescreen.h"
#include "ui_lobbyscreen.h"
#include "client.h"
#include "prompt.h"
#include <QStringListModel>

LobbyScreen::LobbyScreen(QWidget *parent,Client *client) :
    QMainWindow(parent),
    ui(new Ui::LobbyScreen),
    client(client)
{
    ui->setupUi(this);
    connect(client, &Client::updateRoomList, this, &LobbyScreen::handleUpdateRoomList);
    ui->USName->setText(client->getUsername());
    int rank = client->getRank();
    if(rank == 0) ui->Rank->setText("Rank : Unrank");
    if(rank == 1) ui->Rank->setText("Rank : Bronze");
    if(rank == 2) ui->Rank->setText("Rank : Silver");
    if(rank == 3) ui->Rank->setText("Rank : Gold");
    if(rank == 4) ui->Rank->setText("Rank : Platinum");
    if(rank == 5) ui->Rank->setText("Rank : Diamond");
    if(rank == 6) ui->Rank->setText("Rank : Master");
    if(rank == 7) ui->Rank->setText("Rank : Grand Master");
    if(rank == 8) ui->Rank->setText("Rank : Challenger");



}

LobbyScreen::~LobbyScreen()
{
    delete ui;
}


void LobbyScreen::on_PlayGame_clicked()
{
    client->playRandom();
    GameScreen *gc = new GameScreen(this, client);
    gc->show();
    hide();
}

void LobbyScreen::on_CreateRoom_clicked()
{
    client->createRoom();
    GameScreen *gc = new GameScreen(this, client);
    gc->show();
    hide();

}




void LobbyScreen::on_roomList_doubleClicked(const QModelIndex &index)
{
    QString selectedItem = index.data(Qt::DisplayRole).toString();
    QRegularExpression regex("ID: (\\d+)\\s+Room of: (\\S+)\\s+Rank: (\\d+)");
    QRegularExpressionMatch match = regex.match(selectedItem);
    qDebug() << "Selected Item: " << selectedItem;

    QString room = match.captured(1);

    qDebug()<<room;
    client->playByRoom(room);
    GameScreen *gc = new GameScreen(this, client);
    gc->show();
    hide();
}

void LobbyScreen::handleUpdateRoomList(){
    QStringListModel* model = new QStringListModel();
    QStringList dataList;

    dataList =client->getRoomList();

    model->setStringList(dataList);

    ui->roomList->setModel(model);
    ui->roomList->setWrapping(true);
    ui->roomList->setWindowTitle("Room available");
}
