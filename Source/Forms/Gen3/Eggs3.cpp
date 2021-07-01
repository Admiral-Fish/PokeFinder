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

#include "Eggs3.hpp"
#include "ui_Eggs3.h"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen3/Generators/EggGenerator3.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Nature.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Gen3/Profile/ProfileManager3.hpp>
#include <Forms/Models/Gen3/EggModel3.hpp>
#include <QMessageBox>
#include <QSettings>

Eggs3::Eggs3(QWidget *parent) : QWidget(parent), ui(new Ui::Eggs3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();
}

Eggs3::~Eggs3()
{
    QSettings setting;
    setting.beginGroup("eggs3");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Eggs3::updateProfiles()
{
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Eggs3::profilesIndexChanged);

    profiles = ProfileLoader3::getProfiles();
    profiles.insert(profiles.begin(), Profile3());

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("eggs3/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Eggs3::setupModels()
{
    emerald = new EggModel3(ui->tableViewEmerald, Method::EBred);
    rs = new EggModel3(ui->tableViewRS, Method::RSBred);
    frlg = new EggModel3(ui->tableViewFRLG, Method::FRLGBred);

    emeraldMenu = new QMenu(ui->tableViewEmerald);
    rsMenu = new QMenu(ui->tableViewRS);
    frlgMenu = new QMenu(ui->tableViewFRLG);

    ui->tableViewEmerald->setModel(emerald);
    ui->tableViewRS->setModel(rs);
    ui->tableViewFRLG->setModel(frlg);

    ui->textBoxEmeraldInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxEmeraldMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxMinRedraws->setValues(0, 255, 3, 10);
    ui->textBoxMaxRedraws->setValues(0, 255, 3, 10);
    ui->textBoxCalibration->setValues(0, 255, 3, 10);

    ui->textBoxRSSeedHeld->setValues(InputType::Seed16Bit);
    ui->textBoxRSSeedPickup->setValues(InputType::Seed16Bit);
    ui->textBoxRSInitialAdvancesHeld->setValues(InputType::Advance32Bit);
    ui->textBoxRSMaxAdvancesHeld->setValues(InputType::Advance32Bit);
    ui->textBoxRSInitialAdvancesPickup->setValues(InputType::Advance32Bit);
    ui->textBoxRSMaxAdvancesPickup->setValues(InputType::Advance32Bit);

    ui->textBoxFRLGSeedHeld->setValues(InputType::Seed16Bit);
    ui->textBoxFRLGSeedPickup->setValues(InputType::Seed16Bit);
    ui->textBoxFRLGInitialAdvancesHeld->setValues(InputType::Advance32Bit);
    ui->textBoxFRLGMaxAdvancesHeld->setValues(InputType::Advance32Bit);
    ui->textBoxFRLGInitialAdvancesPickup->setValues(InputType::Advance32Bit);
    ui->textBoxFRLGMaxAdvancesPickup->setValues(InputType::Advance32Bit);

    ui->comboBoxEmeraldCompatibility->setup({ 20, 50, 70 });
    ui->comboBoxRSCompatibility->setup({ 20, 50, 70 });
    ui->comboBoxFRLGCompatibility->setup({ 20, 50, 70 });

    ui->comboBoxEmeraldMethod->setup({ Method::EBredPID, Method::EBred, Method::EBredSplit, Method::EBredAlternate });
    ui->comboBoxRSMethod->setup({ Method::RSBred, Method::RSBredSplit, Method::RSBredAlternate });
    ui->comboBoxFRLGMethod->setup({ Method::FRLGBred, Method::FRLGBredSplit, Method::FRLGBredAlternate, Method::FRLGBredMixed });

    ui->filterEmerald->disableControls(Controls::EncounterSlots | Controls::UseDelay | Controls::DisableFilter);
    ui->filterRS->disableControls(Controls::EncounterSlots | Controls::UseDelay | Controls::DisableFilter);
    ui->filterFRLG->disableControls(Controls::EncounterSlots | Controls::UseDelay | Controls::DisableFilter);

    ui->eggSettingsEmerald->setup(Game::Emerald);
    ui->eggSettingsRS->setup(Game::RS);
    ui->eggSettingsFRLG->setup(Game::FRLG);

    QAction *outputTXTEmerald = emeraldMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVEmerald = emeraldMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTEmerald, &QAction::triggered, this, [=] { ui->tableViewEmerald->outputModel(); });
    connect(outputCSVEmerald, &QAction::triggered, this, [=] { ui->tableViewEmerald->outputModel(true); });

    QAction *outputTXTRS = rsMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVRS = rsMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTRS, &QAction::triggered, this, [=] { ui->tableViewRS->outputModel(); });
    connect(outputCSVRS, &QAction::triggered, this, [=] { ui->tableViewRS->outputModel(true); });

    QAction *outputTXTFRLG = frlgMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVFRLG = frlgMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTFRLG, &QAction::triggered, this, [=] { ui->tableViewFRLG->outputModel(); });
    connect(outputCSVFRLG, &QAction::triggered, this, [=] { ui->tableViewFRLG->outputModel(true); });

    connect(ui->pushButtonEmeraldGenerate, &QPushButton::clicked, this, &Eggs3::emeraldGenerate);
    connect(ui->pushButtonRSGenerate, &QPushButton::clicked, this, &Eggs3::rsGenerate);
    connect(ui->pushButtonFRLGGenerate, &QPushButton::clicked, this, &Eggs3::frlgGenerate);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Eggs3::profileManager);
    connect(ui->eggSettingsEmerald, &EggSettings::toggleInheritance, emerald, &EggModel3::toggleInheritance);
    connect(ui->eggSettingsRS, &EggSettings::toggleInheritance, rs, &EggModel3::toggleInheritance);
    connect(ui->eggSettingsFRLG, &EggSettings::toggleInheritance, frlg, &EggModel3::toggleInheritance);
    connect(ui->tableViewEmerald, &QTableView::customContextMenuRequested, this, &Eggs3::tableViewEmeraldContextMenu);
    connect(ui->tableViewRS, &QTableView::customContextMenuRequested, this, &Eggs3::tableViewRSContextMenu);
    connect(ui->tableViewFRLG, &QTableView::customContextMenuRequested, this, &Eggs3::tableViewFRLGContextMenu);

    QSettings setting;
    if (setting.contains("eggs3/geometry"))
    {
        this->restoreGeometry(setting.value("eggs3/geometry").toByteArray());
    }
}

