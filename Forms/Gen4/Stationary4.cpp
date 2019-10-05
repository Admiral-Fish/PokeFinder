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
#include "Stationary4.hpp"
#include "ui_Stationary4.h"
#include <Core/Gen4/Generator4.hpp>
#include <Core/Gen4/IVSearcher4.hpp>
#include <Core/Util/Nature.hpp>
#include <Core/Util/Power.hpp>
#include <Forms/Gen4/ProfileManager4.hpp>
#include <Forms/Gen4/SeedtoTime4.hpp>

namespace PokeFinderForms
{

    Stationary4::Stationary4(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::Stationary4)
    {
        ui->setupUi(this);
        setAttribute(Qt::WA_QuitOnClose, false);

        updateProfiles();
        setupModels();

        qRegisterMetaType<QVector<PokeFinderCore::Frame4>>("QVector<PokeFinderCore::Frame4>");
    }

    Stationary4::~Stationary4()
    {
        QSettings setting;
        setting.beginGroup("stationary4");
        setting.setValue("minDelay", ui->textBoxSearcherMinDelay->text());
        setting.setValue("maxDelay", ui->textBoxSearcherMaxDelay->text());
        setting.setValue("minFrame", ui->textBoxSearcherMinFrame->text());
        setting.setValue("maxFrame", ui->textBoxSearcherMaxFrame->text());
        setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
        setting.setValue("geometry", this->saveGeometry());
        setting.endGroup();

        delete ui;
    }

    void Stationary4::updateProfiles()
    {
        profiles = PokeFinderCore::Profile4::loadProfileList();
        profiles.insert(profiles.begin(), PokeFinderCore::Profile4());

        ui->comboBoxProfiles->clear();

        for (const auto &profile : profiles)
        {
            ui->comboBoxProfiles->addItem(profile.getProfileName());
        }

        QSettings setting;
        int val = setting.value("stationary4/profile", 0).toInt();
        if (val < ui->comboBoxProfiles->count())
        {
            ui->comboBoxProfiles->setCurrentIndex(val);
        }
    }

    void Stationary4::setupModels()
    {
        generatorModel = new PokeFinderModels::Stationary4Model(ui->tableViewGenerator, PokeFinderCore::Method::Method1);
        searcherModel = new PokeFinderModels::Searcher4Model(ui->tableViewSearcher, PokeFinderCore::Method::Method1, true);
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
        ui->textBoxSearcherMinDelay->setValues(InputType::Delay);
        ui->textBoxSearcherMaxDelay->setValues(InputType::Delay);
        ui->textBoxSearcherMinFrame->setValues(InputType::Frame32Bit);
        ui->textBoxSearcherMaxFrame->setValues(InputType::Frame32Bit);

        ui->comboBoxSearcherLead->setItemData(0, PokeFinderCore::Lead::Search);
        ui->comboBoxSearcherLead->setItemData(1, PokeFinderCore::Lead::Synchronize);
        ui->comboBoxSearcherLead->setItemData(2, PokeFinderCore::Lead::CuteCharm);
        ui->comboBoxSearcherLead->setItemData(3, PokeFinderCore::Lead::None);

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

        ui->comboBoxGeneratorLead->addItem(tr("None"));
        ui->comboBoxGeneratorLead->addItems(PokeFinderCore::Nature::getNatures());

        ui->comboBoxGeneratorNature->setup(PokeFinderCore::Nature::getNatures());
        ui->comboBoxSearcherNature->setup(PokeFinderCore::Nature::getNatures());

        ui->comboBoxGeneratorHiddenPower->setup(PokeFinderCore::Power::getPowers());
        ui->comboBoxSearcherHiddenPower->setup(PokeFinderCore::Power::getPowers());

        QAction *outputTXTGenerator = generatorMenu->addAction(tr("Output Results to TXT"));
        QAction *outputCSVGenerator = generatorMenu->addAction(tr("Output Results to CSV"));

        connect(outputTXTGenerator, &QAction::triggered, [ = ]() { ui->tableViewGenerator->outputModelTXT(); });
        connect(outputCSVGenerator, &QAction::triggered, [ = ]() { ui->tableViewGenerator->outputModelCSV(); });

        QAction *seedToTime = searcherMenu->addAction(tr("Generate times for seed"));
        QAction *outputTXTSearcher = searcherMenu->addAction(tr("Output Results to TXT"));
        QAction *outputCSVSearcher = searcherMenu->addAction(tr("Output Results to CSV"));

        connect(seedToTime, &QAction::triggered, this, &Stationary4::seedToTime);
        connect(outputTXTSearcher, &QAction::triggered, [ = ]() { ui->tableViewSearcher->outputModelTXT(); });
        connect(outputCSVSearcher, &QAction::triggered, [ = ]() { ui->tableViewSearcher->outputModelCSV(); });

        QSettings setting;
        setting.beginGroup("stationary4");
        if (setting.contains("minDelay")) ui->textBoxSearcherMinDelay->setText(setting.value("minDelay").toString());
        if (setting.contains("maxDelay")) ui->textBoxSearcherMaxDelay->setText(setting.value("maxDelay").toString());
        if (setting.contains("minFrame")) ui->textBoxSearcherMinFrame->setText(setting.value("minFrame").toString());
        if (setting.contains("maxFrame")) ui->textBoxSearcherMaxFrame->setText(setting.value("maxFrame").toString());
        if (setting.contains("geometry")) this->restoreGeometry(setting.value("geometry").toByteArray());
        setting.endGroup();
    }

