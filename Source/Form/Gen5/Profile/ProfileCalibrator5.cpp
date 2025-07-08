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

#include "ProfileCalibrator5.hpp"
#include "ui_ProfileCalibrator5.h"
#include <Core/Enum/DSType.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Language.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/Searchers/ProfileSearcher5.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Gen5/Profile/ProfileEditor5.hpp>
#include <Form/Util/IVCalculator.hpp>
#include <Model/Gen5/ProfileSearcherModel5.hpp>
#include <QAction>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>

static const QMap<QString, u8> needleMap
    = { { "↑", 0 }, { "↗", 1 }, { "→", 2 }, { "↘", 3 }, { "↓", 4 }, { "↙", 5 }, { "←", 6 }, { "↖", 7 } };

ProfileCalibrator5::ProfileCalibrator5(QWidget *parent) : QWidget(parent), ui(new Ui::ProfileCalibrator5)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    model = new ProfileSearcherModel5(ui->tableView);
    ui->tableView->setModel(model);

    ui->textBoxSeed->setValues(InputType::Seed64Bit);
    ui->textBoxMinVCount->setValues(0, 0xFF, 2, 16);
    ui->textBoxMaxVCount->setValues(0, 0xFF, 2, 16);
    ui->textBoxMinTimer0->setValues(InputType::Seed16Bit);
    ui->textBoxMaxTimer0->setValues(InputType::Seed16Bit);
    ui->textBoxMinGxStat->setValues(0, 99, 2, 16);
    ui->textBoxMaxGxStat->setValues(0, 99, 2, 16);
    ui->textBoxMinVFrame->setValues(0, 99, 2, 16);
    ui->textBoxMaxVFrame->setValues(0, 99, 2, 16);
    ui->textBoxMACAddress->setValues(0, 0xFFFFFFFFFFFF, 12, 16);

    ui->comboBoxVersion->setup({ toInt(Game::Black), toInt(Game::White), toInt(Game::Black2), toInt(Game::White2) });
    ui->comboBoxLanguage->setup({ toInt(Language::English), toInt(Language::Spanish), toInt(Language::French), toInt(Language::Italian),
                                  toInt(Language::German), toInt(Language::Japanese), toInt(Language::Korean) });
    ui->comboBoxDSType->setup({ toInt(DSType::DS), toInt(DSType::DSi), toInt(DSType::DS3) });

    ui->comboBoxKeypress1->addItem(tr("None"), 0);
    ui->comboBoxKeypress2->addItem(tr("None"), 0);
    ui->comboBoxKeypress3->addItem(tr("None"), 0);
    for (int i = 0; i < 12; i++)
    {
        ui->comboBoxKeypress1->addItem(QString::fromStdString(Translator::getKeypress(i)), 1 << i);
        ui->comboBoxKeypress2->addItem(QString::fromStdString(Translator::getKeypress(i)), 1 << i);
        ui->comboBoxKeypress3->addItem(QString::fromStdString(Translator::getKeypress(i)), 1 << i);
    }

    ui->listWidgetNeedles->setFlow(QListView::LeftToRight);

    auto *createProfile = new QAction(tr("Create profile"), ui->tableView);
    connect(createProfile, &QAction::triggered, this, &ProfileCalibrator5::createProfile);
    ui->tableView->addAction(createProfile);

    connect(ui->pushButtonIVCalculator, &QPushButton::clicked, this, &ProfileCalibrator5::openIVCalculator);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &ProfileCalibrator5::search);
    connect(ui->comboBoxVersion, &QComboBox::currentIndexChanged, this, &ProfileCalibrator5::versionIndexChanged);
    connect(ui->comboBoxDSType, &QComboBox::currentIndexChanged, this, &ProfileCalibrator5::dsTypeIndexChanged);
    connect(ui->buttonGroupNeedles, &QButtonGroup::buttonClicked, this, &ProfileCalibrator5::addNeedle);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &ProfileCalibrator5::removeNeedle);
    connect(ui->pushButtonClear, &QPushButton::clicked, this, &ProfileCalibrator5::clearNeedles);

    updateParameters();

    QSettings setting;
    setting.beginGroup("profileCalibrator5");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

ProfileCalibrator5::~ProfileCalibrator5()
{
    QSettings setting;
    setting.beginGroup("profileCalibrator5");
    setting.setValue("geometry", this->saveGeometry());

    delete ui;
}

