/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Eggs4.hpp"
#include "ui_Eggs4.h"

Eggs4::Eggs4(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Eggs4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();

    qRegisterMetaType<vector<Frame4>>("vector<Frame4>");
    connect(this, &Eggs4::updatePID, this, &Eggs4::updateViewPID);
    connect(this, &Eggs4::updateIVs, this, &Eggs4::updateViewIVs);
    connect(this, &Eggs4::updateProgress, this, &Eggs4::updateProgressPID);
    connect(this, &Eggs4::updateProgress, this, &Eggs4::updateProgressIVs);
}

Eggs4::~Eggs4()
{
    QSettings setting;
    setting.setValue("egg4Profile", ui->comboBoxProfiles->currentIndex());

    delete ui;
    delete generatorModel;
    delete searcherIVs;
    delete searcherPID;
}

void Eggs4::changeEvent(QEvent *event)
{
    if (event != NULL)
    {
        switch (event->type())
        {
            case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
            default:
                break;
        }
    }
}

void Eggs4::setupModels()
{
    ui->tableViewGenerator->setModel(generatorModel);

    ui->tableViewIVs->setModel(searcherIVs);

    ui->tableViewPID->setModel(searcherPID);

    ui->textBoxStartingFrame->setValues(1, 32, true);
    ui->textBoxMaxResults->setValues(1, 32, true);
    ui->textBoxTIDGenerator->setValues(0, 48, true);
    ui->textBoxSIDGenerator->setValues(0, 48, true);
    ui->textBoxSeedGenerator->setValues(0, 32, false);

    ui->textBoxMinFrameIVs->setValues(1, 32, true);
    ui->textBoxMaxFrameIVs->setValues(1, 32, true);
    ui->textBoxMinFramePID->setValues(1, 32, true);
    ui->textBoxMaxFramePID->setValues(1, 32, true);
    ui->textBoxMinDelayIVs->setValues(1, 32, true);
    ui->textBoxMaxDelayIVs->setValues(1, 32, true);
    ui->textBoxMinDelayPID->setValues(1, 32, true);
    ui->textBoxMaxDelayPID->setValues(1, 32, true);
    ui->textBoxTIDSearcher->setValues(0, 48, true);
    ui->textBoxSIDSearcher->setValues(0, 48, true);

    ui->comboBoxMethod->setItemData(0, DPPtIVs);
    ui->comboBoxMethod->setItemData(1, Gen4Normal);

    ui->comboBoxNatureGenerator->setup();
    ui->comboBoxHiddenPowerGenerator->setup();
    ui->comboBoxNatureSearcher->setup();
}

void Eggs4::updateProfiles()
{
    profiles = Profile4::loadProfileList();
    profiles.insert(profiles.begin(), Profile4());

    ui->comboBoxProfiles->clear();

    for (int i = 0; i < (int)profiles.size(); i++)
        ui->comboBoxProfiles->addItem(profiles[i].getProfileName());

    QSettings setting;
    int val = setting.value("egg4Profile").toInt();
    if (val < ui->comboBoxProfiles->count())
        ui->comboBoxProfiles->setCurrentIndex(val >= 0 ? val : 0);
}

void Eggs4::refreshProfiles()
{
    emit alertProfiles(4);
}

void Eggs4::on_pushButtonAnyNatureGenerator_clicked()
{
    ui->comboBoxNatureGenerator->uncheckAll();
}

void Eggs4::on_pushButtonAnyHiddenPowerGenerator_clicked()
{
    ui->comboBoxHiddenPowerGenerator->uncheckAll();
}

void Eggs4::on_pushButtonAnyNatureSearcher_clicked()
{
    ui->comboBoxNatureSearcher->uncheckAll();
}

void Eggs4::on_comboBoxProfiles_currentIndexChanged(int index)
{
    auto profile = profiles[index >= 0 ? index : 0];

    ui->textBoxTIDGenerator->setText(QString::number(profile.getTid()));
    ui->textBoxSIDGenerator->setText(QString::number(profile.getSid()));
    ui->textBoxTIDSearcher->setText(QString::number(profile.getTid()));
    ui->textBoxSIDSearcher->setText(QString::number(profile.getSid()));
    ui->profileTID->setText(QString::number(profile.getTid()));
    ui->profileSID->setText(QString::number(profile.getSid()));
    ui->profileGame->setText(profile.getVersionString());
}

void Eggs4::on_pushButtonProfileManager_clicked()
{
    ProfileManager4 *manager = new ProfileManager4();
    connect(manager, SIGNAL(updateProfiles()), this, SLOT(refreshProfiles()));
    manager->show();
}

void Eggs4::updateViewPID(vector<Frame4> frames)
{
    searcherPID->addItems(frames);
}

