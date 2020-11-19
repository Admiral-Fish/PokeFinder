/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Event5.hpp"
#include "ui_Event5.h"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen5/Generators/EventGenerator5.hpp>
#include <Core/Gen5/Keypresses.hpp>
#include <Core/Gen5/ProfileLoader5.hpp>
#include <Core/Gen5/Searchers/EventSearcher5.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>
#include <Forms/Gen5/Profile/ProfileManager5.hpp>
#include <Models/Gen5/EventModel5.hpp>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>

Event5::Event5(QWidget *parent) : QWidget(parent), ui(new Ui::Event5)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();
}

Event5::~Event5()
{
    QSettings setting;
    setting.beginGroup("event5");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Event5::updateProfiles()
{
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Event5::profileIndexChanged);

    profiles = ProfileLoader5::getProfiles();

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("event5/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

bool Event5::hasProfiles() const
{
    return !profiles.empty();
}

void Event5::setupModels()
{
    generatorModel = new EventGeneratorModel5(ui->tableViewGenerator);
    searcherModel = new EventSearcherModel5(ui->tableViewSearcher);
    generatorMenu = new QMenu(ui->tableViewGenerator);
    searcherMenu = new QMenu(ui->tableViewSearcher);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed64Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorEventTID->setValues(InputType::TIDSID);
    ui->textBoxGeneratorEventSID->setValues(InputType::TIDSID);

    ui->textBoxSearcherMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherEventTID->setValues(InputType::TIDSID);
    ui->textBoxSearcherEventSID->setValues(InputType::TIDSID);

    for (const std::string &nature : Translator::getNatures())
    {
        ui->comboBoxGeneratorNature->addItem(QString::fromStdString(nature));
        ui->comboBoxSearcherNature->addItem(QString::fromStdString(nature));
    }

    ui->filterGenerator->disableControls(Controls::EncounterSlots);
    ui->filterSearcher->disableControls(Controls::EncounterSlots | Controls::DisableFilter | Controls::UseDelay);

    std::vector<u16> species(649);
    std::iota(species.begin(), species.end(), 1);
    for (const std::string &specie : Translator::getSpecies(species))
    {
        ui->comboBoxGeneratorSpecies->addItem(QString::fromStdString(specie));
        ui->comboBoxSearcherSpecies->addItem(QString::fromStdString(specie));
    }

    QAction *outputTXTGenerator = generatorMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVGenerator = generatorMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTGenerator, &QAction::triggered, [=]() { ui->tableViewGenerator->outputModel(false); });
    connect(outputCSVGenerator, &QAction::triggered, [=]() { ui->tableViewGenerator->outputModel(true); });

    QAction *outputTXTSearcher = searcherMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVSearcher = searcherMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTSearcher, &QAction::triggered, [=]() { ui->tableViewSearcher->outputModel(false); });
    connect(outputCSVSearcher, &QAction::triggered, [=]() { ui->tableViewSearcher->outputModel(true); });

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Event5::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Event5::search);
    connect(ui->pushButtonCalculateInitialAdvances, &QPushButton::clicked, this, &Event5::calculateInitialAdvances);
    connect(ui->pushButtonGeneratorImport, &QPushButton::clicked, this, &Event5::generatorImportEvent);
    connect(ui->pushButtonSearcherImport, &QPushButton::clicked, this, &Event5::searcherImportEvent);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Event5::profileManager);
    connect(ui->tableViewGenerator, &QTableView::customContextMenuRequested, this, &Event5::tableViewGeneratorContextMenu);
    connect(ui->tableViewSearcher, &QTableView::customContextMenuRequested, this, &Event5::tableViewSearcherContextMenu);

    QSettings setting;
    setting.beginGroup("event5");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

