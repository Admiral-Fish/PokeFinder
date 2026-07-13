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

#include "Eggs8.hpp"
#include "ui_Eggs8.h"
#include <Core/Enum/Game.hpp>
#include <Core/Gen8/Generators/EggGenerator8.hpp>
#include <Core/Gen8/Profile8.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen8/Profile/ProfileManager8.hpp>
#include <Model/Gen8/EggModel8.hpp>
#include <QMessageBox>
#include <QSettings>

static const QString settingPrefix = QStringLiteral("egg8");

Eggs8::Eggs8(QWidget *parent) : QWidget(parent), ui(new Ui::Eggs8)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    ui->profileDisplay->setup(settingPrefix, Game::BDSP);

    model = new EggModel8(ui->tableView);
    ui->tableView->setModel(model);

    ui->textBoxSeed0->setValues(InputType::Seed64Bit);
    ui->textBoxSeed1->setValues(InputType::Seed64Bit);
    ui->textBoxInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxOffset->setValues(InputType::Advance32Bit);

    ui->comboBoxCompatibility->setup({ 20, 50, 70 });

    ui->filter->disableControls(Controls::EncounterSlots | Controls::Height | Controls::Level | Controls::Weight);

    ui->eggSettings->setup(Game::BDSP);

    ui->filter->enableHiddenAbility();

    connect(ui->profileDisplay, &ProfileDisplay8::profileChanged, this, &Eggs8::profileChanged);
    connect(ui->profileDisplay, &ProfileDisplay8::profilesChanged, this, &Eggs8::profilesChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Eggs8::generate);
    connect(ui->eggSettings, &EggSettings::showInheritanceChanged, model, &EggModel8::setShowInheritance);
    connect(ui->filter, &Filter::showStatsChanged, model, &EggModel8::setShowStats);

    updateProfiles();

    QSettings setting;
    setting.beginGroup(settingPrefix);
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

Eggs8::~Eggs8()
{
    QSettings setting;
    setting.beginGroup(settingPrefix);
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Eggs8::updateProfiles()
{
    ui->profileDisplay->updateProfiles();
}

void Eggs8::generate()
{
    bool hiddenAbility = !ui->filter->getDisableFilters() && ui->filter->getAbility() == 2;
    if (!ui->eggSettings->isValid(hiddenAbility, EggSettings::HiddenAbility::Gen8))
    {
        return;
    }
    if (ui->eggSettings->reorderParents())
    {
        QMessageBox box(QMessageBox::Information, tr("Parents Reordered"), tr("Parent were swapped to match the game"));
        box.exec();
    }

    u64 seed0 = ui->textBoxSeed0->getULong();
    u64 seed1 = ui->textBoxSeed1->getULong();
    if (seed0 == 0 && seed1 == 0)
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
    u8 compatability = ui->comboBoxCompatibility->getCurrentUChar();
    if (currentProfile->getOvalCharm())
    {
        compatability = compatability == 20 ? 40 : compatability == 50 ? 80 : 88;
    }
    Daycare daycare = ui->eggSettings->getDaycare();

    auto filter = ui->filter->getFilter<StateFilter>();
    EggGenerator8 generator(initialAdvances, maxAdvances, offset, compatability, daycare, *currentProfile, filter);

    auto states = generator.generate(seed0, seed1);
    model->addItems(states);
}

void Eggs8::profileChanged(const Profile8 &profile)
{
    currentProfile = &profile;
}
