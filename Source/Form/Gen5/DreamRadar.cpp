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

#include "DreamRadar.hpp"
#include "ui_DreamRadar.h"
#include <Core/Gen5/Encounters5.hpp>
#include <Core/Gen5/Generators/DreamRadarGenerator.hpp>
#include <Core/Gen5/Keypresses.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/Searchers/Searcher5.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen5/Profile/ProfileManager5.hpp>
#include <Model/Gen5/DreamRadarModel.hpp>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>

/**
 * @brief Updates available genders for the select Pokemon
 *
 * @param comboBoxSpecie Pokemon specie combo box
 * @param comboBoxGender Pokemon gender combo box
 */
static void updateGenders(ComboBox *comboBoxSpecie, ComboBox *comboBoxGender)
{
    int index = comboBoxSpecie->getCurrentInt();
    if (index == -1)
    {
        comboBoxGender->clear();
        return;
    }

    const DreamRadarTemplate *dreamRadarTemplate = Encounters5::getDreamRadarEncounters(index);
    const PersonalInfo *info = dreamRadarTemplate->getInfo();

    comboBoxGender->clear();
    switch (info->getGender())
    {
    case 255: // Genderless
        comboBoxGender->addItem(QString::fromStdString(Translator::getGender(2)), 2);
        break;
    case 254: // Female
        comboBoxGender->addItem(QString::fromStdString(Translator::getGender(1)), 1);
        break;
    case 0: // Male
        comboBoxGender->addItem(QString::fromStdString(Translator::getGender(0)), 0);
        break;
    default: // Random gender
        comboBoxGender->addItem(QString::fromStdString(Translator::getGender(0)), 0);
        comboBoxGender->addItem(QString::fromStdString(Translator::getGender(1)), 1);
        break;
    }
}

