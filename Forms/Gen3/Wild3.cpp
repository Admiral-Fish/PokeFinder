/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Wild3.hpp"
#include "ui_Wild3.h"

Wild3::Wild3(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Wild3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();

    qRegisterMetaType<QVector<Frame3>>("QVector<Frame3>");
}

Wild3::~Wild3()
{
    QSettings setting;
    setting.setValue("wild3Profile", ui->comboBoxProfiles->currentIndex());

    delete ui;
}

void Wild3::updateProfiles()
{
    profiles = Profile3::loadProfileList();

    QVector<Profile3> temp;

    for (const auto &profile : profiles)
    {
        if (!(profile.getVersion() & Game::GC))
        {
            temp.append(profile);
        }
    }

    profiles = temp;
    profiles.insert(profiles.begin(), Profile3());

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(profile.getProfileName());
    }

    QSettings setting;
    int val = setting.value("wild3Profile").toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val >= 0 ? val : 0);
    }
}

void Wild3::setupModels()
{
    generatorModel = new Wild3Model(ui->tableViewGenerator);
    searcherModel = new Searcher3Model(ui->tableViewSearcher, Method::Method1);
    generatorMenu = new QMenu(ui->tableViewGenerator);
    searcherMenu = new QMenu(ui->tableViewSearcher);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed32Bit);
    ui->textBoxGeneratorTID->setValues(InputType::TIDSID);
    ui->textBoxGeneratorSID->setValues(InputType::TIDSID);
    ui->textBoxGeneratorStartingFrame->setValues(InputType::Frame32Bit);
    ui->textBoxGeneratorMaxResults->setValues(InputType::Frame32Bit);
    ui->textBoxGeneratorDelay->setValues(InputType::Frame32Bit);

    ui->textBoxSearcherTID->setValues(InputType::TIDSID);
    ui->textBoxSearcherSID->setValues(InputType::TIDSID);

    ui->comboBoxGeneratorMethod->setItemData(0, Method::MethodH1);
    ui->comboBoxGeneratorMethod->setItemData(1, Method::MethodH2);
    ui->comboBoxGeneratorMethod->setItemData(2, Method::MethodH4);

    ui->comboBoxSearcherMethod->setItemData(0, Method:: MethodH1);
    ui->comboBoxSearcherMethod->setItemData(1, Method::MethodH2);
    ui->comboBoxSearcherMethod->setItemData(2, Method::MethodH4);

    ui->comboBoxGeneratorEncounter->setItemData(0, Encounter::Grass);
    ui->comboBoxGeneratorEncounter->setItemData(1, Encounter::SafariZone);
    ui->comboBoxGeneratorEncounter->setItemData(2, Encounter::RockSmash);
    ui->comboBoxGeneratorEncounter->setItemData(3, Encounter::Surfing);
    ui->comboBoxGeneratorEncounter->setItemData(4, Encounter::OldRod);
    ui->comboBoxGeneratorEncounter->setItemData(5, Encounter::GoodRod);
    ui->comboBoxGeneratorEncounter->setItemData(6, Encounter::SuperRod);

    ui->comboBoxSearcherEncounter->setItemData(0, Encounter::Grass);
    ui->comboBoxSearcherEncounter->setItemData(1, Encounter::SafariZone);
    ui->comboBoxSearcherEncounter->setItemData(2, Encounter::RockSmash);
    ui->comboBoxSearcherEncounter->setItemData(3, Encounter::Surfing);
    ui->comboBoxSearcherEncounter->setItemData(4, Encounter::OldRod);
    ui->comboBoxSearcherEncounter->setItemData(5, Encounter::GoodRod);
    ui->comboBoxSearcherEncounter->setItemData(6, Encounter::SuperRod);

    ui->comboBoxGeneratorLead->addItem(tr("None"));
    ui->comboBoxGeneratorLead->addItems(Nature::getNatures());

    ui->comboBoxSearcherLead->setItemData(0, Lead::Search);
    ui->comboBoxSearcherLead->setItemData(1, Lead::Synchronize);
    ui->comboBoxSearcherLead->setItemData(2, Lead::CuteCharm);
    ui->comboBoxSearcherLead->setItemData(3, Lead::None);

    ui->comboBoxGeneratorNature->setup(Nature::getNatures());
    ui->comboBoxSearcherNature->setup(Nature::getNatures());

    ui->comboBoxGeneratorHiddenPower->setup(Power::getPowers());
    ui->comboBoxSearcherHiddenPower->setup(Power::getPowers());

    on_comboBoxGeneratorEncounter_currentIndexChanged(0);
    on_comboBoxSearcherEncounter_currentIndexChanged(0);

    QAction *setTargetFrame = generatorMenu->addAction(tr("Set Target Frame"));
    QAction *jumpToTarget = generatorMenu->addAction(tr("Jump to Target Frame"));
    QAction *center1Second = generatorMenu->addAction(tr("Center to +/- 1 Second and Set as Target Frame"));
    QAction *center2Seconds = generatorMenu->addAction(tr("Center to +/- 2 Seconds and Set as Target Frame"));
    QAction *center3Seconds = generatorMenu->addAction(tr("Center to +/- 3 Seconds and Set as Target Frame"));
    QAction *center5Seconds = generatorMenu->addAction(tr("Center to +/- 5 Seconds and Set as Target Frame"));
    QAction *center10Seconds = generatorMenu->addAction(tr("Center to +/- 10 Seconds and Set as Target Frame"));
    QAction *center1Minute = generatorMenu->addAction(tr("Center to +/- 1 Minute and Set as Target Frame"));
    QAction *outputTXTGenerator = generatorMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVGenerator = generatorMenu->addAction(tr("Output Results to CSV"));

    connect(setTargetFrame, &QAction::triggered, this, &Wild3::setTargetFrameGenerator);
    connect(jumpToTarget, &QAction::triggered, this, &Wild3::jumpToTargetGenerator);
    connect(center1Second, &QAction::triggered, this, [ = ]() { centerFramesAndSetTargetGenerator(60);  });
    connect(center2Seconds, &QAction::triggered, this, [ = ]() { centerFramesAndSetTargetGenerator(120); });
    connect(center3Seconds, &QAction::triggered, this, [ = ]() { centerFramesAndSetTargetGenerator(180); });
    connect(center5Seconds, &QAction::triggered, this, [ = ]() { centerFramesAndSetTargetGenerator(300); });
    connect(center10Seconds, &QAction::triggered, this, [ = ]() { centerFramesAndSetTargetGenerator(600); });
    connect(center1Minute, &QAction::triggered, this, [ = ]() { centerFramesAndSetTargetGenerator(3600); });
    connect(outputTXTGenerator, &QAction::triggered, this, [ = ]() { ui->tableViewGenerator->outputModelTXT(); });
    connect(outputCSVGenerator, &QAction::triggered, this, [ = ]() { ui->tableViewGenerator->outputModelCSV(); });

    QAction *copySeedToClipboard = searcherMenu->addAction(tr("Copy Seed to Clipboard"));
    QAction *seedToTime = searcherMenu->addAction(tr("Generate times for seed"));
    QAction *outputTXTSearcher = searcherMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVSearcher = searcherMenu->addAction(tr("Output Results to CSV"));

    connect(copySeedToClipboard, &QAction::triggered, this, &Wild3::copySeedToClipboard);
    connect(seedToTime, &QAction::triggered, this, &Wild3::seedToTime);
    connect(outputTXTSearcher, &QAction::triggered, [ = ]() { ui->tableViewSearcher->outputModelTXT(); });
    connect(outputCSVSearcher, &QAction::triggered, [ = ]() { ui->tableViewSearcher->outputModelCSV(); });
}

