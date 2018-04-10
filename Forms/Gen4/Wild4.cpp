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

    qRegisterMetaType<vector<Frame4>>("vector<Frame4>");
    connect(this, &Wild4::updateView, this, &Wild4::updateViewSearcher);
    connect(this, &Wild4::updateProgress, this, &Wild4::updateProgressBar);
}

void Wild4::changeEvent(QEvent *event)
{
    if (event != NULL)
    {
        switch (event->type())
        {
            case QEvent::LanguageChange:
                ui->retranslateUi(this);
                //updateLocationsSearcher();
                //updateLocationsGenerator();
                break;
            default:
                break;
        }
    }
}

Wild4::~Wild4()
{
    QSettings setting;
    setting.setValue("wild4Profile", ui->comboBoxProfiles->currentIndex());

    delete ui;
    delete g;
    delete s;
}

void Wild4::setupModels()
{
    ui->tableViewGenerator->setModel(g);
    ui->tableViewGenerator->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableViewSearcher->setModel(s);
    ui->tableViewSearcher->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->initialSeedGenerator->setValues(0, 32, false);
    ui->idGenerator->setValues(0, 48, true);
    ui->sidGenerator->setValues(0, 48, true);
    ui->startingFrameGenerator->setValues(1, 32, true);
    ui->maxResultsGenerator->setValues(1, 32, true);
    ui->delayGenerator->setValues(1, 32, true);

    ui->idSearcher->setValues(0, 48, true);
    ui->sidSearcher->setValues(0, 48, true);

    ui->comboBoxMethodGenerator->setItemData(0, MethodJ);
    ui->comboBoxMethodGenerator->setItemData(1, MethodK);
    ui->comboBoxMethodGenerator->setItemData(2, ChainedShiny);

    ui->comboBoxMethodSearcher->setItemData(0, MethodJ);
    ui->comboBoxMethodSearcher->setItemData(1, MethodK);
    ui->comboBoxMethodSearcher->setItemData(2, ChainedShiny);

    ui->comboBoxEncounterGenerator->setItemData(0, Wild);
    ui->comboBoxEncounterGenerator->setItemData(1, Surfing);
    ui->comboBoxEncounterGenerator->setItemData(2, OldRod);
    ui->comboBoxEncounterGenerator->setItemData(3, GoodRod);
    ui->comboBoxEncounterGenerator->setItemData(4, SuperRod);

    ui->comboBoxEncounterSearcher->setItemData(0, Wild);
    ui->comboBoxEncounterSearcher->setItemData(1, Surfing);
    ui->comboBoxEncounterSearcher->setItemData(2, OldRod);
    ui->comboBoxEncounterSearcher->setItemData(3, GoodRod);
    ui->comboBoxEncounterSearcher->setItemData(4, SuperRod);

    ui->comboBoxLeadGenerator->addItem(tr("None"));
    ui->comboBoxLeadGenerator->addItems(Nature::getNatures());

    ui->comboBoxLeadSearcher->setItemData(0, Search);
    ui->comboBoxLeadSearcher->setItemData(1, Synchronize);
    ui->comboBoxLeadSearcher->setItemData(2, CuteCharm);
    ui->comboBoxLeadSearcher->setItemData(3, SuctionCups);
    ui->comboBoxLeadSearcher->setItemData(4, None);

    ui->comboBoxNatureGenerator->setup();
    ui->comboBoxNatureSearcher->setup();

    ui->comboBoxHiddenPowerGenerator->setup();
    ui->comboBoxHiddenPowerSearcher->setup();

    on_comboBoxEncounterGenerator_currentIndexChanged(0);
    on_comboBoxEncounterSearcher_currentIndexChanged(0);
}

void Wild4::updateProfiles()
{
    profiles = Profile4::loadProfileList();

    ui->comboBoxProfiles->clear();

    ui->comboBoxProfiles->addItem(tr("None"));
    for (int i = 0; i < (int)profiles.size(); i++)
        ui->comboBoxProfiles->addItem(profiles.at(i).profileName);

    QSettings setting;
    int val = setting.value("wild4Profile").toInt();
    if (val < ui->comboBoxProfiles->count())
        ui->comboBoxProfiles->setCurrentIndex(val);
}

