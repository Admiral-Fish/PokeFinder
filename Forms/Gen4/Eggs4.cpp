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

#include "Eggs4.hpp"
#include "ui_Eggs4.h"
#include <Core/Gen4/EggSearcher4.hpp>
#include <Core/Util/Nature.hpp>
#include <Core/Util/Power.hpp>
#include <Forms/Gen4/ProfileManager4.hpp>
#include <Forms/Gen4/SeedtoTime4.hpp>
#include <Models/Gen4/Egg4Model.hpp>
#include <QSettings>

namespace PokeFinderForms
{
    Eggs4::Eggs4(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::Eggs4)
    {
        ui->setupUi(this);
        setAttribute(Qt::WA_QuitOnClose, false);

        updateProfiles();
        setupModels();

        qRegisterMetaType<QVector<PokeFinderCore::Frame4>>("QVector<PokeFinderCore::Frame4>");
    }

    Eggs4::~Eggs4()
    {
        QSettings setting;
        setting.beginGroup("eggs4");
        setting.setValue("minDelay", ui->textBoxSearcherMinDelay->text());
        setting.setValue("maxDelay", ui->textBoxSearcherMaxDelay->text());
        setting.setValue("minFrameIV", ui->textBoxSearcherIVMinFrame->text());
        setting.setValue("maxFrameIV", ui->textBoxSearcherIVMaxFrame->text());
        setting.setValue("minFramePID", ui->textBoxSearcherPIDMinFrame->text());
        setting.setValue("maxFramePID", ui->textBoxSearcherPIDMaxFrame->text());
        setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
        setting.setValue("geometry", this->saveGeometry());
        setting.endGroup();

        delete ui;
    }

    void Eggs4::updateProfiles()
    {
        profiles = PokeFinderCore::Profile4::loadProfileList();
        profiles.insert(profiles.begin(), PokeFinderCore::Profile4());

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
        generatorModel
            = new PokeFinderModels::Egg4GeneratorModel(ui->tableViewGenerator, PokeFinderCore::Method::DPPtIVs);
        searcherModel = new PokeFinderModels::Egg4SearcherModel(ui->tableViewSearcher, PokeFinderCore::Method::DPPtIVs);
        searcherMenu = new QMenu(this);

        ui->tableViewGenerator->setModel(generatorModel);
        ui->tableViewSearcher->setModel(searcherModel);

        ui->textBoxGeneratorStartingFrame->setValues(InputType::Frame32Bit);
        ui->textBoxGeneratorMaxResults->setValues(InputType::Frame32Bit);
        ui->textBoxGeneratorTID->setValues(InputType::TIDSID);
        ui->textBoxGeneratorSID->setValues(InputType::TIDSID);
        ui->textBoxGeneratorSeed->setValues(InputType::Seed32Bit);

        ui->textBoxSearcherIVMinFrame->setValues(InputType::Frame32Bit);
        ui->textBoxSearcherIVMaxFrame->setValues(InputType::Frame32Bit);
        ui->textBoxSearcherPIDMinFrame->setValues(InputType::Frame32Bit);
        ui->textBoxSearcherPIDMaxFrame->setValues(InputType::Frame32Bit);
        ui->textBoxSearcherMinDelay->setValues(InputType::Frame32Bit);
        ui->textBoxSearcherMaxDelay->setValues(InputType::Frame32Bit);
        ui->textBoxSearcherTID->setValues(InputType::TIDSID);
        ui->textBoxSearcherSID->setValues(InputType::TIDSID);

        ui->comboBoxGeneratorMethod->setItemData(0, PokeFinderCore::Method::DPPtIVs);
        ui->comboBoxGeneratorMethod->setItemData(1, PokeFinderCore::Method::Gen4Normal);

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

        ui->comboBoxGeneratorNature->setup(PokeFinderCore::Nature::getNatures());
        ui->comboBoxGeneratorHiddenPower->setup(PokeFinderCore::Power::getPowers());

        ui->comboBoxSearcherNature->setup(PokeFinderCore::Nature::getNatures());
        ui->comboBoxSearcherHiddenPower->setup(PokeFinderCore::Power::getPowers());

        QAction *seedToTime = searcherMenu->addAction(tr("Generate times for seed"));
        connect(seedToTime, &QAction::triggered, this, &Eggs4::seedToTime);

        connect(ui->eggSettingsGenerator, &EggSettings::toggleInheritance, generatorModel,
            &PokeFinderModels::Egg4GeneratorModel::toggleInheritance);
        connect(ui->eggSettingsSearcher, &EggSettings::toggleInheritance, searcherModel,
            &PokeFinderModels::Egg4SearcherModel::toggleInheritance);

        QSettings setting;
        setting.beginGroup("eggs4");
        if (setting.contains("minDelay"))
            ui->textBoxSearcherMinDelay->setText(setting.value("minDelayPID").toString());
        if (setting.contains("maxDelay"))
            ui->textBoxSearcherMaxDelay->setText(setting.value("maxDelayPID").toString());
        if (setting.contains("minFrameIV"))
            ui->textBoxSearcherIVMinFrame->setText(setting.value("minFramePID").toString());
        if (setting.contains("maxFrameIV"))
            ui->textBoxSearcherIVMaxFrame->setText(setting.value("maxFramePID").toString());
        if (setting.contains("minFramePID"))
            ui->textBoxSearcherPIDMinFrame->setText(setting.value("minFramePID").toString());
        if (setting.contains("maxFramePID"))
            ui->textBoxSearcherPIDMaxFrame->setText(setting.value("maxFramePID").toString());
        if (setting.contains("geometry"))
            this->restoreGeometry(setting.value("geometry").toByteArray());
        setting.endGroup();
    }

