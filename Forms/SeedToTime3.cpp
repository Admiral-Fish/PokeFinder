#include "SeedToTime3.hpp"
#include "ui_SeedToTime3.h"

SeedToTime3::SeedToTime3(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SeedToTime3)
{
    ui->setupUi(this);

    setupModels();
}

void SeedToTime3::setupModels() {
    ui->seedToTimeSeed->setValues(0, 32, false);

    if(m != NULL)
        delete m;
    m = new QStandardItemModel(this);
    m->setColumnCount(2);
    m->setHorizontalHeaderLabels(QStringList() << "Time" << "Seconds");
    ui->tableViewGenerator->setModel(m);
    ui->tableViewGenerator->verticalHeader()->setVisible(false);
    ui->tableViewGenerator->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

SeedToTime3::~SeedToTime3()
{
    delete ui;
}
