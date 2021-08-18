/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Stationary5.hpp"
#include "ui_Stationary5.h"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen5/Generators/StationaryGenerator5.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Parents/States/StationaryState.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Gen5/Profile/ProfileManager5.hpp>
#include <Forms/Models/Gen5/StationaryModel5.hpp>
#include <QSettings>

Stationary5::Stationary5(QWidget *parent) : QWidget(parent), ui(new Ui::Stationary5)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();
}

Stationary5::~Stationary5()
{
    QSettings setting;
    setting.setValue("stationary5Profile", ui->comboBoxProfiles->currentIndex());

    delete ui;
}

void Stationary5::updateProfiles()
{
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Stationary5::profileIndexChanged);

    profiles = ProfileLoader5::getProfiles();

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("Stationary5Profile").toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val >= 0 ? val : 0);
    }
}

bool Stationary5::hasProfiles() const
{
    return !profiles.empty();
}

void Stationary5::setupModels()
{
    generatorModel = new StationaryGeneratorModel5(ui->tableViewGenerator, Method::Method5IVs);
    // searcherModel = new Searcher5Model(ui->tableViewSearcher, Method::Method1);

    generatorMenu = new QMenu(ui->tableViewGenerator);
    // searcherMenu = new QMenu(ui->tableViewSearcher);

    ui->tableViewGenerator->setModel(generatorModel);
    // ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed32Bit);
    ui->textBoxGeneratorStartingAdvance->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);

    ui->textBoxSearcherMinDelay->setValues(InputType::Delay);
    ui->textBoxSearcherMaxDelay->setValues(InputType::Delay);
    ui->textBoxSearcherMinAdvance->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxAdvance->setValues(InputType::Advance32Bit);

    ui->comboBoxGeneratorMethod->setup({ Method::Method5IVs, Method::Method5CGear, Method::Method5 });
    ui->comboBoxGeneratorEncounter->setup({ Encounter::Stationary, Encounter::Roamer });

    ui->comboBoxSearcherLead->setup({ Lead::Search, Lead::Synchronize, Lead::CuteCharm, Lead::None });

    ui->comboBoxGeneratorLead->addItem(tr("None"));
    for (const std::string &nature : Translator::getNatures())
    {
        ui->comboBoxGeneratorLead->addItem(QString::fromStdString(nature));
    }

    ui->filterGenerator->disableControls(Controls::EncounterSlots);
    ui->filterSearcher->disableControls(Controls::EncounterSlots | Controls::DisableFilter | Controls::UseDelay);

    QAction *outputTXTGenerator = generatorMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVGenerator = generatorMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTGenerator, &QAction::triggered, [=]() { ui->tableViewGenerator->outputModel(false); });
    connect(outputCSVGenerator, &QAction::triggered, [=]() { ui->tableViewGenerator->outputModel(true); });

    // QAction *outputTXTSearcher = searcherMenu->addAction(tr("Output Results to TXT"));
    // QAction *outputCSVSearcher = searcherMenu->addAction(tr("Output Results to CSV"));
    // connect(outputTXTSearcher, &QAction::triggered, [=]() { ui->tableViewSearcher->outputModel(false); });
    // connect(outputCSVSearcher, &QAction::triggered, [=]() { ui->tableViewSearcher->outputModel(true); });

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Stationary5::generate);
    connect(ui->tableViewGenerator, &QTableView::customContextMenuRequested, this, &Stationary5::tableViewGeneratorContextMenu);
    // connect(ui->tableViewSearcher, &QTableView::customContextMenuRequested, this, &Stationary5::tableViewSearcherContextMenu);

    QSettings setting;
    setting.beginGroup("stationary5");
    if (setting.contains("minDelay"))
    {
        ui->textBoxSearcherMinDelay->setText(setting.value("minDelay").toString());
    }
    if (setting.contains("maxDelay"))
    {
        ui->textBoxSearcherMaxDelay->setText(setting.value("maxDelay").toString());
    }
    if (setting.contains("minAdvance"))
    {
        ui->textBoxSearcherMinAdvance->setText(setting.value("minAdvance").toString());
    }
    if (setting.contains("maxAdvance"))
    {
        ui->textBoxSearcherMaxAdvance->setText(setting.value("maxAdvance").toString());
    }
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

