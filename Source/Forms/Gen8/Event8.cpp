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

#include "Event8.hpp"
#include "ui_Event8.h"
#include <Core/Gen8/Generators/EventGenerator8.hpp>
#include <Core/Gen8/Profile8.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Controls/Controls.hpp>
#include <Forms/Gen8/Profile/ProfileManager8.hpp>
#include <Forms/Models/Gen8/EventModel8.hpp>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>

Event8::Event8(QWidget *parent) : QWidget(parent), ui(new Ui::Event8)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();
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
    profiles = ProfileLoader8::getProfiles();

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

void Event8::setupModels()
{
    model = new EventModel8(ui->tableView);
    menu = new QMenu(ui->tableView);

    ui->tableView->setModel(model);

    ui->textBoxSeed0->setValues(InputType::Seed64Bit);
    ui->textBoxSeed1->setValues(InputType::Seed64Bit);
    ui->textBoxInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advance32Bit);
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

    for (const std::string &nature : *Translator::getNatures())
    {
        ui->comboBoxNature->addItem(QString::fromStdString(nature));
    }

    QAction *outputTXT = menu->addAction(tr("Output Results to TXT"));
    QAction *outputCSV = menu->addAction(tr("Output Results to CSV"));
    connect(outputTXT, &QAction::triggered, [=]() { ui->tableView->outputModel(false); });
    connect(outputCSV, &QAction::triggered, [=]() { ui->tableView->outputModel(true); });

    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Event8::profileIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Event8::generate);
    connect(ui->pushButtonImport, &QPushButton::clicked, this, &Event8::importEvent);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Event8::profileManager);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &Event8::tableViewContextMenu);

    updateProfiles();

    QSettings setting;
    setting.beginGroup("event8");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

WB8 Event8::getParameters() const
{
    u8 nature = ui->checkBoxNature->isChecked() ? ui->comboBoxNature->currentIndex() : 255;

    return WB8(ui->textBoxTID->getUShort(), ui->textBoxSID->getUShort(), ui->textBoxEC->getUInt(), ui->textBoxPID->getUInt(),
               ui->comboBoxSpecies->currentIndex() + 1, 0, ui->comboBoxGender->currentIndex(), ui->checkBoxEgg->isChecked(), nature,
               ui->comboBoxAbility->currentIndex(), ui->comboBoxPIDType->currentIndex(), ui->spinBoxRandomIVs->value());
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
    u16 tid = currentProfile->getTID();
    u16 sid = currentProfile->getSID();
    u8 genderRatio = ui->filter->getGenderRatio();
    u32 offset = 0;
    if (ui->filter->useDelay())
    {
        offset = ui->filter->getDelay();
    }

    StateFilter filter(ui->filter->getGender(), ui->filter->getAbility(), ui->filter->getShiny(), ui->filter->getDisableFilters(),
                       ui->filter->getMinIVs(), ui->filter->getMaxIVs(), ui->filter->getNatures(), ui->filter->getHiddenPowers(), {});

    EventGenerator8 generator(initialAdvances, maxAdvances, tid, sid, filter, getParameters());
    generator.setOffset(offset);

    auto states = generator.generate(seed0, seed1);
    model->addItems(states);
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
                QMessageBox error;
                error.setText("Invalid format for wondercard");
                error.exec();
                return;
            }

            std::array<u8, 732> data;
            file.read(reinterpret_cast<char *>(data.data()), 732);
            file.close();

            WB8 wb8(data);

            ui->textBoxTID->setText(QString::number(wb8.getTID()));
            ui->textBoxSID->setText(QString::number(wb8.getSID()));
            ui->textBoxEC->setText(QString::number(wb8.getEC()));
            ui->textBoxPID->setText(QString::number(wb8.getPID()));
            ui->comboBoxSpecies->setCurrentIndex(wb8.getSpecies() - 1);
            ui->checkBoxEgg->setChecked(wb8.isEgg());

            ui->checkBoxNature->setChecked(wb8.getNature() != 255);
            if (ui->checkBoxNature->isChecked())
            {
                ui->comboBoxNature->setCurrentIndex(wb8.getNature());
            }

            ui->comboBoxAbility->setCurrentIndex(wb8.getAbilityType());
            ui->comboBoxPIDType->setCurrentIndex(wb8.getPIDType());
            ui->comboBoxGender->setCurrentIndex(wb8.getGender());
            ui->spinBoxRandomIVs->setValue(wb8.getIVCount());
        }
        else
        {
            QMessageBox error;
            error.setText("There was a problem opening the wondercard");
            error.exec();
            return;
        }
    }
}

void Event8::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = &profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile->getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile->getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(currentProfile->getVersionString()));
    }
}

void Event8::tableViewContextMenu(QPoint pos)
{
    if (model->rowCount() > 0)
    {
        menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }
}

void Event8::profileManager()
{
    auto *manager = new ProfileManager8();
    connect(manager, &ProfileManager8::updateProfiles, this, [=] { emit alertProfiles(8); });
    manager->show();
}
