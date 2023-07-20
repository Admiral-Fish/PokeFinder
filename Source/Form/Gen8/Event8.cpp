/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Event8.hpp"
#include "ui_Event8.h"
#include <Core/Gen8/Generators/EventGenerator8.hpp>
#include <Core/Gen8/Profile8.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen8/Profile/ProfileManager8.hpp>
#include <Model/Gen8/StaticModel8.hpp>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

Event8::Event8(QWidget *parent) : QWidget(parent), ui(new Ui::Event8)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    model = new StaticModel8(ui->tableView);
    ui->tableView->setModel(model);

    ui->textBoxSeed0->setValues(InputType::Seed64Bit);
    ui->textBoxSeed1->setValues(InputType::Seed64Bit);
    ui->textBoxInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxDelay->setValues(InputType::Advance32Bit);
    ui->textBoxTID->setValues(InputType::TIDSID);
    ui->textBoxSID->setValues(InputType::TIDSID);
    ui->textBoxEC->setValues(InputType::Seed32Bit);
    ui->textBoxPID->setValues(InputType::Seed32Bit);

    ui->filter->disableControls(Controls::EncounterSlots);

    ui->filter->enableHiddenAbility();

    std::vector<u16> species(493);
    std::iota(species.begin(), species.end(), 1);
    for (const std::string &specie : Translator::getSpecies(species))
    {
        ui->comboBoxSpecies->addItem(QString::fromStdString(specie));
    }

    for (const std::string &nature : Translator::getNatures())
    {
        ui->comboBoxNature->addItem(QString::fromStdString(nature));
    }

    ui->comboBoxSpecies->enableAutoComplete();

    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Event8::profileIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Event8::generate);
    connect(ui->pushButtonImport, &QPushButton::clicked, this, &Event8::importEvent);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Event8::profileManager);
    connect(ui->filter, &Filter::showStatsChanged, model, &StaticModel8::setShowStats);

    updateProfiles();

    QSettings setting;
    setting.beginGroup("event8");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

Event8::~Event8()
{
    QSettings setting;
    setting.beginGroup("event8");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Event8::updateProfiles()
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
    int val = setting.value("event8/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

WB8 Event8::getParameters() const
{
    u8 nature = ui->checkBoxNature->isChecked() ? ui->comboBoxNature->currentIndex() : 255;
    return WB8(ui->textBoxTID->getUShort(), ui->textBoxSID->getUShort(), ui->textBoxEC->getUInt(), ui->textBoxPID->getUInt(),
               ui->comboBoxSpecies->currentIndex() + 1, 0, ui->comboBoxGender->currentIndex(), ui->checkBoxEgg->isChecked(), nature,
               ui->comboBoxAbility->currentIndex(), ui->comboBoxPIDType->currentIndex(), ui->spinBoxRandomIVs->value(),
               ui->spinBoxLevel->value());
}

void Event8::importEvent()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select a wondercard file", QDir::currentPath(), "Wondercard (*.wb8)");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly))
        {
            if (file.size() != 732)
            {
                QMessageBox msg(QMessageBox::Warning, tr("Invalid format"), tr("Wondercard is not the correct size"));
                msg.exec();
                return;
            }

            std::array<u8, 732> data;
            file.read(reinterpret_cast<char *>(data.data()), 732);
            file.close();

            WB8 wb8(data.data());

            ui->textBoxTID->setText(QString::number(wb8.getTID()));
            ui->textBoxSID->setText(QString::number(wb8.getSID()));
            ui->textBoxEC->setText(QString::number(wb8.getEC()));
            ui->textBoxPID->setText(QString::number(wb8.getPID()));
            ui->comboBoxSpecies->setCurrentIndex(wb8.getSpecie() - 1);
            ui->checkBoxEgg->setChecked(wb8.getEgg());

            ui->checkBoxNature->setChecked(wb8.getNature() != 255);
            if (ui->checkBoxNature->isChecked())
            {
                ui->comboBoxNature->setCurrentIndex(wb8.getNature());
            }

            ui->comboBoxAbility->setCurrentIndex(wb8.getAbility());
            ui->comboBoxPIDType->setCurrentIndex(wb8.getShiny());
            ui->comboBoxGender->setCurrentIndex(wb8.getGender());
            ui->spinBoxRandomIVs->setValue(wb8.getIVCount());
        }
        else
        {
            QMessageBox msg(QMessageBox::Warning, tr("File error"), tr("There was a problem opening the wondercard"));
            msg.exec();
            return;
        }
    }
}

void Event8::generate()
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
    u32 delay = ui->textBoxDelay->getUInt();
    WB8 wb8 = getParameters();

    StateFilter filter = ui->filter->getFilter<StateFilter>();
    EventGenerator8 generator(initialAdvances, maxAdvances, delay, wb8, *currentProfile, filter);

    auto states = generator.generate(seed0, seed1);
    model->addItems(states);
}

void Event8::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = &profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile->getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile->getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(Translator::getGame(currentProfile->getVersion())));
    }
}

void Event8::profileManager()
{
    auto *manager = new ProfileManager8();
    connect(manager, &ProfileManager8::profilesModified, this, [=](int num) { emit profilesModified(num); });
    manager->show();
}
