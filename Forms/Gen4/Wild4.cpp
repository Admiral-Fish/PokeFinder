#include <utility>

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

#include "Wild4.hpp"
#include "ui_Wild4.h"

Wild4::Wild4(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Wild4)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();

    qRegisterMetaType<QVector<Frame4>>("QVector<Frame4>");
    connect(this, &Wild4::updateView, this, &Wild4::updateViewSearcher);
    connect(this, &Wild4::updateProgress, this, &Wild4::updateProgressBar);
}

Wild4::~Wild4()
{
    saveSettings();

    delete ui;
    delete g;
    delete s;
    delete searcherMenu;
}

void Wild4::setupModels()
{
    ui->tableViewGenerator->setModel(g);
    ui->tableViewSearcher->setModel(s);

    ui->initialSeedGenerator->setValues(InputType::Seed32Bit);
    ui->idGenerator->setValues(InputType::TIDSID);
    ui->sidGenerator->setValues(InputType::TIDSID);
    ui->startingFrameGenerator->setValues(InputType::Frame32Bit);
    ui->maxResultsGenerator->setValues(InputType::Frame32Bit);
    ui->delayGenerator->setValues(InputType::Frame32Bit);

    ui->idSearcher->setValues(InputType::TIDSID);
    ui->sidSearcher->setValues(InputType::TIDSID);

    ui->comboBoxLeadGenerator->addItem(tr("None"));
    ui->comboBoxLeadGenerator->addItems(Nature::getNatures());

    ui->comboBoxLeadSearcher->setItemData(0, Lead::Search);
    ui->comboBoxLeadSearcher->setItemData(1, Lead::Synchronize);
    ui->comboBoxLeadSearcher->setItemData(2, Lead::CuteCharm);
    ui->comboBoxLeadSearcher->setItemData(3, Lead::SuctionCups);
    ui->comboBoxLeadSearcher->setItemData(4, Lead::None);

    ui->comboBoxNatureGenerator->setup();
    ui->comboBoxNatureSearcher->setup();

    ui->comboBoxHiddenPowerGenerator->setup();
    ui->comboBoxHiddenPowerSearcher->setup();

    on_comboBoxEncounterGenerator_currentIndexChanged(0);
    on_comboBoxEncounterSearcher_currentIndexChanged(0);

    QAction *seedToTime = new QAction(tr("Generate times for seed"), this);
    connect(seedToTime, &QAction::triggered, this, &Wild4::seedToTime);

    searcherMenu->addAction(seedToTime);

    loadSettings();
}

void Wild4::updateProfiles()
{
    profiles = Profile4::loadProfileList();
    profiles.insert(profiles.begin(), Profile4());

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
        ui->comboBoxProfiles->addItem(profile.getProfileName());

    QSettings setting;
    int val = setting.value("wild4Profile").toInt();
    if (val < ui->comboBoxProfiles->count())
        ui->comboBoxProfiles->setCurrentIndex(val >= 0 ? val : 0);
}

