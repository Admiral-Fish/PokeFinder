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

    qRegisterMetaType<QVector<Frame4>>("QVector<Frame4>");
}

Eggs4::~Eggs4()
{
    QSettings setting;
    if (setting.contains("egg4MinDelayIVs")) ui->textBoxMinDelayPID->setText(setting.value("egg4MinDelayPID").toString());
    if (setting.contains("egg4MaxDelayIVs")) ui->textBoxMaxDelayPID->setText(setting.value("egg4MaxDelayPID").toString());
    if (setting.contains("egg4MinFrameIVs")) ui->textBoxMinFramePID->setText(setting.value("egg4MinFramePID").toString());
    if (setting.contains("egg4MaxFrameIVs")) ui->textBoxMaxFramePID->setText(setting.value("egg4MaxFramePID").toString());
    if (setting.contains("egg4MinDelayPID")) ui->textBoxMinDelayPID->setText(setting.value("egg4MinDelayPID").toString());
    if (setting.contains("egg4MaxDelayPID")) ui->textBoxMaxDelayPID->setText(setting.value("egg4MaxDelayPID").toString());
    if (setting.contains("egg4MinFramePID")) ui->textBoxMinFramePID->setText(setting.value("egg4MinFramePID").toString());
    if (setting.contains("egg4MaxFramePID")) ui->textBoxMaxFramePID->setText(setting.value("egg4MaxFramePID").toString());

    delete ui;
    delete generatorModel;
    delete searcherIVs;
    delete searcherPID;
    delete searcherMenu;
}

void Eggs4::updateProfiles()
{
    profiles = Profile4::loadProfileList();
    profiles.insert(profiles.begin(), Profile4());

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(profile.getProfileName());
    }

    QSettings setting;
    int val = setting.value("egg4Profile").toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val >= 0 ? val : 0);
    }
}

void Eggs4::setupModels()
{
    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewIVs->setModel(searcherIVs);
    ui->tableViewPID->setModel(searcherPID);

    ui->textBoxStartingFrame->setValues(InputType::Frame32Bit);
    ui->textBoxMaxResults->setValues(InputType::Frame32Bit);
    ui->textBoxTIDGenerator->setValues(InputType::TIDSID);
    ui->textBoxSIDGenerator->setValues(InputType::TIDSID);
    ui->textBoxSeedGenerator->setValues(InputType::Seed32Bit);

    ui->textBoxMinFrameIVs->setValues(InputType::Frame32Bit);
    ui->textBoxMaxFrameIVs->setValues(InputType::Frame32Bit);
    ui->textBoxMinFramePID->setValues(InputType::Frame32Bit);
    ui->textBoxMaxFramePID->setValues(InputType::Frame32Bit);
    ui->textBoxMinDelayIVs->setValues(InputType::Frame32Bit);
    ui->textBoxMaxDelayIVs->setValues(InputType::Frame32Bit);
    ui->textBoxMinDelayPID->setValues(InputType::Frame32Bit);
    ui->textBoxMaxDelayPID->setValues(InputType::Frame32Bit);
    ui->textBoxTIDSearcher->setValues(InputType::TIDSID);
    ui->textBoxSIDSearcher->setValues(InputType::TIDSID);

    ui->comboBoxMethod->setItemData(0, DPPtIVs);
    ui->comboBoxMethod->setItemData(1, Gen4Normal);

    ui->comboBoxNatureGenerator->setup();
    ui->comboBoxHiddenPowerGenerator->setup();
    ui->comboBoxNatureSearcher->setup();

    QAction *seedToTime = new QAction(tr("Generate times for seed"), this);
    connect(seedToTime, &QAction::triggered, this, &Eggs4::seedToTime);

    searcherMenu->addAction(seedToTime);

    QSettings setting;
    if (setting.contains("egg4MinDelayIVs")) ui->textBoxMinDelayPID->setText(setting.value("egg4MinDelayPID").toString());
    if (setting.contains("egg4MaxDelayIVs")) ui->textBoxMaxDelayPID->setText(setting.value("egg4MaxDelayPID").toString());
    if (setting.contains("egg4MinFrameIVs")) ui->textBoxMinFramePID->setText(setting.value("egg4MinFramePID").toString());
    if (setting.contains("egg4MaxFrameIVs")) ui->textBoxMaxFramePID->setText(setting.value("egg4MaxFramePID").toString());
    if (setting.contains("egg4MinDelayPID")) ui->textBoxMinDelayPID->setText(setting.value("egg4MinDelayPID").toString());
    if (setting.contains("egg4MaxDelayPID")) ui->textBoxMaxDelayPID->setText(setting.value("egg4MaxDelayPID").toString());
    if (setting.contains("egg4MinFramePID")) ui->textBoxMinFramePID->setText(setting.value("egg4MinFramePID").toString());
    if (setting.contains("egg4MaxFramePID")) ui->textBoxMaxFramePID->setText(setting.value("egg4MaxFramePID").toString());
}

