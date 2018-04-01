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

#include "Stationary3.hpp"
#include "ui_Stationary3.h"

Stationary3::Stationary3(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Stationary3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    ui->label->setVisible(false);
    ui->comboBoxShadow->setVisible(false);

    updateProfiles();
    setupModels();

    qRegisterMetaType<vector<Frame3>>("vector<Frame3>");
    connect(this, SIGNAL(updateView(vector<Frame3>)), this, SLOT(updateViewSearcher(vector<Frame3>)));
    connect(this, &Stationary3::updateProgress, this, &Stationary3::updateProgressBar);
}

Stationary3::~Stationary3()
{
    QSettings setting;
    setting.setValue("stationary3Profile", ui->comboBoxProfiles->currentIndex());

    delete ui;
    delete s;
    delete g;
    delete generatorMenu;
    delete searcherMenu;
}

void Stationary3::changeEvent(QEvent *event)
{
    if (event != NULL)
    {
        switch (event->type())
        {
            case QEvent::LanguageChange:
                ui->retranslateUi(this);
                on_comboBoxMethodSearcher_currentIndexChanged(ui->comboBoxMethodSearcher->currentIndex());
                break;
            default:
                break;
        }
    }
}

void Stationary3::setupModels()
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

    ui->comboBoxMethodGenerator->setItemData(0, Method1);
    ui->comboBoxMethodGenerator->setItemData(1, Method1Reverse);
    ui->comboBoxMethodGenerator->setItemData(2, Method2);
    ui->comboBoxMethodGenerator->setItemData(3, Method4);
    ui->comboBoxMethodGenerator->setItemData(4, XDColo);
    ui->comboBoxMethodGenerator->setItemData(5, Channel);

    ui->comboBoxMethodSearcher->setItemData(0, Method1);
    ui->comboBoxMethodSearcher->setItemData(1, Method1Reverse);
    ui->comboBoxMethodSearcher->setItemData(2, Method2);
    ui->comboBoxMethodSearcher->setItemData(3, Method4);
    ui->comboBoxMethodSearcher->setItemData(4, XDColo);
    ui->comboBoxMethodSearcher->setItemData(5, XD);
    ui->comboBoxMethodSearcher->setItemData(6, Colo);
    ui->comboBoxMethodSearcher->setItemData(7, Channel);

    ui->comboBoxNatureGenerator->setup();
    ui->comboBoxNatureSearcher->setup();

    ui->comboBoxHiddenPowerGenerator->setup();
    ui->comboBoxHiddenPowerSearcher->setup();

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

    connect(setTargetFrame, &QAction::triggered, this, &Stationary3::setTargetFrameGenerator);
    connect(jumpToTarget, &QAction::triggered, this, &Stationary3::jumpToTargetGenerator);
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
    connect(outputToTxt, &QAction::triggered, this, &Stationary3::outputToTxt);
    connect(outputToCSV, &QAction::triggered, this, &Stationary3::outputToCSV);

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

    connect(copySeedToClipboard, &QAction::triggered, this, &Stationary3::copySeedToClipboard);

    searcherMenu->addAction(copySeedToClipboard);
}

void Stationary3::refreshProfiles()
{
    emit alertProfiles(3);
}

void Stationary3::updateProfiles()
{
    profiles = Profile3::loadProfileList();

    ui->comboBoxProfiles->clear();

    ui->comboBoxProfiles->addItem(tr("None"));
    for (int i = 0; i < (int)profiles.size(); i++)
        ui->comboBoxProfiles->addItem(profiles.at(i).profileName);

    QSettings setting;
    int val = setting.value("stationary3Profile").toInt();
    if (val < ui->comboBoxProfiles->count())
        ui->comboBoxProfiles->setCurrentIndex(val);
}

