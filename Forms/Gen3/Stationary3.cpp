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

#include "Stationary3.hpp"
#include "ui_Stationary3.h"

Stationary3::Stationary3(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Stationary3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    ui->labelSearcherShadow->setVisible(false);
    ui->comboBoxSearcherShadow->setVisible(false);

    updateProfiles();
    setupModels();

    qRegisterMetaType<QVector<Frame3>>("QVector<Frame3>");
}

Stationary3::~Stationary3()
{
    QSettings setting;
    setting.setValue("stationary3Profile", ui->comboBoxProfiles->currentIndex());

    delete ui;
}

void Stationary3::updateProfiles()
{
    profiles = Profile3::loadProfileList();
    profiles.insert(profiles.begin(), Profile3());

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(profile.getProfileName());
    }

    QSettings setting;
    int val = setting.value("stationary3Profile").toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val >= 0 ? val : 0);
    }
}

void Stationary3::setupModels()
{
    generatorModel = new Stationary3Model(ui->tableViewGenerator);
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

    ui->comboBoxGeneratorNature->setup();
    ui->comboBoxSearcherNature->setup();

    ui->comboBoxGeneratorHiddenPower->setup();
    ui->comboBoxSearcherHiddenPower->setup();

    QAction *setTargetFrame = generatorMenu->addAction(tr("Set Target Frame"));
    QAction *jumpToTarget = generatorMenu->addAction(tr("Jump to Target Frame"));
    QAction *centerTo1Second = generatorMenu->addAction(tr("Center to +/- 1 Second and Set as Target Frame"));
    QAction *centerTo2Seconds = generatorMenu->addAction(tr("Center to +/- 2 Seconds and Set as Target Frame"));
    QAction *centerTo3Seconds = generatorMenu->addAction(tr("Center to +/- 3 Seconds and Set as Target Frame"));
    QAction *centerTo5Seconds = generatorMenu->addAction(tr("Center to +/- 5 Seconds and Set as Target Frame"));
    QAction *centerTo10Seconds = generatorMenu->addAction(tr("Center to +/- 10 Seconds and Set as Target Frame"));
    QAction *centerTo1Minute = generatorMenu->addAction(tr("Center to +/- 1 Minute and Set as Target Frame"));
    QAction *outputToTxt = generatorMenu->addAction(tr("Output Results to TXT"));
    QAction *outputToCSV = generatorMenu->addAction(tr("Output Results to CSV"));

    connect(setTargetFrame, &QAction::triggered, this, &Stationary3::setTargetFrameGenerator);
    connect(jumpToTarget, &QAction::triggered, this, &Stationary3::jumpToTargetGenerator);
    connect(centerTo1Second, &QAction::triggered, this, [ = ]() { centerFramesAndSetTargetGenerator(60); });
    connect(centerTo2Seconds, &QAction::triggered, this, [ = ]() { centerFramesAndSetTargetGenerator(120); });
    connect(centerTo3Seconds, &QAction::triggered, this, [ = ]() { centerFramesAndSetTargetGenerator(180); });
    connect(centerTo5Seconds, &QAction::triggered, this, [ = ]() { centerFramesAndSetTargetGenerator(300); });
    connect(centerTo10Seconds, &QAction::triggered, this, [ = ]() { centerFramesAndSetTargetGenerator(600); });
    connect(centerTo1Minute, &QAction::triggered, this, [ = ]() { centerFramesAndSetTargetGenerator(3600); });
    connect(outputToTxt, &QAction::triggered, this, &Stationary3::outputToTxt);
    connect(outputToCSV, &QAction::triggered, this, &Stationary3::outputToCSV);

    QAction *copySeedToClipboard = searcherMenu->addAction(tr("Copy Seed to Clipboard"));
    QAction *seedToTime = searcherMenu->addAction(tr("Generate times for seed"));

    connect(copySeedToClipboard, &QAction::triggered, this, &Stationary3::copySeedToClipboard);
    connect(seedToTime, &QAction::triggered, this, &Stationary3::seedToTime);
}

void Stationary3::updateView(const QVector<Frame3> &frames, int progress)
{
    searcherModel->addItems(frames);
    ui->progressBar->setValue(progress);
}

void Stationary3::moveResults(const QString &seed, const QString &method, u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe)
{
    if (!seed.isEmpty())
    {
        ui->textBoxGeneratorSeed->setText(seed);
    }

    for (auto i = 0; i < ui->comboBoxGeneratorMethod->model()->rowCount(); i++)
    {
        if (ui->comboBoxGeneratorMethod->model()->data(ui->comboBoxGeneratorMethod->model()->index(i, 0)).toString() == method)
        {
            ui->comboBoxGeneratorMethod->setCurrentIndex(i);
            break;
        }
    }
    ui->ivFilterGenerator->setValues(hp, atk, def, spa, spd, spe);
}

