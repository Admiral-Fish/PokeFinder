#include "SeedToTime3.hpp"
#include "ui_SeedToTime3.h"

SeedToTime3::SeedToTime3(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SeedToTime3)
{
    ui->setupUi(this);

    start = QDateTime(QDate(2000, 1, 1), QTime(0, 0));

    setupModels();
    ui->tableViewGenerator->setModel(m);
}

void SeedToTime3::setupModels()
{
    ui->seedToTimeSeed->setValues(0, 32, false);

    m->setColumnCount(2);
    m->setHorizontalHeaderLabels(QStringList() << "Time" << "Seconds");
    ui->tableViewGenerator->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewGenerator->viewport()->update();
}

SeedToTime3::~SeedToTime3()
{
    delete ui;
    delete m;
}

void SeedToTime3::on_pushButtonFind_clicked()
{
    m->removeRows(0, m->rowCount());
    ui->tableViewGenerator->viewport()->update();

    u32 seed = ui->seedToTimeSeed->text().toUInt(NULL, 16);
    u32 year = ui->seedToTimeYear->text().toUInt(NULL, 10);
    if(seed > 0xFFFF)
    {
        seed = originSeed(seed);
        ui->seedToTimeSeed->setText(QString::number(seed, 16));
    }

    seedToTime(seed, year);

}

void SeedToTime3::seedToTime(uint32_t seed, uint32_t year)
{
    u32 minDay = 0;
    u32 maxDay = 0;

    if(year < 2000  || year > 2037)
        return;

    QDate temp = QDate(2000, 1, 1);

    if(year != 2000)
    {

        for(u32 x = 2000; x < year; x++)
        {
            for(u32 month = 1; month < 13; month++)
            {
                temp.setDate(x, month, 1);
                minDay += temp.daysInMonth();
                maxDay += temp.daysInMonth();
            }
        }
    }

    for(int month = 1; month < 13; month++)
    {
        temp.setDate(year, month, 1);
        maxDay += temp.daysInMonth();
        for(u32 day = minDay; day < maxDay; day++)
        {
            u32 x1 = (1440 * day) >> 16;
            u32 x2 = x1 + 1;

            u32 y1 = x1 ^ seed;
            u32 y2 = x2 ^ seed;

            u32 v1 = (x1 << 16) | y1;
            u32 v2 = (x2 << 16) | y2;

            for(u32 hour = 0; hour < 24; hour++)
            {
                for(u32 minute = 0; minute < 60; minute++)
                {
                    u32 v = 1440 * day + 960 * (hour / 10) + 60 * (hour % 10) + 16 * (minute / 10) + (minute % 10) + 0x5A0;
                    if(v1 == v || v2 == v)
                    {
                        QDateTime finalTime = QDateTime(start);
                        finalTime = finalTime.addDays(day);
                        finalTime = finalTime.addSecs((hour * 60 * 60) + (minute * 60));
                        QString result = finalTime.toString();
                        int seconds = day * 86400 + hour * 3600 + minute * 60;
                        QList<QStandardItem *> list;
                        QStandardItem *text = new QStandardItem(result);
                        QStandardItem *secondsText = new QStandardItem(QString::number(seconds, 10));
                        list << text << secondsText;
                        m->appendRow(list);
                    }
                }
            }
        }
        minDay += temp.daysInMonth();
    }

    ui->tableViewGenerator->viewport()->update();
}

uint32_t SeedToTime3::originSeed(uint32_t seed)
{
    while(seed > 0xFFFF)
        seed = reverse(seed);
    return seed;
}

uint32_t SeedToTime3::reverse(uint32_t seed)
{
    return seed * 0xEEB9EB65 + 0x0A3561A1;
}
