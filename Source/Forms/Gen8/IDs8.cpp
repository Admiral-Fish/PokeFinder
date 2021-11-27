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

#include "IDs8.hpp"
#include "ui_IDs8.h"
#include <Core/Gen8/Generators/IDGenerator8.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Forms/Gen8/Profile/ProfileManager8.hpp>
#include <Forms/Models/Gen8/IDModel8.hpp>
#include <QMessageBox>
#include <QRegularExpression>
#include <QSettings>

IDs8::IDs8(QWidget *parent) : QWidget(parent), ui(new Ui::IDs8)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModel();
    updateProfiles();
}

IDs8::~IDs8()
{
    QSettings settings;
    settings.setValue("ids8/geometry", this->saveGeometry());

    delete ui;
}

void IDs8::updateProfiles()
{
    profiles = ProfileLoader8::getProfiles();
    profiles.insert(profiles.begin(), Profile8(Game::BD));

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("ids8/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void IDs8::setupModel()
{
    model = new IDModel8(ui->tableView);

    ui->tableView->setModel(model);

    ui->textBoxInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSeed0->setValues(InputType::Seed64Bit);
    ui->textBoxSeed1->setValues(InputType::Seed64Bit);

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &IDs8::generate);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &IDs8::profileManager);
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &IDs8::profilesIndexChanged);

    QSettings setting;
    if (setting.contains("ids8/geometry"))
    {
        this->restoreGeometry(setting.value("ids8/geometry").toByteArray());
    }
}

void IDs8::generate()
{
    model->clearModel();

    u32 initialAdvances = ui->textBoxInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxMaxAdvances->getUInt();
    u64 seed0 = ui->textBoxSeed0->getULong();
    u64 seed1 = ui->textBoxSeed1->getULong();

    std::vector<u16> tidFilter;
    std::vector<u16> sidFilter;
    std::vector<u16> tsvFilter;
    std::vector<u32> g8tidFilter;

    QString inputs = ui->textEditFilter->toPlainText();
    if (ui->radioButtonTID->isChecked())
    {
        QRegularExpression re("^\\d{1,5}$", QRegularExpression::MultilineOption);
        auto matches = re.globalMatch(inputs);
        while (matches.hasNext())
        {
            auto match = matches.next();
            tidFilter.emplace_back(match.captured().toUShort());
        }
    }
    else if (ui->radioButtonSID->isChecked())
    {
        QRegularExpression re("^\\d{1,5}$", QRegularExpression::MultilineOption);
        auto matches = re.globalMatch(inputs);
        while (matches.hasNext())
        {
            auto match = matches.next().captured();
            sidFilter.emplace_back(match.toUShort());
        }
    }
    else if (ui->radioButtonTIDSID->isChecked())
    {
        QRegularExpression re("^(\\d{1,5})/(\\d{1,5})$", QRegularExpression::MultilineOption);
        auto matches = re.globalMatch(inputs);
        while (matches.hasNext())
        {
            auto match = matches.next();
            tidFilter.emplace_back(match.captured(1).toUShort());
            sidFilter.emplace_back(match.captured(2).toUShort());
        }
    }
    else if (ui->radioButtonG8TID->isChecked())
    {
        QRegularExpression re("^\\d{1,6}$", QRegularExpression::MultilineOption);
        auto matches = re.globalMatch(inputs);
        while (matches.hasNext())
        {
            auto match = matches.next().captured();
            g8tidFilter.emplace_back(match.toUInt());
        }
    }

    inputs = ui->textEditTSVFilter->toPlainText();
    QRegularExpression re("^\\d{1,5}$", QRegularExpression::MultilineOption);
    auto matches = re.globalMatch(inputs);
    while (matches.hasNext())
    {
        auto match = matches.next().captured();
        tsvFilter.emplace_back(match.toUShort());
    }

    IDFilter8 filter(tidFilter, sidFilter, tsvFilter, g8tidFilter);
    IDGenerator8 generator(initialAdvances, maxAdvances, filter);

    auto states = generator.generate(seed0, seed1);
    model->addItems(states);
}

void IDs8::profileManager()
{
    auto *manager = new ProfileManager8();
    connect(manager, &ProfileManager8::updateProfiles, [=]() { emit alertProfiles(8); });
    manager->show();
    manager->raise();
}

void IDs8::profilesIndexChanged(int index)
{
    if (index >= 0)
    {
        auto profile = profiles[index];
        ui->labelProfileTIDValue->setText(QString::number(profile.getTID()));
        ui->labelProfileSIDValue->setText(QString::number(profile.getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(profile.getVersionString()));

        QSettings setting;
        setting.setValue("ids8/profile", index);
    }
}
