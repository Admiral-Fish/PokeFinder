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

#include "Static5.hpp"
#include "ui_Static5.h"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen5/Generators/StaticGenerator5.hpp>
#include <Core/Gen5/Keypresses.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/Searchers/StaticSearcher5.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Parents/States/StaticState.hpp>
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>
#include <Forms/Controls/Controls.hpp>
#include <Forms/Gen5/Profile/ProfileManager5.hpp>
#include <Forms/Models/Gen5/StaticModel5.hpp>
#include <QMenu>
#include <QSettings>
#include <QThread>
#include <QTimer>

Static5::Static5(QWidget *parent) : QWidget(parent), ui(new Ui::Static5)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();
}

Static5::~Static5()
{
    QSettings setting;
    setting.setValue("static5Profile", ui->comboBoxProfiles->currentIndex());

    delete ui;
}

void Static5::updateProfiles()
{
    profiles = ProfileLoader5::getProfiles();

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("Static5Profile").toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val >= 0 ? val : 0);
    }
}

bool Static5::hasProfiles() const
{
    return !profiles.empty();
}

void Static5::setupModels()
{
    generatorModel = new StaticGeneratorModel5(ui->tableViewGenerator, Method::Method5IVs);
    searcherModel = new StaticSearcherModel5(ui->tableViewSearcher, Method::Method5IVs);

    generatorMenu = new QMenu(ui->tableViewGenerator);
    searcherMenu = new QMenu(ui->tableViewSearcher);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed64Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);

    ui->textBoxSearcherMinAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxAdvances->setValues(InputType::Advance32Bit);

    ui->comboBoxGeneratorMethod->setup({ toInt(Method::Method5IVs), toInt(Method::Method5CGear), toInt(Method::Method5) });
    ui->comboBoxSearcherMethod->setup({ toInt(Method::Method5IVs), toInt(Method::Method5CGear), toInt(Method::Method5) });
    connect(ui->comboBoxGeneratorMethod, QOverload<int>::of(&ComboBox::currentIndexChanged), this, &Static5::generatorMethodIndexChanged);
    connect(ui->comboBoxSearcherMethod, QOverload<int>::of(&ComboBox::currentIndexChanged), this, &Static5::searcherMethodIndexChanged);

    ui->comboBoxSearcherEncounter->setup({ toInt(Encounter::Static), toInt(Encounter::Roamer) });

    ui->comboBoxSearcherLead->setup({ toInt(Lead::Search), toInt(Lead::Synchronize), toInt(Lead::CuteCharm), toInt(Lead::None) });

    ui->comboBoxGeneratorLead->addItem(tr("None"));
    for (const std::string &nature : Translator::getNatures())
    {
        ui->comboBoxGeneratorLead->addItem(QString::fromStdString(nature));
    }

    ui->filterGenerator->disableControls(Controls::EncounterSlots | Controls::Ability | Controls::Shiny | Controls::Gender
                                         | Controls::GenderRatio | Controls::Natures);
    ui->filterSearcher->disableControls(Controls::EncounterSlots | Controls::DisableFilter | Controls::UseDelay);

    QAction *outputTXTGenerator = generatorMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVGenerator = generatorMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTGenerator, &QAction::triggered, [=]() { ui->tableViewGenerator->outputModel(false); });
    connect(outputCSVGenerator, &QAction::triggered, [=]() { ui->tableViewGenerator->outputModel(true); });

    QAction *outputTXTSearcher = searcherMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVSearcher = searcherMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTSearcher, &QAction::triggered, [=]() { ui->tableViewSearcher->outputModel(false); });
    connect(outputCSVSearcher, &QAction::triggered, [=]() { ui->tableViewSearcher->outputModel(true); });

    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Static5::profileIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Static5::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Static5::search);
    connect(ui->pushButtonGeneratorLead, &QPushButton::clicked, this, &Static5::generatorLead);
    connect(ui->pushButtonCalculateInitialAdvances, &QPushButton::clicked, this, &Static5::calculateInitialAdvances);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Static5::profileManager);
    connect(ui->tableViewGenerator, &QTableView::customContextMenuRequested, this, &Static5::tableViewGeneratorContextMenu);
    connect(ui->tableViewSearcher, &QTableView::customContextMenuRequested, this, &Static5::tableViewSearcherContextMenu);

    updateProfiles();

    QSettings setting;
    setting.beginGroup("static5");
    if (setting.contains("minAdvance"))
    {
        ui->textBoxSearcherMinAdvances->setText(setting.value("minAdvance").toString());
    }
    if (setting.contains("maxAdvance"))
    {
        ui->textBoxSearcherMaxAdvances->setText(setting.value("maxAdvance").toString());
    }
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