void Wild4::on_comboBoxProfiles_currentIndexChanged(int index)
{
    if (index <= 0)
    {
        ui->idGenerator->setText("12345");
        ui->sidGenerator->setText("54321");
        ui->idSearcher->setText("12345");
        ui->sidSearcher->setText("54321");
        ui->profileTID->setText("12345");
        ui->profileSID->setText("54321");
        ui->profileGame->setText(tr("Diamond"));
    }
    else
    {
        auto profile = profiles.at(index - 1);
        ui->idGenerator->setText(QString::number(profile.tid));
        ui->sidGenerator->setText(QString::number(profile.sid));
        ui->idSearcher->setText(QString::number(profile.tid));
        ui->sidSearcher->setText(QString::number(profile.sid));
        ui->profileTID->setText(QString::number(profile.tid));
        ui->profileSID->setText(QString::number(profile.sid));
        ui->profileGame->setText(profile.getVersion());
    }
    //updateLocationsSearcher();
    //updateLocationsGenerator();
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

        ui->comboBoxLeadGenerator->addItem(tr("♂ Lead (50% ♀ Target)"), Lead::CuteCharm50F);
        ui->comboBoxLeadGenerator->addItem(tr("♂ Lead (75% ♀ Target)"), Lead::CuteCharm75F);
        ui->comboBoxLeadGenerator->addItem(tr("♂ Lead (25% ♀ Target)"), Lead::CuteCharm25F);
        ui->comboBoxLeadGenerator->addItem(tr("♂ Lead (12.5% ♀ Target)"), Lead::CuteCharm125F);
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
    ProfileManager4 *manager = new ProfileManager4();
    connect(manager, SIGNAL(updateProfiles()), this, SLOT(refreshProfiles()));
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
    QStringList t;
    Encounter encounter = (Encounter)ui->comboBoxEncounterGenerator->currentData().toInt();

    switch (encounter)
    {
        case Wild:
            t << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11";
            break;
        case Surfing:
        case OldRod:
        case GoodRod:
        case SuperRod:
            t << "0" << "1" << "2" << "3" << "4" << "5";
            break;
    }
    ui->comboBoxSlotGenerator->clear();
    ui->comboBoxSlotGenerator->addItems(t);
    ui->comboBoxSlotGenerator->setup();

    //updateLocationsGenerator();
}

void Wild4::on_comboBoxEncounterSearcher_currentIndexChanged(int index)
{
    QStringList t;
    Encounter encounter = (Encounter)ui->comboBoxEncounterSearcher->currentData().toInt();

    switch (encounter)
    {
        case Wild:
            t << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11";
            break;
        case Surfing:
        case OldRod:
        case GoodRod:
        case SuperRod:
            t << "0" << "1" << "2" << "3" << "4" << "5";
            break;
    }
    ui->comboBoxSlotSearcher->clear();
    ui->comboBoxSlotSearcher->addItems(t);
    ui->comboBoxSlotSearcher->setup();

    //updateLocationsSearcher();
}

void Wild4::on_comboBoxLocationSearcher_currentIndexChanged(int index)
{

}

void Wild4::on_comboBoxPokemonSearcher_currentIndexChanged(int index)
{

}

void Wild4::on_comboBoxLocationGenerator_currentIndexChanged(int index)
{

}

void Wild4::on_comboBoxPokemonGenerator_currentIndexChanged(int index)
{

}

