/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Enum/Method.hpp>
#include <Core/Gen4/Generators/EggGenerator4.hpp>
#include <Core/Gen4/ProfileLoader4.hpp>
#include <Core/Gen4/Searchers/EggSearcher4.hpp>
#include <Core/Parents/Filters/FrameFilter.hpp>
#include <Core/Parents/Frames/EggFrame.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Gen4/Profile/ProfileManager4.hpp>
#include <Forms/Gen4/Tools/SeedtoTime4.hpp>
#include <Models/Gen4/EggModel4.hpp>
#include <QSettings>
#include <QThread>
#include <QTimer>

Eggs4::Eggs4(QWidget *parent) : QWidget(parent), ui(new Ui::Eggs4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();

    qRegisterMetaType<QVector<EggFrame>>("QVector<EggFrame>");
}

Eggs4::~Eggs4()
{
    QSettings setting;
    setting.beginGroup("eggs4");
    setting.setValue("minDelay", ui->textBoxSearcherMinDelay->text());
    setting.setValue("maxDelay", ui->textBoxSearcherMaxDelay->text());
    setting.setValue("minFrameIV", ui->textBoxSearcherIVMinFrame->text());
    setting.setValue("maxFrameIV", ui->textBoxSearcherIVMaxFrame->text());
    setting.setValue("minFramePID", ui->textBoxSearcherPIDMinFrame->text());
    setting.setValue("maxFramePID", ui->textBoxSearcherPIDMaxFrame->text());
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Eggs4::updateProfiles()
{
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Eggs4::profilesIndexChanged);

    profiles = ProfileLoader4::getProfiles();
    profiles.insert(profiles.begin(), Profile4());

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(profile.getName());
    }

    QSettings setting;
    int val = setting.value("eggs4/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Eggs4::setupModels()
{
    generatorModel = new EggGeneratorModel4(ui->tableViewGenerator, Method::DPPtIVs);
    searcherModel = new EggSearcherModel4(ui->tableViewSearcher, Method::DPPtIVs);
    searcherMenu = new QMenu(this);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorInitialFrame->setValues(InputType::Frame32Bit);
    ui->textBoxGeneratorMaxResults->setValues(InputType::Frame32Bit);
    ui->textBoxGeneratorSeed->setValues(InputType::Seed32Bit);

    ui->textBoxSearcherIVMinFrame->setValues(InputType::Frame32Bit);
    ui->textBoxSearcherIVMaxFrame->setValues(InputType::Frame32Bit);
    ui->textBoxSearcherPIDMinFrame->setValues(InputType::Frame32Bit);
    ui->textBoxSearcherPIDMaxFrame->setValues(InputType::Frame32Bit);
    ui->textBoxSearcherMinDelay->setValues(InputType::Frame32Bit);
    ui->textBoxSearcherMaxDelay->setValues(InputType::Frame32Bit);

    ui->comboBoxGeneratorMethod->setup({ Method::DPPtIVs, Method::Gen4Normal });

    ui->filterGenerator->disableControls(Controls::EncounterSlots);
    ui->filterSearcher->disableControls(Controls::EncounterSlots | Controls::UseDelay | Controls::DisableFilter);

    QAction *seedToTime = searcherMenu->addAction(tr("Generate times for seed"));
    connect(seedToTime, &QAction::triggered, this, &Eggs4::seedToTime);

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Eggs4::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Eggs4::search);
    connect(ui->tableViewSearcher, &QTableView::customContextMenuRequested, this, &Eggs4::tableViewSearcherContextMenu);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Eggs4::profileManager);
    connect(ui->eggSettingsGenerator, &EggSettings::toggleInheritance, generatorModel, &EggGeneratorModel4::toggleInheritance);
    connect(ui->eggSettingsSearcher, &EggSettings::toggleInheritance, searcherModel, &EggSearcherModel4::toggleInheritance);

    QSettings setting;
    setting.beginGroup("eggs4");
    if (setting.contains("minDelay"))
    {
        ui->textBoxSearcherMinDelay->setText(setting.value("minDelay").toString());
    }
    if (setting.contains("maxDelay"))
    {
        ui->textBoxSearcherMaxDelay->setText(setting.value("maxDelay").toString());
    }
    if (setting.contains("minFrameIV"))
    {
        ui->textBoxSearcherIVMinFrame->setText(setting.value("minFrameIV").toString());
    }
    if (setting.contains("maxFrameIV"))
    {
        ui->textBoxSearcherIVMaxFrame->setText(setting.value("maxFrameIV").toString());
    }
    if (setting.contains("minFramePID"))
    {
        ui->textBoxSearcherPIDMinFrame->setText(setting.value("minFramePID").toString());
    }
    if (setting.contains("maxFramePID"))
    {
        ui->textBoxSearcherPIDMaxFrame->setText(setting.value("maxFramePID").toString());
    }
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

void Eggs4::updateProgress(const QVector<EggFrame4> &frames, int progress)
{
    searcherModel->addItems(frames);
    ui->progressBarSearcher->setValue(progress);
}