void Stationary3::on_comboBoxProfiles_currentIndexChanged(int index)
{
    if (index <= 0)
    {
        ui->idGenerator->setText("12345");
        ui->sidGenerator->setText("54321");
        ui->idSearcher->setText("12345");
        ui->sidSearcher->setText("54321");
        ui->profileTID->setText("12345");
        ui->profileSID->setText("54321");
        ui->profileGame->setText(tr("Emerald"));
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
}

void Stationary3::on_anyNatureGenerator_clicked()
{
    ui->comboBoxNatureGenerator->uncheckAll();
}

void Stationary3::on_anyHiddenPowerGenerator_clicked()
{
    ui->comboBoxHiddenPowerGenerator->uncheckAll();
}

void Stationary3::on_anyNatureSearcher_clicked()
{
    ui->comboBoxNatureSearcher->uncheckAll();
}

void Stationary3::on_anyHiddenPowerSearcher_clicked()
{
    ui->comboBoxHiddenPowerSearcher->uncheckAll();
}

void Stationary3::updateViewSearcher(vector<Frame3> frames)
{
    s->addItems(frames);
}

void Stationary3::on_checkBoxDelayGenerator_clicked()
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

void Stationary3::on_generate_clicked()
{
    g->clear();

    u32 seed = ui->initialSeedGenerator->text().toUInt(NULL, 16);
    u32 startingFrame = ui->startingFrameGenerator->text().toUInt(NULL, 10);
    u32 maxResults = ui->maxResultsGenerator->text().toUInt(NULL, 10);
    u32 tid = ui->idGenerator->text().toUInt(NULL, 10);
    u32 sid = ui->sidGenerator->text().toUInt(NULL, 10);
    u32 offset = 0;
    if (ui->checkBoxDelayGenerator->isChecked())
        offset = ui->delayGenerator->text().toUInt(NULL, 10);

    int genderRatioIndex = ui->comboBoxGenderRatioGenerator->currentIndex();
    Generator3 generator = Generator3(maxResults, startingFrame, seed, tid, sid, offset);
    FrameCompare compare = FrameCompare(ui->ivFilterGenerator->getEvals(), ui->ivFilterGenerator->getValues(),
                                        ui->comboBoxGenderGenerator->currentIndex(), genderRatioIndex, ui->comboBoxAbilityGenerator->currentIndex(),
                                        ui->comboBoxNatureGenerator->getChecked(), ui->comboBoxHiddenPowerGenerator->getChecked(),
                                        ui->checkBoxShinyGenerator->isChecked(), ui->checkBoxDisableGenerator->isChecked());

    generator.setup((Method)ui->comboBoxMethodGenerator->currentData().toInt(NULL));

    vector<Frame3> frames = generator.generate(compare);
    g->setModel(frames);
}

void Stationary3::search()
{
    u32 tid = ui->idSearcher->text().toUInt(NULL, 10);
    u32 sid = ui->sidSearcher->text().toUInt(NULL, 10);

    int genderRatioIndex = ui->comboBoxGenderRatioSearcher->currentIndex();
    FrameCompare compare = FrameCompare(ui->ivFilterSearcher->getEvals(), ui->ivFilterSearcher->getValues(), ui->comboBoxGenderSearcher->currentIndex(),
                                        genderRatioIndex, ui->comboBoxAbilitySearcher->currentIndex(), ui->comboBoxNatureSearcher->getChecked(),
                                        ui->comboBoxHiddenPowerSearcher->getChecked(), ui->checkBoxShinySearcher->isChecked(), false);
    Searcher3 searcher = Searcher3(tid, sid, genderRatioIndex, compare);

    searcher.setup((Method)ui->comboBoxMethodSearcher->currentData().toInt(NULL));
    if (searcher.frameType == XD || searcher.frameType == Colo)
        searcher.setupNatureLock(ui->comboBoxShadow->currentIndex());

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
                            vector<Frame3> frames = searcher.search(a, b, c, d, e, f);

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

void Stationary3::updateSearch()
{
    while (isSearching && !cancel)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        emit updateProgress();
    }
}

void Stationary3::on_search_clicked()
{
    if (isSearching)
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

        std::thread job(&Stationary3::search, this);
        job.detach();

        std::thread update(&Stationary3::updateSearch, this);
        update.detach();
    }
}

void Stationary3::moveResults(QString seed, QString method, u32 hp, u32 atk, u32 def, u32 spa, u32 spd, u32 spe)
{
    if (seed != "")
        ui->initialSeedGenerator->setText(seed);
    for (auto i = 0; i < ui->comboBoxMethodGenerator->model()->rowCount(); i++)
    {
        if (ui->comboBoxMethodGenerator->model()->data(ui->comboBoxMethodGenerator->model()->index(i, 0)).toString() == method)
        {
            ui->comboBoxMethodGenerator->setCurrentIndex(i);
            break;
        }
    }
    ui->ivFilterGenerator->setValues(hp, atk, def, spa, spd, spe);
}