    void Eggs4::updateProgress(const QVector<PokeFinderCore::Frame4> &frames, int progress)
    {
        searcherModel->addItems(frames);
        ui->progressBarSearcher->setValue(progress);
    }

    void Eggs4::refreshProfiles() { emit alertProfiles(4); }

    void Eggs4::on_pushButtonGenerate_clicked()
    {
        generatorModel->clearModel();

        u32 startingFrame = ui->textBoxGeneratorStartingFrame->getUInt();
        u32 maxResults = ui->textBoxGeneratorMaxResults->getUInt();
        u32 seed = ui->textBoxGeneratorSeed->getUInt();
        u16 tid = ui->textBoxGeneratorTID->getUShort();
        u16 sid = ui->textBoxGeneratorSID->getUShort();

        PokeFinderCore::Method method
            = static_cast<PokeFinderCore::Method>(ui->comboBoxGeneratorMethod->currentData().toInt());

        if (method == PokeFinderCore::Method::Gen4Normal)
        {
            if (ui->checkBoxGeneratorMasuada->isChecked())
            {
                method = PokeFinderCore::Method::Gen4Masuada;
            }
        }
        else
        {
            PokeFinderCore::Game version = profiles.at(ui->comboBoxProfiles->currentIndex()).getVersion();
            method = (version & PokeFinderCore::Game::HGSS) ? PokeFinderCore::Method::HGSSIVs
                                                            : PokeFinderCore::Method::DPPtIVs;
        }

        generatorModel->setMethod(method);

        PokeFinderCore::Egg4 generator(maxResults, startingFrame, tid, sid, method, seed,
            ui->comboBoxGeneratorGenderRatio->currentData().toUInt());
        generator.setParents(ui->eggSettingsGenerator->getParent1(), ui->eggSettingsGenerator->getParent2());

        PokeFinderCore::FrameCompare compare(ui->comboBoxGeneratorGender->currentIndex(),
            ui->comboBoxGeneratorAbility->currentIndex(), ui->checkBoxGeneratorShinyOnly->isChecked(), false,
            ui->ivFilterGenerator->getLower(), ui->ivFilterGenerator->getUpper(),
            ui->comboBoxGeneratorNature->getChecked(), ui->comboBoxGeneratorHiddenPower->getChecked(), QVector<bool>());

        QVector<PokeFinderCore::Frame4> frames = generator.generate(compare);
        generatorModel->addItems(frames);
    }

