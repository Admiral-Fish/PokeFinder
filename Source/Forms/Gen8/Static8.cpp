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

#include "Static8.hpp"
#include "ui_Static8.h"
#include <Core/Enum/Lead.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Gen8/Profile/ProfileManager8.hpp>
#include <Forms/Models/Gen8/StaticModel8.hpp>
#include <QSettings>

Static8::Static8(QWidget *parent) : QWidget(parent), ui(new Ui::Static8)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();
    updateProfiles();
}

Static8::~Static8()
{
    QSettings setting;
    setting.beginGroup("static8");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Static8::updateProfiles()
{
    profiles.clear();
    auto completeProfiles = ProfileLoader8::getProfiles();
    std::copy_if(completeProfiles.begin(), completeProfiles.end(), std::back_inserter(profiles),
                 [](const Profile &profile) { return profile.getVersion() & Game::BDSP; });
    profiles.insert(profiles.begin(), Profile8(Game::BD));

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("static8/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Static8::setupModels()
{
    model = new StaticModel8(ui->tableView);

    menu = new QMenu(ui->tableView);

    ui->tableView->setModel(model);

    ui->textBoxSeed0->setValues(InputType::Seed64Bit);
    ui->textBoxSeed1->setValues(InputType::Seed64Bit);
    ui->textBoxInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advance32Bit);

    ui->toolButtonLead->addAction(tr("None"), Lead::None);
    ui->toolButtonLead->addMenu(tr("Synchronize"), Translator::getNatures());

    QAction *outputTXTGenerator = menu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVGenerator = menu->addAction(tr("Output Results to CSV"));
    connect(outputTXTGenerator, &QAction::triggered, [=] { ui->tableView->outputModel(); });
    connect(outputCSVGenerator, &QAction::triggered, [=] { ui->tableView->outputModel(true); });

    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Static8::profilesIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Static8::generate);
    /*connect(ui->comboBoxEncounter, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &Static8::encounterIndexChanged);
    connect(ui->comboBoxLocation, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &Static8::locationIndexChanged);
    connect(ui->comboBoxPokemon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Static8::pokemonIndexChanged);
    connect(ui->comboBoxTime, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Static8::timeIndexChanged);*/
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &Static8::tableViewContextMenu);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Static8::profileManager);

    // generatorEncounterIndexChanged(0);
    // searcherEncounterIndexChanged(0);

    QSettings setting;
    setting.beginGroup("static8");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

void Static8::generate()
{
    /*auto method = static_cast<Method>(ui->comboBoxGeneratorMethod->getCurrentInt());
    generatorModel->clearModel();
    generatorModel->setMethod(method);

    u32 seed = ui->textBoxGeneratorSeed->getUInt();
    u32 initialAdvances = ui->textBoxGeneratorStartingAdvance->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterGenerator->getGenderRatio();
    u32 offset = 0;
    if (ui->filterGenerator->useDelay())
    {
        offset = ui->filterGenerator->getDelay();
    }

    StateFilter filter(ui->filterGenerator->getGender(), ui->filterGenerator->getAbility(), ui->filterGenerator->getShiny(),
                       ui->filterGenerator->getDisableFilters(), ui->filterGenerator->getMinIVs(), ui->filterGenerator->getMaxIVs(),
                       ui->filterGenerator->getNatures(), ui->filterGenerator->getHiddenPowers(), ui->filterGenerator->getEncounterSlots());

    WildGenerator4 generator(initialAdvances, maxAdvances, tid, sid, genderRatio, method, filter,
                             currentProfile.getVersion() & Game::Platinum);
    generator.setOffset(offset);
    generator.setEncounter(static_cast<Encounter>(ui->comboBoxGeneratorEncounter->getCurrentInt()));

    if (ui->toolButtonGeneratorLead->text().contains(tr("Synchronize")))
    {
        generator.setLead(Lead::Synchronize);
        generator.setSynchNature(ui->toolButtonGeneratorLead->getData());
    }
    else
    {
        generator.setLead(static_cast<Lead>(ui->toolButtonGeneratorLead->getData()));
    }
    generator.setEncounterArea(encounterGenerator[ui->comboBoxGeneratorLocation->currentData().toInt()]);

    auto states = generator.generate(seed);
    generatorModel->addItems(states);*/
}

void Static8::profilesIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile.getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile.getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(currentProfile.getVersionString()));
        ui->labelProfileShinyCharmValue->setText(currentProfile.getShinyCharm() ? tr("Yes") : tr("No"));

        // updateLocations();
        // updateLocations();
    }
}

void Static8::tableViewContextMenu(QPoint pos)
{
    if (model->rowCount() > 0)
    {
        menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }
}

void Static8::profileManager()
{
    auto *manager = new ProfileManager8();
    connect(manager, &ProfileManager8::updateProfiles, this, [=] { emit alertProfiles(8); });
    manager->show();
}
