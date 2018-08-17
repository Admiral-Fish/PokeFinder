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

#include "Wild3.hpp"
#include "ui_Wild3.h"

Wild3::Wild3(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Wild3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();

    qRegisterMetaType<QVector<Frame3>>("QVector<Frame3>");
    connect(this, SIGNAL(updateView(QVector<Frame3>)), this, SLOT(updateViewSearcher(QVector<Frame3>)));
    connect(this, &Wild3::updateProgress, this, &Wild3::updateProgressBar);
}

void Wild3::changeEvent(QEvent *event)
{
    if (event)
    {
        switch (event->type())
        {
            case QEvent::LanguageChange:
                ui->retranslateUi(this);
                updateLocationsSearcher();
                updateLocationsGenerator();
                break;
            default:
                break;
        }
    }
}

Wild3::~Wild3()
{
    QSettings setting;
    setting.setValue("wild3Profile", ui->comboBoxProfiles->currentIndex());

    delete ui;
    delete s;
    delete g;
    delete generatorMenu;
    delete searcherMenu;
}

void Wild3::on_checkBoxDelayGenerator_clicked()
{
    if (ui->checkBoxDelayGenerator->isChecked())
    {
        ui->delayGenerator->setEnabled(true);
    }
    else
    {
        ui->delayGenerator->setEnabled(false);
        ui->delayGenerator->setText("");
    }
}

void Wild3::updateViewSearcher(QVector<Frame3> frames)
{
    s->addItems(frames);
}

void Wild3::updateProfiles()
{
    profiles = Profile3::loadProfileList();

    QVector<Profile3> temp;

    for (auto profile : profiles)
        if (profile.getVersion() != Game::Colosseum && profile.getVersion() != Game::Gales)
            temp.append(profile);

    profiles = temp;
    profiles.insert(profiles.begin(), Profile3());

    ui->comboBoxProfiles->clear();

    for (auto profile : profiles)
        ui->comboBoxProfiles->addItem(profile.getProfileName());

    QSettings setting;
    int val = setting.value("wild3Profile").toInt();
    if (val < ui->comboBoxProfiles->count())
        ui->comboBoxProfiles->setCurrentIndex(val);
}

