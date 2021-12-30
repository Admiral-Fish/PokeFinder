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
#include <Core/Gen8/Encounters8.hpp>
#include <Core/Gen8/Generators/StaticGenerator8.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Parents/StaticTemplate.hpp>
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

    // Do this after profiles are initialized
    categoryIndexChanged(0);
    pokemonIndexChanged(0);
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
                 [](const Profile &profile) { return (profile.getVersion() & Game::BDSP) != Game::None; });
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

    ui->toolButtonLead->addAction(tr("None"), toInt(Lead::None));
    ui->toolButtonLead->addMenu(tr("Synchronize"), Translator::getNatures());
    ui->toolButtonLead->addMenu(tr("Cute Charm"), { tr("♂ Lead"), tr("♀ Lead") }, { toInt(Lead::CuteCharm), toInt(Lead::CuteCharmFemale) });

    ui->comboBoxShiny->setup({ toInt(Shiny::Never), toInt(Shiny::Random) });
    ui->comboBoxAbility->setup({ 0, 1, 2, 255 });

    ui->filter->disableControls(Controls::GenderRatio | Controls::EncounterSlots | Controls::HiddenPowers);

    QAction *outputTXTGenerator = menu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVGenerator = menu->addAction(tr("Output Results to CSV"));
    connect(outputTXTGenerator, &QAction::triggered, [=] { ui->tableView->outputModel(); });
    connect(outputCSVGenerator, &QAction::triggered, [=] { ui->tableView->outputModel(true); });

    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Static8::profilesIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Static8::generate);
    connect(ui->comboBoxCategory, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Static8::categoryIndexChanged);
    connect(ui->comboBoxPokemon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Static8::pokemonIndexChanged);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &Static8::tableViewContextMenu);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Static8::profileManager);

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
    model->clearModel();

    u64 seed0 = ui->textBoxSeed0->getULong();
    u64 seed1 = ui->textBoxSeed1->getULong();
    u32 initialAdvances = ui->textBoxInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxMaxAdvances->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u32 offset = 0;
    if (ui->filter->useDelay())
    {
        offset = ui->filter->getDelay();
    }

    StateFilter filter(ui->filter->getGender(), ui->filter->getAbility(), ui->filter->getShiny(), ui->filter->getDisableFilters(),
                       ui->filter->getMinIVs(), ui->filter->getMaxIVs(), ui->filter->getNatures(), ui->filter->getHiddenPowers(),
                       ui->filter->getEncounterSlots());

    StaticGenerator8 generator(initialAdvances, maxAdvances, tid, sid, filter);
    generator.setOffset(offset);

    if (ui->toolButtonLead->text().contains(tr("Synchronize")))
    {
        generator.setLead(Lead::Synchronize);
        generator.setSynchNature(ui->toolButtonLead->getData());
    }
    else
    {
        generator.setLead(static_cast<Lead>(ui->toolButtonLead->getData()));
    }

    const StaticTemplate *templates = Encounters8::getStaticEncounters(ui->comboBoxCategory->currentIndex());
    if (ui->comboBoxCategory->currentIndex() == 3)
    {
        model->addItems(generator.generateRoamer(seed0, seed1, templates[ui->comboBoxPokemon->currentData().toInt()]));
    }
    else
    {
        model->addItems(generator.generate(seed0, seed1, templates[ui->comboBoxPokemon->currentData().toInt()]));
    }
}

void Static8::profilesIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile.getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile.getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(currentProfile.getVersionString()));
    }
}

void Static8::categoryIndexChanged(int index)
{
    if (index >= 0)
    {
        int size;
        const StaticTemplate *templates = Encounters8::getStaticEncounters(index, &size);

        ui->comboBoxPokemon->clear();
        for (int i = 0; i < size; i++)
        {
            if ((currentProfile.getVersion() & templates[i].getVersion()) != Game::None)
            {
                ui->comboBoxPokemon->addItem(QString::fromStdString(Translator::getSpecies(templates[i].getSpecies())), i);
            }
        }
    }
}

void Static8::pokemonIndexChanged(int index)
{
    if (index >= 0)
    {
        const StaticTemplate *templates = Encounters8::getStaticEncounters(ui->comboBoxCategory->currentIndex());
        StaticTemplate parameter = templates[ui->comboBoxPokemon->currentData().toInt()];

        ui->spinBoxLevel->setValue(parameter.getLevel());
        ui->comboBoxAbility->setCurrentIndex(ui->comboBoxAbility->findData(parameter.getAbility()));
        ui->comboBoxShiny->setCurrentIndex(ui->comboBoxShiny->findData(toInt(parameter.getShiny())));
        ui->spinBoxIVCount->setValue(parameter.getIVCount());
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