    void Eggs4::on_pushButtonSearch_clicked()
    {
        searcherModel->clearModel();

        PokeFinderCore::Method model;
        switch (ui->comboBoxSearcherMethod->currentIndex())
        {
        case 0:
            model = (profiles.at(ui->comboBoxProfiles->currentIndex()).getVersion() & PokeFinderCore::Game::HGSS)
                ? PokeFinderCore::Method::HGSSIVs
                : PokeFinderCore::Method::DPPtIVs;
            break;
        case 1:
            model = ui->checkBoxSearcherMasuada->isChecked() ? PokeFinderCore::Method::Gen4Masuada
                                                             : PokeFinderCore::Method::Gen4Normal;
            break;
        case 2:
            model = PokeFinderCore::Method::Gen4Combined;
            break;
        }
        searcherModel->setMethod(model);

        ui->pushButtonSearch->setEnabled(false);
        ui->pushButtonCancel->setEnabled(true);

        u16 tid = ui->textBoxSearcherTID->getUShort();
        u16 sid = ui->textBoxSearcherSID->getUShort();

        u8 genderRatio = ui->comboBoxSearcherGenderRatio->currentData().toUInt();
        PokeFinderCore::FrameCompare compare(ui->comboBoxSearcherGender->currentIndex(),
            ui->comboBoxSearcherAbility->currentIndex(), ui->checkBoxSearcherShinyOnly->isChecked(), false,
            ui->ivFilterSearcher->getLower(), ui->ivFilterSearcher->getUpper(),
            ui->comboBoxSearcherNature->getChecked(), ui->comboBoxSearcherHiddenPower->getChecked(), QVector<bool>());

        u32 minDelay = ui->textBoxSearcherMinDelay->getUInt();
        u32 maxDelay = ui->textBoxSearcherMaxDelay->getUInt();
        u32 minFrameIV = ui->textBoxSearcherIVMinFrame->getUInt();
        u32 maxFrameIV = ui->textBoxSearcherIVMaxFrame->getUInt();
        u32 minFramePID = ui->textBoxSearcherPIDMinFrame->getUInt();
        u32 maxFramePID = ui->textBoxSearcherPIDMaxFrame->getUInt();

        PokeFinderCore::Method typeIV
            = (profiles.at(ui->comboBoxProfiles->currentIndex()).getVersion() & PokeFinderCore::Game::HGSS)
            ? PokeFinderCore::Method::HGSSIVs
            : PokeFinderCore::Method::DPPtIVs;
        PokeFinderCore::Egg4 generatorIV(maxFrameIV - minFrameIV + 1, minFrameIV, tid, sid, typeIV, 0, genderRatio);
        generatorIV.setParents(ui->eggSettingsSearcher->getParent1(), ui->eggSettingsSearcher->getParent2());

        PokeFinderCore::Method typePID = ui->checkBoxSearcherMasuada->isChecked() ? PokeFinderCore::Method::Gen4Masuada
                                                                                  : PokeFinderCore::Method::Gen4Normal;
        PokeFinderCore::Egg4 generatorPID(
            maxFramePID - minFramePID + 1, minFramePID, tid, sid, typePID, 0, genderRatio);

        ui->progressBarSearcher->setValue(0);
        ui->progressBarSearcher->setMaximum(static_cast<int>(256 * 24 * (maxDelay - minDelay + 1)));

        auto *search = new PokeFinderCore::EggSearcher4(
            generatorIV, generatorPID, compare, minDelay, maxDelay, ui->comboBoxSearcherMethod->currentIndex());

        connect(search, &PokeFinderCore::EggSearcher4::finished, this, [=] {
            ui->pushButtonSearch->setEnabled(true);
            ui->pushButtonCancel->setEnabled(false);
        });
        connect(search, &PokeFinderCore::EggSearcher4::updateProgress, this, &Eggs4::updateProgress);
        connect(ui->pushButtonCancel, &QPushButton::clicked, search, &PokeFinderCore::EggSearcher4::cancelSearch);

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

    void Eggs4::on_tableViewSearcher_customContextMenuRequested(const QPoint &pos)
    {
        if (searcherModel->rowCount() == 0)
        {
            return;
        }

        searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
    }

    void Eggs4::seedToTime()
    {
        QModelIndex index = ui->tableViewSearcher->currentIndex();
        QString seed = searcherModel->data(searcherModel->index(index.row(), 0), Qt::DisplayRole).toString();

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
}