PGF Event5::getGeneratorParameters() const
{
    u8 hp = ui->checkBoxGeneratorHP->isChecked() ? ui->spinBoxGeneratorHP->value() : 255;
    u8 atk = ui->checkBoxGeneratorAtk->isChecked() ? ui->spinBoxGeneratorAtk->value() : 255;
    u8 def = ui->checkBoxGeneratorDef->isChecked() ? ui->spinBoxGeneratorDef->value() : 255;
    u8 spa = ui->checkBoxGeneratorSpA->isChecked() ? ui->spinBoxGeneratorSpA->value() : 255;
    u8 spd = ui->checkBoxGeneratorSpD->isChecked() ? ui->spinBoxGeneratorSpD->value() : 255;
    u8 spe = ui->checkBoxGeneratorSpe->isChecked() ? ui->spinBoxGeneratorSpe->value() : 255;
    u8 nature = ui->checkBoxGeneratorNature->isChecked() ? ui->comboBoxGeneratorNature->currentIndex() : 255;

    return PGF(ui->textBoxGeneratorEventTID->getUShort(), ui->textBoxGeneratorEventSID->getUShort(),
               ui->comboBoxGeneratorSpecies->currentIndex() + 1, nature, ui->comboBoxGeneratorGender->currentIndex(),
               ui->comboBoxGeneratorAbility->currentIndex(), ui->comboBoxGeneratorShiny->currentIndex(), hp, atk, def, spa, spd, spe,
               ui->checkBoxGeneratorEgg->isChecked());
}

PGF Event5::getSearcherParameters() const
{
    u8 hp = ui->checkBoxSearcherHP->isChecked() ? ui->spinBoxSearcherHP->value() : 255;
    u8 atk = ui->checkBoxSearcherAtk->isChecked() ? ui->spinBoxSearcherAtk->value() : 255;
    u8 def = ui->checkBoxSearcherDef->isChecked() ? ui->spinBoxSearcherDef->value() : 255;
    u8 spa = ui->checkBoxSearcherSpA->isChecked() ? ui->spinBoxSearcherSpA->value() : 255;
    u8 spd = ui->checkBoxSearcherSpD->isChecked() ? ui->spinBoxSearcherSpD->value() : 255;
    u8 spe = ui->checkBoxSearcherSpe->isChecked() ? ui->spinBoxSearcherSpe->value() : 255;
    u8 nature = ui->checkBoxSearcherNature->isChecked() ? ui->comboBoxSearcherNature->currentIndex() : 255;

    return PGF(ui->textBoxSearcherEventTID->getUShort(), ui->textBoxSearcherEventSID->getUShort(),
               ui->comboBoxSearcherSpecies->currentIndex() + 1, nature, ui->comboBoxSearcherGender->currentIndex(),
               ui->comboBoxSearcherAbility->currentIndex(), ui->comboBoxSearcherShiny->currentIndex(), hp, atk, def, spa, spd, spe,
               ui->checkBoxSearcherEgg->isChecked());
}

void Event5::updateProgress(const std::vector<SearcherState5<State>> &states, int progress)
{
    searcherModel->addItems(states);
    ui->progressBar->setValue(progress);
}

void Event5::generate()
{
    generatorModel->clearModel();

    u64 seed = ui->textBoxGeneratorSeed->getULong();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterGenerator->getGenderRatio();
    u32 offset = 0;
    if (ui->filterGenerator->useDelay())
    {
        offset = ui->filterGenerator->getDelay();
    }

    StateFilter filter(ui->filterGenerator->getGender(), ui->filterGenerator->getAbility(), ui->filterGenerator->getShiny(),
                       ui->filterGenerator->getDisableFilters(), ui->filterGenerator->getMinIVs(), ui->filterGenerator->getMaxIVs(),
                       ui->filterGenerator->getNatures(), ui->filterGenerator->getHiddenPowers(), {});

    EventGenerator5 generator(initialAdvances, maxAdvances, tid, sid, genderRatio, Method::Method5Event, filter, getGeneratorParameters());
    generator.setOffset(offset);

    auto states = generator.generate(seed);
    generatorModel->addItems(states);
}

