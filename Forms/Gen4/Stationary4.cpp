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

#include "Stationary4.hpp"
#include "ui_Stationary4.h"

Stationary4::Stationary4(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Stationary4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();

    qRegisterMetaType<QVector<Frame4>>("QVector<Frame4>");
}

Stationary4::~Stationary4()
{
    QSettings setting;
    setting.setValue("stationary4Profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("stationary4MinDelay", ui->textBoxSearcherMinDelay->text());
    setting.setValue("stationary4MaxDelay", ui->textBoxSearcherMaxDelay->text());
    setting.setValue("stationary4MinFrame", ui->textBoxSearcherMinFrame->text());
    setting.setValue("stationary4MaxFrame", ui->textBoxSearcherMaxFrame->text());

    delete ui;
}

void Stationary4::updateProfiles()
{
    profiles = Profile4::loadProfileList();
    profiles.insert(profiles.begin(), Profile4());

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(profile.getProfileName());
    }

    QSettings setting;
    int val = setting.value("stationary4Profile").toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val >= 0 ? val : 0);
    }
}

void Stationary4::setupModels()
{
    generatorModel = new Stationary4Model(ui->tableViewGenerator, Method::Method1);
    searcherModel = new Searcher4Model(ui->tableViewSearcher, Method::Method1);
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

    ui->comboBoxSearcherLead->setItemData(0, Lead::Search);
    ui->comboBoxSearcherLead->setItemData(1, Lead::Synchronize);
    ui->comboBoxSearcherLead->setItemData(2, Lead::CuteCharm);
    ui->comboBoxSearcherLead->setItemData(3, Lead::None);

    ui->comboBoxGeneratorLead->addItem(tr("None"));
    ui->comboBoxGeneratorLead->addItems(Nature::getNatures());

    ui->comboBoxGeneratorNature->setup(Nature::getNatures());
    ui->comboBoxSearcherNature->setup(Nature::getNatures());

    ui->comboBoxGeneratorHiddenPower->setup(Power::getPowers());
    ui->comboBoxSearcherHiddenPower->setup(Power::getPowers());

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
    if (setting.contains("stationary4MinDelay")) ui->textBoxSearcherMinDelay->setText(setting.value("stationary4MinDelay").toString());
    if (setting.contains("stationary4MaxDelay")) ui->textBoxSearcherMaxDelay->setText(setting.value("stationary4MaxDelay").toString());
    if (setting.contains("stationary4MinFrame")) ui->textBoxSearcherMinFrame->setText(setting.value("stationary4MinFrame").toString());
    if (setting.contains("stationary4MaxFrame")) ui->textBoxSearcherMaxFrame->setText(setting.value("stationary4MaxFrame").toString());
}

void Stationary4::updateView(const QVector<Frame4> &frames, int progress)
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
    generatorModel->clear();
    generatorModel->setMethod(static_cast<Method>(ui->comboBoxGeneratorMethod->currentData().toInt()));

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
    Generator4 generator = Generator4(maxResults, startingFrame, seed, tid, sid, offset, static_cast<Method>(ui->comboBoxGeneratorMethod->currentData().toInt()));
    FrameCompare compare = FrameCompare(ui->ivFilterGenerator->getLower(), ui->ivFilterGenerator->getUpper(),
                                        ui->comboBoxGeneratorGender->currentIndex(), genderRatioIndex, ui->comboBoxGeneratorAbility->currentIndex(),
                                        ui->comboBoxGeneratorNature->getChecked(), ui->comboBoxGeneratorHiddenPower->getChecked(),
                                        ui->checkBoxGeneratorShinyOnly->isChecked(), ui->checkBoxGeneratorDisableFilters->isChecked());

    generator.setEncounterType(Stationary);
    if (ui->pushButtonGeneratorLead->text() == tr("Cute Charm"))
    {
        generator.setLeadType(static_cast<Lead>(ui->comboBoxGeneratorLead->currentData().toInt()));
    }
    else
    {
        int num = ui->comboBoxGeneratorLead->currentIndex();
        if (num == 0)
        {
            generator.setLeadType(Lead::None);
        }
        else
        {
            generator.setLeadType(Lead::Synchronize);
            generator.setSynchNature(Nature::getAdjustedNature(static_cast<u32>(ui->comboBoxGeneratorLead->currentIndex() - 1)));
        }
    }

    QVector<Frame4> frames = generator.generate(compare);
    generatorModel->setModel(frames);
}

void Stationary4::on_pushButtonSearch_clicked()
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
    Searcher4 searcher = Searcher4(tid, sid, static_cast<u32>(genderRatioIndex), ui->textBoxSearcherMinDelay->getUInt(), ui->textBoxSearcherMaxDelay->getUInt(),
                                   ui->textBoxSearcherMinFrame->getUInt(), ui->textBoxSearcherMaxFrame->getUInt(), compare, static_cast<Method>(ui->comboBoxSearcherMethod->currentData().toInt()));
    searcher.setLeadType(static_cast<Lead>(ui->comboBoxSearcherLead->currentData().toInt()));

    QVector<u8> min = ui->ivFilterSearcher->getLower();
    QVector<u8> max = ui->ivFilterSearcher->getUpper();

    int maxProgress = 1;
    for (int i = 0; i < 6; i++)
    {
        maxProgress *= max.at(i) - min.at(i) + 1;
    }

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(maxProgress);

    auto *search = new IVSearcher4(searcher, min, max);
    auto *timer = new QTimer(search);

    connect(search, &IVSearcher4::finished, timer, &QTimer::stop);
    connect(search, &IVSearcher4::finished, this, [ = ] { ui->pushButtonSearch->setEnabled(true); ui->pushButtonCancel->setEnabled(false); });
    connect(search, &IVSearcher4::finished, this, [ = ] { updateView(search->getResults(), search->currentProgress()); });
    connect(timer, &QTimer::timeout, this, [ = ] { updateView(search->getResults(), search->currentProgress()); });
    connect(ui->pushButtonCancel, &QPushButton::clicked, search, &IVSearcher4::cancelSearch);

    search->start();
    timer->start(1000);
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

    bool flag = profile.getVersion() & Game::HGSS;

    ui->comboBoxGeneratorMethod->clear();
    ui->comboBoxGeneratorMethod->addItem(tr("Method 1"), Method::Method1);
    ui->comboBoxGeneratorMethod->addItem(flag ? tr("Method K") : tr("Method J"), flag ? Method::MethodK : Method::MethodJ);
    ui->comboBoxGeneratorMethod->addItem(tr("Wondercard IVs"), Method::WondercardIVs);

    ui->comboBoxSearcherMethod->clear();
    ui->comboBoxSearcherMethod->addItem(tr("Method 1"), Method::Method1);
    ui->comboBoxSearcherMethod->addItem(flag ? tr("Method K") : tr("Method J"), flag ? Method::MethodK : Method::MethodJ);
    ui->comboBoxSearcherMethod->addItem(tr("Wondercard IVs"), Method::WondercardIVs);
}

void Stationary4::on_pushButtonGeneratorLead_clicked()
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
    else
    {
        ui->pushButtonGeneratorLead->setText(tr("Synchronize"));

        ui->comboBoxGeneratorLead->addItem("None");
        ui->comboBoxGeneratorLead->addItems(Nature::getNatures());
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