void Wild3::updateView(const QVector<Frame3> &frames, int progress)
{
    searcherModel->addItems(frames);
    ui->progressBar->setValue(progress);
}

void Wild3::updateLocationsGenerator()
{
    Encounter encounter = static_cast<Encounter>(ui->comboBoxGeneratorEncounter->currentData().toInt());
    auto profile = profiles[ui->comboBoxProfiles->currentIndex()];

    encounterGenerator = Encounters3(encounter, profile).getEncounters();
    QVector<u8> locs;
    for (const auto &area : encounterGenerator)
    {
        locs.append(area.getLocation());
    }

    QStringList locations = Translator::getLocationsGen3(locs, profile.getVersion());

    ui->comboBoxGeneratorLocation->clear();
    ui->comboBoxGeneratorLocation->addItems(locations);
}

void Wild3::updateLocationsSearcher()
{
    Encounter encounter = static_cast<Encounter>(ui->comboBoxSearcherEncounter->currentData().toInt());
    auto profile = profiles[ui->comboBoxProfiles->currentIndex()];

    encounterSearcher = Encounters3(encounter, profile).getEncounters();
    QVector<u8> locs;
    for (const auto &area : encounterSearcher)
    {
        locs.append(area.getLocation());
    }

    QStringList locations = Translator::getLocationsGen3(locs, profile.getVersion());

    ui->comboBoxSearcherLocation->clear();
    ui->comboBoxSearcherLocation->addItems(locations);
}