void Wild3::setupModels()
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

    ui->comboBoxMethodGenerator->setItemData(0, Method::MethodH1);
    ui->comboBoxMethodGenerator->setItemData(1, Method::MethodH2);
    ui->comboBoxMethodGenerator->setItemData(2, Method::MethodH4);

    ui->comboBoxMethodSearcher->setItemData(0, Method:: MethodH1);
    ui->comboBoxMethodSearcher->setItemData(1, Method::MethodH2);
    ui->comboBoxMethodSearcher->setItemData(2, Method::MethodH4);

    ui->comboBoxEncounterGenerator->setItemData(0, Encounter::Grass);
    ui->comboBoxEncounterGenerator->setItemData(1, Encounter::SafariZone);
    ui->comboBoxEncounterGenerator->setItemData(2, Encounter::RockSmash);
    ui->comboBoxEncounterGenerator->setItemData(3, Encounter::Surfing);
    ui->comboBoxEncounterGenerator->setItemData(4, Encounter::OldRod);
    ui->comboBoxEncounterGenerator->setItemData(5, Encounter::GoodRod);
    ui->comboBoxEncounterGenerator->setItemData(6, Encounter::SuperRod);

    ui->comboBoxEncounterSearcher->setItemData(0, Encounter::Grass);
    ui->comboBoxEncounterSearcher->setItemData(1, Encounter::SafariZone);
    ui->comboBoxEncounterSearcher->setItemData(2, Encounter::RockSmash);
    ui->comboBoxEncounterSearcher->setItemData(3, Encounter::Surfing);
    ui->comboBoxEncounterSearcher->setItemData(4, Encounter::OldRod);
    ui->comboBoxEncounterSearcher->setItemData(5, Encounter::GoodRod);
    ui->comboBoxEncounterSearcher->setItemData(6, Encounter::SuperRod);

    ui->comboBoxLeadGenerator->addItem(tr("None"));
    ui->comboBoxLeadGenerator->addItems(Nature::getNatures());

    ui->comboBoxLeadSearcher->setItemData(0, Lead::Search);
    ui->comboBoxLeadSearcher->setItemData(1, Lead::Synchronize);
    ui->comboBoxLeadSearcher->setItemData(2, Lead::CuteCharm);
    ui->comboBoxLeadSearcher->setItemData(3, Lead::None);

    ui->comboBoxNatureGenerator->setup();
    ui->comboBoxNatureSearcher->setup();

    ui->comboBoxHiddenPowerGenerator->setup();
    ui->comboBoxHiddenPowerSearcher->setup();

    on_comboBoxEncounterGenerator_currentIndexChanged(0);
    on_comboBoxEncounterSearcher_currentIndexChanged(0);

    QAction *setTargetFrame = new QAction(tr("Set Target Frame"), this);
    QAction *jumpToTarget = new QAction(tr("Jump to Target Frame"), this);
    QAction *centerTo1Second = new QAction(tr("Center to +/- 1 Second and Set as Target Frame"), this);
    QAction *centerTo2Seconds = new QAction(tr("Center to +/- 2 Seconds and Set as Target Frame"), this);
    QAction *centerTo3Seconds = new QAction(tr("Center to +/- 3 Seconds and Set as Target Frame"), this);
    QAction *centerTo5Seconds = new QAction(tr("Center to +/- 5 Seconds and Set as Target Frame"), this);
    QAction *centerTo10Seconds = new QAction(tr("Center to +/- 10 Seconds and Set as Target Frame"), this);
    QAction *centerTo1Minute = new QAction(tr("Center to +/- 1 Minute and Set as Target Frame"), this);
    QAction *outputToTxt = new QAction(tr("Output Results to TXT"), this);
    QAction *outputToCSV = new QAction(tr("Output Results to CSV"), this);

    connect(setTargetFrame, &QAction::triggered, this, &Wild3::setTargetFrameGenerator);
    connect(jumpToTarget, &QAction::triggered, this, &Wild3::jumpToTargetGenerator);
    connect(centerTo1Second, &QAction::triggered, this, [ = ]()
    {
        centerFramesAndSetTargetGenerator(60);
    });
    connect(centerTo2Seconds, &QAction::triggered, this, [ = ]()
    {
        centerFramesAndSetTargetGenerator(120);
    });
    connect(centerTo3Seconds, &QAction::triggered, this, [ = ]()
    {
        centerFramesAndSetTargetGenerator(180);
    });
    connect(centerTo5Seconds, &QAction::triggered, this, [ = ]()
    {
        centerFramesAndSetTargetGenerator(300);
    });
    connect(centerTo10Seconds, &QAction::triggered, this, [ = ]()
    {
        centerFramesAndSetTargetGenerator(600);
    });
    connect(centerTo1Minute, &QAction::triggered, this, [ = ]()
    {
        centerFramesAndSetTargetGenerator(3600);
    });
    connect(outputToTxt, &QAction::triggered, this, &Wild3::outputToTxt);
    connect(outputToCSV, &QAction::triggered, this, &Wild3::outputToCSV);

    generatorMenu->addAction(setTargetFrame);
    generatorMenu->addAction(jumpToTarget);
    generatorMenu->addSeparator();
    generatorMenu->addAction(centerTo1Second);
    generatorMenu->addAction(centerTo2Seconds);
    generatorMenu->addAction(centerTo3Seconds);
    generatorMenu->addAction(centerTo5Seconds);
    generatorMenu->addAction(centerTo10Seconds);
    generatorMenu->addAction(centerTo1Minute);
    generatorMenu->addSeparator();
    generatorMenu->addAction(outputToTxt);
    generatorMenu->addAction(outputToCSV);

    QAction *copySeedToClipboard = new QAction(tr("Copy Seed to Clipboard"), this);
    QAction *seedToTime = new QAction(tr("Generate times for seed"), this);

    connect(copySeedToClipboard, &QAction::triggered, this, &Wild3::copySeedToClipboard);
    connect(seedToTime, &QAction::triggered, this, &Wild3::seedToTime);

    searcherMenu->addAction(copySeedToClipboard);
    searcherMenu->addAction(seedToTime);
}

