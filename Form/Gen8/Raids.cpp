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

#include "Raids.hpp"
#include "ui_Raids.h"
#include <Core/Gen8/Den.hpp>
#include <Core/Gen8/Encounters8.hpp>
#include <Core/Gen8/Generators/RaidGenerator.hpp>
#include <Core/Gen8/Profile8.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen8/Profile/ProfileManager8.hpp>
#include <Model/Gen8/StaticModel8.hpp>
#include <QFile>
#include <QSettings>

static const QString settingPrefix = QStringLiteral("raid");

Raids::Raids(QWidget *parent) : QWidget(parent), ui(new Ui::Raids), currentProfile(nullptr)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    ui->profileDisplay->setup(settingPrefix, Game::SwSh);

    model = new StaticModel8(ui->tableView);
    ui->tableView->setModel(model);

    ui->filter->disableControls(Controls::EncounterSlots | Controls::HiddenPowers | Controls::Level);
    ui->filter->enableHiddenAbility();

    ui->comboBoxAbilityType->setup({ 0, 1, 2, 3, 4 });

    ui->textBoxSeed->setValues(InputType::Seed64Bit);
    ui->textBoxInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxOffset->setValues(InputType::Advance32Bit);

    ui->comboBoxGenderRatio->setup({ 255, 0, 254, 31, 63, 127, 191 });

    ui->comboBoxShinyType->setup({ 0, 1, 2 }); // Random, Non-shiny, shiny

    connect(ui->profileDisplay, &ProfileDisplay8::profileChanged, this, &Raids::profileChanged);
    connect(ui->profileDisplay, &ProfileDisplay8::profilesChanged, this, &Raids::profilesChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Raids::generate);
    connect(ui->comboBoxLocation, &QComboBox::currentIndexChanged, this, &Raids::locationIndexChanged);
    connect(ui->comboBoxDen, &QComboBox::currentIndexChanged, this, &Raids::denIndexChanged);
    connect(ui->comboBoxRarity, &QComboBox::currentIndexChanged, this, &Raids::rarityIndexChange);
    connect(ui->comboBoxSpecies, &QComboBox::currentIndexChanged, this, &Raids::specieIndexChanged);
    connect(ui->filter, &Filter::showStatsChanged, model, &StaticModel8::setShowStats);

    updateProfiles();
    locationIndexChanged(0);
    denIndexChanged(0);
    specieIndexChanged(0);

    QSettings setting;
    setting.beginGroup(settingPrefix);
    if (setting.contains("seed"))
    {
        ui->textBoxSeed->setText(setting.value("seed").toString());
    }
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
}

Raids::~Raids()
{
    QSettings setting;
    setting.beginGroup(settingPrefix);
    setting.setValue("geometry", this->saveGeometry());
    setting.setValue("seed", ui->textBoxSeed->text());
    setting.endGroup();

    delete ui;
}

void Raids::updateProfiles()
{
    ui->profileDisplay->updateProfiles();
}

void Raids::denIndexChanged(int index)
{
    if (index >= 0)
    {
        ui->comboBoxSpecies->clear();

        if (ui->comboBoxLocation->currentIndex() == 3)
        {
            const DenEvent *den = Encounters8::getDenEvent(ui->comboBoxDen->currentIndex());
            auto raids = den->getRaids(currentProfile->getVersion());

            for (const auto &raid : raids)
            {
                if (raid.getSpecie() == 0)
                {
                    break;
                }

                ui->comboBoxSpecies->addItem(
                    QString("%1: %2").arg(QString::fromStdString(Translator::getSpecie(raid.getSpecie(), raid.getForm())),
                                          QString::fromStdString(raid.getStarDisplay())));
            }
        }
        else
        {
            const Den *den = Encounters8::getDen(ui->comboBoxDen->getCurrentUShort(), ui->comboBoxRarity->currentIndex());
            auto raids = den->getRaids(currentProfile->getVersion());

            for (const auto &raid : raids)
            {
                ui->comboBoxSpecies->addItem(
                    QString("%1: %2").arg(QString::fromStdString(Translator::getSpecie(raid.getSpecie(), raid.getForm())),
                                          QString::fromStdString(raid.getStarDisplay())));
            }
        }
    }
}

