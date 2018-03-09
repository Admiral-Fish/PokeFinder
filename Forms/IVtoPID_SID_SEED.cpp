#include "IVtoPID_SID_SEED.hpp"
#include "ui_IVtoPID_SID_SEED.h"

IVtoPID_SID_SEED::IVtoPID_SID_SEED(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IVtoPID_SID_SEED)
{
    ui->setupUi(this);

    setupModels();
}

IVtoPID_SID_SEED::~IVtoPID_SID_SEED()
{
    delete ui;
}

void IVtoPID_SID_SEED::setupModels()
{
    vector<QString> natureList = Nature::getNatures();
    QStandardItemModel *model = new QStandardItemModel();
    for (auto i = 0; i < natureList.size(); i++)
    {
        QStandardItem* item = new QStandardItem(natureList[i]);

        model->appendRow(item);
    }
    ui->comboBoxNatureGenerator->setModel(model);

    m->setHorizontalHeaderLabels(QStringList() << "Seed" << "PID" << "Method" << "Ability" << "50%" << "12.5%" << "25%" << "75%" << "SID");
    ui->tableViewGenerator->setModel(m);
    ui->tableViewGenerator->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->textBoxID->setValues(0, 48, true);



}

void IVtoPID_SID_SEED::on_pushButtonFind_clicked()
{
    vector<u32> vals = ui->ivFilterGenerator->getValues();
    u32 hp = vals[0];
    u32 atk = vals[1];
    u32 def = vals[2];
    u32 spa = vals[3];
    u32 spd = vals[4];
    u32 spe = vals[5];

    u32 nature = ui->comboBoxNatureGenerator->currentIndex();

    u32 tid = ui->textBoxID->text().toUInt(NULL, 10);

    getSeeds(hp, atk, def, spa, spd, spe, nature, tid);

}