void Static5::generate()
{
    generatorModel->clearModel();
    auto method = static_cast<Method>(ui->comboBoxGeneratorMethod->getCurrentInt());
    generatorModel->setMethod(method);

    u64 seed = ui->textBoxGeneratorSeed->getULong();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u16 tid = currentProfile->getTID();
    u16 sid = currentProfile->getSID();
    u8 gender = ui->filterGenerator->getGender();
    u8 genderRatio = ui->filterGenerator->getGenderRatio();
    u32 offset = 0;
    auto encounter = static_cast<Encounter>(ui->comboBoxGeneratorEncounter->getCurrentInt());
    if (ui->filterGenerator->useDelay())
    {
        offset = ui->filterGenerator->getDelay();
    }

    StateFilter filter(ui->filterGenerator->getGender(), ui->filterGenerator->getAbility(), ui->filterGenerator->getShiny(),
                       ui->filterGenerator->getDisableFilters(), ui->filterGenerator->getMinIVs(), ui->filterGenerator->getMaxIVs(),
                       ui->filterGenerator->getNatures(), ui->filterGenerator->getHiddenPowers(), {});

    StaticGenerator5 generator(initialAdvances, maxAdvances, tid, sid, gender, genderRatio, method, encounter, filter);
    generator.setOffset(offset);

    if (method == Method::Method5IVs || method == Method::Method5CGear)
    {
        generator.setInitialAdvances(ui->textBoxSearcherMinAdvances->getUInt());

        // temp fix for IV and seed matchup?
        generator.setOffset(offset + (currentProfile->getVersion() == Game::Black || currentProfile->getVersion() == Game::White ? 0 : 2));
    }

    if (ui->pushButtonGeneratorLead->text() == tr("Cute Charm"))
    {
        generator.setLead(static_cast<Lead>(ui->comboBoxGeneratorLead->currentData().toInt()));
    }
    else
    {
        int num = ui->comboBoxGeneratorLead->currentIndex();
        if (num == 0)
        {
            generator.setLead(Lead::None);
        }
        else
        {
            generator.setLead(Lead::Synchronize);
            generator.setSynchNature(static_cast<u8>(ui->comboBoxGeneratorLead->currentIndex() - 1));
        }
    }

    auto states = generator.generate(seed);
    generatorModel->addItems(states);
}

void Static5::search()
{
    searcherModel->clearModel();
    auto method = static_cast<Method>(ui->comboBoxSearcherMethod->getCurrentInt());
    searcherModel->setMethod(method);

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u32 maxAdvances = ui->textBoxSearcherMaxAdvances->getUInt();
    u16 tid = currentProfile->getTID();
    u16 sid = currentProfile->getSID();
    u8 gender = ui->filterSearcher->getGender();
    u8 genderRatio = ui->filterSearcher->getGenderRatio();
    auto encounter = static_cast<Encounter>(ui->comboBoxSearcherEncounter->getCurrentInt());

    StateFilter filter(ui->filterSearcher->getGender(), ui->filterSearcher->getAbility(), ui->filterSearcher->getShiny(),
                       ui->filterSearcher->getDisableFilters(), ui->filterSearcher->getMinIVs(), ui->filterSearcher->getMaxIVs(),
                       ui->filterSearcher->getNatures(), ui->filterSearcher->getHiddenPowers(), {});

    StaticGenerator5 generator(0, maxAdvances, tid, sid, gender, genderRatio, method, encounter, filter);

    if (method == Method::Method5IVs || method == Method::Method5CGear)
    {
        generator.setInitialAdvances(ui->textBoxSearcherMinAdvances->getUInt());
    }

    auto *searcher = new StaticSearcher5(*currentProfile, method);

    Date start = ui->dateEditSearcherStartDate->getDate();
    Date end = ui->dateEditSearcherEndDate->getDate();

    int maxProgress = Keypresses::getKeyPresses(currentProfile->getKeypresses(), currentProfile->getSkipLR()).size();
    maxProgress *= start.daysTo(end) + 1;
    maxProgress *= (currentProfile->getTimer0Max() - currentProfile->getTimer0Min() + 1);
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

void Static5::profileIndexChanged(int index)
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

        generatorMethodIndexChanged(0);
        searcherMethodIndexChanged(0);
    }
}

