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
}

Wild4::~Wild4()
{
    QSettings setting;
    setting.setValue("wild4Profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("wild4MinDelay", ui->textBoxSearcherMinDelay->text());
    setting.setValue("wild4MaxDelay", ui->textBoxSearcherMaxDelay->text());
    setting.setValue("wild4MinFrame", ui->textBoxSearcherMinFrame->text());
    setting.setValue("wild4MaxFrame", ui->textBoxSearcherMaxFrame->text());

    delete ui;
    delete g;
    delete s;
    delete searcherMenu;
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
    int val = setting.value("wild4Profile").toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val >= 0 ? val : 0);
    }
}

void Wild4::setupModels()
{
    ui->tableViewGenerator->setModel(g);
    ui->tableViewSearcher->setModel(s);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed32Bit);
    ui->textBoxGeneratorTID->setValues(InputType::TIDSID);
    ui->textBoxGeneratorSID->setValues(InputType::TIDSID);
    ui->textBoxGeneratorStartingFrame->setValues(InputType::Frame32Bit);
    ui->textBoxGeneratorMaxResults->setValues(InputType::Frame32Bit);
    ui->textBoxGeneratorDelay->setValues(InputType::Frame32Bit);

    ui->textBoxSearcherTID->setValues(InputType::TIDSID);
    ui->textBoxSearcherSID->setValues(InputType::TIDSID);

    ui->comboBoxGeneratorLead->addItem(tr("None"));
    ui->comboBoxGeneratorLead->addItems(Nature::getNatures());

    ui->comboBoxGeneratorNature->setup();
    ui->comboBoxSearcherNature->setup();

    ui->comboBoxGeneratorHiddenPower->setup();
    ui->comboBoxSearcherHiddenPower->setup();

    on_comboBoxGeneratorEncounter_currentIndexChanged(0);
    on_comboBoxSearcherEncounter_currentIndexChanged(0);

    QAction *seedToTime = new QAction(tr("Generate times for seed"), this);
    connect(seedToTime, &QAction::triggered, this, &Wild4::seedToTime);

    searcherMenu->addAction(seedToTime);

    QSettings setting;
    if (setting.contains("wild4MinDelay")) ui->textBoxSearcherMinDelay->setText(setting.value("wild4MinDelay").toString());
    if (setting.contains("wild4MaxDelay")) ui->textBoxSearcherMaxDelay->setText(setting.value("wild4MaxDelay").toString());
    if (setting.contains("wild4MinFrame")) ui->textBoxSearcherMinFrame->setText(setting.value("wild4MinFrame").toString());
    if (setting.contains("wild4MaxFrame")) ui->textBoxSearcherMaxFrame->setText(setting.value("wild4MaxFrame").toString());
}

void Wild4::updateView(const QVector<Frame4> &frames, int progress)
{
    s->addItems(frames);
    ui->progressBar->setValue(progress);
}

