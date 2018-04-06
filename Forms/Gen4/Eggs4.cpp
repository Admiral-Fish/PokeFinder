#include "Eggs4.hpp"
#include "ui_Eggs4.h"

Eggs4::Eggs4(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Eggs4)
{
    ui->setupUi(this);
}

Eggs4::~Eggs4()
{
    delete ui;
}
