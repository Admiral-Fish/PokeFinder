/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Gen8/DenLoader.hpp>
#include <Core/Gen8/Generators/RaidGenerator.hpp>
#include <Core/Gen8/Profile8.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen8/Profile/ProfileManager8.hpp>
#include <Model/Gen8/RaidModel.hpp>
#include <QFile>
#include <QMenu>
#include <QSettings>

Raids::Raids(QWidget *parent) : QWidget(parent), ui(new Ui::Raids), currentProfile(nullptr)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    model = new RaidModel(ui->tableView);
    ui->tableView->setModel(model);

    ui->filter->disableControls(Controls::EncounterSlots | Controls::HiddenPowers);
    ui->filter->enableHiddenAbility();

    ui->comboBoxAbilityType->setItemData(0, 0);
    ui->comboBoxAbilityType->setItemData(1, 1);
    ui->comboBoxAbilityType->setItemData(2, 2);
    ui->comboBoxAbilityType->setItemData(3, 3);
    ui->comboBoxAbilityType->setItemData(4, 4);

    ui->textBoxSeed->setValues(InputType::Seed64Bit);
    ui->textBoxInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxDelay->setValues(InputType::Advance32Bit);

    ui->comboBoxGenderRatio->setItemData(0, 255);
    ui->comboBoxGenderRatio->setItemData(1, 0);
    ui->comboBoxGenderRatio->setItemData(2, 254);
    ui->comboBoxGenderRatio->setItemData(3, 31);
    ui->comboBoxGenderRatio->setItemData(4, 63);
    ui->comboBoxGenderRatio->setItemData(5, 127);
    ui->comboBoxGenderRatio->setItemData(6, 191);

    ui->comboBoxShinyType->setItemData(0, 0); // Random shiny chance
    ui->comboBoxShinyType->setItemData(1, 1); // Forced non-shiny
    ui->comboBoxShinyType->setItemData(2, 2); // Forced shiny

    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Raids::profileManager);
    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &Raids::profileIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Raids::generate);
    connect(ui->comboBoxLocation, &QComboBox::currentIndexChanged, this, &Raids::locationIndexChanged);
    connect(ui->comboBoxDen, &QComboBox::currentIndexChanged, this, &Raids::denIndexChanged);
    connect(ui->comboBoxRarity, &QComboBox::currentIndexChanged, this, &Raids::rarityIndexChange);
    connect(ui->comboBoxSpecies, &QComboBox::currentIndexChanged, this, &Raids::specieIndexChanged);

    updateProfiles();
    locationIndexChanged(0);
    denIndexChanged(0);
    specieIndexChanged(0);

    QSettings setting;
    setting.beginGroup("raid");
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
    setting.beginGroup("raid");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.setValue("seed", ui->textBoxSeed->text());
    setting.endGroup();

    delete ui;
}