DreamRadar::DreamRadar(QWidget *parent) : QWidget(parent), ui(new Ui::DreamRadar)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    generatorModel = new DreamRadarGeneratorModel5(ui->tableViewGenerator);
    ui->tableViewGenerator->setModel(generatorModel);

    searcherModel = new DreamRadarSearcherModel5(ui->tableViewSearcher);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->comboBoxGeneratorSpecie1->addItem(tr("None"), -1);
    ui->comboBoxGeneratorSpecie2->addItem(tr("None"), -1);
    ui->comboBoxGeneratorSpecie3->addItem(tr("None"), -1);
    ui->comboBoxGeneratorSpecie4->addItem(tr("None"), -1);
    ui->comboBoxGeneratorSpecie5->addItem(tr("None"), -1);
    ui->comboBoxGeneratorSpecie6->addItem(tr("None"), -1);

    ui->comboBoxSearcherSpecie1->addItem(tr("None"), -1);
    ui->comboBoxSearcherSpecie2->addItem(tr("None"), -1);
    ui->comboBoxSearcherSpecie3->addItem(tr("None"), -1);
    ui->comboBoxSearcherSpecie4->addItem(tr("None"), -1);
    ui->comboBoxSearcherSpecie5->addItem(tr("None"), -1);
    ui->comboBoxSearcherSpecie6->addItem(tr("None"), -1);

    ui->filterGenerator->enableHiddenAbility();
    ui->filterSearcher->enableHiddenAbility();

    ui->textBoxGeneratorSeed->setValues(InputType::Seed64Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);

    ui->textBoxSearcherInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxAdvances->setValues(InputType::Advance32Bit);

    ui->filterGenerator->disableControls(Controls::EncounterSlots);
    ui->filterSearcher->disableControls(Controls::EncounterSlots | Controls::DisableFilter);

    ui->comboBoxGeneratorSpecie1->enableAutoComplete();
    ui->comboBoxGeneratorSpecie2->enableAutoComplete();
    ui->comboBoxGeneratorSpecie3->enableAutoComplete();
    ui->comboBoxGeneratorSpecie4->enableAutoComplete();
    ui->comboBoxGeneratorSpecie5->enableAutoComplete();
    ui->comboBoxGeneratorSpecie6->enableAutoComplete();

    ui->comboBoxSearcherSpecie1->enableAutoComplete();
    ui->comboBoxSearcherSpecie2->enableAutoComplete();
    ui->comboBoxSearcherSpecie3->enableAutoComplete();
    ui->comboBoxSearcherSpecie4->enableAutoComplete();
    ui->comboBoxSearcherSpecie5->enableAutoComplete();
    ui->comboBoxSearcherSpecie6->enableAutoComplete();

    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &DreamRadar::profileIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &DreamRadar::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &DreamRadar::search);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &DreamRadar::profileManager);
    connect(ui->filterGenerator, &Filter::showStatsChanged, generatorModel, &DreamRadarGeneratorModel5::setShowStats);
    connect(ui->filterSearcher, &Filter::showStatsChanged, searcherModel, &DreamRadarSearcherModel5::setShowStats);

    connect(ui->comboBoxGeneratorSpecie1, &ComboBox::currentIndexChanged, this,
            [=]() { updateGenders(ui->comboBoxGeneratorSpecie1, ui->comboBoxGeneratorGender1); });
    connect(ui->comboBoxGeneratorSpecie2, &ComboBox::currentIndexChanged, this,
            [=]() { updateGenders(ui->comboBoxGeneratorSpecie2, ui->comboBoxGeneratorGender2); });
    connect(ui->comboBoxGeneratorSpecie3, &ComboBox::currentIndexChanged, this,
            [=]() { updateGenders(ui->comboBoxGeneratorSpecie3, ui->comboBoxGeneratorGender3); });
    connect(ui->comboBoxGeneratorSpecie4, &ComboBox::currentIndexChanged, this,
            [=]() { updateGenders(ui->comboBoxGeneratorSpecie4, ui->comboBoxGeneratorGender4); });
    connect(ui->comboBoxGeneratorSpecie5, &ComboBox::currentIndexChanged, this,
            [=]() { updateGenders(ui->comboBoxGeneratorSpecie5, ui->comboBoxGeneratorGender5); });
    connect(ui->comboBoxGeneratorSpecie6, &ComboBox::currentIndexChanged, this,
            [=]() { updateGenders(ui->comboBoxGeneratorSpecie6, ui->comboBoxGeneratorGender6); });

    connect(ui->comboBoxSearcherSpecie1, &ComboBox::currentIndexChanged, this,
            [=]() { updateGenders(ui->comboBoxSearcherSpecie1, ui->comboBoxSearcherGender1); });
    connect(ui->comboBoxSearcherSpecie2, &ComboBox::currentIndexChanged, this,
            [=]() { updateGenders(ui->comboBoxSearcherSpecie2, ui->comboBoxSearcherGender2); });
    connect(ui->comboBoxSearcherSpecie3, &ComboBox::currentIndexChanged, this,
            [=]() { updateGenders(ui->comboBoxSearcherSpecie3, ui->comboBoxSearcherGender3); });
    connect(ui->comboBoxSearcherSpecie4, &ComboBox::currentIndexChanged, this,
            [=]() { updateGenders(ui->comboBoxSearcherSpecie4, ui->comboBoxSearcherGender4); });
    connect(ui->comboBoxSearcherSpecie5, &ComboBox::currentIndexChanged, this,
            [=]() { updateGenders(ui->comboBoxSearcherSpecie5, ui->comboBoxSearcherGender5); });
    connect(ui->comboBoxSearcherSpecie6, &ComboBox::currentIndexChanged, this,
            [=]() { updateGenders(ui->comboBoxSearcherSpecie6, ui->comboBoxSearcherGender6); });

    int size;
    const DreamRadarTemplate *dreamRadarTemplates = Encounters5::getDreamRadarEncounters(&size);
    for (int i = 0; i < size; i++)
    {
        const DreamRadarTemplate *dreamRadarTemplate = &dreamRadarTemplates[i];
        std::string name = Translator::getSpecie(dreamRadarTemplate->getSpecie(), dreamRadarTemplate->getForm());
        if (dreamRadarTemplate->getGenie())
        {
            ui->comboBoxGeneratorSpecie1->addItem(QString::fromStdString(name), i);
            ui->comboBoxSearcherSpecie1->addItem(QString::fromStdString(name), i);
        }
        else
        {
            ui->comboBoxGeneratorSpecie1->addItem(QString::fromStdString(name), i);
            ui->comboBoxGeneratorSpecie2->addItem(QString::fromStdString(name), i);
            ui->comboBoxGeneratorSpecie3->addItem(QString::fromStdString(name), i);
            ui->comboBoxGeneratorSpecie4->addItem(QString::fromStdString(name), i);
            ui->comboBoxGeneratorSpecie5->addItem(QString::fromStdString(name), i);
            ui->comboBoxGeneratorSpecie6->addItem(QString::fromStdString(name), i);

            ui->comboBoxSearcherSpecie1->addItem(QString::fromStdString(name), i);
            ui->comboBoxSearcherSpecie2->addItem(QString::fromStdString(name), i);
            ui->comboBoxSearcherSpecie3->addItem(QString::fromStdString(name), i);
            ui->comboBoxSearcherSpecie4->addItem(QString::fromStdString(name), i);
            ui->comboBoxSearcherSpecie5->addItem(QString::fromStdString(name), i);
            ui->comboBoxSearcherSpecie6->addItem(QString::fromStdString(name), i);
        }
    }

    updateProfiles();

    QSettings setting;
    setting.beginGroup("dreamRadar");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
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

