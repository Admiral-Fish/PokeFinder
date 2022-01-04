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

#include "Eggs8.hpp"
#include "ui_Eggs8.h"
#include <Core/Gen8/Generators/EggGenerator8.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Forms/Gen8/Profile/ProfileManager8.hpp>
#include <Forms/Models/Gen8/EggModel8.hpp>
#include <QMessageBox>
#include <QSettings>

Eggs8::Eggs8(QWidget *parent) : QWidget(parent), ui(new Ui::Eggs8)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();
    updateProfiles();
}

Eggs8::~Eggs8()
{
    QSettings setting;
    setting.beginGroup("egg8");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Eggs8::updateProfiles()
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
    int val = setting.value("egg8/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Eggs8::setupModels()
{
    model = new EggModel8(ui->tableView);
    menu = new QMenu(ui->tableView);

    ui->tableView->setModel(model);

    ui->textBoxSeed0->setValues(InputType::Seed64Bit);
    ui->textBoxSeed1->setValues(InputType::Seed64Bit);
    ui->textBoxInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advance32Bit);

    ui->comboBoxCompatibility->setup({ 20, 50, 70 });

    ui->filter->disableControls(Controls::EncounterSlots);

    ui->eggSettings->setup(Game::BDSP);

    ui->filter->enableHiddenAbility();

    QAction *outputTXT = menu->addAction(tr("Output Results to TXT"));
    QAction *outputCSV = menu->addAction(tr("Output Results to CSV"));
    connect(outputTXT, &QAction::triggered, [=]() { ui->tableView->outputModel(false); });
    connect(outputCSV, &QAction::triggered, [=]() { ui->tableView->outputModel(true); });

    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Eggs8::profileIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Eggs8::generate);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Eggs8::profileManager);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &Eggs8::tableViewContextMenu);
    connect(ui->eggSettings, &EggSettings::toggleInheritance, model, &EggModel8::toggleInheritance);

    QSettings setting;
    setting.beginGroup("egg8");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

void Eggs8::generate()
{
    if (!ui->eggSettings->compatibleParents())
    {
        QMessageBox box(QMessageBox::Warning, tr("Incompatible Parents"), tr("Gender of selected parents are not compatible for breeding"));
        box.exec();
        return;
    }
    if (ui->eggSettings->reorderParents())
    {
        QMessageBox box(QMessageBox::Information, tr("Parents Reordered"), tr("Parent were swapped to match the game"));
        box.exec();
    }

    model->clearModel();

    u64 seed0 = ui->textBoxSeed0->getULong();
    u64 seed1 = ui->textBoxSeed1->getULong();
    u32 initialAdvances = ui->textBoxInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxMaxAdvances->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filter->getGenderRatio();
    u32 offset = 0;
    if (ui->filter->useDelay())
    {
        offset = ui->filter->getDelay();
    }

    u8 compatability = ui->comboBoxCompatibility->getCurrentByte();
    if (currentProfile.getOvalCharm())
    {
        compatability = compatability == 20 ? 40 : compatability == 50 ? 80 : 88;
    }

    StateFilter filter(ui->filter->getGender(), ui->filter->getAbility(), ui->filter->getShiny(), ui->filter->getDisableFilters(),
                       ui->filter->getMinIVs(), ui->filter->getMaxIVs(), ui->filter->getNatures(), {}, {});

    EggGenerator8 generator(initialAdvances, maxAdvances, tid, sid, genderRatio, filter, ui->eggSettings->getDaycareSettings(),
                            currentProfile.getShinyCharm(), compatability);
    generator.setOffset(offset);

    auto states = generator.generate(seed0, seed1);
    model->addItems(states);
}

void Eggs8::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile.getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile.getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(currentProfile.getVersionString()));
        ui->labelProfileShinyCharmValue->setText(currentProfile.getShinyCharm() ? tr("Yes") : tr("No"));
        ui->labelProfileOvalCharmValue->setText(currentProfile.getOvalCharm() ? tr("Yes") : tr("No"));
    }
}

void Eggs8::tableViewContextMenu(QPoint pos)
{
    if (model->rowCount() > 0)
    {
        menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }
}

void Eggs8::profileManager()
{
    auto *manager = new ProfileManager8();
    connect(manager, &ProfileManager8::updateProfiles, this, [=] { emit alertProfiles(8); });
    manager->show();
}