void Wild3::updatePokemonGenerator()
{
    if (ui->comboBoxGeneratorLocation->currentIndex() < 0)
    {
        return;
    }

    auto area = encounterGenerator[ui->comboBoxGeneratorLocation->currentIndex()];
    QVector<u16> species = area.getUniqueSpecies();

    QStringList names = area.getSpecieNames();

    ui->comboBoxGeneratorPokemon->clear();
    ui->comboBoxGeneratorPokemon->addItem("-");
    for (int i = 0; i < species.size(); i++)
    {
        ui->comboBoxGeneratorPokemon->addItem(names[i], species[i]);
    }
}

void Wild3::updatePokemonSearcher()
{
    if (ui->comboBoxSearcherLocation->currentIndex() < 0)
    {
        return;
    }

    auto area = encounterSearcher[ui->comboBoxSearcherLocation->currentIndex()];
    QVector<u16> species = area.getUniqueSpecies();

    QStringList names = area.getSpecieNames();

    ui->comboBoxSearcherPokemon->clear();
    ui->comboBoxSearcherPokemon->addItem("-");
    for (int i = 0; i < species.size(); i++)
    {
        ui->comboBoxSearcherPokemon->addItem(names[i], species[i]);
    }
}

void Wild3::refreshProfiles()
{
    emit alertProfiles(3);
}

void Wild3::on_comboBoxProfiles_currentIndexChanged(int index)
{
    if (index < 0)
    {
        return;
    }

    auto profile = profiles[index];
    QString tid = QString::number(profile.getTID());
    QString sid = QString::number(profile.getSID());

    ui->textBoxGeneratorTID->setText(tid);
    ui->textBoxGeneratorSID->setText(sid);
    ui->textBoxSearcherTID->setText(tid);
    ui->textBoxSearcherSID->setText(sid);
    ui->labelProfileTIDValue->setText(tid);
    ui->labelProfileSIDValue->setText(sid);
    ui->labelProfileGameValue->setText(profile.getVersionString());

    updateLocationsSearcher();
    updateLocationsGenerator();
}