void Wild3::on_anyNatureGenerator_clicked()
{
    ui->comboBoxNatureGenerator->uncheckAll();
}

void Wild3::on_anyHiddenPowerGenerator_clicked()
{
    ui->comboBoxHiddenPowerGenerator->uncheckAll();
}

void Wild3::on_anyNatureSearcher_clicked()
{
    ui->comboBoxNatureSearcher->uncheckAll();
}

void Wild3::on_anyHiddenPowerSearcher_clicked()
{
    ui->comboBoxHiddenPowerSearcher->uncheckAll();
}

void Wild3::on_anySlotGenerator_clicked()
{
    ui->comboBoxSlotGenerator->uncheckAll();
}

void Wild3::on_pushButton_clicked()
{
    ui->comboBoxSlotSearcher->uncheckAll();
}

void Wild3::on_comboBoxProfiles_currentIndexChanged(int index)
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

    updateLocationsSearcher();
    updateLocationsGenerator();
}

void Wild3::refreshProfiles()
{
    emit alertProfiles(3);
}

void Wild3::on_generate_clicked()
{
    g->clear();

    u32 seed = ui->initialSeedGenerator->text().toUInt(nullptr, 16);
    u32 startingFrame = ui->startingFrameGenerator->text().toUInt();
    u32 maxResults = ui->maxResultsGenerator->text().toUInt();
    u16 tid = ui->idGenerator->text().toUShort();
    u16 sid = ui->sidGenerator->text().toUShort();
    u32 offset = 0;
    if (ui->checkBoxDelayGenerator->isChecked())
        offset = ui->delayGenerator->text().toUInt();

    int genderRatioIndex = ui->comboBoxGenderRatioGenerator->currentIndex();
    Generator3 generator = Generator3(maxResults, startingFrame, seed, tid, sid, offset);
    FrameCompare compare = FrameCompare(ui->ivFilterGenerator->getEvals(), ui->ivFilterGenerator->getValues(),
                                        ui->comboBoxGenderGenerator->currentIndex(), genderRatioIndex, ui->comboBoxAbilityGenerator->currentIndex(),
                                        ui->comboBoxNatureGenerator->getChecked(), ui->comboBoxHiddenPowerGenerator->getChecked(),
                                        ui->checkBoxShinyGenerator->isChecked(), ui->checkBoxDisableGenerator->isChecked(), ui->comboBoxSlotGenerator->getChecked());

    generator.setup(static_cast<Method>(ui->comboBoxMethodGenerator->currentData().toInt()));
    generator.setEncounterType(static_cast<Encounter>(ui->comboBoxEncounterGenerator->currentData().toInt()));
    if (ui->pushButtonLeadGenerator->text() == tr("Cute Charm"))
        generator.setLeadType(static_cast<Lead>(ui->comboBoxLeadGenerator->currentData().toInt()));
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

    QVector<Frame3> frames = generator.generate(compare);
    g->setModel(frames);
}

void Wild3::on_search_clicked()
{
    if (isSearching == true)
    {
        cancel = true;
    }
    else
    {
        s->clear();
        s->setMethod(static_cast<Method>(ui->comboBoxMethodGenerator->currentData().toInt()));

        ui->progressBar->setValue(0);
        progress = 0;

        isSearching = true;
        cancel = false;
        ui->search->setText(tr("Cancel"));

        std::thread job(&Wild3::search, this);
        job.detach();

        std::thread update(&Wild3::updateSearch, this);
        update.detach();
    }
}