void IVtoPID_SID_SEED::getSeeds(u32 hp, u32 atk, u32 def, u32 spa, u32 spd, u32 spe, u32 nature, u32 tid)
{
    u32 ivs2 = spe | (spa << 5) | (spd << 10);
    u32 ivs1 = hp | (atk << 5) | (def << 10);

    u32 x_test = ivs2 << 16;
    u32 x_testXD = ivs1 << 16;
    u32 pid = 0, pidXor = 0, sid = 0;
    bool pass = false;

    for(u32 cnt = 0; cnt <= 0xFFFF; cnt++)
    {
        u32 seedXD = x_testXD | cnt;
        LCRNG rngXD = XDRNG(seedXD);
        LCRNG rngXDR = XDRNGR(seedXD);
        u32 rng1XD = rngXD.nextUShort();

        if((rng1XD & 0x7FFF) == ivs2)
        {
            u32 rng2XD = rngXD.nextUShort();
            u32 rng3XD = rngXD.nextUShort();
            u32 rng4XD = rngXD.nextUShort();
            u32 XDColoSeed = rngXDR.nextUInt();
            u32 XDColoSeedXor = XDColoSeed ^ 0x80000000;
            sid = (rng4XD ^ rng3XD ^ tid) & 0xFFF8;

            pid = (rng3XD << 16) | rng4XD;
            if(pid % 25 == nature)
            {
                QList<QStandardItem*> newSeed;
                newSeed << new QStandardItem(QString::number(XDColoSeed, 16)) << new QStandardItem(QString::number(pid, 16)) << new QStandardItem(QString("Colosseum/XD")) << new QStandardItem(QString::number((pid & 1), 16)) << new QStandardItem(QString(((pid & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((pid & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((pid & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((pid & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number(sid, 16));
                m->appendRow(newSeed);
            }

            pidXor = pid ^ 0x80008000;
            if(pidXor % 25 == nature)
            {
                QList<QStandardItem*> newSeed;
                newSeed << new QStandardItem(QString::number(XDColoSeedXor, 16)) << new QStandardItem(QString::number(pidXor, 16)) << new QStandardItem(QString("Colosseum/XD")) << new QStandardItem(QString::number((pidXor & 1), 16)) << new QStandardItem(QString(((pidXor & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((pidXor & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((pidXor & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((pidXor & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number(sid, 16));
                m->appendRow(newSeed);
            }
        }

        u32 seed = x_test | cnt;
        LCRNG rng = PokeRNGR(seed);
        u32 rng1 = rng.nextUShort();

        u32 rng2 = rng.nextUShort();
        u32 rng3 = rng.nextUShort();
        u32 rng4 = rng.nextUShort();

        u32 method1Seed = rng.seed;
        u32 method1SeedXor = method1Seed ^ 0x80000000;
        sid = (rng2 ^ rng3 ^ tid) & 0xFFF8;

        rng.nextUShort();
        u32 method234Seed = rng.seed;
        u32 method234SeedXor = method234Seed ^ 0x80000000;

        if((rng1 & 0x7FFF) == ivs1)
        {
            u32 choppedPID;

            pid = (rng2 << 16) + rng3;
            if(pid % 25 == nature)
            {
                QList<QStandardItem*> newSeed;
                newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(pid, 16)) << new QStandardItem(QString("Method 1")) << new QStandardItem(QString::number((pid & 1), 16)) << new QStandardItem(QString(((pid & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((pid & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((pid & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((pid & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number(sid, 16));
                m->appendRow(newSeed);
            }

            pidXor = pid ^ 0x80008000;
            if(pidXor % 25 == nature)
            {
                QList<QStandardItem*> newSeed;
                newSeed << new QStandardItem(QString::number(method1SeedXor, 16)) << new QStandardItem(QString::number(pidXor, 16)) << new QStandardItem(QString("Method 1")) << new QStandardItem(QString::number((pidXor & 1), 16)) << new QStandardItem(QString(((pidXor & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((pidXor & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((pidXor & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((pidXor & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number(sid, 16));
                m->appendRow(newSeed);
            }

            pid = (rng3 << 16) + rng2;
            if(pid % 25 == nature)
            {
                QList<QStandardItem*> newSeed;
                newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(pid, 16)) << new QStandardItem(QString("Reverse Method 1")) << new QStandardItem(QString::number((pid & 1), 16)) << new QStandardItem(QString(((pid & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((pid & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((pid & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((pid & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number(sid, 16));
                m->appendRow(newSeed);
            }

            pid = pid ^ 0x80008000;
            if(pid % 25 == nature)
            {
                QList<QStandardItem*> newSeed;
                newSeed << new QStandardItem(QString::number(method1SeedXor, 16)) << new QStandardItem(QString::number(pid, 16)) << new QStandardItem(QString("Reverse Method 1")) << new QStandardItem(QString::number((pid & 1), 16)) << new QStandardItem(QString(((pid & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((pid & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((pid & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((pid & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number(sid, 16));
                m->appendRow(newSeed);
            }

            if(pid % 25 == nature)
            {
                if(method1Seed < 0x10000)
                {
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(pid, 16)) << new QStandardItem(QString(((((pid & 0xFFFF) ^ (pid >> 16) ^ 20043 ^ 0) & 0xFFF8) == 0) ? "Wishmkr Shiny" : "Wishmkr")) << new QStandardItem(QString::number((pid & 1), 16)) << new QStandardItem(QString(((pid & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((pid & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((pid & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((pid & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number(sid, 16));
                    m->appendRow(newSeed);
                }
            }

            if(pidXor % 25 == nature)
            {
                if(method1SeedXor < 0x10000)
                {
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1SeedXor, 16)) << new QStandardItem(QString::number(pidXor, 16)) << new QStandardItem(QString(((((pidXor & 0xFFFF) ^ (pidXor >> 16) ^ 20043 ^ 0) & 0xFFF8) == 0) ? "Wishmkr Shiny" : "Wishmkr")) << new QStandardItem(QString::number((pidXor & 1), 16)) << new QStandardItem(QString(((pidXor & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((pidXor & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((pidXor & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((pidXor & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number(sid, 16));
                    m->appendRow(newSeed);
                }
            }

            pid = (rng3 << 16) + rng4;
            sid = (rng3 ^ rng4 ^ tid) & 0xFFF8;
            if(pid % 25 == nature)
            {
                QList<QStandardItem*> newSeed;
                newSeed << new QStandardItem(QString::number(method234Seed, 16)) << new QStandardItem(QString::number(pid, 16)) << new QStandardItem(QString("Method 2")) << new QStandardItem(QString::number((pid & 1), 16)) << new QStandardItem(QString(((pid & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((pid & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((pid & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((pid & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number(sid, 16));
                m->appendRow(newSeed);
            }

            pidXor = pid ^ 0x80008000;
            if(pidXor % 25 == nature)
            {
                QList<QStandardItem*> newSeed;
                newSeed << new QStandardItem(QString::number(method234SeedXor, 16)) << new QStandardItem(QString::number(pidXor, 16)) << new QStandardItem(QString("Method 2")) << new QStandardItem(QString::number((pidXor & 1), 16)) << new QStandardItem(QString(((pidXor & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((pidXor & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((pidXor & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((pidXor & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number(sid, 16));
                m->appendRow(newSeed);
            }

            if(rng3 / 0x5556 != 0)
            {
                choppedPID = rng2 / 0xA3E;
                pass = choppedPID % 25 == nature;
                if(pass)
                {
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(QString("Cute Charm (DPPt)")) << new QStandardItem(QString::number((choppedPID & 1), 16)) << new QStandardItem(QString(((choppedPID & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number((choppedPID ^ tid) & 0xFFF8, 16));
                    m->appendRow(newSeed);
                }

                if(pass)
                {
                    choppedPID += 0x96;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(QString("Cute Charm (DPPt)")) << new QStandardItem(QString::number((choppedPID & 1), 16)) << new QStandardItem(QString(((choppedPID & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number((choppedPID ^ tid) & 0xFFF8, 16));
                    m->appendRow(newSeed);
                }

                if(pass)
                {
                    choppedPID += 0x32;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(QString("Cute Charm (DPPt)")) << new QStandardItem(QString::number((choppedPID & 1), 16)) << new QStandardItem(QString(((choppedPID & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number((choppedPID ^ tid) & 0xFFF8, 16));
                    m->appendRow(newSeed);
                }

                if(pass)
                {
                    choppedPID -= 0x7D;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(QString("Cute Charm (DPPt)")) << new QStandardItem(QString::number((choppedPID & 1), 16)) << new QStandardItem(QString(((choppedPID & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number((choppedPID ^ tid) & 0xFFF8, 16));
                    m->appendRow(newSeed);
                }

                if(pass)
                {
                    choppedPID -= 0x19;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(QString("Cute Charm (DPPt)")) << new QStandardItem(QString::number((choppedPID & 1), 16)) << new QStandardItem(QString(((choppedPID & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number((choppedPID ^ tid) & 0xFFF8, 16));
                    m->appendRow(newSeed);
                }

            }

            if(rng3 % 3 != 0)
            {
                choppedPID = rng2 % 25;
                pass = choppedPID == nature;
                if(pass)
                {
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(QString("Cute Charm (HGSS)")) << new QStandardItem(QString::number((choppedPID & 1), 16)) << new QStandardItem(QString(((choppedPID & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number((choppedPID ^ tid) & 0xFFF8, 16));
                    m->appendRow(newSeed);
                }

                if(pass)
                {
                    choppedPID += 0x96;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(QString("Cute Charm (HGSS)")) << new QStandardItem(QString::number((choppedPID & 1), 16)) << new QStandardItem(QString(((choppedPID & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number((choppedPID ^ tid) & 0xFFF8, 16));
                    m->appendRow(newSeed);
                }

                if(pass)
                {
                    choppedPID += 0x32;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(QString("Cute Charm (HGSS)")) << new QStandardItem(QString::number((choppedPID & 1), 16)) << new QStandardItem(QString(((choppedPID & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number((choppedPID ^ tid) & 0xFFF8, 16));
                    m->appendRow(newSeed);
                }

                if(pass)
                {
                    choppedPID -= 0x7D;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(QString("Cute Charm (HGSS)")) << new QStandardItem(QString::number((choppedPID & 1), 16)) << new QStandardItem(QString(((choppedPID & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number((choppedPID ^ tid) & 0xFFF8, 16));
                    m->appendRow(newSeed);
                }

                if(pass)
                {
                    choppedPID -= 0x19;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(QString("Cute Charm (HGSS)")) << new QStandardItem(QString::number((choppedPID & 1), 16)) << new QStandardItem(QString(((choppedPID & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number((choppedPID ^ tid) & 0xFFF8, 16));
                    m->appendRow(newSeed);
                }
            }

            u32 rng3Xor = rng3 ^ 0x8000;
            u32 rng2Xor = rng2 ^ 0x8000;

            if(rng3Xor / 0x5556 != 0)
            {
                choppedPID = rng2Xor / 0xA3E;
                pass = choppedPID % 25 == nature;
                if(pass)
                {
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1SeedXor, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(QString("Cute Charm (DPPt)")) << new QStandardItem(QString::number((choppedPID & 1), 16)) << new QStandardItem(QString(((choppedPID & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number((choppedPID ^ tid) & 0xFFF8, 16));
                    m->appendRow(newSeed);
                }

                if(pass)
                {
                    choppedPID += 0x96;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1SeedXor, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(QString("Cute Charm (DPPt)")) << new QStandardItem(QString::number((choppedPID & 1), 16)) << new QStandardItem(QString(((choppedPID & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number((choppedPID ^ tid) & 0xFFF8, 16));
                    m->appendRow(newSeed);
                }

                if(pass)
                {
                    choppedPID += 0x32;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1SeedXor, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(QString("Cute Charm (DPPt)")) << new QStandardItem(QString::number((choppedPID & 1), 16)) << new QStandardItem(QString(((choppedPID & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number((choppedPID ^ tid) & 0xFFF8, 16));
                    m->appendRow(newSeed);
                }

                if(pass)
                {
                    choppedPID -= 0x7D;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1SeedXor, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(QString("Cute Charm (DPPt)")) << new QStandardItem(QString::number((choppedPID & 1), 16)) << new QStandardItem(QString(((choppedPID & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number((choppedPID ^ tid) & 0xFFF8, 16));
                    m->appendRow(newSeed);
                }

                if(pass)
                {
                    choppedPID -= 0x19;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1SeedXor, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(QString("Cute Charm (DPPt)")) << new QStandardItem(QString::number((choppedPID & 1), 16)) << new QStandardItem(QString(((choppedPID & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number((choppedPID ^ tid) & 0xFFF8, 16));
                    m->appendRow(newSeed);
                }
            }

            if(rng3Xor % 3 != 0)
            {
                choppedPID = rng2Xor % 25;
                pass = choppedPID == nature;
                if(pass)
                {
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1SeedXor, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(QString("Cute Charm (HGSS)")) << new QStandardItem(QString::number((choppedPID & 1), 16)) << new QStandardItem(QString(((choppedPID & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number((choppedPID ^ tid) & 0xFFF8, 16));
                    m->appendRow(newSeed);
                }

                if(pass)
                {
                    choppedPID += 0x96;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1SeedXor, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(QString("Cute Charm (HGSS)")) << new QStandardItem(QString::number((choppedPID & 1), 16)) << new QStandardItem(QString(((choppedPID & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number((choppedPID ^ tid) & 0xFFF8, 16));
                    m->appendRow(newSeed);
                }

                if(pass)
                {
                    choppedPID += 0x32;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1SeedXor, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(QString("Cute Charm (HGSS)")) << new QStandardItem(QString::number((choppedPID & 1), 16)) << new QStandardItem(QString(((choppedPID & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number((choppedPID ^ tid) & 0xFFF8, 16));
                    m->appendRow(newSeed);
                }

                if(pass)
                {
                    choppedPID -= 0x7D;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1SeedXor, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(QString("Cute Charm (HGSS)")) << new QStandardItem(QString::number((choppedPID & 1), 16)) << new QStandardItem(QString(((choppedPID & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number((choppedPID ^ tid) & 0xFFF8, 16));
                    m->appendRow(newSeed);
                }

                if(pass)
                {
                    choppedPID -= 0x19;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1SeedXor, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(QString("Cute Charm (HGSS)")) << new QStandardItem(QString::number((choppedPID & 1), 16)) << new QStandardItem(QString(((choppedPID & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((choppedPID & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number((choppedPID ^ tid) & 0xFFF8, 16));
                    m->appendRow(newSeed);
                }
            }
        }

        if((rng2 & 0x7FFF) == ivs1)
        {
            pid = (rng3 << 16) + rng4;
            sid = (rng3 ^ rng4 ^ tid) & 0xFFF8;
            if(pid % 25 == nature)
            {
                QList<QStandardItem*> newSeed;
                newSeed << new QStandardItem(QString::number(method234Seed, 16)) << new QStandardItem(QString::number(pid, 16)) << new QStandardItem(QString("Method 4")) << new QStandardItem(QString::number((pid & 1), 16)) << new QStandardItem(QString(((pid & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((pid & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((pid & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((pid & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number(sid, 16));
                m->appendRow(newSeed);
            }

            pidXor = pid ^ 0x80008000;
            if(pidXor % 25 == nature)
            {
                QList<QStandardItem*> newSeed;
                newSeed << new QStandardItem(QString::number(method234SeedXor, 16)) << new QStandardItem(QString::number(pidXor, 16)) << new QStandardItem(QString("Method 4")) << new QStandardItem(QString::number((pidXor & 1), 16)) << new QStandardItem(QString(((pidXor & 0xFF) > 126) ? "M" : "F")) << new QStandardItem(QString(((pidXor & 0xFF) > 30) ? "M" : "F")) << new QStandardItem(QString(((pidXor & 0xFF) > 63) ? "M" : "F")) << new QStandardItem(QString(((pidXor & 0xFF) > 190) ? "M" : "F")) << new QStandardItem(QString::number(sid, 16));
                m->appendRow(newSeed);
            }
        }
    }

    ui->tableViewGenerator->viewport()->update();
}