void Event5::search()
{
    searcherModel->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u32 maxAdvances = ui->textBoxSearcherMaxAdvances->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterSearcher->getGenderRatio();

    StateFilter filter(ui->filterSearcher->getGender(), ui->filterSearcher->getAbility(), ui->filterSearcher->getShiny(),
                       ui->filterSearcher->getDisableFilters(), ui->filterSearcher->getMinIVs(), ui->filterSearcher->getMaxIVs(),
                       ui->filterSearcher->getNatures(), ui->filterSearcher->getHiddenPowers(), {});

    EventGenerator5 generator(0, maxAdvances, tid, sid, genderRatio, Method::Method5Event, filter, getSearcherParameters());
    generator.setOffset(0);

    auto *searcher = new EventSearcher5(currentProfile);

    Date start = ui->dateEditSearcherStartDate->getDate();
    Date end = ui->dateEditSearcherEndDate->getDate();

    int maxProgress = Keypresses::getKeyPresses(currentProfile.getKeypresses(), currentProfile.getSkipLR()).size();
    maxProgress *= 86400 * (start.daysTo(end) + 1);
    maxProgress *= (currentProfile.getTimer0Max() - currentProfile.getTimer0Min() + 1);
    ui->progressBar->setRange(0, maxProgress);

    QSettings settings;
    int threads = settings.value("settings/threads").toInt();

    auto *thread = QThread::create([=] { searcher->startSearch(generator, threads, start, end); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    connect(timer, &QTimer::timeout, [=] { updateProgress(searcher->getResults(), searcher->getProgress()); });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, [=] {
        ui->pushButtonSearch->setEnabled(true);
        ui->pushButtonCancel->setEnabled(false);
        updateProgress(searcher->getResults(), searcher->getProgress());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}

void Event5::generatorImportEvent()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select a wondercard file", QDir::currentPath(), "Wondercard (*.pgf)");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray data = file.readAll();
            file.close();
            if (data.size() != 204)
            {
                QMessageBox error;
                error.setText("Invalid format for wondercard");
                error.exec();
                return;
            }

            PGF pgf(data);

            ui->textBoxGeneratorEventTID->setText(QString::number(pgf.getTID()));
            ui->textBoxGeneratorEventSID->setText(QString::number(pgf.getSID()));

            ui->comboBoxGeneratorSpecies->setCurrentIndex(pgf.getSpecies() - 1);

            ui->checkBoxGeneratorNature->setChecked(pgf.getNature() != 255);
            if (ui->checkBoxGeneratorNature->isChecked())
            {
                ui->comboBoxGeneratorNature->setCurrentIndex(pgf.getNature());
            }

            ui->comboBoxGeneratorGender->setCurrentIndex(pgf.getGender());
            ui->comboBoxGeneratorAbility->setCurrentIndex(pgf.getAbilityType());
            ui->comboBoxGeneratorShiny->setCurrentIndex(pgf.getPIDType());

            std::vector<QCheckBox *> checkBoxes = { ui->checkBoxGeneratorHP,  ui->checkBoxGeneratorAtk, ui->checkBoxGeneratorDef,
                                                    ui->checkBoxGeneratorSpA, ui->checkBoxGeneratorSpD, ui->checkBoxGeneratorSpe };
            std::vector<QSpinBox *> spinBoxes = { ui->spinBoxGeneratorHP,  ui->spinBoxGeneratorAtk, ui->spinBoxGeneratorDef,
                                                  ui->spinBoxGeneratorSpA, ui->spinBoxGeneratorSpD, ui->spinBoxGeneratorSpe };

            for (u8 i = 0; i < 6; i++)
            {
                u8 iv = pgf.getIV(i);
                if (iv != 255)
                {
                    checkBoxes[i]->setChecked(true);
                    spinBoxes[i]->setValue(iv);
                }
                else
                {
                    checkBoxes[i]->setChecked(false);
                }
            }

            ui->checkBoxGeneratorEgg->setChecked(pgf.isEgg());

            ui->filterGenerator->setGenderRatio(PersonalInfo::loadPersonal(5).at(pgf.getSpecies()).getGender());
        }
        else
        {
            QMessageBox error;
            error.setText("There was a problem opening the wondercard");
            error.exec();
            return;
        }
    }
}