void Stationary3::refreshProfiles()
{
    emit alertProfiles(3);
}

void Stationary3::on_comboBoxProfiles_currentIndexChanged(int index)
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

    bool flag = profile.getVersion() & Game::GC;

    ui->comboBoxGeneratorMethod->clear();
    ui->comboBoxSearcherMethod->clear();
    if (flag)
    {
        ui->comboBoxGeneratorMethod->addItem(tr("XD/Colo"), Method::XDColo);
        ui->comboBoxGeneratorMethod->addItem(tr("Channel"), Method::Channel);
        ui->comboBoxSearcherMethod->addItem(tr("XD/Colo"), Method::XDColo);
        ui->comboBoxSearcherMethod->addItem(tr("Gales"), Method::XD);
        ui->comboBoxSearcherMethod->addItem(tr("Colo"), Method::Colo);
        ui->comboBoxSearcherMethod->addItem(tr("Channel"), Method::Channel);
    }
    else
    {
        ui->comboBoxGeneratorMethod->addItem(tr("Method 1"), Method::Method1);
        ui->comboBoxGeneratorMethod->addItem(tr("Method 1 Reverse"), Method::Method1Reverse);
        ui->comboBoxGeneratorMethod->addItem(tr("Method 2"), Method::Method2);
        ui->comboBoxGeneratorMethod->addItem(tr("Method 4"), Method::Method4);
        ui->comboBoxSearcherMethod->addItem(tr("Method 1"), Method::Method1);
        ui->comboBoxSearcherMethod->addItem(tr("Method 1 Reverse"), Method::Method1Reverse);
        ui->comboBoxSearcherMethod->addItem(tr("Method 2"), Method::Method2);
        ui->comboBoxSearcherMethod->addItem(tr("Method 4"), Method::Method4);
    }
}

void Stationary3::on_pushButtonGenerate_clicked()
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
                                        ui->checkBoxGeneratorShinyOnly->isChecked(), ui->checkBoxGeneratorDisableFilters->isChecked());

    generator.setup(static_cast<Method>(ui->comboBoxGeneratorMethod->currentData().toInt()));

    QVector<Frame3> frames = generator.generate(compare);
    generatorModel->setModel(frames);
}

void Stationary3::on_pushButtonSearch_clicked()
{
    searcherModel->clear();
    searcherModel->setMethod(static_cast<Method>(ui->comboBoxSearcherMethod->currentData().toInt()));

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u16 tid = ui->textBoxSearcherTID->getUShort();
    u16 sid = ui->textBoxSearcherSID->getUShort();

    int genderRatioIndex = ui->comboBoxSearcherGenderRatio->currentIndex();
    FrameCompare compare = FrameCompare(ui->ivFilterSearcher->getLower(), ui->ivFilterSearcher->getUpper(), ui->comboBoxSearcherGender->currentIndex(),
                                        genderRatioIndex, ui->comboBoxSearcherAbility->currentIndex(), ui->comboBoxSearcherNature->getChecked(),
                                        ui->comboBoxSearcherHiddenPower->getChecked(), ui->checkBoxSearcherShinyOnly->isChecked(), false);
    Searcher3 searcher = Searcher3(tid, sid, static_cast<u32>(genderRatioIndex), compare);

    searcher.setup(static_cast<Method>(ui->comboBoxSearcherMethod->currentData().toInt()));
    if (searcher.getFrameType() == Method::XD || searcher.getFrameType() == Method::Colo)
    {
        searcher.setupNatureLock(ui->comboBoxSearcherShadow->currentIndex());
    }

    QVector<u8> min = ui->ivFilterSearcher->getLower();
    QVector<u8> max = ui->ivFilterSearcher->getUpper();

    int maxProgress = 1;
    for (int i = 0; i < 6; i++)
    {
        maxProgress *= max[i] - min[i] + 1;
    }

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(maxProgress);

    auto *search = new StationarySearcher3(searcher, min, max);
    auto *timer = new QTimer(search);

    connect(search, &StationarySearcher3::finished, timer, &QTimer::stop);
    connect(search, &StationarySearcher3::finished, this, [ = ] { ui->pushButtonSearch->setEnabled(true); ui->pushButtonCancel->setEnabled(false); });
    connect(search, &StationarySearcher3::finished, this, [ = ] { updateView(search->getResults(), search->currentProgress()); });
    connect(timer, &QTimer::timeout, this, [ = ] { updateView(search->getResults(), search->currentProgress()); });
    connect(ui->pushButtonCancel, &QPushButton::clicked, search, &StationarySearcher3::cancelSearch);

    search->start();
    timer->start(1000);
}

