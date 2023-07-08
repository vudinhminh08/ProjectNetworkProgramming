#ifndef PROMPT_H
#define PROMPT_H

#include <QWidget>
#include <QString>

namespace Ui {
class Prompt;
}

class Prompt : public QWidget
{
    Q_OBJECT

public:
    explicit Prompt(QWidget *parent = nullptr);
    ~Prompt();

public slots:
    void setLabel(QString string);
    void showPromptAboveMain();
private slots:
    void on_pushButton_clicked();

private:
    Ui::Prompt *ui;
};

#endif // PROMPT_H
