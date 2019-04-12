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

#include "GameCube.hpp"
#include "ui_GameCube.h"

GameCube::GameCube(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameCube)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    ui->labelSearcherShadow->setVisible(false);
    ui->comboBoxSearcherShadow->setVisible(false);

    updateProfiles();
    setupModels();

    qRegisterMetaType<QVector<Frame3>>("QVector<Frame3>");
}

GameCube::~GameCube()
{
    QSettings setting;
    setting.setValue("gamecubeProfile", ui->comboBoxProfiles->currentIndex());

    delete ui;
}

void GameCube::updateProfiles()
{
    profiles = { Profile3(tr("None"), Game::Gales, 12345, 54321, Language::English) };
    for (const auto &profile : Profile3::loadProfileList())
    {
        if (profile.getVersion() & Game::GC)
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
    int val = setting.value("gamecubeProfile").toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val >= 0 ? val : 0);
    }
}

void GameCube::setupModels()
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

    ui->comboBoxGeneratorMethod->addItem(tr("XD/Colo"), Method::XDColo);
    ui->comboBoxGeneratorMethod->addItem(tr("Channel"), Method::Channel);
    ui->comboBoxSearcherMethod->addItem(tr("XD/Colo"), Method::XDColo);
    ui->comboBoxSearcherMethod->addItem(tr("Gales"), Method::XD);
    ui->comboBoxSearcherMethod->addItem(tr("Colo"), Method::Colo);
    ui->comboBoxSearcherMethod->addItem(tr("Channel"), Method::Channel);

    QAction *outputTXTGenerator = generatorMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVGenerator = generatorMenu->addAction(tr("Output Results to CSV"));

    connect(outputTXTGenerator, &QAction::triggered, this, [ = ]() { ui->tableViewGenerator->outputModelTXT(); });
    connect(outputCSVGenerator, &QAction::triggered, this, [ = ]() { ui->tableViewGenerator->outputModelCSV(); });

    QAction *copySeedToClipboard = searcherMenu->addAction(tr("Copy Seed to Clipboard"));
    QAction *seedToTime = searcherMenu->addAction(tr("Generate times for seed"));
    QAction *outputTXTSearcher = searcherMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVSearcher = searcherMenu->addAction(tr("Output Results to CSV"));

    connect(copySeedToClipboard, &QAction::triggered, this, &GameCube::copySeedToClipboard);
    connect(seedToTime, &QAction::triggered, this, &GameCube::seedToTime);
    connect(outputTXTSearcher, &QAction::triggered, this, [ = ]() { ui->tableViewSearcher->outputModelTXT(); });
    connect(outputCSVSearcher, &QAction::triggered, this, [ = ]() { ui->tableViewSearcher->outputModelCSV(); });
}

void GameCube::updateView(const QVector<Frame3> &frames, int progress)
{
    searcherModel->addItems(frames);
    ui->progressBar->setValue(progress);
}

void GameCube::refreshProfiles()
{
    emit alertProfiles(3);
}

void GameCube::on_comboBoxProfiles_currentIndexChanged(int index)
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

void GameCube::on_pushButtonGenerate_clicked()
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

    int genderRatioIndex = ui->comboBoxGeneratorGenderRatio->currentIndex();
    Generator3 generator = Generator3(maxResults, startingFrame, seed, tid, sid, offset);
    FrameCompare compare = FrameCompare(ui->ivFilterGenerator->getLower(), ui->ivFilterGenerator->getUpper(),
                                        ui->comboBoxGeneratorGender->currentIndex(), genderRatioIndex, ui->comboBoxGeneratorAbility->currentIndex(),
                                        ui->comboBoxGeneratorNature->getChecked(), ui->comboBoxGeneratorHiddenPower->getChecked(),
                                        ui->checkBoxGeneratorShinyOnly->isChecked(), ui->checkBoxGeneratorDisableFilters->isChecked());

    generator.setup(static_cast<Method>(ui->comboBoxGeneratorMethod->currentData().toInt()));

    QVector<Frame3> frames = generator.generate(compare);
    generatorModel->addItems(frames);
}

void GameCube::on_pushButtonSearch_clicked()
{
    searcherModel->clearModel();
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
        maxProgress *= max.at(i) - min.at(i) + 1;
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

void GameCube::on_comboBoxSearcherMethod_currentIndexChanged(int index)
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

void GameCube::on_tableViewGenerator_customContextMenuRequested(const QPoint &pos)
{
    if (generatorModel->rowCount() == 0)
    {
        return;
    }

    generatorMenu->popup(ui->tableViewGenerator->viewport()->mapToGlobal(pos));
}

void GameCube::on_tableViewSearcher_customContextMenuRequested(const QPoint &pos)
{
    if (searcherModel->rowCount() == 0)
    {
        return;
    }

    searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
}

void GameCube::seedToTime()
{
    QModelIndex index = ui->tableViewSearcher->currentIndex();
    u32 seed = searcherModel->data(searcherModel->index(index.row(), 0), Qt::DisplayRole).toString().toUInt(nullptr, 16);
    auto *rtc = new GameCubeRTC(seed);
    rtc->show();
    rtc->raise();
}

void GameCube::copySeedToClipboard()
{
    QModelIndex index = ui->tableViewSearcher->currentIndex();
    QApplication::clipboard()->setText(searcherModel->data(searcherModel->index(index.row(), 0), Qt::DisplayRole).toString());
}

void GameCube::on_pushButtonProfileManager_clicked()
{
    auto *manager = new ProfileManager3();
    connect(manager, &ProfileManager3::updateProfiles, this, &GameCube::refreshProfiles);
    manager->show();
}
