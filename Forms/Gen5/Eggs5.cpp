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

#include "Eggs5.hpp"
#include "ui_Eggs5.h"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen5/Generators/EggGenerator5.hpp>
#include <Core/Gen5/Keypresses.hpp>
#include <Core/Gen5/ProfileLoader5.hpp>
#include <Core/Gen5/Searchers/EggSearcher5.hpp>
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>
#include <Forms/Gen5/Profile/ProfileManager5.hpp>
#include <Models/Gen5/EggModel5.hpp>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>

Eggs5::Eggs5(QWidget *parent) : QWidget(parent), ui(new Ui::Eggs5)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();
}

Eggs5::~Eggs5()
{
    QSettings setting;
    setting.beginGroup("event5");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Eggs5::updateProfiles()
{
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Eggs5::profileIndexChanged);

    profiles = ProfileLoader5::getProfiles();

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(profile.getName());
    }

    QSettings setting;
    int val = setting.value("event5/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

bool Eggs5::hasProfiles() const
{
    return !profiles.isEmpty();
}

void Eggs5::setupModels()
{
    generatorModel = new EggGeneratorModel5(ui->tableViewGenerator);
    searcherModel = new EggSearcherModel5(ui->tableViewSearcher);
    generatorMenu = new QMenu(ui->tableViewGenerator);
    searcherMenu = new QMenu(ui->tableViewSearcher);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed64Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);

    ui->textBoxSearcherMaxAdvances->setValues(InputType::Advance32Bit);

    ui->filterGenerator->disableControls(Controls::EncounterSlots);
    ui->filterSearcher->disableControls(Controls::EncounterSlots | Controls::DisableFilter | Controls::UseDelay);

    ui->eggSettingsGenerator->setup(5);
    ui->eggSettingsSearcher->setup(5);

    QAction *outputTXTGenerator = generatorMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVGenerator = generatorMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTGenerator, &QAction::triggered, [=]() { ui->tableViewGenerator->outputModel(false); });
    connect(outputCSVGenerator, &QAction::triggered, [=]() { ui->tableViewGenerator->outputModel(true); });

    QAction *outputTXTSearcher = searcherMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVSearcher = searcherMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTSearcher, &QAction::triggered, [=]() { ui->tableViewSearcher->outputModel(false); });
    connect(outputCSVSearcher, &QAction::triggered, [=]() { ui->tableViewSearcher->outputModel(true); });

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Eggs5::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Eggs5::search);
    connect(ui->pushButtonCalculateInitialAdvances, &QPushButton::clicked, this, &Eggs5::calculateInitialAdvances);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Eggs5::profileManager);
    connect(ui->tableViewGenerator, &QTableView::customContextMenuRequested, this, &Eggs5::tableViewGeneratorContextMenu);
    connect(ui->tableViewSearcher, &QTableView::customContextMenuRequested, this, &Eggs5::tableViewSearcherContextMenu);

    QSettings setting;
    setting.beginGroup("event5");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

void Eggs5::updateProgress(const QVector<SearcherState5<EggState>> &states, int progress)
{
    searcherModel->addItems(states);
    ui->progressBar->setValue(progress);
}

void Eggs5::generate()
{
    if (!ui->eggSettingsGenerator->compatibleParents())
    {
        QMessageBox box(QMessageBox::Warning, "Incompatible Parents", "Gender of selected parents are not compatible for breeding");
        box.exec();
        return;
    }
    else if (ui->eggSettingsGenerator->reorderParents())
    {
        QMessageBox box(QMessageBox::Information, "Parents Reordered", "Parent were swapped to match the game");
        box.exec();
    }

    generatorModel->clearModel();
    Method method = (currentProfile.getVersion() & Game::BW) ? Method::BWBred : Method::BW2Bred;

    u64 seed = ui->textBoxGeneratorSeed->getULong();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterGenerator->getGenderRatio();
    u32 offset = 0;
    if (ui->filterGenerator->useDelay())
    {
        offset = ui->filterGenerator->getDelay();
    }

    StateFilter filter(ui->filterGenerator->getGender(), ui->filterGenerator->getAbility(), ui->filterGenerator->getShiny(),
                       ui->filterGenerator->getDisableFilters(), ui->filterGenerator->getMinIVs(), ui->filterGenerator->getMaxIVs(),
                       ui->filterGenerator->getNatures(), ui->filterGenerator->getHiddenPowers(), {});

    EggGenerator5 generator(initialAdvances, maxAdvances, tid, sid, genderRatio, method, filter,
                            ui->eggSettingsGenerator->getDaycareSettings(), currentProfile.getShinyCharm());
    generator.setOffset(offset);

    auto states = generator.generate(seed);
    generatorModel->addItems(states);
}

