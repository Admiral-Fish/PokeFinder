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

#include "Static8.hpp"
#include "ui_Static8.h"
#include <Core/Enum/Lead.hpp>
#include <Core/Gen8/Encounters8.hpp>
#include <Core/Gen8/Generators/StaticGenerator8.hpp>
#include <Core/Gen8/Profile8.hpp>
#include <Core/Gen8/StaticTemplate8.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen8/Profile/ProfileManager8.hpp>
#include <Model/Gen8/StaticModel8.hpp>
#include <QMessageBox>
#include <QSettings>

Static8::Static8(QWidget *parent) : QWidget(parent), ui(new Ui::Static8)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    model = new StaticModel8(ui->tableView);
    ui->tableView->setModel(model);

    ui->textBoxSeed0->setValues(InputType::Seed64Bit);
    ui->textBoxSeed1->setValues(InputType::Seed64Bit);
    ui->textBoxInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxOffset->setValues(InputType::Advance32Bit);

    ui->comboMenuLead->addAction(tr("None"), toInt(Lead::None));
    ui->comboMenuLead->addMenu(tr("Cute Charm"), { tr("♂ Lead"), tr("♀ Lead") }, { toInt(Lead::CuteCharmM), toInt(Lead::CuteCharmF) });
    ui->comboMenuLead->addMenu(tr("Synchronize"), Translator::getNatures());

    ui->comboBoxShiny->setup({ toInt(Shiny::Never), toInt(Shiny::Random) });
    ui->comboBoxAbility->setup({ 0, 1, 2, 255 });

    ui->filter->disableControls(Controls::EncounterSlots | Controls::HiddenPowers);

    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &Static8::profileIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Static8::generate);
    connect(ui->comboBoxCategory, &QComboBox::currentIndexChanged, this, &Static8::categoryIndexChanged);
    connect(ui->comboBoxPokemon, &QComboBox::currentIndexChanged, this, &Static8::pokemonIndexChanged);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Static8::profileManager);
    connect(ui->filter, &Filter::showStatsChanged, model, &StaticModel8::setShowStats);

    updateProfiles();
    categoryIndexChanged(0);
    pokemonIndexChanged(0);

    QSettings setting;
    setting.beginGroup("static8");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
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
    profiles.insert(profiles.begin(), Profile8("-", Game::BD, 12345, 54321, false, false, false));

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

void Static8::categoryIndexChanged(int index)
{
    if (index >= 0)
    {
        int size;
        const StaticTemplate8 *templates = Encounters8::getStaticEncounters(index, &size);

        ui->comboBoxPokemon->clear();
        for (int i = 0; i < size; i++)
        {
            if ((currentProfile->getVersion() & templates[i].getVersion()) != Game::None)
            {
                ui->comboBoxPokemon->addItem(
                    QString::fromStdString(Translator::getSpecie(templates[i].getSpecie(), templates[i].getForm())), i);
            }
        }
    }
}

void Static8::generate()
{
    u64 seed0 = ui->textBoxSeed0->getULong();
    u64 seed1 = ui->textBoxSeed1->getULong();
    if (seed0 == 0 && seed1 == 0)
    {
        QMessageBox msg(QMessageBox::Warning, tr("Missing seeds"), tr("Please insert missing seed information"));
        msg.exec();
        return;
    }

    model->clearModel();

    u32 initialAdvances = ui->textBoxInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxMaxAdvances->getUInt();
    u32 offset = ui->textBoxOffset->getUInt();
    auto lead = ui->comboMenuLead->getEnum<Lead>();
    const StaticTemplate8 *staticTemplate
        = Encounters8::getStaticEncounter(ui->comboBoxCategory->currentIndex(), ui->comboBoxPokemon->getCurrentInt());

    auto filter = ui->filter->getFilter<StateFilter>();
    StaticGenerator8 generator(initialAdvances, maxAdvances, offset, lead, *staticTemplate, *currentProfile, filter);

    auto states = generator.generate(seed0, seed1);
    model->addItems(states);
}

void Static8::pokemonIndexChanged(int index)
{
    if (index >= 0)
    {
        const StaticTemplate8 *staticTemplate
            = Encounters8::getStaticEncounter(ui->comboBoxCategory->currentIndex(), ui->comboBoxPokemon->getCurrentInt());
        ui->spinBoxLevel->setValue(staticTemplate->getLevel());
        ui->comboBoxAbility->setCurrentIndex(ui->comboBoxAbility->findData(staticTemplate->getAbility()));
        ui->comboBoxShiny->setCurrentIndex(ui->comboBoxShiny->findData(toInt(staticTemplate->getShiny())));
        ui->spinBoxIVCount->setValue(staticTemplate->getIVCount());
    }
}

void Static8::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = &profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile->getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile->getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(Translator::getGame(currentProfile->getVersion())));
    }
}

void Static8::profileManager()
{
    auto *manager = new ProfileManager8();
    connect(manager, &ProfileManager8::profilesModified, this, [=](int num) { emit profilesModified(num); });
    manager->show();
}
