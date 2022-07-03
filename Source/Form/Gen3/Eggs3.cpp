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

#include "Eggs3.hpp"
#include "ui_Eggs3.h"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen3/Generators/EggGenerator3.hpp>
#include <Core/Gen3/Profile3.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Nature.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Controls/Controls.hpp>
#include <Forms/Gen3/Profile/ProfileManager3.hpp>
#include <Forms/Models/Gen3/EggModel3.hpp>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>

Eggs3::Eggs3(QWidget *parent) : QWidget(parent), ui(new Ui::Eggs3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

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
    rsfrlg = new EggModel3(ui->tableViewRSFRLG, Method::RSFRLGBred);

    emeraldMenu = new QMenu(ui->tableViewEmerald);
    rsfrlgMenu = new QMenu(ui->tableViewRSFRLG);

    ui->tableViewEmerald->setModel(emerald);
    ui->tableViewRSFRLG->setModel(rsfrlg);

    ui->textBoxEmeraldInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxEmeraldMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxMinRedraws->setValues(0, 255, 3, 10);
    ui->textBoxMaxRedraws->setValues(0, 255, 3, 10);
    ui->textBoxCalibration->setValues(0, 255, 3, 10);

    ui->textBoxSeedHeldRSFRLG->setValues(InputType::Seed16Bit);
    ui->textBoxSeedPickupRSFRLG->setValues(InputType::Seed16Bit);
    ui->textBoxInitialAdvancesHeldRSFRLG->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvancesHeldRSFRLG->setValues(InputType::Advance32Bit);
    ui->textBoxInitialAdvancesPickupRSFRLG->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvancesPickupRSFRLG->setValues(InputType::Advance32Bit);

    ui->comboBoxEmeraldCompatibility->setup({ 20, 50, 70 });
    ui->comboBoxCompatibilityRSFRLG->setup({ 20, 50, 70 });

    ui->comboBoxEmeraldMethod->setup(
        { toInt(Method::EBredPID), toInt(Method::EBred), toInt(Method::EBredSplit), toInt(Method::EBredAlternate) });
    ui->comboBoxMethodRSFRLG->setup(
        { toInt(Method::RSFRLGBred), toInt(Method::RSFRLGBredSplit), toInt(Method::RSFRLGBredAlternate), toInt(Method::RSFRLGBredMixed) });

    ui->filterEmerald->disableControls(Controls::EncounterSlots | Controls::UseDelay | Controls::DisableFilter);
    ui->filterRSFRLG->disableControls(Controls::EncounterSlots | Controls::UseDelay | Controls::DisableFilter);

    ui->eggSettingsEmerald->setup(Game::Emerald);
    ui->eggSettingsRSFRLG->setup(Game::RS | Game::FRLG);

    QAction *outputTXTEmerald = emeraldMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVEmerald = emeraldMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTEmerald, &QAction::triggered, this, [=] { ui->tableViewEmerald->outputModel(); });
    connect(outputCSVEmerald, &QAction::triggered, this, [=] { ui->tableViewEmerald->outputModel(true); });

    QAction *outputTXTRS = rsfrlgMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVRS = rsfrlgMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTRS, &QAction::triggered, this, [=] { ui->tableViewRSFRLG->outputModel(); });
    connect(outputCSVRS, &QAction::triggered, this, [=] { ui->tableViewRSFRLG->outputModel(true); });

    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Eggs3::profilesIndexChanged);
    connect(ui->pushButtonEmeraldGenerate, &QPushButton::clicked, this, &Eggs3::emeraldGenerate);
    connect(ui->pushButtonGenerateRSFRLG, &QPushButton::clicked, this, &Eggs3::rsfrlgGenerate);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Eggs3::profileManager);
    connect(ui->eggSettingsEmerald, &EggSettings::toggleInheritance, emerald, &EggModel3::toggleInheritance);
    connect(ui->eggSettingsRSFRLG, &EggSettings::toggleInheritance, rsfrlg, &EggModel3::toggleInheritance);
    connect(ui->tableViewEmerald, &QTableView::customContextMenuRequested, this, &Eggs3::tableViewEmeraldContextMenu);
    connect(ui->tableViewRSFRLG, &QTableView::customContextMenuRequested, this, &Eggs3::tableViewRSFRLGContextMenu);

    updateProfiles();

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
    u16 tid = currentProfile->getTID();
    u16 sid = currentProfile->getSID();
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

void Eggs3::rsfrlgGenerate()
{
    if (!ui->eggSettingsRSFRLG->compatibleParents())
    {
        QMessageBox box(QMessageBox::Warning, tr("Incompatible Parents"), tr("Gender of selected parents are not compatible for breeding"));
        box.exec();
        return;
    }

    rsfrlg->clearModel();

    u32 initialAdvancesHeld = ui->textBoxInitialAdvancesHeldRSFRLG->getUInt();
    u32 maxAdvancesHeld = ui->textBoxMaxAdvancesHeldRSFRLG->getUInt();
    u16 tid = currentProfile->getTID();
    u16 sid = currentProfile->getSID();
    u8 genderRatio = ui->filterRSFRLG->getGenderRatio();
    auto method = static_cast<Method>(ui->comboBoxMethodRSFRLG->getCurrentInt());
    StateFilter filter(ui->filterRSFRLG->getGender(), ui->filterRSFRLG->getAbility(), ui->filterRSFRLG->getShiny(), false,
                       ui->filterRSFRLG->getMinIVs(), ui->filterRSFRLG->getMaxIVs(), ui->filterRSFRLG->getNatures(),
                       ui->filterRSFRLG->getHiddenPowers(), {});

    EggGenerator3 generator(initialAdvancesHeld, maxAdvancesHeld, tid, sid, genderRatio, method, filter,
                            ui->eggSettingsRSFRLG->getDaycareSettings());
    generator.setCompatability(static_cast<u8>(ui->comboBoxCompatibilityRSFRLG->currentData().toUInt()));
    generator.setInitialAdvancesPickup(ui->textBoxInitialAdvancesPickupRSFRLG->getUInt());
    generator.setMaxAdvancesPickup(ui->textBoxMaxAdvancesPickupRSFRLG->getUInt());

    auto states = generator.generate(ui->textBoxSeedHeldRSFRLG->getUInt(), ui->textBoxSeedPickupRSFRLG->getUInt());
    rsfrlg->addItems(states);
}

void Eggs3::profilesIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = &profiles[index];

        if (currentProfile->getDeadBattery())
        {
            ui->textBoxSeedHeldRSFRLG->setText("5a0");
            ui->textBoxSeedPickupRSFRLG->setText("5a0");
        }

        ui->labelProfileTIDValue->setText(QString::number(currentProfile->getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile->getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(currentProfile->getVersionString()));
    }
}

void Eggs3::tableViewEmeraldContextMenu(QPoint pos)
{
    if (emerald->rowCount() > 0)
    {
        emeraldMenu->popup(ui->tableViewEmerald->viewport()->mapToGlobal(pos));
    }
}

void Eggs3::tableViewRSFRLGContextMenu(QPoint pos)
{
    if (rsfrlg->rowCount() > 0)
    {
        rsfrlgMenu->popup(ui->tableViewRSFRLG->viewport()->mapToGlobal(pos));
    }
}

void Eggs3::profileManager()
{
    auto *manager = new ProfileManager3();
    connect(manager, &ProfileManager3::updateProfiles, this, [=] { emit alertProfiles(3); });
    manager->show();
}