void Wild3::search()
{
    u16 tid = ui->idSearcher->text().toUShort();
    u16 sid = ui->sidSearcher->text().toUShort();

    int genderRatioIndex = ui->comboBoxGenderRatioSearcher->currentIndex();
    FrameCompare compare = FrameCompare(ui->ivFilterSearcher->getEvals(), ui->ivFilterSearcher->getValues(), ui->comboBoxGenderSearcher->currentIndex(),
                                        genderRatioIndex, ui->comboBoxAbilitySearcher->currentIndex(), ui->comboBoxNatureSearcher->getChecked(),
                                        ui->comboBoxHiddenPowerSearcher->getChecked(), ui->checkBoxShinySearcher->isChecked(), false,
                                        ui->comboBoxSlotSearcher->getChecked());
    Searcher3 searcher = Searcher3(tid, sid, static_cast<u32>(genderRatioIndex), compare);

    searcher.setup(static_cast<Method>(ui->comboBoxMethodSearcher->currentData().toInt()));
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
                            QVector<Frame3> frames = searcher.search(a, b, c, d, e, f);

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

void Wild3::updateSearch()
{
    while (isSearching && !cancel)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        emit updateProgress();
    }
}

void Wild3::updateLocationsSearcher()
{
    Encounter encounter = static_cast<Encounter>(ui->comboBoxEncounterSearcher->currentData().toInt());
    Game game = Game::Emerald;

    if (ui->comboBoxProfiles->currentIndex() > 0)
        game = profiles[ui->comboBoxProfiles->currentIndex()].getVersion();

    encounterSearcher = EncounterArea3::getEncounters(encounter, game);
    QVector<int> locs;
    for (EncounterArea3 area : encounterSearcher)
        locs.append(area.getLocation());

    QStringList locations = Translator::getLocationsGen3(locs);

    ui->comboBoxLocationSearcher->clear();
    ui->comboBoxLocationSearcher->addItems(locations);
}

void Wild3::updatePokemonSearcher()
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

void Wild3::updateLocationsGenerator()
{
    Encounter encounter = static_cast<Encounter>(ui->comboBoxEncounterGenerator->currentData().toInt());
    Game game = Game::Emerald;

    if (ui->comboBoxProfiles->currentIndex() > 0)
        game = profiles[ui->comboBoxProfiles->currentIndex()].getVersion();

    encounterGenerator = EncounterArea3::getEncounters(encounter, game);
    QVector<int> locs;
    for (EncounterArea3 area : encounterGenerator)
        locs.append(area.getLocation());

    QStringList locations = Translator::getLocationsGen3(locs);

    ui->comboBoxLocationGenerator->clear();
    ui->comboBoxLocationGenerator->addItems(locations);
}

void Wild3::updatePokemonGenerator()
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

void Wild3::updateProgressBar()
{
    ui->progressBar->setValue(progress);
}

void Wild3::centerFramesAndSetTargetGenerator(u32 centerFrames)
{
    ui->checkBoxDisableGenerator->setChecked(true);

    u32 frameNumber = ui->tableViewGenerator->model()->data(ui->tableViewGenerator->model()->index(lastIndex.row(), 0)).toString().toUInt();

    u32 startingFrame = frameNumber < centerFrames + 1 ? 1 : frameNumber - centerFrames;
    u32 selectedIndex = frameNumber < centerFrames + 1 ? frameNumber - 1U : centerFrames;
    u32 maxFrames = frameNumber < centerFrames + 1 ? frameNumber - 1 + centerFrames + 1 : centerFrames * 2 + 1;

    ui->startingFrameGenerator->setText(QString::number(startingFrame));
    ui->maxResultsGenerator->setText(QString::number(maxFrames));

    on_generate_clicked();

    targetFrame = ui->tableViewGenerator->model()->index(static_cast<int>(selectedIndex), 0);

    jumpToTargetGenerator();
}

void Wild3::seedToTime()
{
    u32 seed = s->data(s->index(lastIndex.row(), 0), Qt::DisplayRole).toString().toUInt(nullptr, 16);
    SeedToTime3 *seedToTime = new SeedToTime3(seed);
    seedToTime->show();
    seedToTime->raise();
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

void Wild3::outputToTxt()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Output to TXT"), "", tr("Text File (*.txt);;All Files (*)"));

    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
        return;

    QString textData = "";
    int rows = g->rowCount();
    int columns = g->columnCount();

    for (int i = 0; i < columns; i++)
    {
        textData += g->headerData(i, Qt::Horizontal, 0).toString();
        if (i == 3 || i == 13)
            textData += "\t\t";
        else
            textData += "\t";
    }

    textData += "\r\n";

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            textData += (g->data(g->index(i, j), 0).toString() != "" ? g->data(g->index(i, j), 0).toString() + "\t" : "-\t");
            if (j == 1 || (j == 13 && g->data(g->index(i, j), 0).toString().length() < 8))
            {
                textData += "\t";
            }
        }
        textData += "\r\n";             // (optional: for new line segmentation)
    }

    QTextStream out(&file);
    out << textData;
    file.close();
}