void Eggs4::updatePID(const QVector<Frame4> &frames, int progress)
{
    searcherPID->addItems(frames);
    ui->progressBarPID->setValue(progress);
}

void Eggs4::updateIVs(const QVector<Frame4> &frames, int progress)
{
    searcherIVs->addItems(frames);
    ui->progressBarIVs->setValue(progress);
}

void Eggs4::refreshProfiles()
{
    emit alertProfiles(4);
}

void Eggs4::on_pushButtonGenerate_clicked()
{
    generatorModel->clear();

    u32 startingFrame = ui->textBoxStartingFrame->text().toUInt();
    u32 maxResults = ui->textBoxMaxResults->text().toUInt();
    u32 seed = ui->textBoxSeedGenerator->text().toUInt(nullptr, 16);
    u16 tid = ui->textBoxTIDGenerator->text().toUShort();
    u16 sid = ui->textBoxSIDGenerator->text().toUShort();

    Method method = static_cast<Method>(ui->comboBoxMethod->currentData().toInt());

    if (method == Method::Gen4Normal)
    {
        if (ui->checkBoxMasuadaGenerator->isChecked())
        {
            method = Method::Gen4Masuada;
        }
    }
    else
    {
        Game version = profiles[ui->comboBoxProfiles->currentIndex()].getVersion();
        method = version & Game::HGSS ? Method::HGSSIVs : Method::DPPtIVs;
    }

    generatorModel->setMethod(method);

    QVector<u8> parent1 =
    {
        static_cast<u8>(ui->parent1HPGenerator->value()), static_cast<u8>(ui->parent1AtkGenerator->value()), static_cast<u8>(ui->parent1DefGenerator->value()),
        static_cast<u8>(ui->parent1SpAGenerator->value()), static_cast<u8>(ui->parent1SpDGenerator->value()), static_cast<u8>(ui->parent1SpeGenerator->value())
    };
    QVector<u8> parent2 =
    {
        static_cast<u8>(ui->parent2HPGenerator->value()), static_cast<u8>(ui->parent2AtkGenerator->value()), static_cast<u8>(ui->parent2DefGenerator->value()),
        static_cast<u8>(ui->parent2SpAGenerator->value()), static_cast<u8>(ui->parent2SpDGenerator->value()), static_cast<u8>(ui->parent2SpeGenerator->value())
    };

    Egg4 generator = Egg4(maxResults, startingFrame, tid, sid, method, seed);
    generator.setParents(parent1, parent2);

    FrameCompare compare = FrameCompare(ui->ivFilterGenerator->getEvals(), ui->ivFilterGenerator->getValues(), ui->comboBoxGenderGenerator->currentIndex(),
                                        ui->comboBoxGenderRatioGenerator->currentIndex(), ui->comboBoxAbilityGenerator->currentIndex(), ui->comboBoxNatureGenerator->getChecked(),
                                        ui->comboBoxHiddenPowerGenerator->getChecked(), ui->checkBoxShinyGenerator->isChecked(), false);

    QVector<Frame4> frames = generator.generate(compare);
    generatorModel->setModel(frames);
}