    void Stationary4::updateProgress(const QVector<PokeFinderCore::Frame4> &frames, int progress)
    {
        searcherModel->addItems(frames);
        ui->progressBar->setValue(progress);
    }

    void Stationary4::refreshProfiles()
    {
        emit alertProfiles(4);
    }

    void Stationary4::on_pushButtonGenerate_clicked()
    {
        generatorModel->clearModel();
        generatorModel->setMethod(static_cast<PokeFinderCore::Method>(ui->comboBoxGeneratorMethod->currentData().toInt()));

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

        u8 genderRatio = ui->comboBoxGeneratorGenderRatio->currentData().toUInt();
        PokeFinderCore::Generator4 generator(maxResults, startingFrame, seed, tid, sid, offset, static_cast<PokeFinderCore::Method>(ui->comboBoxGeneratorMethod->currentData().toInt()), genderRatio);
        PokeFinderCore::FrameCompare compare(ui->comboBoxGeneratorGender->currentIndex(), ui->comboBoxGeneratorAbility->currentIndex(),
                                             ui->checkBoxGeneratorShinyOnly->isChecked(), ui->checkBoxGeneratorDisableFilters->isChecked(),
                                             ui->ivFilterGenerator->getLower(), ui->ivFilterGenerator->getUpper(), ui->comboBoxGeneratorNature->getChecked(),
                                             ui->comboBoxGeneratorHiddenPower->getChecked(), QVector<bool>());

        generator.setEncounterType(PokeFinderCore::Encounter::Stationary);
        if (ui->pushButtonGeneratorLead->text() == tr("Cute Charm"))
        {
            generator.setLeadType(static_cast<PokeFinderCore::Lead>(ui->comboBoxGeneratorLead->currentData().toInt()));
        }
        else
        {
            int num = ui->comboBoxGeneratorLead->currentIndex();
            if (num == 0)
            {
                generator.setLeadType(PokeFinderCore::Lead::None);
            }
            else
            {
                generator.setLeadType(PokeFinderCore::Lead::Synchronize);
                generator.setSynchNature(PokeFinderCore::Nature::getAdjustedNature(static_cast<u32>(ui->comboBoxGeneratorLead->currentIndex() - 1)));
            }
        }

        QVector<PokeFinderCore::Frame4> frames = generator.generate(compare);
        generatorModel->addItems(frames);
    }

    void Stationary4::on_pushButtonSearch_clicked()
    {
        searcherModel->clearModel();
        searcherModel->setMethod(static_cast<PokeFinderCore::Method>(ui->comboBoxSearcherMethod->currentData().toInt()));

        ui->pushButtonSearch->setEnabled(false);
        ui->pushButtonCancel->setEnabled(true);

        u16 tid = ui->textBoxSearcherTID->getUShort();
        u16 sid = ui->textBoxSearcherSID->getUShort();

        u8 genderRatio = ui->comboBoxSearcherGenderRatio->currentData().toUInt();
        PokeFinderCore::FrameCompare compare(ui->comboBoxSearcherGender->currentIndex(), ui->comboBoxSearcherAbility->currentIndex(),
                                             ui->checkBoxSearcherShinyOnly->isChecked(), false,
                                             ui->ivFilterSearcher->getLower(), ui->ivFilterSearcher->getUpper(), ui->comboBoxSearcherNature->getChecked(),
                                             ui->comboBoxSearcherHiddenPower->getChecked(), QVector<bool>());
        PokeFinderCore::Searcher4 searcher(tid, sid, genderRatio, ui->textBoxSearcherMinDelay->getUInt(), ui->textBoxSearcherMaxDelay->getUInt(),
                                           ui->textBoxSearcherMinFrame->getUInt(), ui->textBoxSearcherMaxFrame->getUInt(), compare, static_cast<PokeFinderCore::Method>(ui->comboBoxSearcherMethod->currentData().toInt()));
        searcher.setLeadType(static_cast<PokeFinderCore::Lead>(ui->comboBoxSearcherLead->currentData().toInt()));

        QVector<u8> min = ui->ivFilterSearcher->getLower();
        QVector<u8> max = ui->ivFilterSearcher->getUpper();

        int maxProgress = 1;
        for (int i = 0; i < 6; i++)
        {
            maxProgress *= max.at(i) - min.at(i) + 1;
        }

        ui->progressBar->setValue(0);
        ui->progressBar->setMaximum(maxProgress);

        auto *search = new PokeFinderCore::IVSearcher4(searcher, min, max);

        connect(search, &PokeFinderCore::IVSearcher4::finished, this, [ = ] { ui->pushButtonSearch->setEnabled(true); ui->pushButtonCancel->setEnabled(false); });
        connect(search, &PokeFinderCore::IVSearcher4::updateProgress, this, &Stationary4::updateProgress);
        connect(ui->pushButtonCancel, &QPushButton::clicked, search, &PokeFinderCore::IVSearcher4::cancelSearch);

        search->startSearch();
    }