void Eggs4::updateViewIVs(vector<Frame4> frames)
{
    searcherIVs->addItems(frames);
}

void Eggs4::updateProgressPID()
{
    ui->progressBarPID->setValue(progressPID);
}

void Eggs4::updateProgressIVs()
{
    ui->progressBarIVs->setValue(progressIVs);
}

void Eggs4::updateSearch(int i)
{
    while (isSearching[i] && !cancel[i])
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        emit updateProgress();
    }
}

void Eggs4::on_pushButtonGenerate_clicked()
{
    generatorModel->clear();

    u32 startingFrame = ui->textBoxStartingFrame->text().toUInt();
    u32 maxResults = ui->textBoxMaxResults->text().toUInt();
    u32 seed = ui->textBoxSeedGenerator->text().toUInt(NULL, 16);
    u32 tid = ui->textBoxTIDGenerator->text().toUInt();
    u32 sid = ui->textBoxSIDGenerator->text().toUInt();

    Method method = (Method)ui->comboBoxMethod->currentData().toInt();

    if (method == Gen4Normal)
    {
        if (ui->checkBoxMasuadaGenerator->isChecked())
            method = Gen4Masuada;
    }
    else
    {
        Game version = profiles[ui->comboBoxProfiles->currentIndex()].getVersion();
        if (version == Diamond || version == Pearl || version == Platinum)
            method = DPPtIVs;
        else
            method = HGSSIVs;
    }

    generatorModel->setMethod(method);

    vector<u32> parent1 = { (u32)ui->parent1HPGenerator->value(), (u32)ui->parent1AtkGenerator->value(), (u32)ui->parent1DefGenerator->value(),
                            (u32)ui->parent1SpAGenerator->value(), (u32)ui->parent1SpDGenerator->value(), (u32)ui->parent1SpeGenerator->value()
                          };
    vector<u32> parent2 = { (u32)ui->parent2HPGenerator->value(), (u32)ui->parent2AtkGenerator->value(), (u32)ui->parent2DefGenerator->value(),
                            (u32)ui->parent2SpAGenerator->value(), (u32)ui->parent2SpDGenerator->value(), (u32)ui->parent2SpeGenerator->value()
                          };

    Egg4 generator = Egg4(maxResults, startingFrame, tid, sid, method, seed);
    generator.setParents(parent1, parent2);

    FrameCompare compare = FrameCompare(ui->ivFilterGenerator->getEvals(), ui->ivFilterGenerator->getValues(), ui->comboBoxGenderGenerator->currentIndex(),
                                        ui->comboBoxGenderRatioGenerator->currentIndex(), ui->comboBoxAbilityGenerator->currentIndex(), ui->comboBoxNatureGenerator->getChecked(),
                                        ui->comboBoxHiddenPowerGenerator->getChecked(), ui->checkBoxShinyGenerator->isChecked(), false);

    vector<Frame4> frames = generator.generate(compare);
    generatorModel->setModel(frames);
}

void Eggs4::on_pushButtonGeneratePID_clicked()
{
    if (isSearching[0] == true)
    {
        cancel[0] = true;
    }
    else
    {
        searcherPID->clear();
        searcherPID->setMethod(ui->checkBoxMasuadaSearcher->isChecked() ? Gen4Masuada : Gen4Normal);

        ui->progressBarPID->setValue(0);
        progressPID = 0;

        isSearching[0] = true;
        cancel[0] = false;
        ui->pushButtonGeneratePID->setText(tr("Cancel"));

        std::thread job(&Eggs4::searchPID, this);
        job.detach();

        std::thread update(&Eggs4::updateSearch, this, 0);
        update.detach();
    }
}

void Eggs4::on_pushButtonGenerateIVs_clicked()
{
    if (isSearching[1] == true)
    {
        cancel[1] = true;
    }
    else
    {
        searcherIVs->clear();
        Game version = profiles[ui->comboBoxProfiles->currentIndex()].getVersion();
        searcherIVs->setMethod(version == HeartGold || version == SoulSilver ? HGSSIVs : DPPtIVs);

        ui->progressBarIVs->setValue(0);
        progressIVs = 0;

        isSearching[1] = true;
        cancel[1] = false;
        ui->pushButtonGenerateIVs->setText(tr("Cancel"));

        std::thread job(&Eggs4::searchIVs, this);
        job.detach();

        std::thread update(&Eggs4::updateSearch, this, 1);
        update.detach();
    }
}

