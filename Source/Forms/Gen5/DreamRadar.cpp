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

#include "DreamRadar.hpp"
#include "ui_DreamRadar.h"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen5/Generators/DreamRadarGenerator.hpp>
#include <Core/Gen5/Keypresses.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/Searchers/DreamRadarSearcher.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Controls/Controls.hpp>
#include <Forms/Gen5/Profile/ProfileManager5.hpp>
#include <Forms/Models/Gen5/DreamRadarModel.hpp>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>

DreamRadar::DreamRadar(QWidget *parent) : QWidget(parent), ui(new Ui::DreamRadar)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();
}

DreamRadar::~DreamRadar()
{
    QSettings setting;
    setting.beginGroup("dreamRadar");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void DreamRadar::updateProfiles()
{
    profiles.clear();
    auto completeProfiles = ProfileLoader5::getProfiles();
    std::copy_if(completeProfiles.begin(), completeProfiles.end(), std::back_inserter(profiles),
                 [](const Profile5 &profile) { return (profile.getVersion() & Game::BW2) != Game::None; });

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("dreamRadar/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

bool DreamRadar::hasProfiles() const
{
    return !profiles.empty();
}

void DreamRadar::setupModels()
{
    generatorModel = new DreamRadarGeneratorModel5(ui->tableViewGenerator);
    searcherModel = new DreamRadarSearcherModel5(ui->tableViewSearcher);
    generatorMenu = new QMenu(ui->tableViewGenerator);
    searcherMenu = new QMenu(ui->tableViewSearcher);

    ui->comboBoxGeneratorSpecies1->addItem(tr("None"), 0);
    ui->comboBoxGeneratorSpecies2->addItem(tr("None"), 0);
    ui->comboBoxGeneratorSpecies3->addItem(tr("None"), 0);
    ui->comboBoxGeneratorSpecies4->addItem(tr("None"), 0);
    ui->comboBoxGeneratorSpecies5->addItem(tr("None"), 0);
    ui->comboBoxGeneratorSpecies6->addItem(tr("None"), 0);

    ui->comboBoxSearcherSpecies1->addItem(tr("None"), 0);
    ui->comboBoxSearcherSpecies2->addItem(tr("None"), 0);
    ui->comboBoxSearcherSpecies3->addItem(tr("None"), 0);
    ui->comboBoxSearcherSpecies4->addItem(tr("None"), 0);
    ui->comboBoxSearcherSpecies5->addItem(tr("None"), 0);
    ui->comboBoxSearcherSpecies6->addItem(tr("None"), 0);

    ui->filterGenerator->enableHiddenAbility();
    ui->filterSearcher->enableHiddenAbility();

    std::vector<u16> species = { 641, 642, 645, 483, 484, 487, 249, 250, 79,  120, 137, 163, 174,
                                 175, 213, 238, 280, 333, 374, 425, 436, 442, 447, 479, 517, 561 };
    std::vector<std::string> names = Translator::getSpecies(species);

    for (size_t i = 0; i < species.size(); i++)
    {
        if (i < 3)
        {
            ui->comboBoxGeneratorSpecies1->addItem(QString::fromStdString(names[i]), species[i]);
            ui->comboBoxSearcherSpecies1->addItem(QString::fromStdString(names[i]), species[i]);
        }
        else
        {
            ui->comboBoxGeneratorSpecies1->addItem(QString::fromStdString(names[i]), species[i]);
            ui->comboBoxGeneratorSpecies2->addItem(QString::fromStdString(names[i]), species[i]);
            ui->comboBoxGeneratorSpecies3->addItem(QString::fromStdString(names[i]), species[i]);
            ui->comboBoxGeneratorSpecies4->addItem(QString::fromStdString(names[i]), species[i]);
            ui->comboBoxGeneratorSpecies5->addItem(QString::fromStdString(names[i]), species[i]);
            ui->comboBoxGeneratorSpecies6->addItem(QString::fromStdString(names[i]), species[i]);

            ui->comboBoxSearcherSpecies1->addItem(QString::fromStdString(names[i]), species[i]);
            ui->comboBoxSearcherSpecies2->addItem(QString::fromStdString(names[i]), species[i]);
            ui->comboBoxSearcherSpecies3->addItem(QString::fromStdString(names[i]), species[i]);
            ui->comboBoxSearcherSpecies4->addItem(QString::fromStdString(names[i]), species[i]);
            ui->comboBoxSearcherSpecies5->addItem(QString::fromStdString(names[i]), species[i]);
            ui->comboBoxSearcherSpecies6->addItem(QString::fromStdString(names[i]), species[i]);
        }
    }

    for (const std::string &gender : *Translator::getGenders())
    {
        ui->comboBoxGeneratorGender1->addItem(QString::fromStdString(gender));
        ui->comboBoxGeneratorGender2->addItem(QString::fromStdString(gender));
        ui->comboBoxGeneratorGender3->addItem(QString::fromStdString(gender));
        ui->comboBoxGeneratorGender4->addItem(QString::fromStdString(gender));
        ui->comboBoxGeneratorGender5->addItem(QString::fromStdString(gender));
        ui->comboBoxGeneratorGender6->addItem(QString::fromStdString(gender));

        ui->comboBoxSearcherGender1->addItem(QString::fromStdString(gender));
        ui->comboBoxSearcherGender2->addItem(QString::fromStdString(gender));
        ui->comboBoxSearcherGender3->addItem(QString::fromStdString(gender));
        ui->comboBoxSearcherGender4->addItem(QString::fromStdString(gender));
        ui->comboBoxSearcherGender5->addItem(QString::fromStdString(gender));
        ui->comboBoxSearcherGender6->addItem(QString::fromStdString(gender));
    }

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed64Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);

    ui->textBoxSearcherInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxAdvances->setValues(InputType::Advance32Bit);

    ui->filterGenerator->disableControls(Controls::EncounterSlots);
    ui->filterSearcher->disableControls(Controls::EncounterSlots | Controls::DisableFilter | Controls::UseDelay);

    QAction *outputTXTGenerator = generatorMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVGenerator = generatorMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTGenerator, &QAction::triggered, [=]() { ui->tableViewGenerator->outputModel(false); });
    connect(outputCSVGenerator, &QAction::triggered, [=]() { ui->tableViewGenerator->outputModel(true); });

    QAction *outputTXTSearcher = searcherMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVSearcher = searcherMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTSearcher, &QAction::triggered, [=]() { ui->tableViewSearcher->outputModel(false); });
    connect(outputCSVSearcher, &QAction::triggered, [=]() { ui->tableViewSearcher->outputModel(true); });

    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DreamRadar::profileIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &DreamRadar::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &DreamRadar::search);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &DreamRadar::profileManager);
    connect(ui->tableViewGenerator, &QTableView::customContextMenuRequested, this, &DreamRadar::tableViewGeneratorContextMenu);
    connect(ui->tableViewSearcher, &QTableView::customContextMenuRequested, this, &DreamRadar::tableViewSearcherContextMenu);

    updateProfiles();

    QSettings setting;
    setting.beginGroup("dreamRadar");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