void Wild3::on_pushButtonGenerate_clicked()
{
    generatorModel->clear();

    u32 seed = ui->textBoxGeneratorSeed->getUInt();
    u32 startingFrame = ui->textBoxGeneratorStartingFrame->getUInt();
    u32 maxResults = ui->textBoxGeneratorMaxResults->getUInt();
    u16 tid = ui->textBoxGeneratorTID->getUShort();
    u16 sid = ui->textBoxGeneratorSID->getUShort();
    u32 offset = 0;
    if (ui->checkBoxGeneratorDelay->isChecked())
    {
        offset = ui->textBoxGeneratorDelay->getUInt();
    }

    int genderRatioIndex = ui->comboBoxGeneratorGenderRatio->currentIndex();
    Generator3 generator = Generator3(maxResults, startingFrame, seed, tid, sid, offset);
    FrameCompare compare = FrameCompare(ui->ivFilterGenerator->getLower(), ui->ivFilterGenerator->getUpper(),
                                        ui->comboBoxGeneratorGender->currentIndex(), genderRatioIndex, ui->comboBoxGeneratorAbility->currentIndex(),
                                        ui->comboBoxGeneratorNature->getChecked(), ui->comboBoxGeneratorHiddenPower->getChecked(),
                                        ui->checkBoxGeneratorShinyOnly->isChecked(), ui->checkBoxGeneratorDisableFilters->isChecked(), ui->comboBoxGeneratorEncounterSlot->getChecked());

    generator.setup(static_cast<Method>(ui->comboBoxGeneratorMethod->currentData().toInt()));
    generator.setEncounterType(static_cast<Encounter>(ui->comboBoxGeneratorEncounter->currentData().toInt()));
    if (ui->pushButtonGeneratorLead->text() == tr("Cute Charm"))
    {
        generator.setLeadType(static_cast<Lead>(ui->comboBoxGeneratorLead->currentData().toInt()));
    }
    else
    {
        if (ui->comboBoxGeneratorLead->currentIndex() == 0)
        {
            generator.setLeadType(Lead::None);
        }
        else
        {
            generator.setLeadType(Lead::Synchronize);
            generator.setSynchNature(Nature::getAdjustedNature(static_cast<u32>(ui->comboBoxGeneratorLead->currentIndex() - 1)));
        }
    }
    generator.setEncounter(encounterGenerator[ui->comboBoxGeneratorLocation->currentIndex()]);

    QVector<Frame3> frames = generator.generate(compare);
    generatorModel->setModel(frames);
}

void Wild3::on_pushButtonSearch_clicked()
{
    searcherModel->clear();
    searcherModel->setMethod(static_cast<Method>(ui->comboBoxGeneratorMethod->currentData().toInt()));

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u16 tid = ui->textBoxSearcherTID->getUShort();
    u16 sid = ui->textBoxSearcherSID->getUShort();

    int genderRatioIndex = ui->comboBoxSearcherGenderRatio->currentIndex();
    FrameCompare compare = FrameCompare(ui->ivFilterSearcher->getLower(), ui->ivFilterSearcher->getUpper(), ui->comboBoxSearcherGender->currentIndex(),
                                        genderRatioIndex, ui->comboBoxSearcherAbility->currentIndex(), ui->comboBoxSearcherNature->getChecked(),
                                        ui->comboBoxSearcherHiddenPower->getChecked(), ui->checkBoxSearcherShinyOnly->isChecked(), false,
                                        ui->comboBoxSearcherEncounterSlot->getChecked());
    Searcher3 searcher = Searcher3(tid, sid, static_cast<u32>(genderRatioIndex), compare);

    searcher.setup(static_cast<Method>(ui->comboBoxSearcherMethod->currentData().toInt()));
    searcher.setEncounterType(static_cast<Encounter>(ui->comboBoxSearcherEncounter->currentData().toInt()));
    searcher.setLeadType(static_cast<Lead>(ui->comboBoxSearcherLead->currentData().toInt()));
    searcher.setEncounter(encounterSearcher[ui->comboBoxSearcherLocation->currentIndex()]);

    QVector<u8> min = ui->ivFilterSearcher->getLower();
    QVector<u8> max = ui->ivFilterSearcher->getUpper();

    int maxProgress = 1;
    for (int i = 0; i < 6; i++)
    {
        maxProgress *= max[i] - min[i] + 1;
    }

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(maxProgress);

    auto *search = new IVSearcher3(searcher, min, max);
    auto *timer = new QTimer(search);

    connect(search, &IVSearcher3::finished, timer, &QTimer::stop);
    connect(search, &IVSearcher3::finished, this, [ = ] { ui->pushButtonSearch->setEnabled(true); ui->pushButtonCancel->setEnabled(false); });
    connect(search, &IVSearcher3::finished, this, [ = ] { updateView(search->getResults(), search->currentProgress()); });
    connect(timer, &QTimer::timeout, this, [ = ] { updateView(search->getResults(), search->currentProgress()); });
    connect(ui->pushButtonCancel, &QPushButton::clicked, search, &IVSearcher3::cancelSearch);

    search->start();
    timer->start(1000);
}