void Eggs4::on_pushButtonGeneratePID_clicked()
{
    searcherPID->clear();
    searcherPID->setMethod(ui->checkBoxMasuadaSearcher->isChecked() ? Method::Gen4Masuada : Method::Gen4Normal);

    ui->pushButtonGeneratePID->setEnabled(false);
    ui->pushButtonCancelPID->setEnabled(true);

    u16 tid = ui->textBoxTIDSearcher->text().toUShort();
    u16 sid = ui->textBoxSIDSearcher->text().toUShort();

    int genderRatioIndex = ui->comboBoxGenderRatioSearcher->currentIndex();
    FrameCompare compare = FrameCompare(ui->comboBoxGenderSearcher->currentIndex(), genderRatioIndex, ui->comboBoxAbilitySearcher->currentIndex(),
                                        ui->comboBoxNatureSearcher->getChecked(), ui->checkBoxShinySearcher->isChecked());

    u32 minDelay = ui->textBoxMinDelayPID->text().toUInt();
    u32 maxDelay = ui->textBoxMaxDelayPID->text().toUInt();
    u32 minFrame = ui->textBoxMinFramePID->text().toUInt();
    u32 maxFrame = ui->textBoxMaxFramePID->text().toUInt();

    Method type = ui->checkBoxMasuadaSearcher->isChecked() ? Method::Gen4Masuada : Method::Gen4Normal;
    Egg4 generator = Egg4(maxFrame - minFrame + 1, minFrame, tid, sid, type, 0);

    ui->progressBarPID->setValue(0);
    ui->progressBarPID->setMaximum(static_cast<int>(256 * 24 * (maxDelay - minDelay + 1)));

    auto *search = new PIDSearcher(generator, compare, minDelay, maxDelay);
    auto *timer = new QTimer();

    connect(search, &PIDSearcher::finished, timer, &QTimer::deleteLater);
    connect(search, &PIDSearcher::finished, timer, &QTimer::stop);
    connect(search, &PIDSearcher::finished, this, [ = ] { ui->pushButtonGeneratePID->setEnabled(true); ui->pushButtonCancelPID->setEnabled(false); });
    connect(search, &PIDSearcher::finished, this, [ = ] { updatePID(search->getResults(), search->currentProgress()); });
    connect(timer, &QTimer::timeout, this, [ = ] { updatePID(search->getResults(), search->currentProgress()); });
    connect(ui->pushButtonCancelPID, &QPushButton::clicked, search, &PIDSearcher::cancelSearch);

    search->start();
    timer->start(1000);
}

void Eggs4::on_pushButtonGenerateIVs_clicked()
{
    searcherIVs->clear();
    Game version = profiles[ui->comboBoxProfiles->currentIndex()].getVersion();
    searcherIVs->setMethod(version & Game::HGSS ? Method::HGSSIVs : Method::DPPtIVs);

    ui->pushButtonGenerateIVs->setEnabled(false);
    ui->pushButtonCancelIVs->setEnabled(true);

    QVector<u8> parent1 =
    {
        static_cast<u8>(ui->parent1HPSearcher->value()), static_cast<u8>(ui->parent1AtkSearcher->value()), static_cast<u8>(ui->parent1DefSearcher->value()),
        static_cast<u8>(ui->parent1SpASearcher->value()), static_cast<u8>(ui->parent1SpDSearcher->value()), static_cast<u8>(ui->parent1SpeSearcher->value())
    };
    QVector<u8> parent2 =
    {
        static_cast<u8>(ui->parent2HPSearcher->value()), static_cast<u8>(ui->parent2AtkSearcher->value()), static_cast<u8>(ui->parent2DefSearcher->value()),
        static_cast<u8>(ui->parent2SpASearcher->value()), static_cast<u8>(ui->parent2SpDSearcher->value()), static_cast<u8>(ui->parent2SpeSearcher->value())
    };

    FrameCompare compare = FrameCompare(ui->ivFilterSearcher->getEvals(), ui->ivFilterSearcher->getValues(), ui->comboBoxHiddenPowerSearcher->getChecked());

    u32 minDelay = ui->textBoxMinDelayIVs->text().toUInt();
    u32 maxDelay = ui->textBoxMaxDelayIVs->text().toUInt();
    u32 minFrame = ui->textBoxMinFrameIVs->text().toUInt();
    u32 maxFrame = ui->textBoxMaxFrameIVs->text().toUInt();

    Method type = version & Game::HGSS ? Method::HGSSIVs : Method::DPPtIVs;
    Egg4 generator = Egg4(maxFrame - minFrame + 1, minFrame, 0, 0, type, 0);
    generator.setParents(parent1, parent2);

    ui->progressBarIVs->setValue(0);
    ui->progressBarIVs->setMaximum(static_cast<int>(256 * 24 * (maxDelay - minDelay + 1)));

    auto *search = new IVSearcher(generator, compare, minDelay, maxDelay);
    auto *timer = new QTimer();

    connect(search, &IVSearcher::finished, timer, &QTimer::deleteLater);
    connect(search, &IVSearcher::finished, timer, &QTimer::stop);
    connect(search, &IVSearcher::finished, this, [ = ] { ui->pushButtonGenerateIVs->setEnabled(true); ui->pushButtonCancelIVs->setEnabled(false); });
    connect(search, &IVSearcher::finished, this, [ = ] { updateIVs(search->getResults(), search->currentProgress()); });
    connect(timer, &QTimer::timeout, this, [ = ] { updateIVs(search->getResults(), search->currentProgress()); });
    connect(ui->pushButtonCancelIVs, &QPushButton::clicked, search, &IVSearcher::cancelSearch);

    search->start();
    timer->start(1000);
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
    if (index < 0)
    {
        return;
    }

    auto profile = profiles[index];
    QString tid = QString::number(profile.getTID());
    QString sid = QString::number(profile.getSID());

    ui->textBoxTIDGenerator->setText(tid);
    ui->textBoxSIDGenerator->setText(sid);
    ui->textBoxTIDSearcher->setText(tid);
    ui->textBoxSIDSearcher->setText(sid);
    ui->profileTID->setText(tid);
    ui->profileSID->setText(sid);
    ui->profileGame->setText(profile.getVersionString());
}

