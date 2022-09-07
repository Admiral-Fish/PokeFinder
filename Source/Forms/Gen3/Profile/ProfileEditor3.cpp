/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "ProfileEditor3.hpp"
#include "ui_ProfileEditor3.h"
#include <Core/Enum/Game.hpp>
#include <Core/Gen3/Profile3.hpp>
#include <QMessageBox>
#include <QSettings>

ProfileEditor3::ProfileEditor3(QWidget *parent) : QDialog(parent), ui(new Ui::ProfileEditor3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();
}

ProfileEditor3::ProfileEditor3(const Profile3 &profile, QWidget *parent) : QDialog(parent), ui(new Ui::ProfileEditor3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();

    ui->lineEditProfile->setText(QString::fromStdString(profile.getName()));
    ui->comboBoxVersion->setCurrentIndex(ui->comboBoxVersion->findData(toInt(profile.getVersion())));
    ui->textBoxTID->setText(QString::number(profile.getTID()));
    ui->textBoxSID->setText(QString::number(profile.getSID()));
    ui->checkBoxDeadBattery->setChecked(profile.getDeadBattery());
}

ProfileEditor3::~ProfileEditor3()
{
    QSettings setting;
    setting.setValue("profileEditor3/geometry", this->saveGeometry());

    delete ui;
}

Profile3 ProfileEditor3::getProfile()
{
    return Profile3(ui->lineEditProfile->text().toStdString(), static_cast<Game>(ui->comboBoxVersion->getCurrentUInt()),
                    ui->textBoxTID->getUShort(), ui->textBoxSID->getUShort(), ui->checkBoxDeadBattery->isChecked());
}

void ProfileEditor3::setupModels()
{
    ui->textBoxTID->setValues(InputType::TIDSID);
    ui->textBoxSID->setValues(InputType::TIDSID);

    ui->comboBoxVersion->setup({ toInt(Game::Ruby), toInt(Game::Sapphire), toInt(Game::FireRed), toInt(Game::LeafGreen),
                                 toInt(Game::Emerald), toInt(Game::Gales), toInt(Game::Colosseum) });

    connect(ui->pushButtonOkay, &QPushButton::clicked, this, &ProfileEditor3::okay);
    connect(ui->comboBoxVersion, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ProfileEditor3::versionIndexChanged);

    QSettings setting;
    if (setting.contains("profileEditor3/geometry"))
    {
        this->restoreGeometry(setting.value("profileEditor3/geometry").toByteArray());
    }
}

void ProfileEditor3::okay()
{
    QString input = ui->lineEditProfile->text().trimmed();
    if (input.isEmpty())
    {
        QMessageBox error;
        error.setText(tr("Enter a Profile Name."));
        error.exec();
        return;
    }

    done(QDialog::Accepted);
}

void ProfileEditor3::versionIndexChanged(int index)
{
    if (index >= 0)
    {
        bool flag = (static_cast<Game>(ui->comboBoxVersion->currentData().toInt()) & Game::RS) != Game::None;
        ui->checkBoxDeadBattery->setVisible(flag);
        if (!flag)
        {
            ui->checkBoxDeadBattery->setChecked(false);
        }
    }
}
