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

#include "ChainedSID.hpp"
#include "ui_ChainedSID.h"
#include <Core/Enum/Game.hpp>
#include <Core/Gen4/Tools/ChainedSIDCalc.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <QSettings>
#include <QStandardItemModel>

ChainedSID::ChainedSID(QWidget *parent) : QWidget(parent), ui(new Ui::ChainedSID), chainedCalc(nullptr)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    model = new QStandardItemModel(ui->tableView);
    model->setHorizontalHeaderLabels(QStringList() << tr("IVs") << tr("Ability") << tr("Gender") << tr("Nature"));
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->textBoxTID->setValues(InputType::TIDSID);
    for (const std::string &nature : Translator::getNatures())
    {
        ui->comboBoxNature->addItem(QString::fromStdString(nature));
    }

    for (u16 i = 1; i < 493; i++)
    {
        ui->comboBoxPokemon->addItem(QString::fromStdString(Translator::getSpecie(i)), i);
    }

    connect(ui->comboBoxPokemon, &QComboBox::currentIndexChanged, this, &ChainedSID::pokemonIndexChanged);
    connect(ui->pushButtonCalculate, &QPushButton::clicked, this, &ChainedSID::calculate);
    connect(ui->pushButtonClear, &QPushButton::clicked, this, &ChainedSID::clear);

    pokemonIndexChanged(0);

    QSettings setting;
    if (setting.contains("chainedSID/geometry"))
    {
        this->restoreGeometry(setting.value("chainedSID/geometry").toByteArray());
    }
}

ChainedSID::~ChainedSID()
{
    QSettings setting;
    setting.setValue("chainedSID/geometry", this->saveGeometry());

    delete ui;
    delete chainedCalc;
}

void ChainedSID::calculate()
{
    if (!chainedCalc)
    {
        chainedCalc = new ChainedSIDCalc(ui->textBoxTID->getUShort());
        ui->textBoxTID->setEnabled(false);
    }

    const PersonalInfo *info = PersonalLoader::getPersonal(Game::DPPt, ui->comboBoxPokemon->getCurrentUShort());

    u8 hp = ui->spinBoxHP->value();
    u8 atk = ui->spinBoxAtk->value();
    u8 def = ui->spinBoxDef->value();
    u8 spa = ui->spinBoxSpA->value();
    u8 spd = ui->spinBoxSpD->value();
    u8 spe = ui->spinBoxSpe->value();
    u8 nature = ui->comboBoxNature->currentIndex();
    u16 ability = ui->comboBoxAbility->getCurrentUShort();
    u8 gender = ui->comboBoxGender->getCurrentUChar();

    QList<QStandardItem *> row;
    row << new QStandardItem(QString("%1.%2.%3.%4.%5.%6").arg(hp).arg(atk).arg(def).arg(spa).arg(spd).arg(spe));
    row << new QStandardItem(ui->comboBoxAbility->currentText());
    row << new QStandardItem(ui->comboBoxGender->currentText());
    row << new QStandardItem(ui->comboBoxNature->currentText());
    model->appendRow(row);

    chainedCalc->addEntry(hp, atk, def, spa, spd, spe, ability, gender, nature, info);
    auto sids = chainedCalc->getSIDs();
    if (sids.size() == 1)
    {
        ui->labelPossibleResults->setText(tr("SID Found: %1").arg(sids[0]));
    }
    else
    {
        ui->labelPossibleResults->setText(tr("Possible Results: %1").arg(sids.size()));
    }

    ui->spinBoxHP->setValue(0);
    ui->spinBoxAtk->setValue(0);
    ui->spinBoxDef->setValue(0);
    ui->spinBoxSpA->setValue(0);
    ui->spinBoxSpD->setValue(0);
    ui->spinBoxSpe->setValue(0);
    ui->comboBoxNature->setCurrentIndex(0);
    ui->comboBoxAbility->setCurrentIndex(0);
    ui->comboBoxGender->setCurrentIndex(0);
}

void ChainedSID::clear()
{
    if (chainedCalc)
    {
        delete chainedCalc;
        chainedCalc = nullptr;
        ui->textBoxTID->setEnabled(true);
    }

    model->removeRows(0, model->rowCount());
    ui->labelPossibleResults->setText("Possible Results: 8192");
}

void ChainedSID::pokemonIndexChanged(int index)
{
    if (index >= 0)
    {
        u16 specie = ui->comboBoxPokemon->getCurrentUShort();
        const PersonalInfo *info = PersonalLoader::getPersonal(Game::DPPt, specie);

        ui->comboBoxAbility->clear();
        if (info->getAbility(0) == info->getAbility(1))
        {
            ui->comboBoxAbility->addItem(QString::fromStdString(Translator::getAbility(info->getAbility(0))), info->getAbility(0));
        }
        else
        {
            ui->comboBoxAbility->addItem(QString::fromStdString(Translator::getAbility(info->getAbility(0))), info->getAbility(0));
            ui->comboBoxAbility->addItem(QString::fromStdString(Translator::getAbility(info->getAbility(1))), info->getAbility(1));
        }

        ui->comboBoxGender->clear();
        switch (info->getGender())
        {
        case 255: // Genderless
            ui->comboBoxGender->addItem(QString::fromStdString(Translator::getGender(2)), 2);
            break;
        case 254: // Female
            ui->comboBoxGender->addItem(QString::fromStdString(Translator::getGender(1)), 1);
            break;
        case 0: // Male
            ui->comboBoxGender->addItem(QString::fromStdString(Translator::getGender(0)), 0);
            break;
        default: // Random gender
            ui->comboBoxGender->addItem(QString::fromStdString(Translator::getGender(0)), 0);
            ui->comboBoxGender->addItem(QString::fromStdString(Translator::getGender(1)), 1);
            break;
        }
    }
}
