#include "IDs4.hpp"
#include "ui_IDs4.h"

IDs4::IDs4(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IDs4)
{
    ui->setupUi(this);
}

IDs4::~IDs4()
{
    delete ui;
}
