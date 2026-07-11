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

#include "ProfileDisplay5.hpp"
#include "ui_ProfileDisplay5.h"
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Gen5/Profile/ProfileManager5.hpp>
#include <QSettings>

static const QString settingKey = QStringLiteral("%1/profile");

ProfileDisplay5::ProfileDisplay5(QWidget *parent) : QWidget(parent), ui(new Ui::ProfileDisplay5)
{
    ui->setupUi(this);

    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &ProfileDisplay5::profileIndexChanged);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &ProfileDisplay5::profileManager);
}

ProfileDisplay5::~ProfileDisplay5()
{
    QSettings setting;
    setting.setValue(settingKey.arg(prefix), ui->comboBoxProfiles->currentIndex());

    delete ui;
}

bool ProfileDisplay5::hasProfiles() const
{
    return !profiles.empty();
}

void ProfileDisplay5::setup(const QString &prefix, Game filter)
{
    this->prefix = prefix;
    this->filter = filter;
}

void ProfileDisplay5::updateProfiles()
{
    profiles = ProfileLoader5::getProfiles(filter);

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value(settingKey.arg(prefix), 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void ProfileDisplay5::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        const Profile5 &profile = profiles[index];

        ui->labelTIDValue->setText(QString::number(profile.getTID()));
        ui->labelSIDValue->setText(QString::number(profile.getSID()));
        ui->labelMACAddressValue->setText(QString::number(profile.getMac(), 16));
        ui->labelDSTypeValue->setText(QString::fromStdString(profile.getDSTypeString()));
        ui->labelVCountValue->setText(QString::number(profile.getVCount(), 16));
        ui->labelTimer0Value->setText(QString::number(profile.getTimer0Min(), 16) + "-"
                                             + QString::number(profile.getTimer0Max(), 16));
        ui->labelGxStatValue->setText(QString::number(profile.getGxStat()));
        ui->labelVFrameValue->setText(QString::number(profile.getVFrame()));
        ui->labelKeypressesValue->setText(QString::fromStdString(profile.getKeypressesString()));
        ui->labelGameValue->setText(QString::fromStdString(Translator::getGame(profile.getVersion())));

        emit profileChanged(profile);
    }
}

void ProfileDisplay5::profileManager()
{
    auto *manager = new ProfileManager5();
    connect(manager, &ProfileManager5::profilesChanged, this, [=](int num) {
        updateProfiles();
        emit profilesChanged(num);
    });
    manager->show();
}
