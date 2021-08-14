#include "Poketch.hpp"
#include "ui_Poketch.h"
#include <Core/Util/Utilities.hpp>

Poketch::Poketch(u32 advances, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Poketch)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels(advances);

}

void Poketch::setupModels(u32 advances) {

    connect(ui->pushButtonClose, &QPushButton::clicked, this, &Poketch::on_pushButtonClose_clicked);

    u32 coin = 0;
    u32 happy = 0;
    u32 target = 0;

    if(advances < 12)
    {
        coin = advances;
        ui->labelNote->setText(tr("Do not switch to the happiness application at all"));
    } else
    {
        target = advances - 12;
        happy = target / 12;
        coin = target % 12;

        if(happy == 0)
        {
            ui->labelNote->setText(tr("Switch to the happiness application once but do not click"));
        }
    }

    ui->labelHappyResult->setText(QString::number(happy));
    ui->labelCoinFlipsResult->setText(QString::number(coin));

}

Poketch::~Poketch()
{
    delete ui;
}

void Poketch::on_pushButtonClose_clicked()
{
    this->close();
}
