/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Enum/Shiny.hpp>
#include <Core/Gen5/Generators/EventGenerator5.hpp>
#include <Core/Gen5/Keypresses.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/Searchers/Searcher5.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen5/Profile/ProfileManager5.hpp>
#include <Model/Gen5/EventModel5.hpp>
#include <Model/SortFilterProxyModel.hpp>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>

Event5::Event5(QWidget *parent) : QWidget(parent), ui(new Ui::Event5)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    generatorModel = new EventGeneratorModel5(ui->tableViewGenerator);
    ui->tableViewGenerator->setModel(generatorModel);

    searcherModel = new EventSearcherModel5(ui->tableViewSearcher);
    proxyModel = new SortFilterProxyModel(ui->tableViewSearcher, searcherModel);
    ui->tableViewSearcher->setModel(proxyModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed64Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorOffset->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorEventTID->setValues(InputType::TIDSID);
    ui->textBoxGeneratorEventSID->setValues(InputType::TIDSID);

    ui->textBoxSearcherInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherEventTID->setValues(InputType::TIDSID);
    ui->textBoxSearcherEventSID->setValues(InputType::TIDSID);

    ui->comboBoxGeneratorNature->addItems(Translator::getNatures());
    ui->comboBoxSearcherNature->addItems(Translator::getNatures());

    ui->filterGenerator->disableControls(Controls::EncounterSlots | Controls::Height | Controls::Weight);
    ui->filterSearcher->disableControls(Controls::DisableFilter | Controls::EncounterSlots | Controls::Height | Controls::Weight);

    ui->filterGenerator->enableHiddenAbility();
    ui->filterSearcher->enableHiddenAbility();

    for (const std::string &specie : Translator::getSpecies(649))
    {
        ui->comboBoxGeneratorSpecies->addItem(QString::fromStdString(specie));
        ui->comboBoxSearcherSpecies->addItem(QString::fromStdString(specie));
    }

    ui->comboBoxGeneratorSpecies->enableAutoComplete();
    ui->comboBoxSearcherSpecies->enableAutoComplete();

    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &Event5::profileIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Event5::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Event5::search);
    connect(ui->pushButtonGeneratorImport, &QPushButton::clicked, this, &Event5::generatorImportEvent);
    connect(ui->pushButtonSearcherImport, &QPushButton::clicked, this, &Event5::searcherImportEvent);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Event5::profileManager);
    connect(ui->filterGenerator, &Filter::showStatsChanged, generatorModel, &EventGeneratorModel5::setShowStats);
    connect(ui->filterSearcher, &Filter::showStatsChanged, searcherModel, &EventSearcherModel5::setShowStats);

    updateProfiles();

    QSettings setting;
    setting.beginGroup("event5");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    if (setting.contains("startDate"))
    {
        ui->dateEditSearcherStartDate->setDate(setting.value("startDate").toDate());
    }
    if (setting.contains("endDate"))
    {
        ui->dateEditSearcherEndDate->setDate(setting.value("endDate").toDate());
    }
    setting.endGroup();
}

Event5::~Event5()
{
    QSettings setting;
    setting.beginGroup("event5");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.setValue("startDate", ui->dateEditSearcherStartDate->date());
    setting.setValue("endDate", ui->dateEditSearcherEndDate->date());
    setting.endGroup();

    delete ui;
}

bool Event5::hasProfiles() const
{
    return !profiles.empty();
}

void Event5::updateProfiles()
{
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
               ui->comboBoxGeneratorAbility->currentIndex(), ui->comboBoxGeneratorShiny->getEnum<Shiny>(),
               ui->spinBoxGeneratorLevel->value(), hp, atk, def, spa, spd, spe, ui->checkBoxGeneratorEgg->isChecked());
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
               ui->comboBoxSearcherAbility->currentIndex(), ui->comboBoxSearcherShiny->getEnum<Shiny>(), ui->spinBoxSearcherLevel->value(),
               hp, atk, def, spa, spd, spe, ui->checkBoxSearcherEgg->isChecked());
}

void Event5::generate()
{
    generatorModel->clearModel();

    u64 seed = ui->textBoxGeneratorSeed->getULong();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u32 offset = ui->textBoxGeneratorOffset->getUInt();
    PGF pgf = getGeneratorParameters();

    auto filter = ui->filterGenerator->getFilter<StateFilter>();
    EventGenerator5 generator(initialAdvances, maxAdvances, offset, pgf, *currentProfile, filter);

    auto states = generator.generate(seed);
    generatorModel->addItems(states);
}

