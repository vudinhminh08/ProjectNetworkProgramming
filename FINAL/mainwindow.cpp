#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "signupwindow.h"
#include "prompt.h"
#include "lobbyscreen.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Notify_Label->setVisible(false);
    client.connectToServer();

    QShortcut* shortcut = new QShortcut(Qt::Key_Return, this);
    connect(shortcut, &QShortcut::activated, this, &MainWindow::on_OK_Btn_clicked);

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_OK_Btn_clicked()
{
    QString label =  client.login(ui->usernameInput->text(),ui->passwordInput->text());
    qDebug()<<label;
    if(label.contains("Logged in")){
        LobbyScreen *lobby = new LobbyScreen(this, &client);
        lobby->show();
        hide();
        client.updateRoomList();
    }
    else{
        ui->Notify_Label->setText(label);
        ui->Notify_Label->setVisible(true);
    }
}


void MainWindow::on_SignUp_Btn_clicked()
{
    // Create a new instance of the signup window
    SignUpWindow* signupWindow = new SignUpWindow(this, &client);
    signupWindow->show();
    // Hide the current main window
    hide();
}