void Eggs4::on_tableViewPID_customContextMenuRequested(const QPoint &pos)
{
    if (searcherPID->rowCount() == 0)
    {
        return;
    }

    flag = false;
    searcherMenu->popup(ui->tableViewPID->viewport()->mapToGlobal(pos));
}

void Eggs4::on_tableViewIVs_customContextMenuRequested(const QPoint &pos)
{
    if (searcherIVs->rowCount() == 0)
    {
        return;
    }

    flag = true;
    searcherMenu->popup(ui->tableViewIVs->viewport()->mapToGlobal(pos));
}

void Eggs4::seedToTime()
{
    QString seed;

    if (flag)
    {
        QModelIndex index = ui->tableViewIVs->currentIndex();
        seed = searcherIVs->data(searcherIVs->index(index.row(), 0), Qt::DisplayRole).toString();
    }
    else
    {
        QModelIndex index = ui->tableViewPID->currentIndex();
        seed = searcherPID->data(searcherPID->index(index.row(), 0), Qt::DisplayRole).toString();
    }

    auto *time = new SeedtoTime4(seed, profiles[ui->comboBoxProfiles->currentIndex()]);
    time->show();
    time->raise();
}

void Eggs4::on_pushButtonProfileManager_clicked()
{
    auto *manager = new ProfileManager4();
    connect(manager, SIGNAL(updateProfiles()), this, SLOT(refreshProfiles()));
    manager->show();
}


PIDSearcher::PIDSearcher(const Egg4 &generator, const FrameCompare &compare, u32 minDelay, u32 maxDelay)
{
    this->generator = generator;
    this->compare = compare;
    this->minDelay = minDelay;
    this->maxDelay = maxDelay;
    cancel = false;
    progress = 0;

    connect(this, &PIDSearcher::finished, this, &PIDSearcher::deleteLater);
}

void PIDSearcher::run()
{
    int total = 0;

    for (u16 ab = 0; ab < 256; ab++)
    {
        for (u8 cd = 0; cd < 24; cd++)
        {
            for (u32 efgh = minDelay; efgh <= maxDelay; efgh++)
            {
                if (cancel)
                {
                    return;
                }

                if (total > 10000)
                {
                    progress = static_cast<int>(256 * 24 * (maxDelay - minDelay + 1));
                    break;
                }

                u32 seed = ((ab << 24) | (cd << 16)) + efgh;
                generator.setSeed(seed);

                auto frames = generator.generate(compare);
                progress++;
                total += frames.size();

                QMutexLocker locker(&mutex);
                results.append(frames);
            }
        }
    }
}

int PIDSearcher::currentProgress()
{
    return progress;
}

QVector<Frame4> PIDSearcher::getResults()
{
    QMutexLocker lock(&mutex);
    auto data(results);
    results.clear();
    return data;
}

void PIDSearcher::cancelSearch()
{
    cancel = true;
}


IVSearcher::IVSearcher(const Egg4 &generator, const FrameCompare &compare, u32 minDelay, u32 maxDelay)
{
    this->generator = generator;
    this->compare = compare;
    this->minDelay = minDelay;
    this->maxDelay = maxDelay;
    cancel = false;
    progress = 0;

    connect(this, &IVSearcher::finished, this, &IVSearcher::deleteLater);
}

void IVSearcher::run()
{
    int total = 0;

    for (u16 ab = 0; ab < 256; ab++)
    {
        for (u8 cd = 0; cd < 24; cd++)
        {
            for (u32 efgh = minDelay; efgh <= maxDelay; efgh++)
            {
                if (cancel)
                {
                    return;
                }

                if (total > 10000)
                {
                    progress = static_cast<int>(256 * 24 * (maxDelay - minDelay + 1));
                    break;
                }

                u32 seed = ((ab << 24) | (cd << 16)) + efgh;
                generator.setSeed(seed);

                auto frames = generator.generate(compare);
                progress++;
                total += frames.size();

                QMutexLocker locker(&mutex);
                results.append(frames);
            }
        }
    }
}

int IVSearcher::currentProgress()
{
    return progress;
}

QVector<Frame4> IVSearcher::getResults()
{
    QMutexLocker lock(&mutex);
    auto data(results);
    results.clear();
    return data;
}

void IVSearcher::cancelSearch()
{
    cancel = true;
}
