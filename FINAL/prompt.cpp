#include "prompt.h"
#include "ui_prompt.h"

Prompt::Prompt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Prompt)
{
    ui->setupUi(this);
}

Prompt::~Prompt()
{
    delete ui;
}

void Prompt::setLabel(QString string){
    ui->label->setText(string);
}

void Prompt::on_pushButton_clicked()
{
    hide();
}
void Prompt::showPromptAboveMain()
{
    setWindowModality(Qt::ApplicationModal);  // Set the window modality to application modal
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);  // Set the window flag to stay on top

    // Show the prompt window
    show();
}
