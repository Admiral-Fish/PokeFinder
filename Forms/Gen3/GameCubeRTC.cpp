#include "GameCubeRTC.hpp"
#include "ui_GameCubeRTC.h"

GameCubeRTC::GameCubeRTC(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameCubeRTC)
{
    ui->setupUi(this);

    setupModels();

    qRegisterMetaType<QList<QStandardItem *>>("QList<QStandardItem *>");
    connect(this, SIGNAL(updateView(QList<QStandardItem *>)), this, SLOT(updateTableView(QList<QStandardItem *>)));
}

GameCubeRTC::~GameCubeRTC()
{
    delete ui;
}

void GameCubeRTC::setupModels()
{
    ui->textBoxStartSeed->setValues(0, 32, false);
    ui->textBoxEndSeed->setValues(0, 32, false);
    ui->textBoxMinFrame->setValues(1, 32, true);
    ui->textBoxMaxFrame->setValues(2, 32, true);

    m->setHorizontalHeaderLabels(QStringList() << "Time" << "Frame" << "Seed");
    ui->tableViewGenerator->setModel(m);
    ui->tableViewGenerator->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void GameCubeRTC::updateTableView(QList<QStandardItem*> row)
{
    m->appendRow(row);
    ui->tableViewGenerator->viewport()->update();
}

void GameCubeRTC::calcRTC()
{
    isSearching = true;

    u32 initSeed = ui->textBoxStartSeed->text().toUInt(NULL, 16);
    u32 targetSeed = ui->textBoxEndSeed->text().toUInt(NULL, 16);
    u32 minFrame = ui->textBoxMinFrame->text().toUInt(NULL, 10);
    u32 maxFrame = ui->textBoxMaxFrame->text().toUInt(NULL, 10);

    LCRNG back = XDRNGR(targetSeed);
    back.advanceFrames(minFrame);
    targetSeed = back.seed;

    LCRNG rng = XDRNG(initSeed);

    u32 seconds = 0;
    u32 secoundCount = 0;
    bool targetHit = false;
    int minutes = 0;

    while(!targetHit)
    {
        rng.seed = initSeed;

        for(u32 x = 0; x < maxFrame; x++)
        {
            if(rng.nextUInt() == targetSeed)
            {
                QDateTime finalTime = date.addSecs(seconds);
                QList<QStandardItem*> row;
                QString time = finalTime.toString(Qt::SystemLocaleShortDate);
                row << (time.contains("M") ? new QStandardItem(time.insert((time.indexOf('M') - 2), ":" + QString::number(finalTime.time().second()))) : new QStandardItem(time.append(":" + QString::number(finalTime.time().second())))) << new QStandardItem(QString::number(x + 2 + minFrame)) << new QStandardItem(QString::number(initSeed, 16).toUpper());
                emit updateView(row);
                isSearching = false;
                return;
            }
        }

        initSeed += 40500000;
        seconds += 1;
        secoundCount += 1;

        if(secoundCount == 60)
        {
            minutes += 1;
            secoundCount = 0;
        }
    }

}

void GameCubeRTC::on_pushButtonSearch_clicked()
{
    if(isSearching)
        return;
    m->removeRows(0, m->rowCount());
    ui->tableViewGenerator->viewport()->update();

    std::thread job(&GameCubeRTC::calcRTC, this);
    job.detach();

}