void Wild4::on_comboBoxProfiles_currentIndexChanged(int index)
{
    auto profile = profiles[index >= 0 ? index : 0];
    QString tid = QString::number(profile.getTID());
    QString sid = QString::number(profile.getSID());

    ui->idGenerator->setText(tid);
    ui->sidGenerator->setText(sid);
    ui->idSearcher->setText(tid);
    ui->sidSearcher->setText(sid);
    ui->profileTID->setText(tid);
    ui->profileSID->setText(sid);
    ui->profileGame->setText(profile.getVersionString());
    ui->profileDualSlot->setText(profile.getDualSlotString());
    ui->profileRadio->setText(profile.getRadioString());

    bool flag = profile.getVersion() == Game::HeartGold || profile.getVersion() == Game::SoulSilver;

    ui->comboBoxMethodGenerator->clear();
    ui->comboBoxMethodGenerator->addItem(flag ? tr("Method K") : tr("Method J"), flag ? Method::MethodK : Method::MethodJ);
    if (!flag)
        ui->comboBoxMethodGenerator->addItem(tr("Chained Shiny"), ChainedShiny);

    ui->comboBoxMethodSearcher->clear();
    ui->comboBoxMethodSearcher->addItem(flag ? tr("Method K") : tr("Method J"), flag ? Method::MethodK : Method::MethodJ);
    if (!flag)
        ui->comboBoxMethodSearcher->addItem(tr("Chained Shiny"), Method::ChainedShiny);

    ui->comboBoxEncounterGenerator->clear();
    ui->comboBoxEncounterGenerator->addItem(tr("Grass"), Encounter::Grass);
    if (flag)
        ui->comboBoxEncounterGenerator->addItem(tr("Rock Smash"), Encounter::RockSmash);
    ui->comboBoxEncounterGenerator->addItem(tr("Surfing"), Encounter::Surfing);
    ui->comboBoxEncounterGenerator->addItem(tr("Old Rod"), Encounter::OldRod);
    ui->comboBoxEncounterGenerator->addItem(tr("Good Rod"), Encounter::GoodRod);
    ui->comboBoxEncounterGenerator->addItem(tr("Super Rod"), Encounter::SuperRod);

    ui->comboBoxEncounterSearcher->clear();
    ui->comboBoxEncounterSearcher->addItem(tr("Grass"), Encounter::Grass);
    if (flag)
        ui->comboBoxEncounterSearcher->addItem(tr("Rock Smash"), Encounter::RockSmash);
    ui->comboBoxEncounterSearcher->addItem(tr("Surfing"), Encounter::Surfing);
    ui->comboBoxEncounterSearcher->addItem(tr("Old Rod"), Encounter::OldRod);
    ui->comboBoxEncounterSearcher->addItem(tr("Good Rod"), Encounter::GoodRod);
    ui->comboBoxEncounterSearcher->addItem(tr("Super Rod"), Encounter::SuperRod);

    updateLocationsSearcher();
    updateLocationsGenerator();
}

void Wild4::seedToTime()
{
    QModelIndex index = ui->tableViewSearcher->currentIndex();
    SeedtoTime4 *time = new SeedtoTime4(s->data(s->index(index.row(), 0), Qt::DisplayRole).toString(), profiles[ui->comboBoxProfiles->currentIndex()]);
    time->show();
    time->raise();
}

void Wild4::refreshProfiles()
{
    emit alertProfiles(4);
}

void Wild4::on_anyNatureGenerator_clicked()
{
    ui->comboBoxNatureGenerator->uncheckAll();
}

void Wild4::on_anyHiddenPowerGenerator_clicked()
{
    ui->comboBoxHiddenPowerGenerator->uncheckAll();
}

void Wild4::on_anyNatureSearcher_clicked()
{
    ui->comboBoxNatureSearcher->uncheckAll();
}

void Wild4::on_anyHiddenPowerSearcher_clicked()
{
    ui->comboBoxHiddenPowerSearcher->uncheckAll();
}

void Wild4::on_pushButtonLeadGenerator_clicked()
{
    ui->comboBoxLeadGenerator->clear();
    QString text = ui->pushButtonLeadGenerator->text();
    if (text == tr("Synchronize"))
    {
        ui->pushButtonLeadGenerator->setText(tr("Suction Cups"));
        ui->comboBoxLeadGenerator->setEnabled(false);
    }
    else if (text == tr("Suction Cups"))
    {
        ui->pushButtonLeadGenerator->setText(tr("Cute Charm"));
        ui->comboBoxLeadGenerator->setEnabled(true);

        ui->comboBoxLeadGenerator->addItem(tr("♂ Lead"), Lead::CuteCharmFemale);
        ui->comboBoxLeadGenerator->addItem(tr("♀ Lead (50% ♂ Target)"), Lead::CuteCharm50M);
        ui->comboBoxLeadGenerator->addItem(tr("♀ Lead (75% ♂ Target)"), Lead::CuteCharm75M);
        ui->comboBoxLeadGenerator->addItem(tr("♀ Lead (25% ♂ Target)"), Lead::CuteCharm25M);
        ui->comboBoxLeadGenerator->addItem(tr("♀ Lead (87.5% ♂ Target)"), Lead::CuteCharm875M);
    }
    else
    {
        ui->pushButtonLeadGenerator->setText(tr("Synchronize"));

        ui->comboBoxLeadGenerator->addItem("None");
        ui->comboBoxLeadGenerator->addItems(Nature::getNatures());
    }
}

void Wild4::on_pushButtonProfileManager_clicked()
{
    auto *manager = new ProfileManager4();
    connect(manager, &ProfileManager4::updateProfiles, this, &Wild4::refreshProfiles);
    manager->show();
}

void Wild4::on_anySlotGenerator_clicked()
{
    ui->comboBoxSlotGenerator->uncheckAll();
}