void Eggs3::emeraldGenerate()
{
    if (!ui->eggSettingsEmerald->compatibleParents())
    {
        QMessageBox box(QMessageBox::Warning, tr("Incompatible Parents"), tr("Gender of selected parents are not compatible for breeding"));
        box.exec();
        return;
    }

    auto method = static_cast<Method>(ui->comboBoxEmeraldMethod->getCurrentInt());
    emerald->clearModel();
    emerald->setMethod(method);

    u32 initialAdvances = ui->textBoxEmeraldInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxEmeraldMaxAdvances->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterEmerald->getGenderRatio();

    StateFilter filter(ui->filterEmerald->getGender(), ui->filterEmerald->getAbility(), ui->filterEmerald->getShiny(), false,
                       ui->filterEmerald->getMinIVs(), ui->filterEmerald->getMaxIVs(), ui->filterEmerald->getNatures(),
                       ui->filterEmerald->getHiddenPowers(), {});

    EggGenerator3 generator(initialAdvances, maxAdvances, tid, sid, genderRatio, method, filter,
                            ui->eggSettingsEmerald->getDaycareSettings());
    generator.setMinRedraw(ui->textBoxMinRedraws->getUChar());
    generator.setMaxRedraw(ui->textBoxMaxRedraws->getUChar());
    generator.setCalibration(ui->textBoxCalibration->getUChar());
    generator.setCompatability(ui->comboBoxEmeraldCompatibility->getCurrentByte());

    auto states = generator.generate();
    emerald->addItems(states);
}