void Wild4::updateLocationsGenerator()
{
    Encounter encounter = static_cast<Encounter>(ui->comboBoxGeneratorEncounter->currentData().toInt());
    int time = ui->comboBoxGeneratorTime->currentIndex();
    auto profile = profiles[ui->comboBoxProfiles->currentIndex()];

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
    auto profile = profiles[ui->comboBoxProfiles->currentIndex()];

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

void Wild4::updatePokemonSearcher()
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

void Wild4::refreshProfiles()
{
    emit alertProfiles(4);
}

void Wild4::on_pushButtonGenerate_clicked()
{
    g->clear();
    g->setMethod(static_cast<Method>(ui->comboBoxGeneratorMethod->currentData().toInt()));

    u32 seed = ui->textBoxGeneratorSeed->text().toUInt(nullptr, 16);
    u32 startingFrame = ui->textBoxGeneratorStartingFrame->text().toUInt();
    u32 maxResults = ui->textBoxGeneratorMaxResults->text().toUInt();
    u16 tid = ui->textBoxGeneratorTID->text().toUShort();
    u16 sid = ui->textBoxGeneratorSID->text().toUShort();
    u32 offset = 0;
    if (ui->checkBoxGeneratorDelay->isChecked())
    {
        offset = ui->textBoxGeneratorDelay->text().toUInt();
    }

    int genderRatioIndex = ui->comboBoxGeneratorGenderRatio->currentIndex();
    Generator4 generator = Generator4(maxResults, startingFrame, seed, tid, sid, offset, static_cast<Method>(ui->comboBoxGeneratorMethod->currentData().toInt()));
    FrameCompare compare = FrameCompare(ui->ivFilterGenerator->getLower(), ui->ivFilterGenerator->getUpper(),
                                        ui->comboBoxGeneratorGender->currentIndex(), genderRatioIndex, ui->comboBoxGeneratorAbility->currentIndex(),
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
    g->setModel(frames);
}

void Wild4::on_pushButtonSearch_clicked()
{
    s->clear();
    s->setMethod(static_cast<Method>(ui->comboBoxSearcherMethod->currentData().toInt()));

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u16 tid = ui->textBoxSearcherTID->text().toUShort();
    u16 sid = ui->textBoxSearcherSID->text().toUShort();

    int genderRatioIndex = ui->comboBoxSearcherGenderRatio->currentIndex();
    FrameCompare compare = FrameCompare(ui->ivFilterSearcher->getLower(), ui->ivFilterSearcher->getUpper(), ui->comboBoxSearcherGender->currentIndex(),
                                        genderRatioIndex, ui->comboBoxSearcherAbility->currentIndex(), ui->comboBoxSearcherNature->getChecked(),
                                        ui->comboBoxSearcherHiddenPower->getChecked(), ui->checkBoxSearcherShinyOnly->isChecked(), false,
                                        ui->comboBoxSearcherEncounterSlot->getChecked());
    Searcher4 searcher = Searcher4(tid, sid, static_cast<u32>(genderRatioIndex), ui->textBoxSearcherMinDelay->text().toUInt(), ui->textBoxSearcherMaxDelay->text().toUInt(),
                                   ui->textBoxSearcherMinFrame->text().toUInt(), ui->textBoxSearcherMaxFrame->text().toUInt(), compare, static_cast<Method>(ui->comboBoxSearcherMethod->currentData().toInt()));

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

    auto *search = new WildSearcher4(searcher, min, max);
    auto *timer = new QTimer();

    connect(search, &WildSearcher4::finished, timer, &QTimer::deleteLater);
    connect(search, &WildSearcher4::finished, timer, &QTimer::stop);
    connect(search, &WildSearcher4::finished, this, [ = ] { ui->pushButtonSearch->setEnabled(true); ui->pushButtonCancel->setEnabled(false); });
    connect(search, &WildSearcher4::finished, this, [ = ] { updateView(search->getResults(), search->currentProgress()); });
    connect(timer, &QTimer::timeout, this, [ = ] { updateView(search->getResults(), search->currentProgress()); });
    connect(ui->pushButtonCancel, &QPushButton::clicked, search, &WildSearcher4::cancelSearch);

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
    ui->comboBoxGeneratorEncounterSlot->clear();
    ui->comboBoxGeneratorEncounterSlot->addItems(t);
    ui->comboBoxGeneratorEncounterSlot->setup();

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
    ui->comboBoxSearcherEncounterSlot->clear();
    ui->comboBoxSearcherEncounterSlot->addItems(t);
    ui->comboBoxSearcherEncounterSlot->setup();

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
    QVector<bool> flags = encounterGenerator[ui->comboBoxGeneratorLocation->currentIndex()].getSlots(num);

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
    QVector<bool> flags = encounterSearcher[ui->comboBoxSearcherLocation->currentIndex()].getSlots(num);

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
    auto *time = new SeedtoTime4(s->data(s->index(index.row(), 0), Qt::DisplayRole).toString(), profiles[ui->comboBoxProfiles->currentIndex()]);
    time->show();
    time->raise();
}

void Wild4::on_tableViewSearcher_customContextMenuRequested(const QPoint &pos)
{
    if (s->rowCount() == 0)
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


WildSearcher4::WildSearcher4(const Searcher4 &searcher, const QVector<u8> &min, const QVector<u8> &max)
{
    this->searcher = searcher;
    this->min = min;
    this->max = max;
    cancel = false;
    progress = 0;

    connect(this, &WildSearcher4::finished, this, &WildSearcher4::deleteLater);
}

void WildSearcher4::run()
{
    for (u8 a = min[0]; a <= max[0]; a++)
    {
        for (u8 b = min[1]; b <= max[1]; b++)
        {
            for (u8 c = min[2]; c <= max[2]; c++)
            {
                for (u8 d = min[3]; d <= max[3]; d++)
                {
                    for (u8 e = min[4]; e <= max[4]; e++)
                    {
                        for (u8 f = min[5]; f <= max[5]; f++)
                        {
                            if (cancel)
                            {
                                return;
                            }

                            auto frames = searcher.search(a, b, c, d, e, f);
                            progress++;

                            QMutexLocker locker(&mutex);
                            results.append(frames);
                        }
                    }
                }
            }
        }
    }
}

int WildSearcher4::currentProgress() const
{
    return progress;
}

QVector<Frame4> WildSearcher4::getResults()
{
    QMutexLocker locker(&mutex);
    auto data(results);
    results.clear();
    return data;
}

void WildSearcher4::cancelSearch()
{
    cancel = true;
}