void ProfileCalibrator5::updateParameters()
{
    auto version = static_cast<Game>(ui->comboBoxVersion->getCurrentUInt());
    auto dsType = static_cast<DSType>(ui->comboBoxDSType->getCurrentInt());

    switch (dsType)
    {
    case DSType::DS:
        if ((version & Game::BW) != Game::None)
        {
            ui->textBoxMinVCount->setText("50");
            ui->textBoxMaxVCount->setText("70");
            ui->textBoxMinTimer0->setText("C60");
            ui->textBoxMaxTimer0->setText("CA0");
        }
        else
        {
            ui->textBoxMinVCount->setText("70");
            ui->textBoxMaxVCount->setText("90");
            ui->textBoxMinTimer0->setText("10E0");
            ui->textBoxMaxTimer0->setText("1130");
        }
        break;
    case DSType::DSi:
    case DSType::DS3:
        if ((version & Game::BW) != Game::None)
        {
            ui->textBoxMinVCount->setText("80");
            ui->textBoxMaxVCount->setText("92");
            ui->textBoxMinTimer0->setText("1140");
            ui->textBoxMaxTimer0->setText("12D0");
        }
        else
        {
            ui->textBoxMinVCount->setText("A0");
            ui->textBoxMaxVCount->setText("C0");
            ui->textBoxMinTimer0->setText("1400");
            ui->textBoxMaxTimer0->setText("1900");
        }
        break;
    }

    ui->textBoxMinGxStat->setText("6");
    ui->textBoxMaxGxStat->setText("6");
    ui->textBoxMinVFrame->setText("0");
    ui->textBoxMaxVFrame->setText("10");
}

void ProfileCalibrator5::addNeedle(QAbstractButton *button)
{
    ui->listWidgetNeedles->addItem(button->text());
}

void ProfileCalibrator5::clearNeedles()
{
    ui->listWidgetNeedles->clear();
}

void ProfileCalibrator5::createProfile()
{
    int row = ui->tableView->currentIndex().row();
    if (row < 0)
    {
        QMessageBox msg(QMessageBox::Warning, tr("No row selected"), tr("Please select a row"));
        msg.exec();
        return;
    }

    auto version = ui->comboBoxVersion->getEnum<Game>();
    auto language = ui->comboBoxLanguage->getEnum<Language>();
    auto dsType = ui->comboBoxDSType->getEnum<DSType>();
    u64 mac = ui->textBoxMACAddress->getULong();
    auto state = model->getItem(row);

    std::unique_ptr<ProfileEditor5> dialog(new ProfileEditor5(version, language, dsType, mac, state));
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile5 profile = dialog->getProfile();
        ProfileLoader5::addProfile(profile);
        emit profilesModified(5);
    }
}

void ProfileCalibrator5::dsTypeIndexChanged(int index)
{
    if (index >= 0)
    {
        updateParameters();
    }
}

void ProfileCalibrator5::openIVCalculator() const
{
    auto *iv = new IVCalculator();
    connect(iv, &IVCalculator::ivsCalculated, this, &ProfileCalibrator5::updateIVs);
    iv->show();
}

void ProfileCalibrator5::removeNeedle()
{
    auto *item = ui->listWidgetNeedles->takeItem(ui->listWidgetNeedles->count() - 1);
    delete item;
}