void Wild4::on_anySlotSearcher_clicked()
{
    ui->comboBoxSlotSearcher->uncheckAll();
}

void Wild4::on_comboBoxEncounterGenerator_currentIndexChanged(int index)
{
    (void) index;
    QStringList t;
    Encounter encounter = static_cast<Encounter>(ui->comboBoxEncounterGenerator->currentData().toInt());

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
    ui->comboBoxSlotGenerator->clear();
    ui->comboBoxSlotGenerator->addItems(t);
    ui->comboBoxSlotGenerator->setup();

    updateLocationsGenerator();
}

void Wild4::on_comboBoxEncounterSearcher_currentIndexChanged(int index)
{
    (void) index;
    QStringList t;
    Encounter encounter = static_cast<Encounter>(ui->comboBoxEncounterSearcher->currentData().toInt());

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
    ui->comboBoxSlotSearcher->clear();
    ui->comboBoxSlotSearcher->addItems(t);
    ui->comboBoxSlotSearcher->setup();

    updateLocationsSearcher();
}

void Wild4::on_comboBoxLocationSearcher_currentIndexChanged(int index)
{
    (void) index;
    updatePokemonSearcher();
}

void Wild4::on_comboBoxPokemonSearcher_currentIndexChanged(int index)
{
    if (index <= 0)
    {
        ui->comboBoxSlotSearcher->uncheckAll();
        return;
    }

    u32 num = ui->comboBoxPokemonSearcher->currentData().toUInt();
    QVector<bool> flags = encounterSearcher[ui->comboBoxLocationSearcher->currentIndex()].getSlots(num);

    ui->comboBoxSlotSearcher->setChecks(flags);
}

void Wild4::on_comboBoxLocationGenerator_currentIndexChanged(int index)
{
    (void) index;
    updatePokemonGenerator();
}

void Wild4::on_comboBoxPokemonGenerator_currentIndexChanged(int index)
{
    if (index <= 0)
    {
        ui->comboBoxSlotGenerator->uncheckAll();
        return;
    }

    u32 num = ui->comboBoxPokemonGenerator->currentData().toUInt();
    QVector<bool> flags = encounterGenerator[ui->comboBoxLocationGenerator->currentIndex()].getSlots(num);

    ui->comboBoxSlotGenerator->setChecks(flags);
}

void Wild4::on_pushButtonGenerate_clicked()
{
    g->clear();
    g->setMethod(static_cast<Method>(ui->comboBoxMethodGenerator->currentData().toInt()));

    u32 seed = ui->initialSeedGenerator->text().toUInt(nullptr, 16);
    u32 startingFrame = ui->startingFrameGenerator->text().toUInt();
    u32 maxResults = ui->maxResultsGenerator->text().toUInt();
    u16 tid = ui->idGenerator->text().toUShort();
    u16 sid = ui->sidGenerator->text().toUShort();
    u32 offset = 0;
    if (ui->checkBoxDelayGenerator->isChecked())
        offset = ui->delayGenerator->text().toUInt();

    int genderRatioIndex = ui->comboBoxGenderRatioGenerator->currentIndex();
    Generator4 generator = Generator4(maxResults, startingFrame, seed, tid, sid, offset, static_cast<Method>(ui->comboBoxMethodGenerator->currentData().toInt()));
    FrameCompare compare = FrameCompare(ui->ivFilterGenerator->getEvals(), ui->ivFilterGenerator->getValues(),
                                        ui->comboBoxGenderGenerator->currentIndex(), genderRatioIndex, ui->comboBoxAbilityGenerator->currentIndex(),
                                        ui->comboBoxNatureGenerator->getChecked(), ui->comboBoxHiddenPowerGenerator->getChecked(),
                                        ui->checkBoxShinyGenerator->isChecked(), ui->checkBoxDisableGenerator->isChecked(), ui->comboBoxSlotGenerator->getChecked());

    generator.setEncounterType(static_cast<Encounter>(ui->comboBoxEncounterGenerator->currentData().toInt()));
    if (ui->pushButtonLeadGenerator->text() == tr("Cute Charm"))
        generator.setLeadType((static_cast<Lead>(ui->comboBoxLeadGenerator->currentData().toInt())));
    else if (ui->pushButtonLeadGenerator->text() == tr("Suction Cups"))
        generator.setLeadType(Lead::SuctionCups);
    else
    {
        int num = ui->comboBoxLeadGenerator->currentIndex();
        if (num == 0)
        {
            generator.setLeadType(Lead::None);
        }
        else
        {
            generator.setLeadType(Lead::Synchronize);
            generator.setSynchNature(Nature::getAdjustedNature(static_cast<u32>(ui->comboBoxLeadGenerator->currentIndex() - 1)));
        }
    }
    generator.setEncounter(encounterGenerator[ui->comboBoxLocationGenerator->currentIndex()]);

    QVector<Frame4> frames = generator.generate(compare);
    g->setModel(frames);
}

