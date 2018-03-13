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
}

Pandora::~Pandora()
{
    delete ui;
    delete xdcolo;
    delete frlge;
    delete rs;
}

void Pandora::changeEvent(QEvent *event)
{
    if (event != NULL)
    {
        switch (event->type())
        {
            case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
            default:
                break;
        }
    }
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

    ui->textBoxPIDXD->setValues(0, 32, false);
    ui->textBoxPRNG->setValues(0, 32, false);
    ui->textBoxMinFrameXD->setValues(0, 32, true);
    ui->textBoxMaxFrameXD->setValues(0, 32, true);

    xdcolo->setHorizontalHeaderLabels(QStringList() << tr("Frame") << tr("ID") << tr("SID"));
    ui->tableViewXDColo->setModel(xdcolo);
    ui->tableViewXDColo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    frlge->setHorizontalHeaderLabels(QStringList() << tr("Frame") << tr("ID") << tr("SID"));
    ui->tableViewFRLGE->setModel(frlge);
    ui->tableViewFRLGE->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    rs->setHorizontalHeaderLabels(QStringList() << tr("Frame") << tr("ID") << tr("SID"));
    ui->tableViewRS->setModel(rs);
    ui->tableViewRS->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void Pandora::on_pushButtonFindFRLGE_clicked()
{
    frlge->removeRows(0, frlge->rowCount());

    u32 id = ui->textBoxTID->text().toUInt();
    u32 pid = ui->textBoxPID->text().toUInt(NULL, 16);
    LCRNG rng = PokeRNG(id);

    u32 minFrame = ui->textBoxMinFrame->text().toUInt();
    u32 maxResults = ui->textBoxMaxFrame->text().toUInt();

    rng.advanceFrames(minFrame - 1);

    u32 sid = rng.nextUShort();

    u32 max = minFrame + maxResults;
    for (u32 frame = minFrame; frame <= max; ++frame)
    {
        sid = rng.nextUShort();

        if (Utilities::shiny(pid, id, sid))
            frlge->appendRow(QList<QStandardItem *>() << new QStandardItem(QString::number(frame)) << new QStandardItem(QString::number(id)) << new QStandardItem(QString::number(sid)));
    }
}

void Pandora::on_pushButtonFindRS_clicked()
{
    rs->removeRows(0, frlge->rowCount());

    u32 seed;
    bool usePID = ui->checkBoxPID->isChecked();
    bool useSID = ui->checkBoxSID->isChecked();
    bool useTID = ui->checkBoxTID->isChecked();
    u32 pid = ui->textBoxPIDRS->text().toUInt(NULL, 16);
    u32 searchSID = ui->textBoxSIDRS->text().toUInt();
    u32 searchTID = ui->textBoxTIDRS->text().toUInt();

    u32 minFrame = ui->textBoxMinFrameRS->text().toUInt();
    u32 maxResults = ui->textBoxMaxFrameRS->text().toUInt();

    if(ui->radioButtonInitSeedRS->isChecked())
        seed = ui->textBoxInitSeedRS->text().toUInt(NULL, 16);
    else
        seed = Utilities::calcGen3Seed(ui->dateEdit->date(), ui->spinBoxHr->value(), ui->spinBoxMin->value());

    LCRNG rng = PokeRNG(seed);

    rng.advanceFrames(minFrame);

    u32 tid = rng.nextUShort();
    u32 sid;

    u32 max = minFrame + maxResults;
    for (u32 frame = minFrame; frame <= max; ++frame)
    {
        sid = tid;
        tid = rng.nextUShort();

        if ((!usePID || Utilities::shiny(pid, tid, sid)) && (!useTID || searchTID == tid) && (!useSID || searchSID == sid))
            rs->appendRow(QList<QStandardItem *>() << new QStandardItem(QString::number(frame)) << new QStandardItem(QString::number(tid)) << new QStandardItem(QString::number(sid)));
    }
}

void Pandora::on_pushButtonFindXD_clicked()
{
    xdcolo->removeRows(0, frlge->rowCount());

    u32 prng = ui->textBoxPRNG->text().toUInt(NULL, 16);
    u32 pid = ui->textBoxPIDXD->text().toUInt(NULL, 16);
    u32 minFrame = ui->textBoxMinFrameXD->text().toUInt();
    u32 maxResults = ui->textBoxMaxFrameXD->text().toUInt();

    LCRNG rng = XDRNG(prng);

    rng.advanceFrames(minFrame + 1);

    u32 sid = rng.nextUShort();
    u32 tid;

    u32 max = minFrame + maxResults;
    for (u32 frame = minFrame; frame <= max; ++frame)
    {
        tid = sid;
        sid = rng.nextUShort();

        if (Utilities::shiny(pid, tid, sid))
            xdcolo->appendRow(QList<QStandardItem *>() << new QStandardItem(QString::number(frame)) << new QStandardItem(QString::number(tid)) << new QStandardItem(QString::number(sid)));
    }
}

void Pandora::on_checkBoxBattery_stateChanged(int arg1)
{
    if (arg1 == Qt::Unchecked)
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
    if (arg1 == Qt::Checked)
        ui->textBoxPIDRS->setEnabled(true);
    else
        ui->textBoxPIDRS->setEnabled(false);
}

void Pandora::on_checkBoxTID_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        ui->textBoxTIDRS->setEnabled(true);
    else
        ui->textBoxTIDRS->setEnabled(false);
}

void Pandora::on_checkBoxSID_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        ui->textBoxSIDRS->setEnabled(true);
    else
        ui->textBoxSIDRS->setEnabled(false);
}

void Pandora::on_radioButtonDateRS_toggled(bool checked)
{
    if (checked)
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
    if (checked)
    {
        ui->textBoxInitSeedRS->setEnabled(true);
    }
    else
    {
        ui->textBoxInitSeedRS->setEnabled(false);
    }
}