void Eggs4::generate()
{
    generatorModel->clearModel();

    u32 initialFrame = ui->textBoxGeneratorInitialFrame->getUInt();
    u32 maxResults = ui->textBoxGeneratorMaxResults->getUInt();
    u32 seed = ui->textBoxGeneratorSeed->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();

    auto method = static_cast<Method>(ui->comboBoxGeneratorMethod->currentData().toInt());
    if (method == Method::Gen4Normal)
    {
        if (ui->checkBoxGeneratorMasuada->isChecked())
        {
            method = Method::Gen4Masuada;
        }
    }
    else
    {
        Game version = currentProfile.getVersion();
        method = (version & Game::HGSS) ? Method::HGSSIVs : Method::DPPtIVs;
    }
    generatorModel->setMethod(method);

    FrameFilter filter(ui->filterGenerator->getGender(), ui->filterGenerator->getAbility(), ui->filterGenerator->getShiny(), false,
                       ui->filterGenerator->getMinIVs(), ui->filterGenerator->getMaxIVs(), ui->filterGenerator->getNatures(),
                       ui->filterGenerator->getHiddenPowers(), {});

    EggGenerator4 generator(initialFrame, maxResults, tid, sid, ui->filterGenerator->getGenderRatio(), method, filter);
    generator.setParents(ui->eggSettingsGenerator->getParent1(), ui->eggSettingsGenerator->getParent2());

    auto frames = generator.generate(seed);
    generatorModel->addItems(frames);
}

void Eggs4::search()
{
    searcherModel->clearModel();

    Method methodModel;
    switch (ui->comboBoxSearcherMethod->currentIndex())
    {
    case 0:
        methodModel = (currentProfile.getVersion() & Game::HGSS) ? Method::HGSSIVs : Method::DPPtIVs;
        break;
    case 1:
        methodModel = ui->checkBoxSearcherMasuada->isChecked() ? Method::Gen4Masuada : Method::Gen4Normal;
        break;
    case 2:
        methodModel = Method::Gen4Combined;
        break;
    }
    searcherModel->setMethod(methodModel);

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterSearcher->getGenderRatio();

    FrameFilter filter(ui->filterSearcher->getGender(), ui->filterSearcher->getAbility(), ui->filterSearcher->getShiny(), false,
                       ui->filterSearcher->getMinIVs(), ui->filterSearcher->getMaxIVs(), ui->filterSearcher->getNatures(),
                       ui->filterSearcher->getHiddenPowers(), {});

    u32 minDelay = ui->textBoxSearcherMinDelay->getUInt();
    u32 maxDelay = ui->textBoxSearcherMaxDelay->getUInt();
    u32 minFrameIV = ui->textBoxSearcherIVMinFrame->getUInt();
    u32 maxFrameIV = ui->textBoxSearcherIVMaxFrame->getUInt();
    u32 minFramePID = ui->textBoxSearcherPIDMinFrame->getUInt();
    u32 maxFramePID = ui->textBoxSearcherPIDMaxFrame->getUInt();

    Method methodIV = (currentProfile.getVersion() & Game::HGSS) ? Method::HGSSIVs : Method::DPPtIVs;
    EggGenerator4 generatorIV(minFrameIV, maxFrameIV, tid, sid, genderRatio, methodIV, filter);
    generatorIV.setParents(ui->eggSettingsSearcher->getParent1(), ui->eggSettingsSearcher->getParent2());

    Method methodPID = ui->checkBoxSearcherMasuada->isChecked() ? Method::Gen4Masuada : Method::Gen4Normal;
    EggGenerator4 generatorPID(minFramePID, maxFramePID, tid, sid, genderRatio, methodPID, filter);

    ui->progressBarSearcher->setValue(0);
    ui->progressBarSearcher->setMaximum(static_cast<int>(256 * 24 * (maxDelay - minDelay + 1)));

    auto *searcher = new EggSearcher4(tid, sid, genderRatio, methodModel, filter);
    searcher->setGenerators(generatorIV, generatorPID);
    searcher->setType(ui->comboBoxSearcherMethod->currentIndex());

    auto *thread = QThread::create([=] { searcher->startSearch(minDelay, maxDelay); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    connect(timer, &QTimer::timeout, [=] { updateProgress(searcher->getResults(), searcher->getProgress()); });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, [=] {
        ui->pushButtonSearch->setEnabled(true);
        ui->pushButtonCancel->setEnabled(false);
        updateProgress(searcher->getResults(), searcher->getProgress());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}

void Eggs4::profilesIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = profiles.at(index);

        ui->labelProfileTIDValue->setText(QString::number(currentProfile.getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile.getSID()));
        ui->labelProfileGameValue->setText(currentProfile.getVersionString());
    }
}

void Eggs4::tableViewSearcherContextMenu(QPoint pos)
{
    if (searcherModel->rowCount() > 0)
    {
        searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
    }
}

void Eggs4::seedToTime()
{
    QModelIndex index = ui->tableViewSearcher->currentIndex();
    QString seed = searcherModel->data(searcherModel->index(index.row(), 0)).toString();

    auto *time = new SeedtoTime4(seed, currentProfile);
    time->show();
    time->raise();
}

void Eggs4::profileManager()
{
    auto *manager = new ProfileManager4();
    connect(manager, &ProfileManager4::updateProfiles, this, [=] { emit alertProfiles(4); });
    manager->show();
}