void Eggs5::search()
{
    if (!ui->eggSettingsSearcher->compatibleParents())
    {
        QMessageBox box(QMessageBox::Warning, "Incompatible Parents", "Gender of selected parents are not compatible for breeding");
        box.exec();
        return;
    }
    else if (ui->eggSettingsSearcher->reorderParents())
    {
        QMessageBox box(QMessageBox::Information, "Parents Reordered", "Parent were swapped to match the game");
        box.exec();
    }

    searcherModel->clearModel();
    Method method = (currentProfile.getVersion() & Game::BW) ? Method::BWBred : Method::BW2Bred;

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u32 maxAdvances = ui->textBoxSearcherMaxAdvances->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterSearcher->getGenderRatio();

    StateFilter filter(ui->filterSearcher->getGender(), ui->filterSearcher->getAbility(), ui->filterSearcher->getShiny(),
                       ui->filterSearcher->getDisableFilters(), ui->filterSearcher->getMinIVs(), ui->filterSearcher->getMaxIVs(),
                       ui->filterSearcher->getNatures(), ui->filterSearcher->getHiddenPowers(), {});

    EggGenerator5 generator(0, maxAdvances, tid, sid, genderRatio, method, filter, ui->eggSettingsSearcher->getDaycareSettings(),
                            currentProfile.getShinyCharm());
    generator.setOffset(0);

    EggSearcher5 *searcher = new EggSearcher5(generator, currentProfile);

    QDate start = ui->dateEditSearcherStartDate->date();
    QDate end = ui->dateEditSearcherEndDate->date();

    int maxProgress = Keypresses::getKeyPresses(currentProfile.getKeypresses(), currentProfile.getSkipLR()).size();
    maxProgress *= 86400 * (start.daysTo(end) + 1);
    maxProgress *= (currentProfile.getTimer0Max() - currentProfile.getTimer0Min() + 1);
    ui->progressBar->setRange(0, maxProgress);

    QSettings settings;
    int threads = settings.value("settings/threads", QThread::idealThreadCount()).toInt();

    auto *thread = QThread::create([=] { searcher->startSearch(threads, start, end); });
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

void Eggs5::calculateInitialAdvances()
{
    Game version = currentProfile.getVersion();

    u8 initialAdvances;
    if (version & Game::BW)
    {
        initialAdvances = Utilities::initialAdvancesBW(ui->textBoxGeneratorSeed->getULong());
    }
    else
    {
        initialAdvances = Utilities::initialAdvancesBW2(ui->textBoxGeneratorSeed->getULong(), currentProfile.getMemoryLink());
    }

    ui->textBoxGeneratorInitialAdvances->setText(QString::number(initialAdvances));
}

void Eggs5::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = profiles.at(index);

        ui->labelProfileTIDValue->setText(QString::number(currentProfile.getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile.getSID()));
        ui->labelProfileMACAddressValue->setText(QString::number(currentProfile.getMac(), 16));
        ui->labelProfileDSTypeValue->setText(currentProfile.getDSTypeString());
        ui->labelProfileVCountValue->setText(QString::number(currentProfile.getVCount(), 16));
        ui->labelProfileTimer0Value->setText(QString::number(currentProfile.getTimer0Min(), 16) + "-"
                                             + QString::number(currentProfile.getTimer0Max(), 16));
        ui->labelProfileGxStatValue->setText(QString::number(currentProfile.getGxStat()));
        ui->labelProfileVFrameValue->setText(QString::number(currentProfile.getVFrame()));
        ui->labelProfileKeypressesValue->setText(currentProfile.getKeypressesString());
        ui->labelProfileGameValue->setText(currentProfile.getVersionString());
    }
}

void Eggs5::tableViewGeneratorContextMenu(QPoint pos)
{
    if (generatorModel->rowCount() > 0)
    {
        generatorMenu->popup(ui->tableViewGenerator->viewport()->mapToGlobal(pos));
    }
}

void Eggs5::tableViewSearcherContextMenu(QPoint pos)
{
    if (searcherModel->rowCount() == 0)
    {
        searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
    }
}

void Eggs5::profileManager()
{
    auto *manager = new ProfileManager5();
    connect(manager, &ProfileManager5::updateProfiles, this, [=] { emit alertProfiles(5); });
    manager->show();
}
