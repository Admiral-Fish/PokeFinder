#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_generate_clicked()
{
    bool pass;
    QMessageBox error;
    QString input;

    uint32_t seed = 0;
    input = ui->initialSeed->text();
    if (input != "")
    {
        seed = input.toUInt(&pass, 16);
        if (!pass)
        {
            error.setText("Please enter seed in valid hexadecimal format.");
            error.exec();
            return;
        }
    }

    uint32_t startingFrame = 1;
    input = ui->startingFrame->text();
    if (input != "")
    {
        startingFrame = input.toUInt(&pass, 10);
        if (!pass)
        {
            error.setText("Please enter starting frame in valid decimal format.");
            error.exec();
            return;
        }
        else if (startingFrame > 0xffffffff)
        {
            error.setText("Please enter a value lower then 4294967296.");
            error.exec();
            return;
        }
    }

    uint32_t maxResults = 100000;
    input = ui->maxResults->text();
    if (input != "")
    {
        maxResults = input.toUInt(&pass, 10);
        if (!pass)
        {
            error.setText("Please enter max results in valid decimal format.");
            error.exec();
            return;
        }
        else if (maxResults > 0xffffffff)
        {
            error.setText("Please enter a value lower then 4294967296.");
            error.exec();
            return;
        }
    }

    uint32_t tid = 12345;
    input = ui->id->text();
    if (input != "")
    {
        tid = input.toUInt(&pass, 10);
        if (!pass)
        {
            error.setText("Please enter Trainer ID in valid decimal format.");
            error.exec();
            return;
        }
        else if (tid > 0xffff)
        {
            error.setText("Please enter a value lower then 65536.");
            error.exec();
            return;
        }
    }

    uint32_t sid = 54321;
    input = ui->sid->text();
    if (input != "")
    {
        sid = input.toUInt(&pass, 10);
        if (!pass)
        {
            error.setText("Please enter Trainer SID in valid decimal format.");
            error.exec();
            return;
        }
        else if (sid > 0xffff)
        {
            error.setText("Please enter a value lower then 65536.");
            error.exec();
            return;
        }
    }

    ui->tableWidget->clearContents();
    GeneratorGen3 generator = GeneratorGen3(maxResults, startingFrame, seed, tid, sid);
    int method = ui->comboBoxMethod->currentIndex();

    if (method == 0)
        generator.FrameType = Method1;
    else if (method == 1)
        generator.FrameType = Method2;
    else if (method == 2)
        generator.FrameType = Method4;
    else if (method == 3)
        generator.FrameType = XDColo;
    else
        generator.FrameType = Channel;

    std::vector<FrameGen3> frames = generator.Generate();
    QTableWidgetItem *item;
    int size = frames.size();
    ui->tableWidget->setRowCount(size);
    for (int i = 0; i < size; i++)
    {
        item = new QTableWidgetItem(QString::number(frames[i].frame), 0);
        ui->tableWidget->setItem(i, 0, item);
        item = new QTableWidgetItem(QString::fromStdString(frames[i].getTime()), 0);
        ui->tableWidget->setItem(i, 1, item);
        item = new QTableWidgetItem(QString::number(frames[i].pid, 16).toUpper(), 0);
        ui->tableWidget->setItem(i, 2, item);
        item = new QTableWidgetItem(QString::fromStdString(frames[i].getShiny()), 0);
        ui->tableWidget->setItem(i, 3, item);
        item = new QTableWidgetItem(QString::fromStdString(frames[i].getNature()), 0);
        ui->tableWidget->setItem(i, 4, item);
        item = new QTableWidgetItem(QString::number(frames[i].ability), 0);
        ui->tableWidget->setItem(i, 5, item);
        item = new QTableWidgetItem(QString::number(frames[i].hp), 0);
        ui->tableWidget->setItem(i, 6, item);
        item = new QTableWidgetItem(QString::number(frames[i].atk), 0);
        ui->tableWidget->setItem(i, 7, item);
        item = new QTableWidgetItem(QString::number(frames[i].def), 0);
        ui->tableWidget->setItem(i, 8, item);
        item = new QTableWidgetItem(QString::number(frames[i].spa), 0);
        ui->tableWidget->setItem(i, 9, item);
        item = new QTableWidgetItem(QString::number(frames[i].spd), 0);
        ui->tableWidget->setItem(i, 10, item);
        item = new QTableWidgetItem(QString::number(frames[i].spe), 0);
        ui->tableWidget->setItem(i, 11, item);
        item = new QTableWidgetItem(QString::fromStdString(frames[i].getPower()), 0);
        ui->tableWidget->setItem(i, 12, item);
        item = new QTableWidgetItem(QString::number(frames[i].power), 0);
        ui->tableWidget->setItem(i, 13, item);
        item = new QTableWidgetItem(QString::fromStdString(frames[i].getFemale125()), 0);
        ui->tableWidget->setItem(i, 14, item);
        item = new QTableWidgetItem(QString::fromStdString(frames[i].getFemale25()), 0);
        ui->tableWidget->setItem(i, 15, item);
        item = new QTableWidgetItem(QString::fromStdString(frames[i].getFemale50()), 0);
        ui->tableWidget->setItem(i, 16, item);
        item = new QTableWidgetItem(QString::fromStdString(frames[i].getFemale75()), 0);
        ui->tableWidget->setItem(i, 17, item);
    }
    ui->tableWidget->resizeColumnsToContents();
}
