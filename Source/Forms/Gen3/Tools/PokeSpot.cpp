/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "PokeSpot.hpp"
#include "ui_PokeSpot.h"
#include <Core/Gen3/Generators/PokeSpotGenerator.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Models/Gen3/PokeSpotModel.hpp>
#include <QSettings>

PokeSpot::PokeSpot(QWidget *parent) : QWidget(parent), ui(new Ui::PokeSpot)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

PokeSpot::~PokeSpot()
{
    QSettings setting;
    setting.beginGroup("pokespot");
    setting.setValue("tid", ui->textBoxTID->text());
    setting.setValue("sid", ui->textBoxSID->text());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void PokeSpot::setupModels()
{
    model = new PokeSpotModel(ui->tableView);
    ui->tableView->setModel(model);

    menu = new QMenu(ui->tableView);

    ui->textBoxSeed->setValues(InputType::Seed32Bit);
    ui->textBoxStartingAdvance->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxTID->setValues(InputType::TIDSID);
    ui->textBoxSID->setValues(InputType::TIDSID);

    ui->filter->disableControls(Controls::IVs | Controls::UseDelay | Controls::HiddenPowers | Controls::DisableFilter
                                | Controls::EncounterSlots);

    ui->comboBoxSpotType->setup();

    QAction *outputTXT = menu->addAction(tr("Output Results to TXT"));
    QAction *outputCSV = menu->addAction(tr("Output Results to CSV"));
    connect(outputTXT, &QAction::triggered, this, [=] { ui->tableView->outputModel(); });
    connect(outputCSV, &QAction::triggered, this, [=] { ui->tableView->outputModel(true); });

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &PokeSpot::generate);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &PokeSpot::tableViewContextMenu);

    QSettings setting;
    setting.beginGroup("pokespot");
    if (setting.contains("tid"))
    {
        ui->textBoxTID->setText(setting.value("tid").toString());
    }
    if (setting.contains("sid"))
    {
        ui->textBoxSID->setText(setting.value("sid").toString());
    }
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

void PokeSpot::generate()
{
    model->clearModel();

    u32 seed = ui->textBoxSeed->getUInt();
    u32 initialAdvances = ui->textBoxStartingAdvance->getUInt();
    u32 maxAdvances = ui->textBoxMaxAdvances->getUInt();
    u16 tid = ui->textBoxTID->getUShort();
    u16 sid = ui->textBoxSID->getUShort();
    u8 genderRatio = ui->filter->getGenderRatio();

    StateFilter filter(ui->filter->getGender(), ui->filter->getAbility(), ui->filter->getShiny(), false, {}, {}, ui->filter->getNatures(),
                       {}, {});

    PokeSpotGenerator generator(initialAdvances, maxAdvances, tid, sid, genderRatio, filter);

    auto states = generator.generate(seed, ui->comboBoxSpotType->getChecked());
    model->addItems(states);
}

void PokeSpot::tableViewContextMenu(QPoint pos)
{
    if (model->rowCount() > 0)
    {
        menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }
}