void Eggs4::searchPID()
{
    u32 tid = ui->textBoxTIDSearcher->text().toUInt(NULL, 10);
    u32 sid = ui->textBoxSIDSearcher->text().toUInt(NULL, 10);

    int genderRatioIndex = ui->comboBoxGenderRatioSearcher->currentIndex();
    FrameCompare compare = FrameCompare(ui->comboBoxGenderSearcher->currentIndex(), genderRatioIndex, ui->comboBoxAbilitySearcher->currentIndex(),
                                        ui->comboBoxNatureSearcher->getChecked(), ui->checkBoxShinySearcher->isChecked());

    u32 minDelay = ui->textBoxMinDelayPID->text().toUInt();
    u32 maxDelay = ui->textBoxMaxDelayPID->text().toUInt();
    u32 minFrame = ui->textBoxMinFramePID->text().toUInt();
    u32 maxFrame = ui->textBoxMaxFramePID->text().toUInt();

    Method type = ui->checkBoxMasuadaSearcher->isChecked() ? Gen4Masuada : Gen4Normal;
    Egg4 generator = Egg4(maxFrame - minFrame + 1, minFrame, tid, sid, type, 0);

    ui->progressBarPID->setMaximum(256 * 24 * (maxDelay - minDelay + 1));

    for (u32 ab = 0; ab < 256; ab++)
    {
        for (u32 cd = 0; cd < 24; cd++)
        {
            for (u32 efgh = minDelay; efgh <= maxDelay; efgh++)
            {
                u32 seed = ((ab << 24) | (cd << 16)) + efgh;
                generator.setSeed(seed);

                auto frames = generator.generate(compare);

                if (!frames.empty())
                    emit updatePID(frames);

                progressPID++;

                if (searcherPID->rowCount() > 10000)
                {
                    progressPID = 256 * 24 * (maxDelay - minDelay + 1);
                    break;
                }

                if (cancel[0])
                {
                    isSearching[0] = false;
                    ui->pushButtonGeneratePID->setText(tr("Search"));
                    emit updateProgress();
                    return;
                }
            }
        }
    }
    isSearching[0] = false;
    ui->pushButtonGeneratePID->setText(tr("Search"));
    emit updateProgress();
}

void Eggs4::searchIVs()
{
    vector<u32> parent1 = { (u32)ui->parent1HPSearcher->value(), (u32)ui->parent1AtkSearcher->value(), (u32)ui->parent1DefSearcher->value(),
                            (u32)ui->parent1SpASearcher->value(), (u32)ui->parent1SpDSearcher->value(), (u32)ui->parent1SpeSearcher->value()
                          };
    vector<u32> parent2 = { (u32)ui->parent2HPSearcher->value(), (u32)ui->parent2AtkSearcher->value(), (u32)ui->parent2DefSearcher->value(),
                            (u32)ui->parent2SpASearcher->value(), (u32)ui->parent2SpDSearcher->value(), (u32)ui->parent2SpeSearcher->value()
                          };

    FrameCompare compare = FrameCompare(ui->ivFilterSearcher->getEvals(), ui->ivFilterSearcher->getValues(), ui->comboBoxHiddenPowerSearcher->getChecked());

    u32 minDelay = ui->textBoxMinDelayIVs->text().toUInt();
    u32 maxDelay = ui->textBoxMaxDelayIVs->text().toUInt();
    u32 minFrame = ui->textBoxMinFrameIVs->text().toUInt();
    u32 maxFrame = ui->textBoxMaxFrameIVs->text().toUInt();

    Game version = profiles[ui->comboBoxProfiles->currentIndex()].getVersion();
    Method type = version == HeartGold || version == SoulSilver ? HGSSIVs : DPPtIVs;
    Egg4 generator = Egg4(maxFrame - minFrame + 1, minFrame, 0, 0, type, 0);
    generator.setParents(parent1, parent2);

    ui->progressBarIVs->setMaximum(256 * 24 * (maxDelay - minDelay + 1));

    for (u32 ab = 0; ab < 256; ab++)
    {
        for (u32 cd = 0; cd < 24; cd++)
        {
            for (u32 efgh = minDelay; efgh <= maxDelay; efgh++)
            {
                u32 seed = ((ab << 24) | (cd << 16)) + efgh;
                generator.setSeed(seed);

                auto frames = generator.generate(compare);

                if (!frames.empty())
                    emit updateIVs(frames);

                progressIVs++;

                if (searcherIVs->rowCount() > 10000)
                {
                    progressIVs = 256 * 24 * (maxDelay - minDelay + 1);
                    break;
                }

                if (cancel[1])
                {
                    isSearching[1] = false;
                    ui->pushButtonGenerateIVs->setText(tr("Search"));
                    emit updateProgress();
                    return;
                }
            }
        }
    }
    isSearching[1] = false;
    ui->pushButtonGenerateIVs->setText(tr("Search"));
    emit updateProgress();
}
