#ifndef LOBBYSCREEN_H
#define LOBBYSCREEN_H

#include <QWidget>
#include <QMainWindow>
#include "client.h"

namespace Ui {
class LobbyScreen;
}

class LobbyScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit LobbyScreen(QWidget *parent = nullptr, Client *client = nullptr);
    ~LobbyScreen();

private slots:

    void on_PlayGame_clicked();

    void on_CreateRoom_clicked();

    void on_roomList_doubleClicked(const QModelIndex &index);

    void handleUpdateRoomList();

private:
    Ui::LobbyScreen *ui;
    Client *client;

};

#endif // LOBBYSCREEN_H