void Stationary3::on_comboBoxSearcherMethod_currentIndexChanged(int index)
{
    (void) index;
    Method method = static_cast<Method>(ui->comboBoxSearcherMethod->currentData().toInt());
    ui->comboBoxSearcherShadow->clear();

    if (method == Method::XD)
    {
        QStringList s = Translator::getSpecies(
        {
            334, 24, 354, 12, 113, 301, 85, 149, 51, 355, 125, 83, 55, 88, 58,
            316, 316, 316, 107, 106, 97, 115, 131, 165, 108, 337, 219, 126, 82,
            296, 310, 105, 303, 52, 122, 177, 299, 322, 46, 17, 204, 127, 62, 261,
            57, 280, 78, 20, 315, 302, 373, 123, 273, 273, 273, 86, 285, 143, 361,
            338, 21, 363, 363, 363, 167, 121, 220, 114, 49, 100, 37, 70
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

        ui->comboBoxSearcherShadow->addItems(s);
        ui->comboBoxSearcherShadow->setVisible(true);
        ui->labelSearcherShadow->setVisible(true);
    }
    else if (method == Method::Colo)
    {
        QStringList s = Translator::getSpecies({ 207, 214, 296, 179, 198, 212, 175, 217 });
        s[3] += tr(" (E-Reader)");
        s[5] += tr(" (E-Reader)");
        s[6] += tr(" (E-Reader)");
        ui->comboBoxSearcherShadow->addItems(s);
        ui->comboBoxSearcherShadow->setVisible(true);
        ui->labelSearcherShadow->setVisible(true);
    }
    else
    {
        ui->comboBoxSearcherShadow->setVisible(false);
        ui->labelSearcherShadow->setVisible(false);
    }
}

void Stationary3::on_tableViewGenerator_customContextMenuRequested(const QPoint &pos)
{
    if (generatorModel->rowCount() == 0)
    {
        return;
    }

    lastIndex = ui->tableViewGenerator->indexAt(pos);
    generatorMenu->popup(ui->tableViewGenerator->viewport()->mapToGlobal(pos));
}

void Stationary3::on_tableViewSearcher_customContextMenuRequested(const QPoint &pos)
{
    if (searcherModel->rowCount() == 0)
    {
        return;
    }

    lastIndex = ui->tableViewSearcher->indexAt(pos);
    searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
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

void Stationary3::centerFramesAndSetTargetGenerator(u32 centerFrames)
{
    ui->checkBoxGeneratorDisableFilters->setChecked(true);

    u32 frameNumber = ui->tableViewGenerator->model()->data(ui->tableViewGenerator->model()->index(lastIndex.row(), 0)).toString().toUInt();

    u32 startingFrame = frameNumber < centerFrames + 1 ? 1 : frameNumber - centerFrames;
    u32 selectedIndex = frameNumber < centerFrames + 1 ? frameNumber - 1 : centerFrames;
    u32 maxFrames = frameNumber < centerFrames + 1 ? frameNumber - 1 + centerFrames + 1 : centerFrames * 2 + 1;

    ui->textBoxGeneratorStartingFrame->setText(QString::number(startingFrame));
    ui->textBoxGeneratorMaxResults->setText(QString::number(maxFrames));

    on_pushButtonGenerate_clicked();

    targetFrame = ui->tableViewGenerator->model()->index(static_cast<int>(selectedIndex), 0);

    jumpToTargetGenerator();
}

void Stationary3::seedToTime()
{
    u32 seed = searcherModel->data(searcherModel->index(lastIndex.row(), 0), Qt::DisplayRole).toString().toUInt(nullptr, 16);
    auto *seedToTime = new SeedToTime3(seed);
    seedToTime->show();
    seedToTime->raise();
}

void Stationary3::outputToTxt()
{
    Utilities::outputModelTXT(generatorModel);
}

void Stationary3::outputToCSV()
{
    Utilities::outputModelCSV(generatorModel);
}

void Stationary3::copySeedToClipboard()
{
    QApplication::clipboard()->setText(searcherModel->data(searcherModel->index(lastIndex.row(), 0), Qt::DisplayRole).toString());
}

void Stationary3::on_pushButtonProfileManager_clicked()
{
    auto *manager = new ProfileManager3();
    connect(manager, &ProfileManager3::updateProfiles, this, &Stationary3::refreshProfiles);
    manager->show();
}

StationarySearcher3::StationarySearcher3(const Searcher3 &searcher, const QVector<u8> &min, const QVector<u8> &max)
{
    this->searcher = searcher;
    this->min = min;
    this->max = max;
    cancel = false;
    progress = 0;

    connect(this, &StationarySearcher3::finished, this, &StationarySearcher3::deleteLater);
}

void StationarySearcher3::run()
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

int StationarySearcher3::currentProgress() const
{
    return progress;
}

QVector<Frame3> StationarySearcher3::getResults()
{
    QMutexLocker locker(&mutex);
    auto data(results);
    results.clear();
    return data;
}

void StationarySearcher3::cancelSearch()
{
    cancel = true;
}
