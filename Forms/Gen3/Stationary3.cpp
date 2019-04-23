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

#include <QClipboard>
#include <QSettings>
#include "Stationary3.hpp"
#include "ui_Stationary3.h"
#include <Core/Gen3/Generator3.hpp>
#include <Core/Gen3/IVSearcher3.hpp>
#include <Core/Util/Nature.hpp>
#include <Core/Util/Power.hpp>
#include <Forms/Gen3/ProfileManager3.hpp>
#include <Forms/Gen3/SeedToTime3.hpp>

Stationary3::Stationary3(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Stationary3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();

    qRegisterMetaType<QVector<Frame3>>("QVector<Frame3>");
}

Stationary3::~Stationary3()
{
    QSettings setting;
    setting.beginGroup("stationary3");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("size", this->size());
    setting.endGroup();

    delete ui;
}

void Stationary3::updateProfiles()
{
    profiles = { Profile3() };
    for (const auto &profile : Profile3::loadProfileList())
    {
        if (!(profile.getVersion() & Game::GC))
        {
            profiles.append(profile);
        }
    }

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(profile.getProfileName());
    }

    QSettings setting;
    int val = setting.value("stationary3/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
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

    ui->comboBoxGeneratorNature->setup(Nature::getNatures());
    ui->comboBoxSearcherNature->setup(Nature::getNatures());

    ui->comboBoxGeneratorHiddenPower->setup(Power::getPowers());
    ui->comboBoxSearcherHiddenPower->setup(Power::getPowers());

    ui->comboBoxGeneratorMethod->addItem(tr("Method 1"), Method::Method1);
    ui->comboBoxGeneratorMethod->addItem(tr("Method 1 Reverse"), Method::Method1Reverse);
    ui->comboBoxGeneratorMethod->addItem(tr("Method 2"), Method::Method2);
    ui->comboBoxGeneratorMethod->addItem(tr("Method 4"), Method::Method4);
    ui->comboBoxSearcherMethod->addItem(tr("Method 1"), Method::Method1);
    ui->comboBoxSearcherMethod->addItem(tr("Method 1 Reverse"), Method::Method1Reverse);
    ui->comboBoxSearcherMethod->addItem(tr("Method 2"), Method::Method2);
    ui->comboBoxSearcherMethod->addItem(tr("Method 4"), Method::Method4);

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

    connect(setTargetFrame, &QAction::triggered, this, &Stationary3::setTargetFrameGenerator);
    connect(jumpToTarget, &QAction::triggered, this, &Stationary3::jumpToTargetGenerator);
    connect(center1Second, &QAction::triggered, this, [ = ]() { centerFramesAndSetTargetGenerator(60); });
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

    connect(copySeedToClipboard, &QAction::triggered, this, &Stationary3::copySeedToClipboard);
    connect(seedToTime, &QAction::triggered, this, &Stationary3::seedToTime);
    connect(outputTXTSearcher, &QAction::triggered, this, [ = ]() { ui->tableViewSearcher->outputModelTXT(); });
    connect(outputCSVSearcher, &QAction::triggered, this, [ = ]() { ui->tableViewSearcher->outputModelCSV(); });

    QSettings setting;
    if (setting.contains("stationary3/size")) this->resize(setting.value("stationary3/size").toSize());
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

void Stationary3::updateProgress(const QVector<Frame3> &frames, int progress)
{
    searcherModel->addItems(frames);
    ui->progressBar->setValue(progress);
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
}

void Stationary3::on_pushButtonGenerate_clicked()
{
    generatorModel->clearModel();

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

    u8 genderRatio = ui->comboBoxGeneratorGenderRatio->currentData().toInt();
    Generator3 generator = Generator3(maxResults, startingFrame, seed, tid, sid, offset, genderRatio);
    FrameCompare compare = FrameCompare(ui->ivFilterGenerator->getLower(), ui->ivFilterGenerator->getUpper(),
                                        ui->comboBoxGeneratorGender->currentIndex(), ui->comboBoxGeneratorAbility->currentIndex(),
                                        ui->comboBoxGeneratorNature->getChecked(), ui->comboBoxGeneratorHiddenPower->getChecked(),
                                        ui->checkBoxGeneratorShinyOnly->isChecked(), ui->checkBoxGeneratorDisableFilters->isChecked());

    generator.setup(static_cast<Method>(ui->comboBoxGeneratorMethod->currentData().toInt()));

    QVector<Frame3> frames = generator.generate(compare);
    generatorModel->addItems(frames);
}

void Stationary3::on_pushButtonSearch_clicked()
{
    searcherModel->clearModel();
    searcherModel->setMethod(static_cast<Method>(ui->comboBoxSearcherMethod->currentData().toInt()));

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u16 tid = ui->textBoxSearcherTID->getUShort();
    u16 sid = ui->textBoxSearcherSID->getUShort();

    u8 genderRatio = ui->comboBoxSearcherGenderRatio->currentData().toUInt();
    FrameCompare compare = FrameCompare(ui->ivFilterSearcher->getLower(), ui->ivFilterSearcher->getUpper(), ui->comboBoxSearcherGender->currentIndex(),
                                        ui->comboBoxSearcherAbility->currentIndex(), ui->comboBoxSearcherNature->getChecked(),
                                        ui->comboBoxSearcherHiddenPower->getChecked(), ui->checkBoxSearcherShinyOnly->isChecked(), false);
    Searcher3 searcher = Searcher3(tid, sid, genderRatio, compare);

    searcher.setup(static_cast<Method>(ui->comboBoxSearcherMethod->currentData().toInt()));

    QVector<u8> min = ui->ivFilterSearcher->getLower();
    QVector<u8> max = ui->ivFilterSearcher->getUpper();

    int maxProgress = 1;
    for (int i = 0; i < 6; i++)
    {
        maxProgress *= max.at(i) - min.at(i) + 1;
    }

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(maxProgress);

    auto *search = new IVSearcher3(searcher, min, max);

    connect(search, &IVSearcher3::finished, this, [ = ] { ui->pushButtonSearch->setEnabled(true); ui->pushButtonCancel->setEnabled(false); });
    connect(search, &IVSearcher3::updateProgress, this, &Stationary3::updateProgress);
    connect(ui->pushButtonCancel, &QPushButton::clicked, search, &IVSearcher3::cancelSearch);

    search->startSearch();
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
