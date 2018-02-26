#include "Egg3.hpp"
#include "ui_Egg3.h"

Egg3::Egg3(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Egg3)
{
    ui->setupUi(this);
}

Egg3::~Egg3()
{
    delete ui;
}

void Egg3::setupModels()
{

}

void Egg3::changeEvent(QEvent *event)
{
    if (event != NULL)
    {
        switch (event->type())
        {
            case QEvent::LanguageChange:
                ui->retranslateUi(this);
                setupModels();
                break;
            default:
                break;
        }
    }
}