void ProfileCalibrator5::search()
{
    DateTime dt = ui->dateTimeEdit->getDateTime();
    int minSeconds = ui->spinBoxMinSeconds->value();
    int maxSeconds = ui->spinBoxMaxSeconds->value();
    u8 minVCount = ui->textBoxMinVCount->getUChar();
    u8 maxVCount = ui->textBoxMaxVCount->getUChar();
    u16 minTimer0 = ui->textBoxMinTimer0->getUShort();
    u16 maxTimer0 = ui->textBoxMaxTimer0->getUShort();
    u8 minGxStat = ui->textBoxMinGxStat->getUChar();
    u8 maxGxStat = ui->textBoxMaxGxStat->getUChar();
    u8 minVFrame = ui->textBoxMinVFrame->getUChar();
    u8 maxVFrame = ui->textBoxMaxVFrame->getUChar();
    bool softReset = ui->checkBoxSoftReset->isChecked();
    auto version = static_cast<Game>(ui->comboBoxVersion->getCurrentUInt());
    auto language = static_cast<Language>(ui->comboBoxLanguage->getCurrentInt());
    auto dsType = static_cast<DSType>(ui->comboBoxDSType->getCurrentInt());
    u64 mac = ui->textBoxMACAddress->getULong();
    auto buttons = static_cast<Buttons>(ui->comboBoxKeypress1->getCurrentUShort() | ui->comboBoxKeypress2->getCurrentUShort()
                                        | ui->comboBoxKeypress3->getCurrentUShort());

    if (minSeconds > maxSeconds || minVCount > maxVCount || minTimer0 > maxTimer0 || minGxStat > maxGxStat || minVFrame > maxVFrame)
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid Input"), tr("Some min values are greater then max values"));
        msg.exec();
        return;
    }

    model->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    ProfileSearcher5 *searcher;
    if (ui->tabWidgetType->currentIndex() == 0) // IV Search
    {
        std::array<u8, 6> minIVs = { static_cast<u8>(ui->spinBoxMinHP->value()),  static_cast<u8>(ui->spinBoxMinAtk->value()),
                                     static_cast<u8>(ui->spinBoxMinDef->value()), static_cast<u8>(ui->spinBoxMinSpA->value()),
                                     static_cast<u8>(ui->spinBoxMinSpD->value()), static_cast<u8>(ui->spinBoxMinSpe->value()) };
        std::array<u8, 6> maxIVs = { static_cast<u8>(ui->spinBoxMaxHP->value()),  static_cast<u8>(ui->spinBoxMaxAtk->value()),
                                     static_cast<u8>(ui->spinBoxMaxDef->value()), static_cast<u8>(ui->spinBoxMaxSpA->value()),
                                     static_cast<u8>(ui->spinBoxMaxSpD->value()), static_cast<u8>(ui->spinBoxMaxSpe->value()) };

        searcher = new ProfileIVSearcher5(dt.getDate(), dt.getTime(), minSeconds, maxSeconds, minVCount, maxVCount, minTimer0, maxTimer0,
                                          minGxStat, maxGxStat, softReset, version, language, dsType, mac, buttons, minIVs, maxIVs);
    }
    else if (ui->tabWidgetType->currentIndex() == 1) // Needle Search
    {
        bool unovaLink = ui->radioButtonUnovaLink->isChecked();
        bool memoryLink = ui->checkBoxMemoryLink->isChecked();

        std::vector<u8> needles;
        for (int i = 0; i < ui->listWidgetNeedles->count(); i++)
        {
            auto *item = ui->listWidgetNeedles->item(i);
            needles.emplace_back(needleMap[item->text()]);
        }

        searcher = new ProfileNeedleSearcher5(dt.getDate(), dt.getTime(), minSeconds, maxSeconds, minVCount, maxVCount, minTimer0,
                                              maxTimer0, minGxStat, maxGxStat, softReset, version, language, dsType, mac, buttons, needles,
                                              unovaLink, memoryLink);
    }
    else // Seed search
    {
        u64 seed = ui->textBoxSeed->getULong();
        searcher = new ProfileSeedSearcher5(dt.getDate(), dt.getTime(), minSeconds, maxSeconds, minVCount, maxVCount, minTimer0, maxTimer0,
                                            minGxStat, maxGxStat, softReset, version, language, dsType, mac, buttons, seed);
    }

    int maxProgress = (maxTimer0 - minTimer0 + 1) * (maxGxStat - minGxStat + 1) * (maxVFrame - minVFrame + 1);
    ui->progressBar->setRange(0, maxProgress);

    QSettings settings;
    int threads = settings.value("settings/threads").toInt();

    auto *thread = QThread::create([=] { searcher->startSearch(threads, minVFrame, maxVFrame); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    connect(timer, &QTimer::timeout, this, [=] {
        model->addItems(searcher->getResults());
        ui->progressBar->setValue(searcher->getProgress());
    });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, this, [=] {
        ui->pushButtonSearch->setEnabled(true);
        ui->pushButtonCancel->setEnabled(false);
        model->addItems(searcher->getResults());
        ui->progressBar->setValue(searcher->getProgress());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}

void ProfileCalibrator5::updateIVs(const std::array<std::vector<u8>, 6> &ivs)
{
    QVector<QSpinBox *> minIVs
        = { ui->spinBoxMinHP, ui->spinBoxMinAtk, ui->spinBoxMinDef, ui->spinBoxMinSpA, ui->spinBoxMinSpD, ui->spinBoxMinSpe };
    QVector<QSpinBox *> maxIVs
        = { ui->spinBoxMaxHP, ui->spinBoxMaxAtk, ui->spinBoxMaxDef, ui->spinBoxMaxSpA, ui->spinBoxMaxSpD, ui->spinBoxMaxSpe };

    for (size_t i = 0; i < ivs.size(); i++)
    {
        std::vector<u8> iv = ivs[i];

        u8 min = 0;
        u8 max = 31;

        // Vector is sorted, grab first/last as min/max
        if (!iv.empty())
        {
            min = iv.front();
            max = iv.back();
        }

        minIVs[i]->setValue(min);
        maxIVs[i]->setValue(max);
    }
}

void ProfileCalibrator5::versionIndexChanged(int index)
{
    if (index >= 0)
    {
        updateParameters();
    }
}