void Raids::generate()
{
    if (!ui->filter->isValid())
    {
        return;
    }

    model->clearModel();

    u32 initialAdvances = ui->textBoxInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxMaxAdvances->getUInt();
    u32 offset = ui->textBoxOffset->getUInt();
    u64 seed = ui->textBoxSeed->getULong();
    u8 level = ui->spinBoxLevel->value();

    auto filter = ui->filter->getFilter<StateFilter>();
    RaidGenerator generator(initialAdvances, maxAdvances, offset, *currentProfile, filter);

    if (ui->comboBoxLocation->currentIndex() == 3)
    {
        const DenEvent *den = Encounters8::getDenEvent(ui->comboBoxDen->currentIndex());
        Raid raid = den->getRaid(ui->comboBoxSpecies->currentIndex(), currentProfile->getVersion());

        auto states = generator.generate(seed, level, raid);
        model->addItems(states);
    }
    else
    {
        const Den *den = Encounters8::getDen(ui->comboBoxDen->getCurrentUShort(), ui->comboBoxRarity->currentIndex());
        Raid raid = den->getRaid(ui->comboBoxSpecies->currentIndex(), currentProfile->getVersion());

        auto states = generator.generate(seed, level, raid);
        model->addItems(states);
    }
}

void Raids::locationIndexChanged(int index)
{
    if (index >= 0)
    {
        ui->comboBoxDen->clear();

        if (index < 3)
        {
            u16 start = index == 0 ? 0 : index == 1 ? 100 : 190;
            u16 end = index == 0 ? 100 : index == 1 ? 190 : 276;
            u16 offset = index == 0 ? 0 : index == 1 ? 100 : 190;

            std::vector<u16> indices(end - start);
            std::vector<u16> locationIndices(end - start);

            std::iota(indices.begin(), indices.end(), start);
            std::ranges::transform(indices, locationIndices.begin(), [](u16 i) { return Encounters8::getDenLocation(i); });

            auto locations = Translator::getLocations(locationIndices, Game::SwSh);
            for (u16 i : indices)
            {
                if (i == 16)
                {
                    continue;
                }

                QString str = QString("%1: %2").arg(i + 1 - offset).arg(QString::fromStdString(locations[i - offset]));
                ui->comboBoxDen->addItem(str, i);
            }

            ui->spinBoxLevel->setEnabled(true);
        }
        else
        {
            for (u8 i = 1; i < 70; i++)
            {
                ui->comboBoxDen->addItem(QString(tr("Wild Area Event %1")).arg(i));
            }

            ui->spinBoxLevel->setEnabled(false);
        }
    }
}

void Raids::profileChanged(const Profile8 &profile)
{
    currentProfile = &profile;
}

void Raids::rarityIndexChange(int index)
{
    if (index >= 0)
    {
        denIndexChanged(ui->comboBoxDen->currentIndex());
    }
}

void Raids::specieIndexChanged(int index)
{
    if (index >= 0)
    {
        if (ui->comboBoxLocation->currentIndex() == 3)
        {
            const DenEvent *den = Encounters8::getDenEvent(ui->comboBoxDen->currentIndex());
            Raid raid = den->getRaid(ui->comboBoxSpecies->currentIndex(), currentProfile->getVersion());
            const PersonalInfo *info = PersonalLoader::getPersonal(Game::SwSh, raid.getSpecie(), raid.getForm());

            ui->spinBoxIVCount->setValue(raid.getIVCount());
            ui->comboBoxAbilityType->setCurrentIndex(ui->comboBoxAbilityType->findData(raid.getAbility()));
            ui->comboBoxGenderType->setCurrentIndex(raid.getGender());
            ui->comboBoxGenderRatio->setCurrentIndex(ui->comboBoxGenderRatio->findData(info->getGender()));
            ui->comboBoxShinyType->setCurrentIndex(ui->comboBoxShinyType->findData(toInt(raid.getShiny())));
            ui->labelGigantamaxValue->setText(raid.getGigantamax() ? tr("Yes") : tr("No"));
            ui->spinBoxLevel->setValue(raid.getLevel());
        }
        else
        {
            const Den *den = Encounters8::getDen(ui->comboBoxDen->getCurrentUShort(), ui->comboBoxRarity->currentIndex());
            Raid raid = den->getRaid(ui->comboBoxSpecies->currentIndex(), currentProfile->getVersion());
            const PersonalInfo *info = PersonalLoader::getPersonal(Game::SwSh, raid.getSpecie(), raid.getForm());

            ui->spinBoxIVCount->setValue(raid.getIVCount());
            ui->comboBoxAbilityType->setCurrentIndex(ui->comboBoxAbilityType->findData(raid.getAbility()));
            ui->comboBoxGenderType->setCurrentIndex(raid.getGender());
            ui->comboBoxGenderRatio->setCurrentIndex(ui->comboBoxGenderRatio->findData(info->getGender()));
            ui->comboBoxShinyType->setCurrentIndex(ui->comboBoxShinyType->findData(toInt(raid.getShiny())));
            ui->labelGigantamaxValue->setText(raid.getGigantamax() ? tr("Yes") : tr("No"));
        }
    }
}
