#include "Wild8.hpp"
#include "ui_Wild8.h"

Wild8::Wild8(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Wild8)
{
    ui->setupUi(this);
}

Wild8::~Wild8()
{
    delete ui;
}