void Raids::updateProfiles()
{
    profiles.clear();
    auto completeProfiles = ProfileLoader8::getProfiles();
    std::copy_if(completeProfiles.begin(), completeProfiles.end(), std::back_inserter(profiles),
                 [](const Profile &profile) { return (profile.getVersion() & Game::SwSh) != Game::None; });
    profiles.insert(profiles.begin(), Profile8("-", Game::Sword, 12345, 54321, false, false));

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("raid/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Raids::denIndexChanged(int index)
{
    if (index >= 0)
    {
        ui->comboBoxSpecies->clear();

        if (ui->comboBoxDen->getCurrentInt() == 65535)
        {
            DenEvent den = DenLoader::getEvent();
            auto raids = den.getRaids(currentProfile->getVersion());

            for (const auto &raid : raids)
            {
                ui->comboBoxSpecies->addItem(QString("%1: %2").arg(QString::fromStdString(*Translator::getSpecie(raid.getSpecie())),
                                                                   QString::fromStdString(raid.getStarDisplay())));
            }
        }
        else
        {
            Den den = DenLoader::getDen(ui->comboBoxDen->currentData().toInt(), ui->comboBoxRarity->currentIndex());
            auto raids = den.getRaids(currentProfile->getVersion());

            for (const auto &raid : raids)
            {
                ui->comboBoxSpecies->addItem(QString("%1: %2").arg(QString::fromStdString(*Translator::getSpecie(raid.getSpecie())),
                                                                   QString::fromStdString(raid.getStarDisplay())));
            }
        }
    }
}

void Raids::generate()
{
    model->clearModel();

    u32 initialAdvances = ui->textBoxInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxMaxAdvances->getUInt();
    u32 offset = ui->textBoxDelay->getUInt();
    u64 seed = ui->textBoxSeed->getULong();
    u16 tid = currentProfile->getTID();
    u16 sid = currentProfile->getSID();
    u8 level = ui->spinBoxLevel->value();

    StateFilter8 filter(ui->filter->getGender(), ui->filter->getAbility(), ui->filter->getShiny(), ui->filter->getDisableFilters(),
                        ui->filter->getMinIVs(), ui->filter->getMaxIVs(), ui->filter->getNatures(), ui->filter->getHiddenPowers());

    RaidGenerator generator(initialAdvances, maxAdvances, offset, tid, sid, filter);
    if (ui->comboBoxDen->getCurrentInt() == 65535)
    {
        DenEvent den = DenLoader::getEvent();
        Raid raid = den.getRaid(ui->comboBoxSpecies->currentIndex(), currentProfile->getVersion());

        auto states = generator.generate(seed, level, raid);
        model->addItems(states);
    }
    else
    {
        Den den = DenLoader::getDen(ui->comboBoxDen->getCurrentInt(), ui->comboBoxRarity->currentIndex());
        Raid raid = den.getRaid(ui->comboBoxSpecies->currentIndex(), currentProfile->getVersion());

        auto states = generator.generate(seed, level, raid);
        model->addItems(states);
    }
}

void Raids::locationIndexChanged(int index)
{
    if (index >= 0)
    {
        ui->comboBoxDen->clear();
        if (QFile::exists(QApplication::applicationDirPath() + "/nests_event.json"))
        {
            ui->comboBoxDen->addItem(tr("Event"), 65535);
        }

        u16 start = index == 0 ? 0 : index == 1 ? 100 : 190;
        u16 end = index == 0 ? 100 : index == 1 ? 190 : 276;
        u16 offset = index == 0 ? 0 : index == 1 ? 100 : 190;

        std::vector<u16> indices(end - start);
        std::vector<u16> locationIndices(end - start);

        std::iota(indices.begin(), indices.end(), start);
        std::transform(indices.begin(), indices.end(), locationIndices.begin(), [](u16 i) { return DenLoader::getLocation(i); });

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
    }
}

void Raids::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = &profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile->getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile->getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(*Translator::getGame(currentProfile->getVersion())));
    }
}

void Raids::profileManager()
{
    auto *manager = new ProfileManager8();
    connect(manager, &ProfileManager8::profilesModified, this, [=](int num) { emit profilesModified(num); });
    manager->show();
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
        if (ui->comboBoxDen->currentData().toInt() == 65535)
        {
            DenEvent den = DenLoader::getEvent();
            Raid raid = den.getRaid(static_cast<u8>(ui->comboBoxSpecies->currentIndex()), currentProfile->getVersion());
            const PersonalInfo *info = PersonalLoader::getPersonal(Game::SwSh, raid.getSpecie(), raid.getForm());

            ui->spinBoxIVCount->setValue(raid.getIVCount());
            ui->comboBoxAbilityType->setCurrentIndex(ui->comboBoxAbilityType->findData(raid.getAbility()));
            ui->comboBoxGenderType->setCurrentIndex(raid.getGender());
            ui->comboBoxGenderRatio->setCurrentIndex(ui->comboBoxGenderRatio->findData(info->getGender()));
            ui->comboBoxShinyType->setCurrentIndex(ui->comboBoxShinyType->findData(toInt(raid.getShinyType())));
            ui->labelGigantamaxValue->setText(raid.getGigantamax() ? tr("Yes") : tr("No"));
        }
        else
        {
            Den den = DenLoader::getDen(ui->comboBoxDen->currentData().toInt(), ui->comboBoxRarity->currentIndex());
            Raid raid = den.getRaid(static_cast<u8>(ui->comboBoxSpecies->currentIndex()), currentProfile->getVersion());
            const PersonalInfo *info = PersonalLoader::getPersonal(Game::SwSh, raid.getSpecie(), raid.getForm());

            ui->spinBoxIVCount->setValue(raid.getIVCount());
            ui->comboBoxAbilityType->setCurrentIndex(ui->comboBoxAbilityType->findData(raid.getAbility()));
            ui->comboBoxGenderType->setCurrentIndex(raid.getGender());
            ui->comboBoxGenderRatio->setCurrentIndex(ui->comboBoxGenderRatio->findData(info->getGender()));
            ui->comboBoxShinyType->setCurrentIndex(ui->comboBoxShinyType->findData(toInt(raid.getShinyType())));
            ui->labelGigantamaxValue->setText(raid.getGigantamax() ? tr("Yes") : tr("No"));
        }
    }
}
