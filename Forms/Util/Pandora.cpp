#include "Pandora.hpp"
#include "ui_Pandora.h"

Pandora::Pandora(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Pandora)
{
    ui->setupUi(this);

    setupModels();

    qRegisterMetaType<QList<QStandardItem *>>("QList<QStandardItem *>");
    connect(this, SIGNAL(updateGen3(QList<QStandardItem *>)), this, SLOT(updateGen3Model(QList<QStandardItem *>)));
}

Pandora::~Pandora()
{
    delete ui;
}

void Pandora::setupModels()
{
    ui->textBoxPID->setValues(0, 32, false);
    ui->textBoxTID->setValues(0, 48, true);
    ui->textBoxMinFrame->setValues(0, 32, true);
    ui->textBoxMaxFrame->setValues(0, 32, true);

    gen3->setHorizontalHeaderLabels(QStringList() << "Frame" << "ID" << "SID");
    ui->tableViewGenerator->setModel(gen3);
    ui->tableViewGenerator->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void Pandora::updateGen3Model(QList<QStandardItem *> row)
{
    gen3->appendRow(row);
}

void Pandora::searchFRLGE()
{
    u32 id = ui->textBoxTID->text().toUInt();
    u32 pid = ui->textBoxPID->text().toUInt(NULL, 16);
    LCRNG rng = PokeRNG(id);

    u32 minFrame = ui->textBoxMinFrame->text().toUInt();
    u32 maxFrame = ui->textBoxMaxFrame->text().toUInt();

    isSearching = true;

    for(auto i = 1; i < minFrame; i++) rng.nextUInt();

    u32 sid = rng.nextUShort();

    for(int frame = minFrame; frame <= maxFrame; ++frame)
    {
        sid = rng.nextUShort();

        if(Utilities::shiny(pid, id, sid))
        {
            emit updateGen3(QList<QStandardItem *>() << new QStandardItem(QString::number(frame)) << new QStandardItem(QString::number(id)) << new QStandardItem(QString::number(sid)));
        }
    }

    isSearching = false;

}

void Pandora::on_pushButtonFindFRLGE_clicked()
{
    if(isSearching)
        return;

    gen3->removeRows(0, gen3->rowCount());
    ui->tableViewGenerator->setModel(gen3);
    ui->tableViewGenerator->viewport()->update();

    std::thread job(&Pandora::searchFRLGE, this);
    job.detach();
}
