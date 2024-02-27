/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "JirachiAdvancer.hpp"
#include "ui_JirachiAdvancer.h"
#include <Core/Gen3/Tools/JirachiPattern.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <QMessageBox>
#include <QSettings>

JirachiAdvancer::JirachiAdvancer(QWidget *parent) : QDialog(parent), ui(new Ui::JirachiAdvancer)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    ui->textBoxStartingSeed->setValues(InputType::Seed32Bit);
    ui->textBoxTargetSeed->setValues(InputType::Seed32Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxBruteForceRange->setValues(InputType::Advance32Bit);

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &JirachiAdvancer::generate);

    QSettings setting;
    if (setting.contains("jirachiAdvancer/geometry"))
    {
        this->restoreGeometry(setting.value("jirachiAdvancer/geometry").toByteArray());
    }
}

JirachiAdvancer::~JirachiAdvancer()
{
    QSettings setting;
    setting.setValue("jirachiAdvancer/geometry", this->saveGeometry());

    delete ui;
}

void JirachiAdvancer::generate()
{
    u32 startingSeed = ui->textBoxStartingSeed->getUInt();
    u32 targetSeed = JirachiPattern::computeJirachiSeed(ui->textBoxTargetSeed->getUInt());
    u32 maxAdvances = ui->textBoxMaxAdvances->getUInt();
    u32 bruteForceRange = ui->textBoxBruteForceRange->getUInt();

    u32 advances = XDRNG::distance(startingSeed, targetSeed);
    if (advances > maxAdvances)
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid advance"), tr("Target is outside of specified advance range"));
        msg.exec();
        return;
    }

    auto actions = JirachiPattern::calculateActions(startingSeed, advances, bruteForceRange);
    if (actions.empty())
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid target"), tr("Target seed is unobtainable"));
        msg.exec();
        return;
    }

    ui->listWidget->clear();

    int num = 1;
    for (u8 action : actions)
    {
        if (action == 0)
        {
            ui->listWidget->addItem(QString("%1: Reload menu").arg(num++));
        }
        else if (action == 1)
        {
            ui->listWidget->addItem(QString("%1: Reject Jirachi").arg(num++));
        }
        else if (action == 2)
        {
            ui->listWidget->addItem(QString("%1: Special Cutscene").arg(num++));
        }
        else
        {
            ui->listWidget->addItem(QString("%1: Accept Jirachi").arg(num++));
        }
    }
}
