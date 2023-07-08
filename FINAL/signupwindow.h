#ifndef SIGNUPWINDOW_H
#define SIGNUPWINDOW_H

#include <QMainWindow>
#include "client.h"

namespace Ui {
class SignUpWindow;
}

class SignUpWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SignUpWindow(QWidget *parent = nullptr, Client *client = nullptr );
    ~SignUpWindow();

private slots:
    void on_Signup_Button_clicked();

private:
    Ui::SignUpWindow *ui;
    Client *m_client;
};

#endif // SIGNUPWINDOW_H