void Eggs3::rsGenerate()
{
    if (!ui->eggSettingsRS->compatibleParents())
    {
        QMessageBox box(QMessageBox::Warning, tr("Incompatible Parents"), tr("Gender of selected parents are not compatible for breeding"));
        box.exec();
        return;
    }

    rs->clearModel();

    u32 initialAdvancesHeld = ui->textBoxRSInitialAdvancesHeld->getUInt();
    u32 maxAdvancesHeld = ui->textBoxRSMaxAdvancesHeld->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterRS->getGenderRatio();
    auto method = static_cast<Method>(ui->comboBoxRSMethod->getCurrentInt());
    StateFilter filter(ui->filterRS->getGender(), ui->filterRS->getAbility(), ui->filterRS->getShiny(), false, ui->filterRS->getMinIVs(),
                       ui->filterRS->getMaxIVs(), ui->filterRS->getNatures(), ui->filterRS->getHiddenPowers(), {});

    EggGenerator3 generator(initialAdvancesHeld, maxAdvancesHeld, tid, sid, genderRatio, method, filter,
                            ui->eggSettingsRS->getDaycareSettings());
    generator.setCompatability(static_cast<u8>(ui->comboBoxRSCompatibility->currentData().toUInt()));
    generator.setInitialAdvancesPickup(ui->textBoxRSInitialAdvancesPickup->getUInt());
    generator.setMaxAdvancesPickup(ui->textBoxRSMaxAdvancesPickup->getUInt());

    auto states = generator.generate(ui->textBoxRSSeedHeld->getUInt(), ui->textBoxRSSeedPickup->getUInt());
    rs->addItems(states);
}

void Eggs3::frlgGenerate()
{
    if (!ui->eggSettingsFRLG->compatibleParents())
    {
        QMessageBox box(QMessageBox::Warning, tr("Incompatible Parents"), tr("Gender of selected parents are not compatible for breeding"));
        box.exec();
        return;
    }

    frlg->clearModel();

    u32 initialAdvancesHeld = ui->textBoxFRLGInitialAdvancesHeld->getUInt();
    u32 maxAdvancesHeld = ui->textBoxFRLGMaxAdvancesHeld->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterFRLG->getGenderRatio();
    auto method = static_cast<Method>(ui->comboBoxFRLGMethod->currentData().toUInt());

    StateFilter filter(ui->filterFRLG->getGender(), ui->filterFRLG->getAbility(), ui->filterFRLG->getShiny(), false,
                       ui->filterFRLG->getMinIVs(), ui->filterFRLG->getMaxIVs(), ui->filterFRLG->getNatures(),
                       ui->filterFRLG->getHiddenPowers(), {});

    EggGenerator3 generator(initialAdvancesHeld, maxAdvancesHeld, tid, sid, genderRatio, method, filter,
                            ui->eggSettingsFRLG->getDaycareSettings());
    generator.setCompatability(static_cast<u8>(ui->comboBoxFRLGCompatibility->currentData().toUInt()));
    generator.setInitialAdvancesPickup(ui->textBoxFRLGInitialAdvancesPickup->getUInt());
    generator.setMaxAdvancesPickup(ui->textBoxFRLGMaxAdvancesPickup->getUInt());

    auto states = generator.generate(ui->textBoxFRLGSeedHeld->getUInt(), ui->textBoxFRLGSeedPickup->getUInt());
    frlg->addItems(states);
}

void Eggs3::profilesIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = profiles[index];

        if (currentProfile.getDeadBattery())
        {
            ui->textBoxRSSeedHeld->setText("5a0");
            ui->textBoxRSSeedPickup->setText("5a0");
        }

        ui->labelProfileTIDValue->setText(QString::number(currentProfile.getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile.getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(currentProfile.getVersionString()));
    }
}

void Eggs3::tableViewEmeraldContextMenu(QPoint pos)
{
    if (emerald->rowCount() > 0)
    {
        emeraldMenu->popup(ui->tableViewEmerald->viewport()->mapToGlobal(pos));
    }
}

void Eggs3::tableViewRSContextMenu(QPoint pos)
{
    if (rs->rowCount() > 0)
    {
        rsMenu->popup(ui->tableViewRS->viewport()->mapToGlobal(pos));
    }
}

void Eggs3::tableViewFRLGContextMenu(QPoint pos)
{
    if (frlg->rowCount() > 0)
    {
        frlgMenu->popup(ui->tableViewFRLG->viewport()->mapToGlobal(pos));
    }
}

void Eggs3::profileManager()
{
    auto *manager = new ProfileManager3();
    connect(manager, &ProfileManager3::updateProfiles, this, [=] { emit alertProfiles(3); });
    manager->show();
}