void Event5::generatorImportEvent()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select a wondercard file", QDir::currentPath(), "Wondercard (*.pgf)");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly))
        {
            if (file.size() != 204)
            {
                QMessageBox msg(QMessageBox::Warning, tr("Invalid format"), tr("Wondercard is not the correct size"));
                msg.exec();
                return;
            }

            u8 data[204];
            file.read(reinterpret_cast<char *>(data), sizeof(data));
            file.close();

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
            ui->comboBoxGeneratorAbility->setCurrentIndex(pgf.getAbility());
            ui->comboBoxGeneratorShiny->setCurrentIndex(ui->comboBoxGeneratorShiny->findData(toInt(pgf.getShiny())));
            ui->spinBoxGeneratorLevel->setValue(pgf.getLevel());

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

            ui->checkBoxGeneratorEgg->setChecked(pgf.getEgg());
        }
        else
        {
            QMessageBox msg(QMessageBox::Warning, tr("File error"), tr("There was a problem opening the wondercard"));
            msg.exec();
            return;
        }
    }
}

void Event5::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = &profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile->getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile->getSID()));
        ui->labelProfileMACAddressValue->setText(QString::number(currentProfile->getMac(), 16));
        ui->labelProfileDSTypeValue->setText(QString::fromStdString(currentProfile->getDSTypeString()));
        ui->labelProfileVCountValue->setText(QString::number(currentProfile->getVCount(), 16));
        ui->labelProfileTimer0Value->setText(QString::number(currentProfile->getTimer0Min(), 16) + "-"
                                             + QString::number(currentProfile->getTimer0Max(), 16));
        ui->labelProfileGxStatValue->setText(QString::number(currentProfile->getGxStat()));
        ui->labelProfileVFrameValue->setText(QString::number(currentProfile->getVFrame()));
        ui->labelProfileKeypressesValue->setText(QString::fromStdString(currentProfile->getKeypressesString()));
        ui->labelProfileGameValue->setText(QString::fromStdString(Translator::getGame(currentProfile->getVersion())));
    }
}

void Event5::profileManager()
{
    auto *manager = new ProfileManager5();
    connect(manager, &ProfileManager5::profilesModified, this, [=](int num) { emit profilesModified(num); });
    manager->show();
}

void Event5::search()
{
    Date start = ui->dateEditSearcherStartDate->getDate();
    Date end = ui->dateEditSearcherEndDate->getDate();
    if (start > end)
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid date range"), tr("Start date is after end date"));
        msg.exec();
        return;
    }

    searcherModel->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u32 initialAdvances = ui->textBoxSearcherInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxSearcherMaxAdvances->getUInt();
    PGF pgf = getSearcherParameters();

    auto filter = ui->filterSearcher->getFilter<StateFilter>();
    EventGenerator5 generator(initialAdvances, maxAdvances, 0, pgf, *currentProfile, filter);
    auto *searcher = new Searcher5<EventGenerator5, EventState5>(generator, *currentProfile);
    searcher->setMaxProgress(searcher->getMaxProgress(start, end));

    QSettings settings;
    int threads = settings.value("settings/threads").toInt();

    auto *thread = QThread::create([=] { searcher->startSearch(threads, start, end); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    connect(timer, &QTimer::timeout, this, [=] {
        searcherModel->addItems(searcher->getResults());
        ui->progressBar->setValue(searcher->getProgress());
    });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, this, [=] {
        ui->pushButtonSearch->setEnabled(true);
        ui->pushButtonCancel->setEnabled(false);
        searcherModel->addItems(searcher->getResults());
        ui->progressBar->setValue(searcher->getProgress());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}

void Event5::searcherImportEvent()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select a wondercard file", QDir::currentPath(), "Wondercard (*.pgf)");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly))
        {
            if (file.size() != 204)
            {
                QMessageBox msg(QMessageBox::Warning, tr("Invalid format"), tr("Wondercard is not the correct size"));
                msg.exec();
                return;
            }

            std::array<u8, 204> data;
            file.read(reinterpret_cast<char *>(data.data()), 204);
            file.close();

            PGF pgf(data.data());

            ui->textBoxSearcherEventTID->setText(QString::number(pgf.getTID()));
            ui->textBoxSearcherEventSID->setText(QString::number(pgf.getSID()));

            ui->comboBoxSearcherSpecies->setCurrentIndex(pgf.getSpecies() - 1);

            ui->checkBoxSearcherNature->setChecked(pgf.getNature() != 255);
            if (ui->checkBoxSearcherNature->isChecked())
            {
                ui->comboBoxSearcherNature->setCurrentIndex(pgf.getNature());
            }

            ui->comboBoxSearcherGender->setCurrentIndex(pgf.getGender());
            ui->comboBoxSearcherAbility->setCurrentIndex(pgf.getAbility());
            ui->comboBoxSearcherShiny->setCurrentIndex(ui->comboBoxSearcherShiny->findData(toInt(pgf.getShiny())));
            ui->spinBoxSearcherLevel->setValue(pgf.getLevel());

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

            ui->checkBoxSearcherEgg->setChecked(pgf.getEgg());
        }
        else
        {
            QMessageBox msg(QMessageBox::Warning, tr("File error"), tr("There was a problem opening the wondercard"));
            msg.exec();
            return;
        }
    }
}
