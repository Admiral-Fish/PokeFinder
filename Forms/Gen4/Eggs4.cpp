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
#include "Eggs4.hpp"
#include "ui_Eggs4.h"
#include <Core/Gen4/EggSearcher4.hpp>
#include <Core/Util/Nature.hpp>
#include <Core/Util/Power.hpp>
#include <Forms/Gen4/ProfileManager4.hpp>
#include <Forms/Gen4/SeedtoTime4.hpp>

Eggs4::Eggs4(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Eggs4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();

    qRegisterMetaType<QVector<Frame4>>("QVector<Frame4>");
}

Eggs4::~Eggs4()
{
    QSettings setting;
    setting.beginGroup("eggs4");
    setting.setValue("minDelayIVs", ui->textBoxSearcherIVsMinDelay->text());
    setting.setValue("maxDelayIVs", ui->textBoxSearcherIVsMaxDelay->text());
    setting.setValue("minFrameIVs", ui->textBoxSearcherIVsMinFrame->text());
    setting.setValue("maxFrameIVs", ui->textBoxSearcherIVsMaxFrame->text());
    setting.setValue("minDelayPID", ui->textBoxSearcherPIDMinDelay->text());
    setting.setValue("maxDelayPID", ui->textBoxSearcherPIDMaxDelay->text());
    setting.setValue("minFramePID", ui->textBoxSearcherPIDMinFrame->text());
    setting.setValue("maxFramePID", ui->textBoxSearcherPIDMaxFrame->text());
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Eggs4::updateProfiles()
{
    profiles = Profile4::loadProfileList();
    profiles.insert(profiles.begin(), Profile4());

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(profile.getProfileName());
    }

    QSettings setting;
    int val = setting.value("eggs4/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Eggs4::setupModels()
{
    generatorModel = new Egg4GeneratorModel(ui->tableViewGenerator, Method::DPPtIVs);
    searcherIVs = new Egg4SearcherModel(ui->tableViewIVs, Method::DPPtIVs);
    searcherPID = new Egg4SearcherModel(ui->tableViewPID, Method::Gen4Normal);
    searcherMenu = new QMenu(this);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewIVs->setModel(searcherIVs);
    ui->tableViewPID->setModel(searcherPID);

    ui->textBoxGeneratorStartingFrame->setValues(InputType::Frame32Bit);
    ui->textBoxGeneratorMaxResults->setValues(InputType::Frame32Bit);
    ui->textBoxGeneratorTID->setValues(InputType::TIDSID);
    ui->textBoxGeneratorSID->setValues(InputType::TIDSID);
    ui->textBoxGeneratorSeed->setValues(InputType::Seed32Bit);

    ui->textBoxSearcherIVsMinFrame->setValues(InputType::Frame32Bit);
    ui->textBoxSearcherIVsMaxFrame->setValues(InputType::Frame32Bit);
    ui->textBoxSearcherPIDMinFrame->setValues(InputType::Frame32Bit);
    ui->textBoxSearcherPIDMaxFrame->setValues(InputType::Frame32Bit);
    ui->textBoxSearcherIVsMinDelay->setValues(InputType::Frame32Bit);
    ui->textBoxSearcherIVsMaxDelay->setValues(InputType::Frame32Bit);
    ui->textBoxSearcherPIDMinDelay->setValues(InputType::Frame32Bit);
    ui->textBoxSearcherPIDMaxDelay->setValues(InputType::Frame32Bit);
    ui->textBoxSearcherTID->setValues(InputType::TIDSID);
    ui->textBoxSearcherSID->setValues(InputType::TIDSID);

    ui->comboBoxGeneratorMethod->setItemData(0, Method::DPPtIVs);
    ui->comboBoxGeneratorMethod->setItemData(1, Method::Gen4Normal);

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

    ui->comboBoxGeneratorNature->setup(Nature::getNatures());
    ui->comboBoxGeneratorHiddenPower->setup(Power::getPowers());

    ui->comboBoxSearcherNature->setup(Nature::getNatures());
    ui->comboBoxSearcherHiddenPower->setup(Power::getPowers());

    QAction *seedToTime = searcherMenu->addAction(tr("Generate times for seed"));
    connect(seedToTime, &QAction::triggered, this, &Eggs4::seedToTime);

    connect(ui->eggSettingsGenerator, &EggSettings::toggleInheritance, generatorModel, &Egg4GeneratorModel::toggleInheritance);
    connect(ui->eggSettingsSearcher, &EggSettings::toggleInheritance, searcherIVs, &Egg4SearcherModel::toggleInheritance);

    QSettings setting;
    setting.beginGroup("eggs4");
    if (setting.contains("minDelayIVs")) ui->textBoxSearcherIVsMinDelay->setText(setting.value("minDelayPID").toString());
    if (setting.contains("maxDelayIVs")) ui->textBoxSearcherIVsMaxDelay->setText(setting.value("maxDelayPID").toString());
    if (setting.contains("minFrameIVs")) ui->textBoxSearcherIVsMinFrame->setText(setting.value("minFramePID").toString());
    if (setting.contains("maxFrameIVs")) ui->textBoxSearcherIVsMaxFrame->setText(setting.value("maxFramePID").toString());
    if (setting.contains("minDelayPID")) ui->textBoxSearcherPIDMinDelay->setText(setting.value("minDelayPID").toString());
    if (setting.contains("maxDelayPID")) ui->textBoxSearcherPIDMaxDelay->setText(setting.value("maxDelayPID").toString());
    if (setting.contains("minFramePID")) ui->textBoxSearcherPIDMinFrame->setText(setting.value("minFramePID").toString());
    if (setting.contains("maxFramePID")) ui->textBoxSearcherPIDMaxFrame->setText(setting.value("maxFramePID").toString());
    if (setting.contains("geometry")) this->restoreGeometry(setting.value("geometry").toByteArray());
    setting.endGroup();
}

void Eggs4::updateProgressPID(const QVector<Frame4> &frames, int progress)
{
    searcherPID->addItems(frames);
    ui->progressBarPID->setValue(progress);
}

void Eggs4::updateProgressIVs(const QVector<Frame4> &frames, int progress)
{
    searcherIVs->addItems(frames);
    ui->progressBarIVs->setValue(progress);
}

void Eggs4::refreshProfiles()
{
    emit alertProfiles(4);
}

void Eggs4::on_pushButtonGenerate_clicked()
{
    generatorModel->clearModel();

    u32 startingFrame = ui->textBoxGeneratorStartingFrame->getUInt();
    u32 maxResults = ui->textBoxGeneratorMaxResults->getUInt();
    u32 seed = ui->textBoxGeneratorSeed->getUInt();
    u16 tid = ui->textBoxGeneratorTID->getUShort();
    u16 sid = ui->textBoxGeneratorSID->getUShort();

    Method method = static_cast<Method>(ui->comboBoxGeneratorMethod->currentData().toInt());

    if (method == Method::Gen4Normal)
    {
        if (ui->checkBoxGeneratorMasuada->isChecked())
        {
            method = Method::Gen4Masuada;
        }
    }
    else
    {
        Game version = profiles.at(ui->comboBoxProfiles->currentIndex()).getVersion();
        method = version & Game::HGSS ? Method::HGSSIVs : Method::DPPtIVs;
    }

    generatorModel->setMethod(method);

    Egg4 generator(maxResults, startingFrame, tid, sid, method, seed, ui->comboBoxGeneratorGenderRatio->currentData().toUInt());
    generator.setParents(ui->eggSettingsGenerator->getParent1(), ui->eggSettingsGenerator->getParent2());

    FrameCompare compare(ui->comboBoxGeneratorGender->currentIndex(), ui->comboBoxGeneratorAbility->currentIndex(),
                         ui->checkBoxGeneratorShinyOnly->isChecked(), false,
                         ui->ivFilterGenerator->getLower(), ui->ivFilterGenerator->getUpper(),
                         ui->comboBoxGeneratorNature->getChecked(), ui->comboBoxGeneratorHiddenPower->getChecked(), QVector<bool>());

    QVector<Frame4> frames = generator.generate(compare);
    generatorModel->addItems(frames);
}

void Eggs4::on_pushButtonSearchPID_clicked()
{
    searcherPID->clearModel();
    searcherPID->setMethod(ui->checkBoxSearcherMasuada->isChecked() ? Method::Gen4Masuada : Method::Gen4Normal);

    ui->pushButtonSearchPID->setEnabled(false);
    ui->pushButtonCancelPID->setEnabled(true);

    u16 tid = ui->textBoxSearcherTID->getUShort();
    u16 sid = ui->textBoxSearcherSID->getUShort();

    u8 genderRatio = ui->comboBoxSearcherGenderRatio->currentData().toUInt();
    FrameCompare compare(ui->comboBoxSearcherGender->currentIndex(), ui->comboBoxSearcherAbility->currentIndex(),
                         ui->checkBoxSearcherShinyOnly->isChecked(), false, QVector<u8>(), QVector<u8>(),
                         ui->comboBoxSearcherNature->getChecked(), QVector<bool>(), QVector<bool>());

    u32 minDelay = ui->textBoxSearcherPIDMinDelay->getUInt();
    u32 maxDelay = ui->textBoxSearcherPIDMaxDelay->getUInt();
    u32 minFrame = ui->textBoxSearcherPIDMinFrame->getUInt();
    u32 maxFrame = ui->textBoxSearcherPIDMaxFrame->getUInt();

    Method type = ui->checkBoxSearcherMasuada->isChecked() ? Method::Gen4Masuada : Method::Gen4Normal;
    Egg4 generator(maxFrame - minFrame + 1, minFrame, tid, sid, type, 0, genderRatio);

    ui->progressBarPID->setValue(0);
    ui->progressBarPID->setMaximum(static_cast<int>(256 * 24 * (maxDelay - minDelay + 1)));

    auto *search = new EggSearcher4(generator, compare, minDelay, maxDelay);

    connect(search, &EggSearcher4::finished, this, [ = ] { ui->pushButtonSearchPID->setEnabled(true); ui->pushButtonCancelPID->setEnabled(false); });
    connect(search, &EggSearcher4::updateProgress, this, &Eggs4::updateProgressPID);
    connect(ui->pushButtonCancelPID, &QPushButton::clicked, search, &EggSearcher4::cancelSearch);

    search->startSearch();
}

void Eggs4::on_pushButtonSearchIVs_clicked()
{
    searcherIVs->clearModel();
    Game version = profiles.at(ui->comboBoxProfiles->currentIndex()).getVersion();
    searcherIVs->setMethod(version & Game::HGSS ? Method::HGSSIVs : Method::DPPtIVs);

    ui->pushButtonSearchIVs->setEnabled(false);
    ui->pushButtonCancelIVs->setEnabled(true);

    FrameCompare compare(0, 0, false, false, ui->ivFilterSearcher->getLower(), ui->ivFilterSearcher->getUpper(),
                         QVector<bool>(), ui->comboBoxSearcherHiddenPower->getChecked(), QVector<bool>());

    u32 minDelay = ui->textBoxSearcherIVsMinDelay->getUInt();
    u32 maxDelay = ui->textBoxSearcherIVsMaxDelay->getUInt();
    u32 minFrame = ui->textBoxSearcherIVsMinFrame->getUInt();
    u32 maxFrame = ui->textBoxSearcherIVsMaxFrame->getUInt();

    Method type = version & Game::HGSS ? Method::HGSSIVs : Method::DPPtIVs;
    Egg4 generator(maxFrame - minFrame + 1, minFrame, 0, 0, type, 0, 0);
    generator.setParents(ui->eggSettingsSearcher->getParent1(), ui->eggSettingsSearcher->getParent2());

    ui->progressBarIVs->setValue(0);
    ui->progressBarIVs->setMaximum(static_cast<int>(256 * 24 * (maxDelay - minDelay + 1)));

    auto *search = new EggSearcher4(generator, compare, minDelay, maxDelay);

    connect(search, &EggSearcher4::finished, this, [ = ] { ui->pushButtonSearchIVs->setEnabled(true); ui->pushButtonCancelIVs->setEnabled(false); });
    connect(search, &EggSearcher4::updateProgress, this, &Eggs4::updateProgressIVs);
    connect(ui->pushButtonCancelIVs, &QPushButton::clicked, search, &EggSearcher4::cancelSearch);

    search->startSearch();
}

void Eggs4::on_comboBoxProfiles_currentIndexChanged(int index)
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

void Eggs4::on_tableViewPID_customContextMenuRequested(const QPoint &pos)
{
    if (searcherPID->rowCount() == 0)
    {
        return;
    }

    flag = false;
    searcherMenu->popup(ui->tableViewPID->viewport()->mapToGlobal(pos));
}

void Eggs4::on_tableViewIVs_customContextMenuRequested(const QPoint &pos)
{
    if (searcherIVs->rowCount() == 0)
    {
        return;
    }

    flag = true;
    searcherMenu->popup(ui->tableViewIVs->viewport()->mapToGlobal(pos));
}

void Eggs4::seedToTime()
{
    QString seed;

    if (flag)
    {
        QModelIndex index = ui->tableViewIVs->currentIndex();
        seed = searcherIVs->data(searcherIVs->index(index.row(), 0), Qt::DisplayRole).toString();
    }
    else
    {
        QModelIndex index = ui->tableViewPID->currentIndex();
        seed = searcherPID->data(searcherPID->index(index.row(), 0), Qt::DisplayRole).toString();
    }

    auto *time = new SeedtoTime4(seed, profiles.at(ui->comboBoxProfiles->currentIndex()));
    time->show();
    time->raise();
}

void Eggs4::on_pushButtonProfileManager_clicked()
{
    auto *manager = new ProfileManager4();
    connect(manager, &ProfileManager4::updateProfiles, this, &Eggs4::refreshProfiles);
    manager->show();
}
