#include "PIDtoIVs.hpp"
#include "ui_PIDtoIVs.h"

PIDtoIVs::PIDtoIVs(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PIDtoIVs)
{
    ui->setupUi(this);

    for(u32 i = 0; i < 256; i++)
    {

        u32 right = 0x41c64e6d * i + 0x6073;
        ushort val = right >> 16;
        flags[val] = true;
        low8[val] = i;
        --val;
        flags[val] = true;
        low8[val] = i;
    }

    setupModels();

}

PIDtoIVs::~PIDtoIVs()
{
    delete ui;
}

void PIDtoIVs::setupModels()
{
    ui->pidInput->setValues(0, 32, false);

    m->setHorizontalHeaderLabels(QStringList() << "Seed" << "Method" << "IVs");
    ui->tabePIDToIV->setModel(m);
    ui->tabePIDToIV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void PIDtoIVs::calcFromPID(u32 pid)
{
    calcMethod124(pid);
    calcMethodXD(pid);
}

void PIDtoIVs::calcMethod124(u32 pid)
{
    u32 pidl = (pid & 0xFFFF) << 16;
    u32 pidh = pid & 0xFFFF0000;

    u32 k1 = pidh - pidl * 0x41c64e6d;
    for(u32 cnt = 0; cnt < 256; cnt++, k1 -= 0xc64e6d00)
    {
        u32 test = k1 >> 16;
        if(test == 13938)
        {
            test = test;
        }

        if(flags[test])
        {
            u32 fullFirst = (pidl | (cnt << 8) | low8[test]);
            u32 fullSecond = forward(fullFirst);
            if((fullSecond & 0xFFFF0000) == pidh)
                addSeed(reverse(fullFirst), forward(fullSecond));
        }
    }
}

void PIDtoIVs::calcMethodXD(u32 pid)
{
    long first = pid & 0xFFFF0000;
    long second = (pid & 0xFFFF) << 16;
    u32 fullFirst;

    long t = ((second - 0x343fd * first) - 0x259ec4) % 0x100000000;
    t = t < 0 ? t + 0x100000000 : t;
    long kmax = (0x343fabc02 - t) / 0x100000000;
    for(long k = 0; k <= kmax; k++, t += 0x100000000)
    {
        if((t % 0x343fd) < 0x10000)
        {
            fullFirst = first | (t / 0x343fd);
            u32 iv2 = reverseXD(reverseXD(fullFirst));
            u32 iv1 = reverseXD(iv2);
            addSeedGC(reverseXD(iv1), iv1, iv2);
        }
    }
}

void PIDtoIVs::addSeed(u32 seed, u32 iv1)
{
    QString monsterSeed = QString::number(seed, 16);
    m->appendRow(QList<QStandardItem *>() << new QStandardItem(monsterSeed) << new QStandardItem(QString("Method 1")) << new QStandardItem(calcIVs1(iv1)));
    m->appendRow(QList<QStandardItem *>() << new QStandardItem(monsterSeed) << new QStandardItem(QString("Method 2")) << new QStandardItem(calcIVs2(forward(iv1))));
    m->appendRow(QList<QStandardItem *>() << new QStandardItem(monsterSeed) << new QStandardItem(QString("Method 4")) << new QStandardItem(calcIVs4(iv1)));
    ui->tabePIDToIV->viewport()->update();
}

void PIDtoIVs::addSeedGC(u32 seed, u32 iv1, u32 iv2)
{
    m->appendRow(QList<QStandardItem *>() << new QStandardItem(QString::number(seed, 16)) << new QStandardItem(QString("XD/Colo")) << new QStandardItem(calcIVsXD(iv1, iv2)));
    ui->tabePIDToIV->viewport()->update();
}

QString PIDtoIVs::calcIVs1(u32 iv1)
{
    QString ivs = "";
    u32 iv2 = forward(iv1) >> 16;
    iv1 >>= 16;

    for(u32 x = 0; x < 3; x++)
    {
        u32 q = x * 5;
        u32 iv = (iv1 >> q) & 31;
        ivs += QString::number(iv);
        ivs += ".";
    }

    u32 iV = (iv2 >> 5) & 31;
    ivs += QString::number(iV);
    ivs += ".";

    iV = (iv2 >> 10) & 31;
    ivs += QString::number(iV);
    ivs += ".";

    iV = iv2 & 31;
    ivs += QString::number(iV);

    return ivs;
}

QString PIDtoIVs::calcIVs2(u32 iv1)
{
    QString ivs = "";
    u32 iv2 = forward(iv1) >> 16;
    iv1 >>= 16;

    for (u32 x = 0; x < 3; x++)
    {
        u32 q = x * 5;
        uint iv = (iv1 >> q) & 31;
        ivs += QString::number(iv);
        ivs += ".";
    }

    u32 iV = (iv2 >> 5) & 31;
    ivs += QString::number(iV);
    ivs += ".";

    iV = (iv2 >> 10) & 31;
    ivs += QString::number(iV);
    ivs += ".";

    iV = iv2 & 31;
    ivs += QString::number(iV);

    return ivs;
}

QString PIDtoIVs::calcIVs4(u32 iv1)
{
    QString ivs = "";
    u32 iv2 = forward(forward(iv1)) >> 16;
    iv1 >>= 16;

    for (u32 x = 0; x < 3; x++)
    {
        u32 q = x * 5;
        u32 iv = (iv1 >> q) & 31;
        ivs += QString::number(iv);
        ivs += ".";
    }

    u32 iV = (iv2 >> 5) & 31;
    ivs += QString::number(iV);
    ivs += ".";

    iV = (iv2 >> 10) & 31;
    ivs += QString::number(iV);
    ivs += ".";

    iV = iv2 & 31;
    ivs += QString::number(iV);

    return ivs;
}

QString PIDtoIVs::calcIVsXD(u32 iv1, u32 iv2)
{
    QString ivs = "";
    iv1 >>= 16;
    iv2 >>= 16;

    for (u32 x = 0; x < 3; x++)
    {
        u32 q = x * 5;
        u32 iv = (iv1 >> q) & 31;
        ivs += QString::number(iv);
        ivs += ".";
    }

    u32 iV = (iv2 >> 5) & 31;
    ivs += QString::number(iV);
    ivs += ".";

    iV = (iv2 >> 10) & 31;
    ivs += QString::number(iV);
    ivs += ".";

    iV = iv2 & 31;
    ivs += QString::number(iV);

    return ivs;
}

u32 PIDtoIVs::forward(u32 seed)
{
    return seed * 0x41c64e6d + 0x6073;
}

u32 PIDtoIVs::reverse(u32 seed)
{
    return seed * 0xeeb9eb65 + 0xa3561a1;
}

u32 PIDtoIVs::reverseXD(u32 seed)
{
    return seed * 0xB9B33155 + 0xA170F641;
}



void PIDtoIVs::on_pushButtonGenerate_clicked()
{
    m->removeRows(0, m->rowCount());
    u32 pid = ui->pidInput->text().toUInt(NULL, 16);
    calcFromPID(pid);

}