void DreamRadar::generate()
{
    auto radarSlots = getGeneratorSettings();
    if (radarSlots.empty())
    {
        QMessageBox message(QMessageBox::Warning, tr("Missing settings"), tr("Enter information for at least 1 slot"));
        message.exec();
        return;
    }

    generatorModel->clearModel();

    u64 seed = ui->textBoxGeneratorSeed->getULong();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u16 tid = currentProfile->getTID();
    u16 sid = currentProfile->getSID();
    u8 genderRatio = ui->filterGenerator->getGenderRatio();
    u32 offset = 0;
    if (ui->filterGenerator->useDelay())
    {
        offset = ui->filterGenerator->getDelay();
    }

    StateFilter filter(ui->filterGenerator->getGender(), ui->filterGenerator->getAbility(), ui->filterGenerator->getShiny(),
                       ui->filterGenerator->getDisableFilters(), ui->filterGenerator->getMinIVs(), ui->filterGenerator->getMaxIVs(),
                       ui->filterGenerator->getNatures(), ui->filterGenerator->getHiddenPowers(), {});

    DreamRadarGenerator generator(initialAdvances, maxAdvances, tid, sid, genderRatio, Method::DreamRadar, filter, radarSlots);
    generator.setOffset(offset);

    auto states = generator.generate(seed, currentProfile->getMemoryLink());
    generatorModel->addItems(states);
}