void Stationary5::generate()
{
    generatorModel->clearModel();
    auto method = static_cast<Method>(ui->comboBoxGeneratorMethod->getCurrentInt());
    generatorModel->setMethod(method);

    u64 seed = ui->textBoxGeneratorSeed->getULong();
    u32 initialAdvances = ui->textBoxGeneratorStartingAdvance->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterGenerator->getGenderRatio();
    u32 offset = 0;
    auto encounter = static_cast<Encounter>(ui->comboBoxGeneratorEncounter->getCurrentInt());
    if (ui->filterGenerator->useDelay())
    {
        offset = ui->filterGenerator->getDelay();
    }

    StateFilter filter(ui->filterGenerator->getGender(), ui->filterGenerator->getAbility(), ui->filterGenerator->getShiny(),
                       ui->filterGenerator->getDisableFilters(), ui->filterGenerator->getMinIVs(), ui->filterGenerator->getMaxIVs(),
                       ui->filterGenerator->getNatures(), ui->filterGenerator->getHiddenPowers(), {});

    StationaryGenerator5 generator(initialAdvances, maxAdvances, tid, sid, genderRatio, method, encounter, filter);
    generator.setOffset(offset);

    if (ui->pushButtonGeneratorLead->text() == tr("Cute Charm"))
    {
        generator.setLead(static_cast<Lead>(ui->comboBoxGeneratorLead->currentData().toInt()));
    }
    else
    {
        int num = ui->comboBoxGeneratorLead->currentIndex();
        if (num == 0)
        {
            generator.setLead(Lead::None);
        }
        else
        {
            generator.setLead(Lead::Synchronize);
            generator.setSynchNature(static_cast<u8>(ui->comboBoxGeneratorLead->currentIndex() - 1));
        }
    }

    auto states = generator.generate(seed);
    generatorModel->addItems(states);
}

void Stationary5::search()
{
    /*searcherModel->clear();
    searcherModel->setMethod(static_cast<Method>(ui->comboBoxSearcherMethod->currentData().toInt()));

   ui->pushButtonSearch->setEnabled(false);
   ui->pushButtonCancel->setEnabled(true);

   u16 tid = ui->textBoxSearcherTID->getUShort();
   u16 sid = ui->textBoxSearcherSID->getUShort();

   u8 genderRatio = ui->comboBoxSearcherGenderRatio->currentData().toInt();
   AdvanceCompare compare(ui->comboBoxSearcherAbility->currentIndex(), ui->comboBoxSearcherAbility->currentIndex(),
       ui->checkBoxSearcherShinyOnly->isChecked(), false, ui->ivFilterSearcher->getLower(),
       ui->ivFilterSearcher->getLower(), ui->comboBoxSearcherNature->getChecked(),
       ui->comboBoxSearcherHiddenPower->getChecked(), std::vector<bool>());
    Searcher5 searcher(tid, sid, static_cast<u32>(genderRatioIndex), ui->textBoxSearcherMinDelay->getUInt(),
    ui->textBoxSearcherMaxDelay->getUInt(),
                                  ui->textBoxSearcherMinAdvance->getUInt(), ui->textBoxSearcherMaxAdvance->getUInt(),
                                  compare,
                                  static_cast<Method>(ui->comboBoxSearcherMethod->currentData().toInt()));

    searcher.setLeadType(static_cast<Lead>(ui->comboBoxSearcherLead->currentData().toInt()));

   std::vector<u8> min = ui->ivFilterSearcher->getLower();
   std::vector<u8> max = ui->ivFilterSearcher->getUpper();

   int maxProgress = 1;
   for (int i = 0; i < 6; i++)
   {
       maxProgress *= max[i] - min[i] + 1;
   }

   ui->progressBar->setValue(0);
   ui->progressBar->setMaximum(maxProgress);

   auto *search = new IVSearcher5(searcher, min, max);
   auto *timer = new QTimer(search);

   connect(search, &IVSearcher5::finished, timer, &QTimer::stop);
   connect(search, &IVSearcher5::finished, this, [ = ] { ui->pushButtonSearch->setEnabled(true);
   ui->pushButtonCancel->setEnabled(false); }); connect(search, &IVSearcher5::finished, this, [ = ] {
   updateView(search->getResults(), search->currentProgress()); }); connect(timer, &QTimer::timeout, this, [ = ] {
   updateView(search->getResults(), search->currentProgress()); }); connect(ui->pushButtonCancel,
   &QPushButton::clicked, search, &IVSearcher5::cancelSearch);

   search->start();
   timer->start(1000);*/
}