    void Stationary4::on_comboBoxProfiles_currentIndexChanged(int index)
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

        bool flag = profile.getVersion() & PokeFinderCore::Game::HGSS;

        ui->comboBoxGeneratorMethod->clear();
        ui->comboBoxGeneratorMethod->addItem(tr("Method 1"), PokeFinderCore::Method::Method1);
        ui->comboBoxGeneratorMethod->addItem(flag ? tr("Method K") : tr("Method J"), flag ? PokeFinderCore::Method::MethodK : PokeFinderCore::Method::MethodJ);
        ui->comboBoxGeneratorMethod->addItem(tr("Wondercard IVs"), PokeFinderCore::Method::WondercardIVs);

        ui->comboBoxSearcherMethod->clear();
        ui->comboBoxSearcherMethod->addItem(tr("Method 1"), PokeFinderCore::Method::Method1);
        ui->comboBoxSearcherMethod->addItem(flag ? tr("Method K") : tr("Method J"), flag ? PokeFinderCore::Method::MethodK : PokeFinderCore::Method::MethodJ);
        ui->comboBoxSearcherMethod->addItem(tr("Wondercard IVs"), PokeFinderCore::Method::WondercardIVs);
    }

    void Stationary4::on_pushButtonGeneratorLead_clicked()
    {
        ui->comboBoxGeneratorLead->clear();
        QString text = ui->pushButtonGeneratorLead->text();
        if (text == tr("Synchronize"))
        {
            ui->pushButtonGeneratorLead->setText(tr("Cute Charm"));
            ui->comboBoxGeneratorLead->setEnabled(true);

            ui->comboBoxGeneratorLead->addItem(tr("♂ Lead"), PokeFinderCore::Lead::CuteCharmFemale);
            ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (50% ♂ Target)"), PokeFinderCore::Lead::CuteCharm50M);
            ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (75% ♂ Target)"), PokeFinderCore::Lead::CuteCharm75M);
            ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (25% ♂ Target)"), PokeFinderCore::Lead::CuteCharm25M);
            ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (87.5% ♂ Target)"), PokeFinderCore::Lead::CuteCharm875M);
        }
        else
        {
            ui->pushButtonGeneratorLead->setText(tr("Synchronize"));

            ui->comboBoxGeneratorLead->addItem("None");
            ui->comboBoxGeneratorLead->addItems(PokeFinderCore::Nature::getNatures());
        }
    }

    void Stationary4::seedToTime()
    {
        QModelIndex index = ui->tableViewSearcher->currentIndex();
        auto *time = new SeedtoTime4(searcherModel->data(searcherModel->index(index.row(), 0), Qt::DisplayRole).toString(), profiles[ui->comboBoxProfiles->currentIndex()]);
        time->show();
        time->raise();
    }

    void Stationary4::on_tableViewGenerator_customContextMenuRequested(const QPoint &pos)
    {
        if (generatorModel->rowCount() == 0)
        {
            return;
        }

        generatorMenu->popup(ui->tableViewGenerator->viewport()->mapToGlobal(pos));
    }

    void Stationary4::on_tableViewSearcher_customContextMenuRequested(const QPoint &pos)
    {
        if (searcherModel->rowCount() == 0)
        {
            return;
        }

        searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
    }

    void Stationary4::on_pushButtonProfileManager_clicked()
    {
        auto *manager = new ProfileManager4();
        connect(manager, &ProfileManager4::updateProfiles, this, &Stationary4::refreshProfiles);
        manager->show();
    }

}