void Static5::generatorLead()
{
    QString text = ui->pushButtonGeneratorLead->text();
    ui->comboBoxGeneratorLead->clear();
    if (text == tr("Synchronize"))
    {
        ui->pushButtonGeneratorLead->setText(tr("Cute Charm"));
        ui->comboBoxGeneratorLead->setEnabled(true);

        ui->comboBoxGeneratorLead->addItem(tr("♂ Lead"), toInt(Lead::CuteCharmFemale));
        ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (50% ♂ Target)"), toInt(Lead::CuteCharm50M));
        ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (75% ♂ Target)"), toInt(Lead::CuteCharm75M));
        ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (25% ♂ Target)"), toInt(Lead::CuteCharm25M));
        ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (87.5% ♂ Target)"), toInt(Lead::CuteCharm875M));
    }
    else if (text == tr("Cute Charm"))
    {
        ui->pushButtonGeneratorLead->setText(tr("Suction Cups"));
        ui->comboBoxGeneratorLead->setEnabled(false);
    }
    else
    {
        ui->pushButtonGeneratorLead->setText(tr("Synchronize"));
        ui->comboBoxGeneratorLead->setEnabled(true);

        ui->comboBoxGeneratorLead->addItem("None");
        for (const std::string &nature : Translator::getNatures())
        {
            ui->comboBoxGeneratorLead->addItem(QString::fromStdString(nature));
        }
    }
}

void Static5::calculateInitialAdvances()
{
    Game version = currentProfile->getVersion();

    u8 initialAdvances;
    if ((version & Game::BW) != Game::None)
    {
        initialAdvances = Utilities::initialAdvancesBW(ui->textBoxGeneratorSeed->getULong());
    }
    else
    {
        initialAdvances = Utilities::initialAdvancesBW2(ui->textBoxGeneratorSeed->getULong(), currentProfile->getMemoryLink());
    }

    ui->textBoxGeneratorInitialAdvances->setText(QString::number(initialAdvances));
}

void Static5::generatorMethodIndexChanged(int index)
{
    Method method = static_cast<Method>(ui->comboBoxGeneratorMethod->getCurrentByte());

    ui->comboBoxGeneratorEncounter->clear();
    if (currentProfile->getVersion() == Game::Black2 || currentProfile->getVersion() == Game::White2)
    {
        switch (method)
        {
        case Method::Method5:
        {
            ui->comboBoxGeneratorEncounter->addItems({ tr("Static"), tr("Roamer"), tr("Hidden Grotto") });
            ui->comboBoxGeneratorEncounter->setup({ toInt(Encounter::Static), toInt(Encounter::Roamer), toInt(Encounter::HiddenGrotto) });

            ui->pushButtonCalculateInitialAdvances->setVisible(true);

            ui->filterGenerator->disableControls(Controls::IVs | Controls::HiddenPowers);
            ui->filterGenerator->enableControls(Controls::Ability | Controls::Shiny | Controls::Gender | Controls::GenderRatio
                                                | Controls::Natures);
            break;
        }
        case Method::Method5IVs:
        case Method::Method5CGear:
        default:
        {
            ui->comboBoxGeneratorEncounter->addItems({ tr("Static"), tr("Roamer") });
            ui->comboBoxGeneratorEncounter->setup({ toInt(Encounter::Static), toInt(Encounter::Roamer) });

            ui->pushButtonCalculateInitialAdvances->setVisible(false);

            ui->filterGenerator->enableControls(Controls::IVs | Controls::HiddenPowers);
            ui->filterGenerator->disableControls(Controls::Ability | Controls::Shiny | Controls::Gender | Controls::GenderRatio
                                                 | Controls::Natures);
        }
        }
    }
    else
    {
        switch (method)
        {
        case Method::Method5:
        {
            ui->comboBoxGeneratorEncounter->addItems({ tr("Static"), tr("Roamer") });
            ui->comboBoxGeneratorEncounter->setup({ toInt(Encounter::Static), toInt(Encounter::Roamer) });

            ui->pushButtonCalculateInitialAdvances->setVisible(true);

            ui->filterGenerator->disableControls(Controls::IVs | Controls::HiddenPowers);
            ui->filterGenerator->enableControls(Controls::Ability | Controls::Shiny | Controls::Gender | Controls::GenderRatio
                                                | Controls::Natures);
            break;
        }
        case Method::Method5IVs:
        case Method::Method5CGear:
        default:
        {
            ui->comboBoxGeneratorEncounter->addItems({ tr("Static"), tr("Roamer") });
            ui->comboBoxGeneratorEncounter->setup({ toInt(Encounter::Static), toInt(Encounter::Roamer) });

            ui->pushButtonCalculateInitialAdvances->setVisible(false);

            ui->filterGenerator->enableControls(Controls::IVs | Controls::HiddenPowers);
            ui->filterGenerator->disableControls(Controls::Ability | Controls::Shiny | Controls::Gender | Controls::GenderRatio
                                                 | Controls::Natures);
        }
        }
    }
}