void Stationary5::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile.getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile.getSID()));
        ui->labelProfileMACAddressValue->setText(QString::number(currentProfile.getMac(), 16));
        ui->labelProfileDSTypeValue->setText(QString::fromStdString(currentProfile.getDSTypeString()));
        ui->labelProfileVCountValue->setText(QString::number(currentProfile.getVCount(), 16));
        ui->labelProfileTimer0Value->setText(QString::number(currentProfile.getTimer0Min(), 16) + "-"
                                             + QString::number(currentProfile.getTimer0Max(), 16));
        ui->labelProfileGxStatValue->setText(QString::number(currentProfile.getGxStat()));
        ui->labelProfileVFrameValue->setText(QString::number(currentProfile.getVFrame()));
        ui->labelProfileKeypressesValue->setText(QString::fromStdString(currentProfile.getKeypressesString()));
        ui->labelProfileGameValue->setText(QString::fromStdString(currentProfile.getVersionString()));
    }
}

void Stationary5::generatorLead()
{
    ui->comboBoxGeneratorLead->clear();
    QString text = ui->pushButtonGeneratorLead->text();
    if (text == tr("Synchronize"))
    {
        ui->pushButtonGeneratorLead->setText(tr("Cute Charm"));
        ui->comboBoxGeneratorLead->setEnabled(true);

        ui->comboBoxGeneratorLead->addItem(tr("♂ Lead"), Lead::CuteCharmFemale);
        ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (50% ♂ Target)"), Lead::CuteCharm50M);
        ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (75% ♂ Target)"), Lead::CuteCharm75M);
        ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (25% ♂ Target)"), Lead::CuteCharm25M);
        ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (87.5% ♂ Target)"), Lead::CuteCharm875M);
    }
    else if (text == tr("Cute Charm"))
    {
        ui->pushButtonGeneratorLead->setText(tr("Suction Cups"));
        ui->comboBoxGeneratorLead->setEnabled(false);
    }
    else
    {
        ui->pushButtonGeneratorLead->setText(tr("Synchronize"));
        ui->comboBoxGeneratorLead->setEnabled(true);

        ui->comboBoxGeneratorLead->addItem("None");
        // ui->comboBoxGeneratorLead->addItems(Translator::getNatures());
    }
}

void Stationary5::tableViewGeneratorContextMenu(const QPoint &pos)
{
    if (generatorModel->rowCount() > 0)
    {
        generatorMenu->popup(ui->tableViewGenerator->viewport()->mapToGlobal(pos));
    }
}

void Stationary5::tableViewSearcherContextMenu(const QPoint &pos)
{
    /*if (searcherModel->rowCount() == 0)
    {
        searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
    }*/
}

void Stationary5::profileManager()
{
    auto *manager = new ProfileManager5();
    connect(manager, &ProfileManager5::updateProfiles, this, [=] { emit alertProfiles(5); });
    manager->show();
}