void Event5::searcherImportEvent()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select a wondercard file", QDir::currentPath(), "Wondercard (*.pgf)");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray data = file.readAll();
            file.close();
            if (data.size() != 204)
            {
                QMessageBox error;
                error.setText("Invalid format for wondercard");
                error.exec();
                return;
            }

            PGF pgf(data);

            ui->textBoxSearcherEventTID->setText(QString::number(pgf.getTID()));
            ui->textBoxSearcherEventSID->setText(QString::number(pgf.getSID()));

            ui->comboBoxSearcherSpecies->setCurrentIndex(pgf.getSpecies() - 1);

            ui->checkBoxSearcherNature->setChecked(pgf.getNature() != 255);
            if (ui->checkBoxSearcherNature->isChecked())
            {
                ui->comboBoxSearcherNature->setCurrentIndex(pgf.getNature());
            }

            ui->comboBoxSearcherGender->setCurrentIndex(pgf.getGender());
            ui->comboBoxSearcherAbility->setCurrentIndex(pgf.getAbilityType());
            ui->comboBoxSearcherShiny->setCurrentIndex(pgf.getPIDType());

            std::vector<QCheckBox *> checkBoxes = { ui->checkBoxSearcherHP,  ui->checkBoxSearcherAtk, ui->checkBoxSearcherDef,
                                                    ui->checkBoxSearcherSpA, ui->checkBoxSearcherSpD, ui->checkBoxSearcherSpe };
            std::vector<QSpinBox *> spinBoxes = { ui->spinBoxSearcherHP,  ui->spinBoxSearcherAtk, ui->spinBoxSearcherDef,
                                                  ui->spinBoxSearcherSpA, ui->spinBoxSearcherSpD, ui->spinBoxSearcherSpe };

            for (u8 i = 0; i < 6; i++)
            {
                u8 iv = pgf.getIV(i);
                if (iv != 255)
                {
                    checkBoxes[i]->setChecked(true);
                    spinBoxes[i]->setValue(iv);
                }
                else
                {
                    checkBoxes[i]->setChecked(false);
                }
            }

            ui->checkBoxSearcherEgg->setChecked(pgf.isEgg());

            ui->filterSearcher->setGenderRatio(PersonalInfo::loadPersonal(5).at(pgf.getSpecies()).getGender());
        }
        else
        {
            QMessageBox error;
            error.setText("There was a problem opening the wondercard");
            error.exec();
            return;
        }
    }
}

void Event5::calculateInitialAdvances()
{
    Game version = currentProfile.getVersion();

    u8 initialAdvances;
    if (version & Game::BW)
    {
        initialAdvances = Utilities::initialAdvancesBW(ui->textBoxGeneratorSeed->getULong());
    }
    else
    {
        initialAdvances = Utilities::initialAdvancesBW2(ui->textBoxGeneratorSeed->getULong(), currentProfile.getMemoryLink());
    }

    ui->textBoxGeneratorInitialAdvances->setText(QString::number(initialAdvances));
}

void Event5::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = profiles.at(index);

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

void Event5::tableViewGeneratorContextMenu(QPoint pos)
{
    if (generatorModel->rowCount() > 0)
    {
        generatorMenu->popup(ui->tableViewGenerator->viewport()->mapToGlobal(pos));
    }
}

void Event5::tableViewSearcherContextMenu(QPoint pos)
{
    if (searcherModel->rowCount() == 0)
    {
        searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
    }
}

void Event5::profileManager()
{
    auto *manager = new ProfileManager5();
    connect(manager, &ProfileManager5::updateProfiles, this, [=] { emit alertProfiles(5); });
    manager->show();
}