void Wild4::on_generate_clicked()
{
    g->clear();
    g->setMethod((Method)ui->comboBoxMethodGenerator->currentData().toInt());

    u32 seed = ui->initialSeedGenerator->text().toUInt(NULL, 16);
    u32 startingFrame = ui->startingFrameGenerator->text().toUInt(NULL, 10);
    u32 maxResults = ui->maxResultsGenerator->text().toUInt(NULL, 10);
    u32 tid = ui->idGenerator->text().toUInt(NULL, 10);
    u32 sid = ui->sidGenerator->text().toUInt(NULL, 10);
    u32 offset = 0;
    if (ui->checkBoxDelayGenerator->isChecked())
        offset = ui->delayGenerator->text().toUInt(NULL, 10);

    int genderRatioIndex = ui->comboBoxGenderRatioGenerator->currentIndex();
    Generator4 generator = Generator4(maxResults, startingFrame, seed, tid, sid, offset, (Method)ui->comboBoxMethodGenerator->currentData().toInt());
    FrameCompare compare = FrameCompare(ui->ivFilterGenerator->getEvals(), ui->ivFilterGenerator->getValues(),
                                        ui->comboBoxGenderGenerator->currentIndex(), genderRatioIndex, ui->comboBoxAbilityGenerator->currentIndex(),
                                        ui->comboBoxNatureGenerator->getChecked(), ui->comboBoxHiddenPowerGenerator->getChecked(),
                                        ui->checkBoxShinyGenerator->isChecked(), ui->checkBoxDisableGenerator->isChecked(), ui->comboBoxSlotGenerator->getChecked());

    generator.encounterType = (Encounter)ui->comboBoxEncounterGenerator->currentData().toInt();
    if (ui->pushButtonLeadGenerator->text() == tr("Cute Charm"))
        generator.leadType = (Lead)ui->comboBoxLeadGenerator->currentData().toInt();
    else if (ui->pushButtonLeadGenerator->text() == tr("Suction Cups"))
        generator.leadType = SuctionCups;
    else
    {
        int num = ui->comboBoxLeadGenerator->currentIndex();
        if (num == 0)
        {
            generator.leadType = None;
        }
        else
        {
            generator.leadType = Synchronize;
            generator.synchNature = Nature::getAdjustedNature(ui->comboBoxLeadGenerator->currentIndex() - 1);
        }
    }

    vector<Frame4> frames = generator.generate(compare);
    g->setModel(frames);
}

void Wild4::search()
{
    u32 tid = ui->idSearcher->text().toUInt(NULL, 10);
    u32 sid = ui->sidSearcher->text().toUInt(NULL, 10);

    int genderRatioIndex = ui->comboBoxGenderRatioSearcher->currentIndex();
    FrameCompare compare = FrameCompare(ui->ivFilterSearcher->getEvals(), ui->ivFilterSearcher->getValues(), ui->comboBoxGenderSearcher->currentIndex(),
                                        genderRatioIndex, ui->comboBoxAbilitySearcher->currentIndex(), ui->comboBoxNatureSearcher->getChecked(),
                                        ui->comboBoxHiddenPowerSearcher->getChecked(), ui->checkBoxShinySearcher->isChecked(), false,
                                        ui->comboBoxSlotSearcher->getChecked());
    Searcher4 searcher = Searcher4(tid, sid, genderRatioIndex, ui->minDelay->text().toUInt(), ui->maxDelay->text().toUInt(), ui->minFrame->text().toUInt(), ui->maxFrame->text().toUInt(), compare, (Method)ui->comboBoxMethodSearcher->currentData().toInt());

    searcher.encounterType = (Encounter)ui->comboBoxEncounterSearcher->currentData().toInt();
    searcher.leadType = (Lead)ui->comboBoxLeadSearcher->currentData().toInt();

    vector<u32> min = ui->ivFilterSearcher->getLower();
    vector<u32> max = ui->ivFilterSearcher->getUpper();

    ui->progressBar->setMaximum((max[0] - min[0] + 1) * (max[1] - min[1] + 1) * (max[2] - min[2] + 1) * (max[3] - min[3] + 1) * (max[4] - min[4] + 1) * (max[5] - min[5] + 1));

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
                            vector<Frame4> frames = searcher.search(a, b, c, d, e, f);

                            if (!frames.empty())
                                emit updateView(frames);

                            progress++;

                            if (cancel)
                            {
                                isSearching = false;
                                ui->search->setText(tr("Search"));
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
    ui->search->setText(tr("Search"));
    emit updateProgress();
}

void Wild4::on_search_clicked()
{
    if (isSearching == true)
    {
        cancel = true;
    }
    else
    {
        s->clear();
        s->setMethod((Method)ui->comboBoxMethodSearcher->currentData().toInt(NULL));

        ui->progressBar->setValue(0);
        progress = 0;

        isSearching = true;
        cancel = false;
        ui->search->setText(tr("Cancel"));

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

void Wild4::updateProgressBar()
{
    ui->progressBar->setValue(progress);
}

void Wild4::updateViewSearcher(vector<Frame4> frames)
{
    s->addItems(frames);
}