bool DreamRadar::hasProfiles() const
{
    return !profiles.empty();
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

std::vector<DreamRadarTemplate> DreamRadar::getGeneratorSettings() const
{
    std::vector<DreamRadarTemplate> radarTemplates;

    std::array<ComboBox *, 6> species = { ui->comboBoxGeneratorSpecie1, ui->comboBoxGeneratorSpecie2, ui->comboBoxGeneratorSpecie3,
                                          ui->comboBoxGeneratorSpecie4, ui->comboBoxGeneratorSpecie5, ui->comboBoxGeneratorSpecie6 };
    std::array<ComboBox *, 6> genders = { ui->comboBoxGeneratorGender1, ui->comboBoxGeneratorGender2, ui->comboBoxGeneratorGender3,
                                          ui->comboBoxGeneratorGender4, ui->comboBoxGeneratorGender5, ui->comboBoxGeneratorGender6 };

    for (size_t i = 0; i < 6; i++)
    {
        int index = species[i]->getCurrentInt();
        if (index == -1)
        {
            break;
        }

        const DreamRadarTemplate *radarTemplate = Encounters5::getDreamRadarEncounters(index);
        radarTemplates.emplace_back(*radarTemplate);
        radarTemplates[i].setGender(genders[i]->getCurrentUChar());
    }

    return radarTemplates;
}

std::vector<DreamRadarTemplate> DreamRadar::getSearcherSettings() const
{
    std::vector<DreamRadarTemplate> radarTemplates;

    std::array<ComboBox *, 6> species = { ui->comboBoxSearcherSpecie1, ui->comboBoxSearcherSpecie2, ui->comboBoxSearcherSpecie3,
                                          ui->comboBoxSearcherSpecie4, ui->comboBoxSearcherSpecie5, ui->comboBoxSearcherSpecie6 };
    std::array<ComboBox *, 6> genders = { ui->comboBoxSearcherGender1, ui->comboBoxSearcherGender2, ui->comboBoxSearcherGender3,
                                          ui->comboBoxSearcherGender4, ui->comboBoxSearcherGender5, ui->comboBoxSearcherGender6 };

    for (size_t i = 0; i < 6; i++)
    {
        int index = species[i]->getCurrentInt();
        if (index == -1)
        {
            break;
        }

        const DreamRadarTemplate *radarTemplate = Encounters5::getDreamRadarEncounters(index);
        radarTemplates.emplace_back(*radarTemplate);
        radarTemplates[i].setGender(genders[i]->getCurrentUChar());
    }

    return radarTemplates;
}

void DreamRadar::generate()
{
    auto radarTemplates = getGeneratorSettings();
    if (radarTemplates.empty())
    {
        QMessageBox msg(QMessageBox::Warning, tr("Missing settings"), tr("Enter information for at least 1 slot"));
        msg.exec();
        return;
    }

    generatorModel->clearModel();

    u64 seed = ui->textBoxGeneratorSeed->getULong();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();

    auto filter = ui->filterGenerator->getFilter<StateFilter>();
    DreamRadarGenerator generator(initialAdvances, maxAdvances, ui->spinBoxGeneratorBadges->value(), radarTemplates, *currentProfile,
                                  filter);

    auto states = generator.generate(seed);
    generatorModel->addItems(states);
}

void DreamRadar::search()
{
    Date start = ui->dateEditSearcherStartDate->getDate();
    Date end = ui->dateEditSearcherEndDate->getDate();
    if (start > end)
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid date range"), tr("Start date is after end date"));
        msg.exec();
        return;
    }

    auto radarTemplates = getSearcherSettings();
    if (radarTemplates.empty())
    {
        QMessageBox msg(QMessageBox::Warning, tr("Missing settings"), tr("Enter information for at least 1 slot"));
        msg.exec();
        return;
    }

    searcherModel->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u32 initialAdvances = ui->textBoxSearcherInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxSearcherMaxAdvances->getUInt();

    auto filter = ui->filterSearcher->getFilter<StateFilter>();
    DreamRadarGenerator generator(initialAdvances, maxAdvances, ui->spinBoxSearcherBadges->value(), radarTemplates, *currentProfile,
                                  filter);
    auto *searcher = new Searcher5<DreamRadarGenerator, DreamRadarState>(generator, *currentProfile);

    int maxProgress = Keypresses::getKeypresses(*currentProfile).size();
    maxProgress *= start.daysTo(end) + 1;
    maxProgress *= currentProfile->getTimer0Max() - currentProfile->getTimer0Min() + 1;
    ui->progressBar->setRange(0, maxProgress);

    QSettings settings;
    int threads = settings.value("settings/threads").toInt();

    auto *thread = QThread::create([=] { searcher->startSearch(threads, start, end); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    connect(timer, &QTimer::timeout, this, [=] {
        searcherModel->addItems(searcher->getResults());
        ui->progressBar->setValue(searcher->getProgress());
    });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, this, [=] {
        ui->pushButtonSearch->setEnabled(true);
        ui->pushButtonCancel->setEnabled(false);
        searcherModel->addItems(searcher->getResults());
        ui->progressBar->setValue(searcher->getProgress());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
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
        ui->labelProfileGameValue->setText(QString::fromStdString(Translator::getGame(currentProfile->getVersion())));
    }
}

void DreamRadar::profileManager()
{
    auto *manager = new ProfileManager5();
    connect(manager, &ProfileManager5::profilesModified, this, [=](int num) { emit profilesModified(num); });
    manager->show();
}
