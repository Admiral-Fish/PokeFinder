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

#include "Eggs7.hpp"
#include "ui_Eggs7.h"
#include <Core/Enum/Game.hpp>
#include <Core/Gen7/Generators/EggGenerator7.hpp>
#include <Core/Gen7/Profile7.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen7/Profile/ProfileManager7.hpp>
#include <Model/Gen7/EggModel7.hpp>
#include <QMessageBox>
#include <QSettings>

static const QString settingPrefix = QStringLiteral("egg7");

Eggs7::Eggs7(QWidget *parent) : QWidget(parent), ui(new Ui::Eggs7)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    ui->profileDisplay->setup(settingPrefix, Game::Gen7);

    model = new EggModel7(ui->tableView);
    ui->tableView->setModel(model);

    ui->textBoxSeed0->setValues(InputType::Seed32Bit);
    ui->textBoxSeed1->setValues(InputType::Seed32Bit);
    ui->textBoxSeed2->setValues(InputType::Seed32Bit);
    ui->textBoxSeed3->setValues(InputType::Seed32Bit);
    ui->textBoxInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxOffset->setValues(InputType::Advance32Bit);

    ui->filter->disableControls(Controls::Height | Controls::Weight | Controls::Wild);

    ui->eggSettings->setup(Game::BDSP);

    ui->filter->enableHiddenAbility();

    connect(ui->profileDisplay, &ProfileDisplay7::profileChanged, this, &Eggs7::profileChanged);
    connect(ui->profileDisplay, &ProfileDisplay7::profilesChanged, this, &Eggs7::profilesChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Eggs7::generate);
    connect(ui->eggSettings, &EggSettings::showInheritanceChanged, model, &EggModel7::setShowInheritance);
    connect(ui->filter, &Filter::showStatsChanged, model, &EggModel7::setShowStats);

    updateProfiles();

    QSettings setting;
    setting.beginGroup(settingPrefix);
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

Eggs7::~Eggs7()
{
    QSettings setting;
    setting.beginGroup(settingPrefix);
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Eggs7::updateProfiles()
{
    ui->profileDisplay->updateProfiles();
}

void Eggs7::generate()
{
    bool hiddenAbility = !ui->filter->getDisableFilters() && ui->filter->getAbility() == 2;
    if (!ui->eggSettings->isValid(hiddenAbility))
    {
        return;
    }
    if (ui->eggSettings->reorderParents())
    {
        QMessageBox box(QMessageBox::Information, tr("Parents Reordered"), tr("Parent were swapped to match the game"));
        box.exec();
    }

    u32 seed0 = ui->textBoxSeed0->getUInt();
    u32 seed1 = ui->textBoxSeed1->getUInt();
    u64 seed2 = ui->textBoxSeed2->getUInt();
    u64 seed3 = ui->textBoxSeed3->getUInt();
    if (seed0 == 0 && seed1 == 0 && seed2 == 0 && seed3 == 0)
    {
        QMessageBox msg(QMessageBox::Warning, tr("Missing seeds"), tr("Please insert missing seed information"));
        msg.exec();
        return;
    }

    if (!ui->filter->isValid())
    {
        return;
    }

    model->clearModel();

    u32 initialAdvances = ui->textBoxInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxMaxAdvances->getUInt();
    u32 offset = ui->textBoxOffset->getUInt();
    Daycare daycare = ui->eggSettings->getDaycare(false);

    auto filter = ui->filter->getFilter<StateFilter>();
    EggGenerator7 generator(initialAdvances, maxAdvances, offset, daycare, *currentProfile, filter);

    auto states = generator.generate(seed0, seed1, seed2, seed3);
    model->addItems(states);
}

void Eggs7::profileChanged(const Profile7 &profile)
{
    currentProfile = &profile;

    auto state = currentProfile->getEggSeed();
    ui->textBoxSeed0->setText(QString::number(state[0], 16));
    ui->textBoxSeed1->setText(QString::number(state[1], 16));
    ui->textBoxSeed2->setText(QString::number(state[2], 16));
    ui->textBoxSeed3->setText(QString::number(state[3], 16));
}