void DreamRadar::search()
{
    auto radarSlots = getSearcherSettings();
    if (radarSlots.empty())
    {
        QMessageBox message(QMessageBox::Warning, tr("Missing settings"), tr("Enter information for at least 1 slot"));
        message.exec();
        return;
    }

    searcherModel->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u32 initialAdvances = ui->textBoxSearcherInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxSearcherMaxAdvances->getUInt();
    u16 tid = currentProfile->getTID();
    u16 sid = currentProfile->getSID();
    u8 genderRatio = ui->filterSearcher->getGenderRatio();

    StateFilter filter(ui->filterSearcher->getGender(), ui->filterSearcher->getAbility(), ui->filterSearcher->getShiny(),
                       ui->filterSearcher->getDisableFilters(), ui->filterSearcher->getMinIVs(), ui->filterSearcher->getMaxIVs(),
                       ui->filterSearcher->getNatures(), ui->filterSearcher->getHiddenPowers(), {});

    DreamRadarGenerator generator(initialAdvances, maxAdvances, tid, sid, genderRatio, Method::DreamRadar, filter, radarSlots);
    generator.setOffset(0);

    auto *searcher = new DreamRadarSearcher(*currentProfile);

    Date start = ui->dateEditSearcherStartDate->getDate();
    Date end = ui->dateEditSearcherEndDate->getDate();

    int maxProgress = Keypresses::getKeyPresses(currentProfile->getKeypresses(), currentProfile->getSkipLR()).size();
    maxProgress *= start.daysTo(end) + 1;
    maxProgress *= currentProfile->getTimer0Max() - currentProfile->getTimer0Min() + 1;
    ui->progressBar->setRange(0, maxProgress);

    QSettings settings;
    int threads = settings.value("settings/threads").toInt();

    auto *thread = QThread::create([=] { searcher->startSearch(generator, threads, start, end); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    connect(timer, &QTimer::timeout, [=] {
        searcherModel->addItems(searcher->getResults());
        ui->progressBar->setValue(searcher->getProgress());
    });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, [=] {
        ui->pushButtonSearch->setEnabled(true);
        ui->pushButtonCancel->setEnabled(false);
        searcherModel->addItems(searcher->getResults());
        ui->progressBar->setValue(searcher->getProgress());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}

std::vector<DreamRadarSlot> DreamRadar::getGeneratorSettings()
{
    std::vector<DreamRadarSlot> radarSlots;

    std::array<u16, 3> genies = { 641, 642, 645 };
    std::array<u16, 5> legends = { 483, 484, 487, 249, 250 };
    auto info = PersonalLoader::getPersonal(Game::BW2);

    std::array<QComboBox *, 6> species = { ui->comboBoxGeneratorSpecies1, ui->comboBoxGeneratorSpecies2, ui->comboBoxGeneratorSpecies3,
                                           ui->comboBoxGeneratorSpecies4, ui->comboBoxGeneratorSpecies5, ui->comboBoxGeneratorSpecies6 };
    std::array<QComboBox *, 6> genders = { ui->comboBoxGeneratorGender1, ui->comboBoxGeneratorGender2, ui->comboBoxGeneratorGender3,
                                           ui->comboBoxGeneratorGender4, ui->comboBoxGeneratorGender5, ui->comboBoxGeneratorGender6 };

    for (size_t i = 0; i < 6; i++)
    {
        u16 specie = species[i]->currentData().toUInt();
        if (specie != 0)
        {
            u8 type = 2;
            if (std::find(genies.begin(), genies.end(), specie) != genies.end())
            {
                type = 0;
            }
            else if (std::find(legends.begin(), legends.end(), specie) != legends.end())
            {
                type = 1;
            }

            u8 genderRatio = info[specie].getGender();
            // TODO  lock gender boxes for forced gender PIDs
            u8 gender = genderRatio == 255 ? 2 : genders[i]->currentIndex();

            radarSlots.emplace_back(type, gender, genderRatio);
        }
    }

    return radarSlots;
}

std::vector<DreamRadarSlot> DreamRadar::getSearcherSettings()
{
    std::vector<DreamRadarSlot> radarSlots;

    std::array<u16, 3> genies = { 641, 642, 645 };
    std::array<u16, 5> legends = { 483, 484, 487, 249, 250 };
    auto info = PersonalLoader::getPersonal(Game::BW2);

    std::array<QComboBox *, 6> species = { ui->comboBoxSearcherSpecies1, ui->comboBoxSearcherSpecies2, ui->comboBoxSearcherSpecies3,
                                           ui->comboBoxSearcherSpecies4, ui->comboBoxSearcherSpecies5, ui->comboBoxSearcherSpecies6 };
    std::array<QComboBox *, 6> genders = { ui->comboBoxSearcherGender1, ui->comboBoxSearcherGender2, ui->comboBoxSearcherGender3,
                                           ui->comboBoxSearcherGender4, ui->comboBoxSearcherGender5, ui->comboBoxSearcherGender6 };

    for (size_t i = 0; i < 6; i++)
    {
        u16 specie = species[i]->currentData().toUInt();
        if (specie != 0)
        {
            u8 type = 2;
            if (std::find(genies.begin(), genies.end(), specie) != genies.end())
            {
                type = 0;
            }
            else if (std::find(legends.begin(), legends.end(), specie) != legends.end())
            {
                type = 1;
            }

            u8 genderRatio = info[specie].getGender();
            // TODO  lock gender boxes for forced gender PIDs
            u8 gender = genderRatio == 255 ? 2 : genders[i]->currentIndex();

            radarSlots.emplace_back(type, gender, genderRatio);
        }
    }

    return radarSlots;
}

void DreamRadar::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = &profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile->getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile->getSID()));
        ui->labelProfileMACAddressValue->setText(QString::number(currentProfile->getMac(), 16));
        ui->labelProfileDSTypeValue->setText(QString::fromStdString(currentProfile->getDSTypeString()));
        ui->labelProfileVCountValue->setText(QString::number(currentProfile->getVCount(), 16));
        ui->labelProfileTimer0Value->setText(QString::number(currentProfile->getTimer0Min(), 16) + "-"
                                             + QString::number(currentProfile->getTimer0Max(), 16));
        ui->labelProfileGxStatValue->setText(QString::number(currentProfile->getGxStat()));
        ui->labelProfileVFrameValue->setText(QString::number(currentProfile->getVFrame()));
        ui->labelProfileKeypressesValue->setText(QString::fromStdString(currentProfile->getKeypressesString()));
        ui->labelProfileGameValue->setText(QString::fromStdString(currentProfile->getVersionString()));
    }
}

void DreamRadar::tableViewGeneratorContextMenu(QPoint pos)
{
    if (generatorModel->rowCount() > 0)
    {
        generatorMenu->popup(ui->tableViewGenerator->viewport()->mapToGlobal(pos));
    }
}

void DreamRadar::tableViewSearcherContextMenu(QPoint pos)
{
    if (searcherModel->rowCount() == 0)
    {
        searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
    }
}

void DreamRadar::profileManager()
{
    auto *manager = new ProfileManager5();
    connect(manager, &ProfileManager5::updateProfiles, this, [=] { emit alertProfiles(5); });
    manager->show();
}