void Static5::searcherMethodIndexChanged(int index)
{
    Method method = static_cast<Method>(ui->comboBoxSearcherMethod->getCurrentByte());
    ui->comboBoxSearcherEncounter->clear();
    if (currentProfile->getVersion() == Game::Black2 || currentProfile->getVersion() == Game::White2)
    {
        switch (method)
        {
        case Method::Method5:
        {
            ui->comboBoxSearcherEncounter->addItems({ tr("Static"), tr("Roamer"), tr("Hidden Grotto") });
            ui->comboBoxSearcherEncounter->setup({ toInt(Encounter::Static), toInt(Encounter::Roamer), toInt(Encounter::HiddenGrotto) });

            ui->filterSearcher->disableControls(Controls::IVs | Controls::HiddenPowers);
            ui->filterSearcher->enableControls(Controls::Ability | Controls::Shiny | Controls::Gender | Controls::GenderRatio
                                               | Controls::Natures);
            break;
        }
        case Method::Method5IVs:
        case Method::Method5CGear:
        default:
        {
            ui->comboBoxSearcherEncounter->addItems({ tr("Static"), tr("Roamer") });
            ui->comboBoxSearcherEncounter->setup({ toInt(Encounter::Static), toInt(Encounter::Roamer) });

            ui->filterSearcher->enableControls(Controls::IVs | Controls::HiddenPowers);
            ui->filterSearcher->disableControls(Controls::Ability | Controls::Shiny | Controls::Gender | Controls::GenderRatio
                                                | Controls::Natures);
        }
        }
    }
    else
    {
        switch (method)
        {
        case Method::Method5:
        {
            ui->comboBoxSearcherEncounter->addItems({ tr("Static"), tr("Roamer") });
            ui->comboBoxSearcherEncounter->setup({ toInt(Encounter::Static), toInt(Encounter::Roamer), toInt(Encounter::HiddenGrotto) });

            ui->filterSearcher->disableControls(Controls::IVs | Controls::HiddenPowers);
            ui->filterSearcher->enableControls(Controls::Ability | Controls::Shiny | Controls::Gender | Controls::GenderRatio
                                               | Controls::Natures);
            break;
        }
        case Method::Method5IVs:
        case Method::Method5CGear:
        default:
        {
            ui->comboBoxSearcherEncounter->addItems({ tr("Static"), tr("Roamer") });
            ui->comboBoxSearcherEncounter->setup({ toInt(Encounter::Static), toInt(Encounter::Roamer) });

            ui->filterSearcher->enableControls(Controls::IVs | Controls::HiddenPowers);
            ui->filterSearcher->disableControls(Controls::Ability | Controls::Shiny | Controls::Gender | Controls::GenderRatio
                                                | Controls::Natures);
        }
        }
    }
}

void Static5::tableViewGeneratorContextMenu(const QPoint &pos)
{
    if (generatorModel->rowCount() > 0)
    {
        generatorMenu->popup(ui->tableViewGenerator->viewport()->mapToGlobal(pos));
    }
}

void Static5::tableViewSearcherContextMenu(const QPoint &pos)
{
    if (searcherModel->rowCount() == 0)
    {
        searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
    }
}

void Static5::profileManager()
{
    auto *manager = new ProfileManager5();
    connect(manager, &ProfileManager5::updateProfiles, this, [=] { emit alertProfiles(5); });
    manager->show();
}