void Wild3::on_tableViewGenerator_customContextMenuRequested(const QPoint &pos)
{
    if (generatorModel->rowCount() == 0)
    {
        return;
    }

    lastIndex = ui->tableViewGenerator->indexAt(pos);
    generatorMenu->popup(ui->tableViewGenerator->viewport()->mapToGlobal(pos));
}

void Wild3::on_tableViewSearcher_customContextMenuRequested(const QPoint &pos)
{
    if (searcherModel->rowCount() == 0)
    {
        return;
    }

    lastIndex = ui->tableViewSearcher->indexAt(pos);
    searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
}

void Wild3::setTargetFrameGenerator()
{
    targetFrame = lastIndex;
}

void Wild3::jumpToTargetGenerator()
{
    ui->tableViewGenerator->scrollTo(targetFrame, QAbstractItemView::PositionAtTop);
    ui->tableViewGenerator->selectionModel()->select(targetFrame, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

void Wild3::centerFramesAndSetTargetGenerator(u32 centerFrames)
{
    ui->checkBoxGeneratorDisableFilters->setChecked(true);

    u32 frameNumber = ui->tableViewGenerator->model()->data(ui->tableViewGenerator->model()->index(lastIndex.row(), 0)).toString().toUInt();

    u32 startingFrame = frameNumber < centerFrames + 1 ? 1 : frameNumber - centerFrames;
    u32 selectedIndex = frameNumber < centerFrames + 1 ? frameNumber - 1U : centerFrames;
    u32 maxFrames = frameNumber < centerFrames + 1 ? frameNumber - 1 + centerFrames + 1 : centerFrames * 2 + 1;

    ui->textBoxGeneratorStartingFrame->setText(QString::number(startingFrame));
    ui->textBoxGeneratorMaxResults->setText(QString::number(maxFrames));

    on_pushButtonGenerate_clicked();

    targetFrame = ui->tableViewGenerator->model()->index(static_cast<int>(selectedIndex), 0);

    jumpToTargetGenerator();
}

void Wild3::seedToTime()
{
    u32 seed = searcherModel->data(searcherModel->index(lastIndex.row(), 0), Qt::DisplayRole).toString().toUInt(nullptr, 16);
    auto *seedToTime = new SeedToTime3(seed);
    seedToTime->show();
    seedToTime->raise();
}

void Wild3::copySeedToClipboard()
{
    QApplication::clipboard()->setText(searcherModel->data(searcherModel->index(lastIndex.row(), 0), Qt::DisplayRole).toString());
}

void Wild3::on_pushButtonGeneratorLead_clicked()
{
    ui->comboBoxGeneratorLead->clear();
    QString text = ui->pushButtonGeneratorLead->text();
    if (text == tr("Synchronize"))
    {
        ui->pushButtonGeneratorLead->setText(tr("Cute Charm"));

        ui->comboBoxGeneratorLead->addItem(tr("♂ Lead (50% ♀ Target)"), Lead::CuteCharm50F);
        ui->comboBoxGeneratorLead->addItem(tr("♂ Lead (75% ♀ Target)"), Lead::CuteCharm75F);
        ui->comboBoxGeneratorLead->addItem(tr("♂ Lead (25% ♀ Target)"), Lead::CuteCharm25F);
        ui->comboBoxGeneratorLead->addItem(tr("♂ Lead (12.5% ♀ Target)"), Lead::CuteCharm125F);
        ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (50% ♂ Target)"), Lead::CuteCharm50M);
        ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (75% ♂ Target)"), Lead::CuteCharm75M);
        ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (25% ♂ Target)"), Lead::CuteCharm25M);
        ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (87.5% ♂ Target)"), Lead::CuteCharm875M);
    }
    else
    {
        ui->pushButtonGeneratorLead->setText(tr("Synchronize"));

        ui->comboBoxGeneratorLead->addItem("None");
        ui->comboBoxGeneratorLead->addItems(Nature::getNatures());
    }
}

void Wild3::on_comboBoxGeneratorEncounter_currentIndexChanged(int index)
{
    (void)index;
    QStringList t;
    Encounter encounter = static_cast<Encounter>(ui->comboBoxGeneratorEncounter->currentData().toInt());

    switch (encounter)
    {
        case Encounter::Grass:
        case Encounter::SafariZone:
            t << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11";
            break;
        case Encounter::RockSmash:
        case Encounter::Surfing:
        case Encounter::SuperRod:
            t << "0" << "1" << "2" << "3" << "4";
            break;
        case Encounter::OldRod:
            t << "0" << "1";
            break;
        case Encounter::GoodRod:
            t << "0" << "1" << "2";
            break;
        default:
            break;
    }

    ui->comboBoxGeneratorEncounterSlot->setup(t);
    updateLocationsGenerator();
}

void Wild3::on_comboBoxSearcherEncounter_currentIndexChanged(int index)
{
    (void)index;
    QStringList t;
    Encounter encounter = static_cast<Encounter>(ui->comboBoxSearcherEncounter->currentData().toInt());

    switch (encounter)
    {
        case Encounter::Grass:
        case Encounter::SafariZone:
            t << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11";
            break;
        case Encounter::RockSmash:
        case Encounter::Surfing:
        case Encounter::SuperRod:
            t << "0" << "1" << "2" << "3" << "4";
            break;
        case Encounter::OldRod:
            t << "0" << "1";
            break;
        case Encounter::GoodRod:
            t << "0" << "1" << "2";
            break;
        default:
            break;
    }
    ui->comboBoxSearcherEncounterSlot->setup(t);
    updateLocationsSearcher();
}

void Wild3::on_comboBoxGeneratorLocation_currentIndexChanged(int index)
{
    if (index >= 0)
    {
        ui->textBoxGeneratorDelay->setText(QString::number(encounterGenerator[index].getDelay()));
    }
    updatePokemonGenerator();
}

void Wild3::on_comboBoxSearcherLocation_currentIndexChanged(int index)
{
    (void) index;
    updatePokemonSearcher();
}

void Wild3::on_comboBoxGeneratorPokemon_currentIndexChanged(int index)
{
    if (index <= 0)
    {
        ui->comboBoxGeneratorEncounterSlot->resetChecks();
        return;
    }

    u16 num = ui->comboBoxGeneratorPokemon->currentData().toUInt();
    QVector<bool> flags = encounterGenerator[ui->comboBoxGeneratorLocation->currentIndex()].getSlots(num);

    ui->comboBoxGeneratorEncounterSlot->setChecks(flags);
}

void Wild3::on_comboBoxSearcherPokemon_currentIndexChanged(int index)
{
    if (index <= 0)
    {
        ui->comboBoxSearcherEncounterSlot->resetChecks();
        return;
    }

    u16 num = ui->comboBoxSearcherPokemon->currentData().toUInt();
    QVector<bool> flags = encounterSearcher[ui->comboBoxSearcherLocation->currentIndex()].getSlots(num);

    ui->comboBoxSearcherEncounterSlot->setChecks(flags);
}

void Wild3::on_pushButtonProfileManager_clicked()
{
    auto *manager = new ProfileManager3();
    connect(manager, &ProfileManager3::updateProfiles, this, &Wild3::refreshProfiles);
    manager->show();
}
