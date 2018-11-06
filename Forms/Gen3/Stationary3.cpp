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

    qRegisterMetaType<QVector<Frame3>>("QVector<Frame3>");
    connect(this, SIGNAL(updateView(QVector<Frame3>)), this, SLOT(updateViewSearcher(QVector<Frame3>)));
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

void Stationary3::setupModels()
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
    QAction *seedToTime = new QAction(tr("Generate times for seed"), this);

    connect(copySeedToClipboard, &QAction::triggered, this, &Stationary3::copySeedToClipboard);
    connect(seedToTime, &QAction::triggered, this, &Stationary3::seedToTime);

    searcherMenu->addAction(copySeedToClipboard);
    searcherMenu->addAction(seedToTime);
}

void Stationary3::refreshProfiles()
{
    emit alertProfiles(3);
}

void Stationary3::updateProfiles()
{
    profiles = Profile3::loadProfileList();
    profiles.insert(profiles.begin(), Profile3());

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
        ui->comboBoxProfiles->addItem(profile.getProfileName());

    QSettings setting;
    int val = setting.value("stationary3Profile").toInt();
    if (val < ui->comboBoxProfiles->count())
        ui->comboBoxProfiles->setCurrentIndex(val);
}

void Stationary3::on_comboBoxProfiles_currentIndexChanged(int index)
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

    bool flag = profile.getVersion() == Game::Gales || profile.getVersion() == Game::Colosseum;

    ui->comboBoxMethodGenerator->clear();
    ui->comboBoxMethodSearcher->clear();
    if (flag)
    {
        ui->comboBoxMethodGenerator->addItem(tr("XD/Colo"), Method::XDColo);
        ui->comboBoxMethodGenerator->addItem(tr("Channel"), Method::Channel);
        ui->comboBoxMethodSearcher->addItem(tr("XD/Colo"), Method::XDColo);
        ui->comboBoxMethodSearcher->addItem(tr("Gales"), Method::XD);
        ui->comboBoxMethodSearcher->addItem(tr("Colo"), Method::Colo);
        ui->comboBoxMethodSearcher->addItem(tr("Channel"), Method::Channel);
    }
    else
    {
        ui->comboBoxMethodGenerator->addItem(tr("Method 1"), Method::Method1);
        ui->comboBoxMethodGenerator->addItem(tr("Method 1 Reverse"), Method::Method1Reverse);
        ui->comboBoxMethodGenerator->addItem(tr("Method 2"), Method::Method2);
        ui->comboBoxMethodGenerator->addItem(tr("Method 4"), Method::Method4);
        ui->comboBoxMethodSearcher->addItem(tr("Method 1"), Method::Method1);
        ui->comboBoxMethodSearcher->addItem(tr("Method 1 Reverse"), Method::Method1Reverse);
        ui->comboBoxMethodSearcher->addItem(tr("Method 2"), Method::Method2);
        ui->comboBoxMethodSearcher->addItem(tr("Method 4"), Method::Method4);
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

void Stationary3::updateViewSearcher(QVector<Frame3> frames)
{
    s->addItems(std::move(frames));
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
                                        ui->checkBoxShinyGenerator->isChecked(), ui->checkBoxDisableGenerator->isChecked());

    generator.setup(static_cast<Method>(ui->comboBoxMethodGenerator->currentData().toInt()));

    QVector<Frame3> frames = generator.generate(compare);
    g->setModel(frames);
}

void Stationary3::search()
{
    u16 tid = ui->idSearcher->text().toUShort();
    u16 sid = ui->sidSearcher->text().toUShort();

    int genderRatioIndex = ui->comboBoxGenderRatioSearcher->currentIndex();
    FrameCompare compare = FrameCompare(ui->ivFilterSearcher->getEvals(), ui->ivFilterSearcher->getValues(), ui->comboBoxGenderSearcher->currentIndex(),
                                        genderRatioIndex, ui->comboBoxAbilitySearcher->currentIndex(), ui->comboBoxNatureSearcher->getChecked(),
                                        ui->comboBoxHiddenPowerSearcher->getChecked(), ui->checkBoxShinySearcher->isChecked(), false);
    Searcher3 searcher = Searcher3(tid, sid, static_cast<u32>(genderRatioIndex), compare);

    searcher.setup(static_cast<Method>(ui->comboBoxMethodSearcher->currentData().toInt()));
    if (searcher.getFrameType() == Method::XD || searcher.getFrameType() == Method::Colo)
        searcher.setupNatureLock(ui->comboBoxShadow->currentIndex());

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
        s->setMethod(static_cast<Method>(ui->comboBoxMethodSearcher->currentData().toInt()));

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

void Stationary3::moveResults(const QString &seed, const QString &method, u32 hp, u32 atk, u32 def, u32 spa, u32 spd, u32 spe)
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
    Method method = static_cast<Method>(ui->comboBoxMethodSearcher->currentData().toInt());
    ui->comboBoxShadow->clear();

    if (method == Method::XD)
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
    else if (method == Method::Colo)
    {
        QStringList s = Translator::getSpecies({ 207, 214, 296, 179, 198, 212, 175, 217 });
        s[3] += tr(" (E-Reader)");
        s[5] += tr(" (E-Reader)");
        s[6] += tr(" (E-Reader)");
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

    u32 startingFrame = frameNumber < centerFrames + 1 ? 1 : frameNumber - centerFrames;
    u32 selectedIndex = frameNumber < centerFrames + 1 ? frameNumber - 1 : centerFrames;
    u32 maxFrames = frameNumber < centerFrames + 1 ? frameNumber - 1 + centerFrames + 1 : centerFrames * 2 + 1;

    ui->startingFrameGenerator->setText(QString::number(startingFrame));
    ui->maxResultsGenerator->setText(QString::number(maxFrames));

    on_generate_clicked();

    targetFrame = ui->tableViewGenerator->model()->index(static_cast<int>(selectedIndex), 0);

    jumpToTargetGenerator();
}

void Stationary3::seedToTime()
{
    u32 seed = s->data(s->index(lastIndex.row(), 0), Qt::DisplayRole).toString().toUInt(nullptr, 16);
    auto *seedToTime = new SeedToTime3(seed);
    seedToTime->show();
    seedToTime->raise();
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
    QApplication::clipboard()->setText(s->data(s->index(lastIndex.row(), 0), Qt::DisplayRole).toString());
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
    auto *manager = new ProfileManager3();
    connect(manager, &ProfileManager3::updateProfiles, this, &Stationary3::refreshProfiles);
    manager->show();
}
