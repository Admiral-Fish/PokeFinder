#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
        else if (startingFrame > 0xffffffff)
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

    FrameGen3 frame = FrameGen3();
    frame.hp = 31;
    frame.atk = 31;
    frame.def = 31;
    frame.spa = 31;
    frame.spd = 31;
    frame.spe = 31;
    frame.seed = 0xFFFFFFFF;
    ui->tableView->horizontalHeader()->setVisible(true);
}