void Wild4::search()
{
    u16 tid = ui->idSearcher->text().toUShort();
    u16 sid = ui->sidSearcher->text().toUShort();

    int genderRatioIndex = ui->comboBoxGenderRatioSearcher->currentIndex();
    FrameCompare compare = FrameCompare(ui->ivFilterSearcher->getEvals(), ui->ivFilterSearcher->getValues(), ui->comboBoxGenderSearcher->currentIndex(),
                                        genderRatioIndex, ui->comboBoxAbilitySearcher->currentIndex(), ui->comboBoxNatureSearcher->getChecked(),
                                        ui->comboBoxHiddenPowerSearcher->getChecked(), ui->checkBoxShinySearcher->isChecked(), false,
                                        ui->comboBoxSlotSearcher->getChecked());
    Searcher4 searcher = Searcher4(tid, sid, static_cast<u32>(genderRatioIndex), ui->minDelay->text().toUInt(), ui->maxDelay->text().toUInt(), ui->minFrame->text().toUInt(), ui->maxFrame->text().toUInt(), compare, static_cast<Method>(ui->comboBoxMethodSearcher->currentData().toInt()));

    searcher.setEncounterType(static_cast<Encounter>(ui->comboBoxEncounterSearcher->currentData().toInt()));
    searcher.setLeadType(static_cast<Lead>(ui->comboBoxLeadSearcher->currentData().toInt()));
    searcher.setEncounter(encounterSearcher[ui->comboBoxLocationSearcher->currentIndex()]);

    QVector<u32> min = ui->ivFilterSearcher->getLower();
    QVector<u32> max = ui->ivFilterSearcher->getUpper();

    ui->progressBar->setMaximum(static_cast<int>((max[0] - min[0] + 1) * (max[1] - min[1] + 1) * (max[2] - min[2] + 1) * (max[3] - min[3] + 1) * (max[4] - min[4] + 1) * (max[5] - min[5] + 1)));

    for (u32 a = min[0]; a <= max[0]; a++)
    {
        for (u32 b = min[1]; b <= max[1]; b++)
        {
            for (u32 c = min[2]; c <= max[2]; c++)
            {
                for (u32 d = min[3]; d <= max[3]; d++)
                {
                    for (u32 e = min[4]; e <= max[4]; e++)
                    {
                        for (u32 f = min[5]; f <= max[5]; f++)
                        {
                            QVector<Frame4> frames = searcher.search(a, b, c, d, e, f);

                            if (!frames.empty())
                                emit updateView(frames);

                            progress++;

                            if (cancel)
                            {
                                isSearching = false;
                                ui->pushButtonSearch->setText(tr("Search"));
                                emit updateProgress();
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
    isSearching = false;
    ui->pushButtonSearch->setText(tr("Search"));
    emit updateProgress();
}

void Wild4::on_pushButtonSearch_clicked()
{
    if (isSearching)
    {
        cancel = true;
    }
    else
    {
        s->clear();
        s->setMethod(static_cast<Method>(ui->comboBoxMethodSearcher->currentData().toInt()));

        ui->progressBar->setValue(0);
        progress = 0;

        isSearching = true;
        cancel = false;
        ui->pushButtonSearch->setText(tr("Cancel"));

        std::thread job(&Wild4::search, this);
        job.detach();

        std::thread update(&Wild4::updateSearch, this);
        update.detach();
    }
}

void Wild4::updateSearch()
{
    while (isSearching && !cancel)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        emit updateProgress();
    }
}

void Wild4::updateLocationsSearcher()
{
    Encounter encounter = static_cast<Encounter>(ui->comboBoxEncounterSearcher->currentData().toInt());
    Game game = Game::Diamond;
    Game dual = Game::Blank;
    int time = ui->comboBoxTimeSearcher->currentIndex();
    int sound = 0;

    if (ui->comboBoxProfiles->currentIndex() >= 0)
    {
        auto profile = profiles[ui->comboBoxProfiles->currentIndex()];
        game = profile.getVersion();
        dual = profile.getDualSlot();
        sound = profile.getRadio();
    }

    encounterSearcher = Encounters4(encounter, game, dual, time, sound).getEncounters();
    QVector<int> locs;
    for (const auto &area : encounterSearcher)
        locs.push_back(area.getLocation());

    QStringList locations = Translator::getLocationsGen4(locs, game);

    ui->comboBoxLocationSearcher->clear();
    ui->comboBoxLocationSearcher->addItems(locations);
}

void Wild4::updatePokemonSearcher()
{
    if (ui->comboBoxLocationSearcher->currentIndex() < 0)
        return;

    auto area = encounterSearcher[ui->comboBoxLocationSearcher->currentIndex()];
    QVector<int> species = area.getUniqueSpecies();

    QStringList names = area.getSpecieNames();

    ui->comboBoxPokemonSearcher->clear();
    ui->comboBoxPokemonSearcher->addItem("-");
    for (int i = 0; i < species.size(); i++)
        ui->comboBoxPokemonSearcher->addItem(names[i], species[i]);
}

void Wild4::updateLocationsGenerator()
{
    Encounter encounter = static_cast<Encounter>(ui->comboBoxEncounterGenerator->currentData().toInt());
    Game game = Game::Diamond;
    Game dual = Game::Blank;
    int time = ui->comboBoxTimeGenerator->currentIndex();
    int sound = 0;

    if (ui->comboBoxProfiles->currentIndex() >= 0)
    {
        auto profile = profiles[ui->comboBoxProfiles->currentIndex()];
        game = profile.getVersion();
        dual = profile.getDualSlot();
        sound = profile.getRadio();
    }

    encounterGenerator = Encounters4(encounter, game, dual, time, sound).getEncounters();
    QVector<int> locs;
    for (const auto &area : encounterGenerator)
        locs.append(area.getLocation());

    QStringList locations = Translator::getLocationsGen4(locs, game);

    ui->comboBoxLocationGenerator->clear();
    ui->comboBoxLocationGenerator->addItems(locations);
}

void Wild4::updatePokemonGenerator()
{
    if (ui->comboBoxLocationGenerator->currentIndex() < 0)
        return;

    auto area = encounterGenerator[ui->comboBoxLocationGenerator->currentIndex()];
    QVector<int> species = area.getUniqueSpecies();

    QStringList names = area.getSpecieNames();

    ui->comboBoxPokemonGenerator->clear();
    ui->comboBoxPokemonGenerator->addItem("-");
    for (int i = 0; i < species.size(); i++)
        ui->comboBoxPokemonGenerator->addItem(names[i], species[i]);
}

void Wild4::loadSettings()
{
    QSettings setting;
    if (setting.contains("wild4MinDelay")) ui->minDelay->setText(setting.value("wild4MinDelay").toString());
    if (setting.contains("wild4MaxDelay")) ui->maxDelay->setText(setting.value("wild4MaxDelay").toString());
    if (setting.contains("wild4MinFrame")) ui->minFrame->setText(setting.value("wild4MinFrame").toString());
    if (setting.contains("wild4MaxFrame")) ui->maxFrame->setText(setting.value("wild4MaxFrame").toString());
}

void Wild4::saveSettings()
{
    QSettings setting;
    setting.setValue("wild4Profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("wild4MinDelay", ui->minDelay->text());
    setting.setValue("wild4MaxDelay", ui->maxDelay->text());
    setting.setValue("wild4MinFrame", ui->minFrame->text());
    setting.setValue("wild4MaxFrame", ui->maxFrame->text());
}

void Wild4::updateProgressBar()
{
    ui->progressBar->setValue(progress);
}

void Wild4::updateViewSearcher(const QVector<Frame4> &frames)
{
    s->addItems(frames);
}

void Wild4::on_comboBoxTimeGenerator_currentIndexChanged(int index)
{
    index = ui->comboBoxLocationGenerator->currentIndex();
    updateLocationsGenerator();
    ui->comboBoxLocationGenerator->setCurrentIndex(index);
}

void Wild4::on_comboBoxTimeSearcher_currentIndexChanged(int index)
{
    index = ui->comboBoxLocationSearcher->currentIndex();
    updateLocationsSearcher();
    ui->comboBoxLocationSearcher->setCurrentIndex(index);
}

void Wild4::on_tableViewSearcher_customContextMenuRequested(const QPoint &pos)
{
    if (s->rowCount() == 0)
        return;

    searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
}
