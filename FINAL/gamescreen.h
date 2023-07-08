#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <QWidget>
#include <QMainWindow>
#include "client.h"

namespace Ui {
class GameScreen;
}

class GameScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameScreen(QWidget *parent = nullptr, Client *client = nullptr);
    ~GameScreen();

private slots:
    void handleStarted();
    void handleGetAnswer();
    void on_SpinButton_clicked();
    void on_EnterButton_clicked();
    void on_KeyWordButton_clicked();
    void on_LetterButton_clicked();
    void onEndGame();
    void roomNotFoundNotification();

private:
    Ui::GameScreen *ui;
    Client *client;
    int Point;
    int turnPoint;
    QString input;
    bool preTurn;
};

#endif // GAMESCREEN_H
