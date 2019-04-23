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

#include <QSettings>
#include <QTimer>
#include "Wild4.hpp"
#include "ui_Wild4.h"
#include <Core/Gen4/Encounters4.hpp>
#include <Core/Gen4/Generator4.hpp>
#include <Core/Gen4/IVSearcher4.hpp>
#include <Core/Util/Nature.hpp>
#include <Core/Util/Power.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Gen4/ProfileManager4.hpp>
#include <Forms/Gen4/SeedtoTime4.hpp>

Wild4::Wild4(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Wild4)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();

    qRegisterMetaType<QVector<Frame4>>("QVector<Frame4>");
}

Wild4::~Wild4()
{
    QSettings setting;
    setting.beginGroup("wild4");
    setting.setValue("minDelay", ui->textBoxSearcherMinDelay->text());
    setting.setValue("maxDelay", ui->textBoxSearcherMaxDelay->text());
    setting.setValue("minFrame", ui->textBoxSearcherMinFrame->text());
    setting.setValue("maxFrame", ui->textBoxSearcherMaxFrame->text());
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("size", this->size());
    setting.endGroup();

    delete ui;
}

void Wild4::updateProfiles()
{
    profiles = Profile4::loadProfileList();
    profiles.insert(profiles.begin(), Profile4());

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(profile.getProfileName());
    }

    QSettings setting;
    int val = setting.value("wild4/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Wild4::setupModels()
{
    generatorModel = new Wild4Model(ui->tableViewGenerator, Method::MethodJ);
    searcherModel = new Searcher4Model(ui->tableViewSearcher, Method::Method1);
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
    ui->textBoxSearcherMinDelay->setValues(InputType::Delay);
    ui->textBoxSearcherMaxDelay->setValues(InputType::Delay);
    ui->textBoxSearcherMinFrame->setValues(InputType::Frame32Bit);
    ui->textBoxSearcherMaxFrame->setValues(InputType::Frame32Bit);

    ui->comboBoxGeneratorGenderRatio->setItemData(0, 0);
    ui->comboBoxGeneratorGenderRatio->setItemData(1, 127);
    ui->comboBoxGeneratorGenderRatio->setItemData(2, 191);
    ui->comboBoxGeneratorGenderRatio->setItemData(3, 63);
    ui->comboBoxGeneratorGenderRatio->setItemData(4, 31);
    ui->comboBoxGeneratorGenderRatio->setItemData(5, 1);
    ui->comboBoxGeneratorGenderRatio->setItemData(6, 2);

    ui->comboBoxSearcherGenderRatio->setItemData(0, 0);
    ui->comboBoxSearcherGenderRatio->setItemData(1, 127);
    ui->comboBoxSearcherGenderRatio->setItemData(2, 191);
    ui->comboBoxSearcherGenderRatio->setItemData(3, 63);
    ui->comboBoxSearcherGenderRatio->setItemData(4, 31);
    ui->comboBoxSearcherGenderRatio->setItemData(5, 1);
    ui->comboBoxSearcherGenderRatio->setItemData(6, 2);

    ui->comboBoxGeneratorLead->addItem(tr("None"));
    ui->comboBoxGeneratorLead->addItems(Nature::getNatures());

    ui->comboBoxGeneratorNature->setup(Nature::getNatures());
    ui->comboBoxSearcherNature->setup(Nature::getNatures());

    ui->comboBoxGeneratorHiddenPower->setup(Power::getPowers());
    ui->comboBoxSearcherHiddenPower->setup(Power::getPowers());

    on_comboBoxGeneratorEncounter_currentIndexChanged(0);
    on_comboBoxSearcherEncounter_currentIndexChanged(0);

    QAction *outputTXTGenerator = generatorMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVGenerator = generatorMenu->addAction(tr("Output Results to CSV"));

    connect(outputTXTGenerator, &QAction::triggered, [ = ]() { ui->tableViewGenerator->outputModelTXT(); });
    connect(outputCSVGenerator, &QAction::triggered, [ = ]() { ui->tableViewGenerator->outputModelCSV(); });

    QAction *seedToTime = searcherMenu->addAction(tr("Generate times for seed"));
    QAction *outputTXTSearcher = searcherMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVSearcher = searcherMenu->addAction(tr("Output Results to CSV"));

    connect(seedToTime, &QAction::triggered, this, &Wild4::seedToTime);
    connect(outputTXTSearcher, &QAction::triggered, [ = ]() { ui->tableViewSearcher->outputModelTXT(); });
    connect(outputCSVSearcher, &QAction::triggered, [ = ]() { ui->tableViewSearcher->outputModelCSV(); });

    QSettings setting;
    setting.beginGroup("wild4");
    if (setting.contains("minDelay")) ui->textBoxSearcherMinDelay->setText(setting.value("minDelay").toString());
    if (setting.contains("maxDelay")) ui->textBoxSearcherMaxDelay->setText(setting.value("maxDelay").toString());
    if (setting.contains("minFrame")) ui->textBoxSearcherMinFrame->setText(setting.value("minFrame").toString());
    if (setting.contains("maxFrame")) ui->textBoxSearcherMaxFrame->setText(setting.value("maxFrame").toString());
    if (setting.contains("size")) this->resize(setting.value("size").toSize());
    setting.endGroup();
}

void Wild4::updateView(const QVector<Frame4> &frames, int progress)
{
    searcherModel->addItems(frames);
    ui->progressBar->setValue(progress);
}

void Wild4::updateLocationsGenerator()
{
    Encounter encounter = static_cast<Encounter>(ui->comboBoxGeneratorEncounter->currentData().toInt());
    int time = ui->comboBoxGeneratorTime->currentIndex();
    auto profile = profiles.at(ui->comboBoxProfiles->currentIndex());

    encounterGenerator = Encounters4(encounter, time, profile).getEncounters();
    QVector<u8> locs;
    for (const auto &area : encounterGenerator)
    {
        locs.append(area.getLocation());
    }

    QStringList locations = Translator::getLocationsGen4(locs, profile.getVersion());

    ui->comboBoxGeneratorLocation->clear();
    ui->comboBoxGeneratorLocation->addItems(locations);
}

void Wild4::updateLocationsSearcher()
{
    Encounter encounter = static_cast<Encounter>(ui->comboBoxSearcherEncounter->currentData().toInt());
    int time = ui->comboBoxSearcherTime->currentIndex();
    auto profile = profiles.at(ui->comboBoxProfiles->currentIndex());

    encounterSearcher = Encounters4(encounter, time, profile).getEncounters();
    QVector<u8> locs;
    for (const auto &area : encounterSearcher)
    {
        locs.push_back(area.getLocation());
    }

    QStringList locations = Translator::getLocationsGen4(locs, profile.getVersion());

    ui->comboBoxSearcherLocation->clear();
    ui->comboBoxSearcherLocation->addItems(locations);
}

void Wild4::updatePokemonGenerator()
{
    if (ui->comboBoxGeneratorLocation->currentIndex() < 0)
    {
        return;
    }

    auto area = encounterGenerator.at(ui->comboBoxGeneratorLocation->currentIndex());
    QVector<u16> species = area.getUniqueSpecies();

    QStringList names = area.getSpecieNames();

    ui->comboBoxGeneratorPokemon->clear();
    ui->comboBoxGeneratorPokemon->addItem("-");
    for (auto i = 0; i < species.size(); i++)
    {
        ui->comboBoxGeneratorPokemon->addItem(names.at(i), species.at(i));
    }
}

void Wild4::updatePokemonSearcher()
{
    if (ui->comboBoxSearcherLocation->currentIndex() < 0)
    {
        return;
    }

    auto area = encounterSearcher.at(ui->comboBoxSearcherLocation->currentIndex());
    QVector<u16> species = area.getUniqueSpecies();

    QStringList names = area.getSpecieNames();

    ui->comboBoxSearcherPokemon->clear();
    ui->comboBoxSearcherPokemon->addItem("-");
    for (auto i = 0; i < species.size(); i++)
    {
        ui->comboBoxSearcherPokemon->addItem(names.at(i), species.at(i));
    }
}

void Wild4::refreshProfiles()
{
    emit alertProfiles(4);
}

void Wild4::on_pushButtonGenerate_clicked()
{
    generatorModel->clearModel();
    generatorModel->setMethod(static_cast<Method>(ui->comboBoxGeneratorMethod->currentData().toInt()));

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

    u8 genderRatio = ui->comboBoxGeneratorGenderRatio->currentData().toUInt();
    Generator4 generator(maxResults, startingFrame, seed, tid, sid, offset, static_cast<Method>(ui->comboBoxGeneratorMethod->currentData().toInt()), genderRatio);
    FrameCompare compare(ui->ivFilterGenerator->getLower(), ui->ivFilterGenerator->getUpper(),
                         ui->comboBoxGeneratorGender->currentIndex(), ui->comboBoxGeneratorAbility->currentIndex(),
                         ui->comboBoxGeneratorNature->getChecked(), ui->comboBoxGeneratorHiddenPower->getChecked(),
                         ui->checkBoxGeneratorShinyOnly->isChecked(), ui->checkBoxGeneratorDisableFilters->isChecked(), ui->comboBoxGeneratorEncounterSlot->getChecked());

    generator.setEncounterType(static_cast<Encounter>(ui->comboBoxGeneratorEncounter->currentData().toInt()));
    if (ui->pushButtonGeneratorLead->text() == tr("Cute Charm"))
    {
        generator.setLeadType((static_cast<Lead>(ui->comboBoxGeneratorLead->currentData().toInt())));
    }
    else if (ui->pushButtonGeneratorLead->text() == tr("Suction Cups"))
    {
        generator.setLeadType(Lead::SuctionCups);
    }
    else
    {
        int num = ui->comboBoxGeneratorLead->currentIndex();
        if (num == 0)
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

    QVector<Frame4> frames = generator.generate(compare);
    generatorModel->addItems(frames);
}

void Wild4::on_pushButtonSearch_clicked()
{
    searcherModel->clearModel();
    searcherModel->setMethod(static_cast<Method>(ui->comboBoxSearcherMethod->currentData().toInt()));

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u16 tid = ui->textBoxSearcherTID->getUShort();
    u16 sid = ui->textBoxSearcherSID->getUShort();

    u8 genderRatio = ui->comboBoxSearcherGenderRatio->currentData().toUInt();
    FrameCompare compare(ui->ivFilterSearcher->getLower(), ui->ivFilterSearcher->getUpper(), ui->comboBoxSearcherGender->currentIndex(),
                         ui->comboBoxSearcherAbility->currentIndex(), ui->comboBoxSearcherNature->getChecked(),
                         ui->comboBoxSearcherHiddenPower->getChecked(), ui->checkBoxSearcherShinyOnly->isChecked(), false,
                         ui->comboBoxSearcherEncounterSlot->getChecked());
    Searcher4 searcher(tid, sid, genderRatio, ui->textBoxSearcherMinDelay->getUInt(), ui->textBoxSearcherMaxDelay->getUInt(),
                       ui->textBoxSearcherMinFrame->getUInt(), ui->textBoxSearcherMaxFrame->getUInt(), compare, static_cast<Method>(ui->comboBoxSearcherMethod->currentData().toInt()));

    searcher.setEncounterType(static_cast<Encounter>(ui->comboBoxSearcherEncounter->currentData().toInt()));
    searcher.setLeadType(static_cast<Lead>(ui->comboBoxSearcherLead->currentData().toInt()));
    searcher.setEncounter(encounterSearcher[ui->comboBoxSearcherLocation->currentIndex()]);

    QVector<u8> min = ui->ivFilterSearcher->getLower();
    QVector<u8> max = ui->ivFilterSearcher->getUpper();

    int maxProgress = 1;
    for (int i = 0; i < 6; i++)
    {
        maxProgress *= max.at(i) - min.at(i) + 1;
    }

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(maxProgress);

    auto *search = new IVSearcher4(searcher, min, max);
    auto *timer = new QTimer(search);

    connect(search, &IVSearcher4::finished, timer, &QTimer::stop);
    connect(search, &IVSearcher4::finished, this, [ = ] { ui->pushButtonSearch->setEnabled(true); ui->pushButtonCancel->setEnabled(false); });
    connect(search, &IVSearcher4::finished, this, [ = ] { updateView(search->getResults(), search->currentProgress()); });
    connect(timer, &QTimer::timeout, this, [ = ] { updateView(search->getResults(), search->currentProgress()); });
    connect(ui->pushButtonCancel, &QPushButton::clicked, search, &IVSearcher4::cancelSearch);

    search->start();
    timer->start(1000);
}

void Wild4::on_comboBoxProfiles_currentIndexChanged(int index)
{
    if (index < 0)
    {
        return;
    }

    auto profile = profiles.at(index);
    QString tid = QString::number(profile.getTID());
    QString sid = QString::number(profile.getSID());

    ui->textBoxGeneratorTID->setText(tid);
    ui->textBoxGeneratorSID->setText(sid);
    ui->textBoxSearcherTID->setText(tid);
    ui->textBoxSearcherSID->setText(sid);
    ui->labelProfileTIDValue->setText(tid);
    ui->labelProfileSIDValue->setText(sid);
    ui->labelProfileGameValue->setText(profile.getVersionString());
    ui->labelProfileDualSlotValue->setText(profile.getDualSlotString());
    ui->labelProfileRadioValue->setText(profile.getRadioString());
    ui->labelProfilePokeRadarValue->setText(profile.getRadar() ? tr("True") : tr("False"));
    ui->labelProfileSwarmValue->setText(profile.getSwarm() ? tr("True") : tr("False"));

    bool flag = profile.getVersion() & Game::HGSS;

    ui->comboBoxGeneratorMethod->clear();
    ui->comboBoxGeneratorMethod->addItem(flag ? tr("Method K") : tr("Method J"), flag ? Method::MethodK : Method::MethodJ);
    if (!flag)
    {
        ui->comboBoxGeneratorMethod->addItem(tr("Chained Shiny"), ChainedShiny);
    }

    ui->comboBoxSearcherMethod->clear();
    ui->comboBoxSearcherMethod->addItem(flag ? tr("Method K") : tr("Method J"), flag ? Method::MethodK : Method::MethodJ);
    if (!flag)
    {
        ui->comboBoxSearcherMethod->addItem(tr("Chained Shiny"), Method::ChainedShiny);
    }

    ui->comboBoxGeneratorEncounter->clear();
    ui->comboBoxGeneratorEncounter->addItem(tr("Grass"), Encounter::Grass);
    if (flag)
    {
        ui->comboBoxGeneratorEncounter->addItem(tr("Rock Smash"), Encounter::RockSmash);
    }
    ui->comboBoxGeneratorEncounter->addItem(tr("Surfing"), Encounter::Surfing);
    ui->comboBoxGeneratorEncounter->addItem(tr("Old Rod"), Encounter::OldRod);
    ui->comboBoxGeneratorEncounter->addItem(tr("Good Rod"), Encounter::GoodRod);
    ui->comboBoxGeneratorEncounter->addItem(tr("Super Rod"), Encounter::SuperRod);

    ui->comboBoxSearcherEncounter->clear();
    ui->comboBoxSearcherEncounter->addItem(tr("Grass"), Encounter::Grass);
    if (flag)
    {
        ui->comboBoxSearcherEncounter->addItem(tr("Rock Smash"), Encounter::RockSmash);
    }
    ui->comboBoxSearcherEncounter->addItem(tr("Surfing"), Encounter::Surfing);
    ui->comboBoxSearcherEncounter->addItem(tr("Old Rod"), Encounter::OldRod);
    ui->comboBoxSearcherEncounter->addItem(tr("Good Rod"), Encounter::GoodRod);
    ui->comboBoxSearcherEncounter->addItem(tr("Super Rod"), Encounter::SuperRod);

    ui->comboBoxSearcherLead->clear();
    ui->comboBoxSearcherLead->addItem(tr("Any"), Lead::Search);
    ui->comboBoxSearcherLead->addItem(tr("Synchronize"), Lead::Synchronize);
    ui->comboBoxSearcherLead->addItem(tr("Cute Charm"), Lead::CuteCharm);
    if (flag)
    {
        ui->comboBoxSearcherLead->addItem("Suction Cups", Lead::SuctionCups);
    }
    ui->comboBoxSearcherLead->addItem("None", Lead::None);

    ui->pushButtonGeneratorLead->setText(tr("Synchronize"));
    ui->comboBoxGeneratorLead->addItem("None");
    ui->comboBoxGeneratorLead->addItems(Nature::getNatures());

    updateLocationsSearcher();
    updateLocationsGenerator();
}

void Wild4::on_pushButtonGeneratorLead_clicked()
{
    ui->comboBoxGeneratorLead->clear();
    QString text = ui->pushButtonGeneratorLead->text();
    if (text == tr("Synchronize"))
    {
        auto profile = profiles[ui->comboBoxProfiles->currentIndex()];
        bool flag = profile.getVersion() & Game::HGSS;
        if (flag)
        {
            ui->pushButtonGeneratorLead->setText(tr("Suction Cups"));
            ui->comboBoxGeneratorLead->setEnabled(false);
        }
        else
        {
            ui->pushButtonGeneratorLead->setText(tr("Cute Charm"));
            ui->comboBoxGeneratorLead->setEnabled(true);

            ui->comboBoxGeneratorLead->addItem(tr("♂ Lead"), Lead::CuteCharmFemale);
            ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (50% ♂ Target)"), Lead::CuteCharm50M);
            ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (75% ♂ Target)"), Lead::CuteCharm75M);
            ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (25% ♂ Target)"), Lead::CuteCharm25M);
            ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (87.5% ♂ Target)"), Lead::CuteCharm875M);
        }
    }
    else if (text == tr("Suction Cups"))
    {
        ui->pushButtonGeneratorLead->setText(tr("Cute Charm"));
        ui->comboBoxGeneratorLead->setEnabled(true);

        ui->comboBoxGeneratorLead->addItem(tr("♂ Lead"), Lead::CuteCharmFemale);
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

void Wild4::on_comboBoxGeneratorEncounter_currentIndexChanged(int index)
{
    (void) index;
    QStringList t;
    Encounter encounter = static_cast<Encounter>(ui->comboBoxGeneratorEncounter->currentData().toInt());

    switch (encounter)
    {
        case Encounter::Grass:
            t << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11";
            break;
        case Encounter::Surfing:
        case Encounter::OldRod:
        case Encounter::GoodRod:
        case Encounter::SuperRod:
            t << "0" << "1" << "2" << "3" << "4";
            break;
        case Encounter::RockSmash:
            t << "0" << "1";
            break;
        default:
            break;
    }

    ui->comboBoxGeneratorEncounterSlot->setup(t);
    updateLocationsGenerator();
}

void Wild4::on_comboBoxSearcherEncounter_currentIndexChanged(int index)
{
    (void) index;
    QStringList t;
    Encounter encounter = static_cast<Encounter>(ui->comboBoxSearcherEncounter->currentData().toInt());

    switch (encounter)
    {
        case Encounter::Grass:
            t << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11";
            break;
        case Encounter::Surfing:
        case Encounter::OldRod:
        case Encounter::GoodRod:
        case Encounter::SuperRod:
            t << "0" << "1" << "2" << "3" << "4";
            break;
        case Encounter::RockSmash:
            t << "0" << "1";
            break;
        default:
            break;
    }

    ui->comboBoxSearcherEncounterSlot->setup(t);
    updateLocationsSearcher();
}

void Wild4::on_comboBoxGeneratorLocation_currentIndexChanged(int index)
{
    (void) index;
    updatePokemonGenerator();
}

void Wild4::on_comboBoxSearcherLocation_currentIndexChanged(int index)
{
    (void) index;
    updatePokemonSearcher();
}

void Wild4::on_comboBoxGeneratorPokemon_currentIndexChanged(int index)
{
    if (index <= 0)
    {
        ui->comboBoxGeneratorEncounterSlot->resetChecks();
        return;
    }

    u16 num = ui->comboBoxGeneratorPokemon->currentData().toUInt();
    QVector<bool> flags = encounterGenerator.at(ui->comboBoxGeneratorLocation->currentIndex()).getSlots(num);

    ui->comboBoxGeneratorEncounterSlot->setChecks(flags);
}

void Wild4::on_comboBoxSearcherPokemon_currentIndexChanged(int index)
{
    if (index <= 0)
    {
        ui->comboBoxSearcherEncounterSlot->resetChecks();
        return;
    }

    u16 num = ui->comboBoxSearcherPokemon->currentData().toUInt();
    QVector<bool> flags = encounterSearcher.at(ui->comboBoxSearcherLocation->currentIndex()).getSlots(num);

    ui->comboBoxSearcherEncounterSlot->setChecks(flags);
}

void Wild4::on_comboBoxGeneratorTime_currentIndexChanged(int index)
{
    index = ui->comboBoxGeneratorLocation->currentIndex();
    updateLocationsGenerator();
    ui->comboBoxGeneratorLocation->setCurrentIndex(index);
}

void Wild4::on_comboBoxSearcherTime_currentIndexChanged(int index)
{
    index = ui->comboBoxSearcherLocation->currentIndex();
    updateLocationsSearcher();
    ui->comboBoxSearcherLocation->setCurrentIndex(index);
}

void Wild4::seedToTime()
{
    QModelIndex index = ui->tableViewSearcher->currentIndex();
    auto *time = new SeedtoTime4(searcherModel->data(searcherModel->index(index.row(), 0), Qt::DisplayRole).toString(), profiles[ui->comboBoxProfiles->currentIndex()]);
    time->show();
    time->raise();
}

void Wild4::on_tableViewGenerator_customContextMenuRequested(const QPoint &pos)
{
    if (generatorModel->rowCount() == 0)
    {
        return;
    }

    generatorMenu->popup(ui->tableViewGenerator->viewport()->mapToGlobal(pos));
}

void Wild4::on_tableViewSearcher_customContextMenuRequested(const QPoint &pos)
{
    if (searcherModel->rowCount() == 0)
    {
        return;
    }

    searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
}

void Wild4::on_pushButtonProfileManager_clicked()
{
    auto *manager = new ProfileManager4();
    connect(manager, &ProfileManager4::updateProfiles, this, &Wild4::refreshProfiles);
    manager->show();
}
