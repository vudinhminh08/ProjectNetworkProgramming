#include "signupwindow.h"
#include "ui_signupwindow.h"
#include "mainwindow.h"
#include "prompt.h"

SignUpWindow::SignUpWindow(QWidget *parent, Client *client) :
    QMainWindow(parent),
    ui(new Ui::SignUpWindow),
    m_client(client)

{
    ui->setupUi(this);
    ui->signupNotification->setVisible(false);
}

SignUpWindow::~SignUpWindow()
{
    delete ui;
}

void SignUpWindow::on_Signup_Button_clicked()
{
    QString username = ui->usernameInput->text();
    QString password = ui->passwordInput->text();
    QString reEnterPassword = ui->passwordReEnter->text();
    if(password != reEnterPassword){
        ui->signupNotification->setVisible(true);
        ui->signupNotification->setText("Re-enter password is not match");
    }
    else{
        QString label = m_client->signup(username,password);
        if (label == "ok "){
            close();  // Close the current SignUpWindow
            QMainWindow* mainWindow = qobject_cast<QMainWindow*>(parent());  // Get the parent main window
            if (mainWindow)
            {

                mainWindow->show();  // Show the main window
                Prompt *prompt = new Prompt();

                prompt->setLabel("Your account is created!\nYou now can login with your account");
                prompt->show();
                return;
            }
        }
        else {
            ui->signupNotification->setVisible(true);
            ui->signupNotification->setText(label);
        }
    }
}

