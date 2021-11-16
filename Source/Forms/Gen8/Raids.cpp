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

#include "Raids.hpp"
#include "ui_Raids.h"
#include <Core/Gen8/DenLoader.hpp>
#include <Core/Gen8/Generators/RaidGenerator.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Gen8/Profile/ProfileManager8.hpp>
#include <Forms/Models/Gen8/RaidModel.hpp>
#include <QSettings>

Raids::Raids(QWidget *parent) : QWidget(parent), ui(new Ui::Raids)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();
    updateProfiles();
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
                 [](const Profile &profile) { return profile.getVersion() & Game::SwSh; });
    profiles.insert(profiles.begin(), Profile8());

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

void Raids::setupModels()
{
    model = new RaidModel(ui->tableView);
    ui->tableView->setModel(model);

    menu = new QMenu(ui->tableView);

    ui->filter->disableControls(Controls::EncounterSlots | Controls::HiddenPowers | Controls::UseDelay | Controls::GenderRatio);
    ui->filter->enableHiddenAbility();

    ui->comboBoxAbilityType->setItemData(0, 0);
    ui->comboBoxAbilityType->setItemData(1, 1);
    ui->comboBoxAbilityType->setItemData(2, 2);
    ui->comboBoxAbilityType->setItemData(3, 3);
    ui->comboBoxAbilityType->setItemData(4, 4);

    ui->textBoxSeed->setValues(InputType::Seed64Bit);
    ui->textBoxInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advance32Bit);

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

    locationIndexChanged(0);
    denIndexChanged(0);
    speciesIndexChanged(0);

    QAction *outputTXT = menu->addAction(tr("Output Results to TXT"));
    QAction *outputCSV = menu->addAction(tr("Output Results to CSV"));
    connect(outputTXT, &QAction::triggered, this, [=] { ui->tableView->outputModel(false); });
    connect(outputCSV, &QAction::triggered, this, [=] { ui->tableView->outputModel(true); });

    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Raids::profileManager);
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Raids::profileIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Raids::generate);
    connect(ui->comboBoxLocation, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Raids::locationIndexChanged);
    connect(ui->comboBoxDen, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Raids::denIndexChanged);
    connect(ui->comboBoxRarity, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Raids::rarityIndexChange);
    connect(ui->comboBoxSpecies, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Raids::speciesIndexChanged);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &Raids::tableViewContextMenu);

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

void Raids::generate()
{
    Den den = DenLoader::getDen(ui->comboBoxDen->currentData().toInt(), ui->comboBoxRarity->currentIndex());
    Raid raid = den.getRaid(static_cast<u8>(ui->comboBoxSpecies->currentIndex()), currentProfile.getVersion());
    model->clearModel();

    u32 initialAdvances = ui->textBoxInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxMaxAdvances->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();

    StateFilter filter(ui->filter->getGender(), ui->filter->getAbility(), ui->filter->getShiny(), ui->filter->getDisableFilters(),
                       ui->filter->getMinIVs(), ui->filter->getMaxIVs(), ui->filter->getNatures(), {}, {});

    RaidGenerator generator(initialAdvances, maxAdvances, tid, sid, filter, raid);

    u64 seed = ui->textBoxSeed->getULong();

    auto states = generator.generate(seed);
    model->addItems(states);
}

void Raids::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile.getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile.getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(currentProfile.getVersionString()));
    }
}

void Raids::locationIndexChanged(int index)
{
    if (index >= 0)
    {
        ui->comboBoxDen->clear();

        u16 start, end, offset;
        if (index == 0)
        {
            start = 0;
            end = 100;
            offset = 0;
        }
        else if (index == 1)
        {
            start = 100;
            end = 190;
            offset = 100;
        }
        else
        {
            start = 190;
            end = 276;
            offset = 190;
        }

        for (u16 denID = start; denID < end; denID++)
        {
            if (denID == 16)
            {
                continue;
            }

            auto location = Translator::getLocations({ DenLoader::getLocation(denID) }, Game::SwSh);
            ui->comboBoxDen->addItem(QString("%1: %2").arg(denID + 1 - offset).arg(QString::fromStdString(location[0])), denID);
        }
    }
}

void Raids::denIndexChanged(int index)
{
    if (index >= 0)
    {
        Den den = DenLoader::getDen(ui->comboBoxDen->currentData().toInt(), ui->comboBoxRarity->currentIndex());

        ui->comboBoxSpecies->clear();
        auto raids = den.getRaids(currentProfile.getVersion());
        for (const auto &raid : raids)
        {
            ui->comboBoxSpecies->addItem(QString("%1: %2").arg(QString::fromStdString(Translator::getSpecies(raid.getSpecies())),
                                                               QString::fromStdString(raid.getStarDisplay())));
        }
    }
}

void Raids::rarityIndexChange(int index)
{
    if (index >= 0)
    {
        denIndexChanged(ui->comboBoxDen->currentIndex());
    }
}

void Raids::speciesIndexChanged(int index)
{
    if (index >= 0)
    {
        Den den = DenLoader::getDen(ui->comboBoxDen->currentData().toInt(), ui->comboBoxRarity->currentIndex());
        Raid raid = den.getRaid(static_cast<u8>(index), currentProfile.getVersion());
        PersonalInfo info = PersonalLoader8::getPersonal(raid.getSpecies(), raid.getAltForm());

        ui->spinBoxIVCount->setValue(raid.getIVCount());
        ui->comboBoxAbilityType->setCurrentIndex(ui->comboBoxAbilityType->findData(raid.getAbility()));
        ui->comboBoxGenderType->setCurrentIndex(raid.getGender());
        ui->comboBoxGenderRatio->setCurrentIndex(ui->comboBoxGenderRatio->findData(info.getGender()));
        ui->comboBoxShinyType->setCurrentIndex(ui->comboBoxShinyType->findData(raid.getShinyType()));
        ui->labelGigantamaxValue->setText(raid.getGigantamax() ? tr("Yes") : tr("No"));

        // int abilityIndex = ui->comboBoxAbility->currentIndex();

        // ui->comboBoxAbility->setItemText(1, "1: " + QString::fromStdString(Translator::getAbility(info.getAbility1())));
        // ui->comboBoxAbility->setItemText(2, "2: " + QString::fromStdString(Translator::getAbility(info.getAbility2())));

        /*ui->comboBoxAbility->removeItem(3);
        if (raid.getAbility() == 2 || raid.getAbility() == 4)
        {
            ui->comboBoxAbility->addItem("H: " + QString::fromStdString(Translator::getAbility(info.getAbilityH())), 2);
        }

        if (abilityIndex < ui->comboBoxAbility->count())
        {
            ui->comboBoxAbility->setCurrentIndex(abilityIndex);
        }
        else
        {
            ui->comboBoxAbility->setCurrentIndex(0);
        }*/
    }
}

void Raids::tableViewContextMenu(QPoint pos)
{
    if (model->rowCount() > 0)
    {
        menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }
}

void Raids::profileManager()
{
    auto *manager = new ProfileManager8();
    connect(manager, &ProfileManager8::updateProfiles, this, [=] { emit alertProfiles(8); });
    manager->show();
}