void Stationary3::on_comboBoxMethodSearcher_currentIndexChanged(int index)
{
    (void) index;
    Method method = (Method)ui->comboBoxMethodSearcher->currentData().toInt(NULL);
    ui->comboBoxShadow->clear();

    if (method == XD)
    {
        QStringList s = Translator::getSpecies({ 334, 24, 354, 12, 113, 301, 85, 149, 51, 355, 125, 83, 55, 88, 58, 316,
                                               316, 316, 107, 106, 97, 115, 131, 165, 108, 337, 219, 126, 82, 296, 310,
                                               105, 303, 52, 122, 177, 299, 322, 46, 17, 204, 127, 62, 261, 57, 280, 78,
                                               20, 315, 302, 373, 123, 273, 273, 273, 86, 285, 143, 361, 338, 21, 363, 363,
                                               363, 167, 121, 220, 114, 49, 100, 37, 70
                                               });

        s[15] += tr(" (Citadark)");
        s[16] += tr(" (Initial)");
        s[17] += tr(" (Phenac)");
        s[52] += tr(" (Citadark)");
        s[53] += tr(" (Initial)");
        s[54] += tr(" (Phenac)");
        s[61] += tr(" (Citadark)");
        s[62] += tr(" (Initial)");
        s[63] += tr(" (Phenac)");

        ui->comboBoxShadow->addItems(s);
        ui->comboBoxShadow->setVisible(true);
        ui->label->setVisible(true);
    }
    else if (method == Colo)
    {
        QStringList s = Translator::getSpecies({207, 214, 296, 198, 217});
        ui->comboBoxShadow->addItems(s);
        ui->comboBoxShadow->setVisible(true);
        ui->label->setVisible(true);
    }
    else
    {
        ui->comboBoxShadow->setVisible(false);
        ui->label->setVisible(false);
    }
}

void Stationary3::centerFramesAndSetTargetGenerator(u32 centerFrames)
{
    ui->checkBoxDisableGenerator->setChecked(true);

    u32 frameNumber = ui->tableViewGenerator->model()->data(ui->tableViewGenerator->model()->index(lastIndex.row(), 0)).toString().toUInt();

    u32 startingFrame = frameNumber < centerFrames + 1U ? 1U : frameNumber - centerFrames;
    u32 selectedIndex = frameNumber < centerFrames + 1U ? frameNumber - 1U : centerFrames;
    u32 maxFrames = frameNumber < centerFrames + 1U ? frameNumber - 1U + centerFrames + 1 : centerFrames * 2 + 1;

    ui->startingFrameGenerator->setText(QString::number(startingFrame));
    ui->maxResultsGenerator->setText(QString::number(maxFrames));

    on_generate_clicked();

    targetFrame = ui->tableViewGenerator->model()->index(selectedIndex, 0);

    jumpToTargetGenerator();
}

void Stationary3::setTargetFrameGenerator()
{
    targetFrame = lastIndex;
}

void Stationary3::jumpToTargetGenerator()
{
    ui->tableViewGenerator->scrollTo(targetFrame, QAbstractItemView::PositionAtTop);
    ui->tableViewGenerator->selectionModel()->select(targetFrame, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

void Stationary3::outputToTxt()
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
        if (i == 1 || i == 11)
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
            if (j == 11 && g->data(g->index(i, j), 0).toString().length() < 8)
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

void Stationary3::outputToCSV()
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
            textData += (g->data(g->index(i, j), 0).toString() != "" ? g->data(g->index(i, j), 0).toString() + "\t" : "-\t");
            if (j != columns - 1)
                textData += ", ";
        }
        textData += "\n";             // (optional: for new line segmentation)
    }

    QTextStream out(&file);
    out << textData;
    file.close();
}

void Stationary3::copySeedToClipboard()
{
    QApplication::clipboard()->setText(ui->tableViewSearcher->model()->data(ui->tableViewSearcher->model()->index(lastIndex.row(), 0)).toString());
}

void Stationary3::updateProgressBar()
{
    ui->progressBar->setValue(progress);
}

void Stationary3::on_tableViewGenerator_customContextMenuRequested(const QPoint &pos)
{
    if (g->rowCount() == 0)
        return;

    lastIndex = ui->tableViewGenerator->indexAt(pos);

    generatorMenu->popup(ui->tableViewGenerator->viewport()->mapToGlobal(pos));
}

void Stationary3::on_tableViewSearcher_customContextMenuRequested(const QPoint &pos)
{
    if (s->rowCount() == 0)
        return;

    lastIndex = ui->tableViewSearcher->indexAt(pos);

    searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
}

void Stationary3::on_pushButtonProfileManager_clicked()
{
    ProfileManager3 *manager = new ProfileManager3();
    connect(manager, SIGNAL(updateProfiles()), this, SLOT(refreshProfiles()));
    manager->show();
}
