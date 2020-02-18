#include "ProfileCalibrator5.hpp"
#include "ui_ProfileCalibrator5.h"

ProfileCalibrator5::ProfileCalibrator5(QWidget *parent) : QWidget(parent), ui(new Ui::ProfileCalibrator5)
{
    ui->setupUi(this);
}

ProfileCalibrator5::~ProfileCalibrator5()
{
    delete ui;
}