void Wild3::outputToCSV()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Output to CSV"), "", tr("CSV File (*.csv);;All Files (*)"));

    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
        return;

    QString textData = "";
    int rows = g->rowCount();
    int columns = g->columnCount();

    for (int i = 0; i < columns; i++)
    {
        textData += g->headerData(i, Qt::Horizontal, 0).toString();
        if (i != columns - 1)
            textData += ", ";
    }

    textData += "\n";

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            textData += g->data(g->index(i, j), 0).toString();
            if (j != columns - 1)
                textData += ", ";
        }
        textData += "\n";             // (optional: for new line segmentation)
    }

    QTextStream out(&file);
    out << textData;
    file.close();
}

void Wild3::on_tableViewGenerator_customContextMenuRequested(const QPoint &pos)
{
    if (g->rowCount() == 0)
        return;

    lastIndex = ui->tableViewGenerator->indexAt(pos);

    generatorMenu->popup(ui->tableViewGenerator->viewport()->mapToGlobal(pos));
}

void Wild3::on_tableViewSearcher_customContextMenuRequested(const QPoint &pos)
{
    if (s->rowCount() == 0)
        return;

    lastIndex = ui->tableViewSearcher->indexAt(pos);

    searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
}

void Wild3::copySeedToClipboard()
{
    QApplication::clipboard()->setText(s->data(s->index(lastIndex.row(), 0), Qt::DisplayRole).toString());
}

void Wild3::on_pushButtonLeadGenerator_clicked()
{
    ui->comboBoxLeadGenerator->clear();
    QString text = ui->pushButtonLeadGenerator->text();
    if (text == tr("Synchronize"))
    {
        ui->pushButtonLeadGenerator->setText(tr("Cute Charm"));

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

void Wild3::on_pushButtonProfileManager_clicked()
{
    ProfileManager3 *manager = new ProfileManager3();
    connect(manager, SIGNAL(updateProfiles()), this, SLOT(refreshProfiles()));
    manager->show();
}

void Wild3::on_comboBoxEncounterGenerator_currentIndexChanged(int index)
{
    (void)index;
    QStringList t;
    Encounter encounter = static_cast<Encounter>(ui->comboBoxEncounterGenerator->currentData().toInt());

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
    ui->comboBoxSlotGenerator->clear();
    ui->comboBoxSlotGenerator->addItems(t);
    ui->comboBoxSlotGenerator->setup();

    updateLocationsGenerator();
}

void Wild3::on_comboBoxEncounterSearcher_currentIndexChanged(int index)
{
    (void)index;
    QStringList t;
    Encounter encounter = static_cast<Encounter>(ui->comboBoxEncounterSearcher->currentData().toInt());

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
    ui->comboBoxSlotSearcher->clear();
    ui->comboBoxSlotSearcher->addItems(t);
    ui->comboBoxSlotSearcher->setup();

    updateLocationsSearcher();
}

void Wild3::on_comboBoxLocationSearcher_currentIndexChanged(int index)
{
    (void) index;
    updatePokemonSearcher();
}

void Wild3::on_comboBoxPokemonSearcher_currentIndexChanged(int index)
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

void Wild3::on_comboBoxLocationGenerator_currentIndexChanged(int index)
{
    if (index >= 0)
    {
        ui->checkBoxDelayGenerator->setChecked(true);
        ui->delayGenerator->setText(QString::number(encounterGenerator[index].getDelay()));
    }
    updatePokemonGenerator();
}

void Wild3::on_comboBoxPokemonGenerator_currentIndexChanged(int index)
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
