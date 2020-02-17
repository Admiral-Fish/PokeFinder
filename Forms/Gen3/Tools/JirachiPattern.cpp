/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "JirachiPattern.hpp"
#include "ui_JirachiPattern.h"
#include <Core/Gen3/Tools/JirachiPatternCalculator.hpp>
#include <QSettings>
#include <QStandardItemModel>

JirachiPattern::JirachiPattern(QWidget *parent) : QWidget(parent), ui(new Ui::JirachiPattern)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

JirachiPattern::~JirachiPattern()
{
    QSettings setting;
    setting.setValue("jirachiPattern/geometry", this->saveGeometry());

    delete ui;
}

void JirachiPattern::setupModels()
{
    model = new QStandardItemModel(ui->tableView);

    ui->textBoxSeed->setValues(InputType::Seed32Bit);

    model->setHorizontalHeaderLabels({ tr("Pattern") });
    ui->tableView->setModel(model);

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &JirachiPattern::generate);

    QSettings setting;
    if (setting.contains("jirachiPattern/geometry"))
    {
        this->restoreGeometry(setting.value("jirachiPattern/geometry").toByteArray());
    }
}

void JirachiPattern::generate()
{
    model->removeRows(0, model->rowCount());

    u32 seed = ui->textBoxSeed->getUInt();

    QStringList patterns = JirachiPatternCalculator::getPatterns(seed);
    if (patterns.isEmpty())
    {
        model->appendRow(new QStandardItem(tr("Spread Impossible")));
    }
    else
    {
        for (const auto &pattern : patterns)
        {
            model->appendRow(new QStandardItem(pattern));
        }
    }
}
