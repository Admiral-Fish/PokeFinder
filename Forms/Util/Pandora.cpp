#include "Pandora.hpp"
#include "ui_Pandora.h"

Pandora::Pandora(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Pandora)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

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

    ui->textBoxPIDRS->setValues(0, 32, false);
    ui->textBoxTIDRS->setValues(0, 48, true);
    ui->textBoxSIDRS->setValues(0, 48, true);
    ui->textBoxInitSeedRS->setValues(0, 32, false);
    ui->textBoxMinFrameRS->setValues(0, 32, true);
    ui->textBoxMaxFrameRS->setValues(0, 32, true);

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

    for(auto i = 1; i < minFrame; ++i) rng.nextUInt();

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

void Pandora::searchRS()
{
    u32 seed;
    bool usePID = ui->checkBoxPID->isChecked();
    bool useSID = ui->checkBoxSID->isChecked();
    bool useTID = ui->checkBoxTID->isChecked();
    u32 pid = ui->textBoxPIDRS->text().toUInt(NULL, 16);
    u32 searchSID = ui->textBoxSIDRS->text().toUInt();
    u32 searchTID = ui->textBoxTIDRS->text().toUInt();

    u32 minFrame = ui->textBoxMinFrameRS->text().toUInt();
    u32 maxFrame = ui->textBoxMaxFrameRS->text().toUInt();

    if(ui->radioButtonInitSeedRS->isChecked())
        seed = ui->textBoxInitSeedRS->text().toUInt(NULL, 16);
    else
        seed = Utilities::calcGen3Seed(ui->dateEdit->date(), ui->spinBoxHr->value(), ui->spinBoxMin->value());

    LCRNG rng = PokeRNG(seed);

    isSearching = true;

    for(int i = 0; i < minFrame; ++i) rng.nextUInt();

    u32 id = rng.nextUShort();

    for(int frame = minFrame; frame <= maxFrame; ++frame)
    {
        u32 sid = id;
        id = rng.nextUShort();

        if((!usePID || Utilities::shiny(pid, id, sid)) && (!useTID || searchTID == id) && (!useSID || searchSID == sid))
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

void Pandora::on_pushButtonFindRS_clicked()
{
    if(isSearching)
        return;

    gen3->removeRows(0, gen3->rowCount());
    ui->tableViewGenerator->setModel(gen3);
    ui->tableViewGenerator->viewport()->update();

    std::thread job(&Pandora::searchRS, this);
    job.detach();
}

void Pandora::on_checkBoxBattery_stateChanged(int arg1)
{
    if(arg1 == Qt::Unchecked)
    {
        ui->radioButtonDateRS->setEnabled(true);
        ui->radioButtonInitSeedRS->setEnabled(true);
        on_radioButtonDateRS_toggled(ui->radioButtonDateRS->isChecked());
        on_radioButtonInitSeedRS_toggled(ui->radioButtonInitSeedRS->isChecked());
    }
    else
    {
        ui->radioButtonDateRS->setEnabled(false);
        ui->radioButtonInitSeedRS->setEnabled(false);
        on_radioButtonDateRS_toggled(false);
        on_radioButtonInitSeedRS_toggled(false);
    }
}

void Pandora::on_checkBoxPID_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
        ui->textBoxPIDRS->setEnabled(true);
    else
        ui->textBoxPIDRS->setEnabled(false);
}

void Pandora::on_checkBoxTID_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
        ui->textBoxTIDRS->setEnabled(true);
    else
        ui->textBoxTIDRS->setEnabled(false);
}

void Pandora::on_checkBoxSID_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
        ui->textBoxSIDRS->setEnabled(true);
    else
        ui->textBoxSIDRS->setEnabled(false);
}

void Pandora::on_radioButtonDateRS_toggled(bool checked)
{
    if(checked)
    {
        ui->dateEdit->setEnabled(true);
        ui->spinBoxHr->setEnabled(true);
        ui->spinBoxMin->setEnabled(true);
    }
    else
    {
        ui->dateEdit->setEnabled(false);
        ui->spinBoxHr->setEnabled(false);
        ui->spinBoxMin->setEnabled(false);
    }
}

void Pandora::on_radioButtonInitSeedRS_toggled(bool checked)
{
    if(checked)
    {
        ui->textBoxInitSeedRS->setEnabled(true);
    }
    else
    {
        ui->textBoxInitSeedRS->setEnabled(false);
    }
}
