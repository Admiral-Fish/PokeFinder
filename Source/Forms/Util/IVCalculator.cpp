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

#include "IVCalculator.hpp"
#include "ui_IVCalculator.h"
#include <Core/Enum/Game.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Util/IVChecker.hpp>
#include <Core/Util/Translator.hpp>
#include <QCompleter>
#include <QMessageBox>
#include <QSettings>
#include <QSpinBox>

IVCalculator::IVCalculator(QWidget *parent) : QWidget(parent), ui(new Ui::IVCalculator)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

IVCalculator::~IVCalculator()
{
    QSettings setting;
    setting.setValue("ivCalculator/geometry", this->saveGeometry());

    delete ui;
}

void IVCalculator::setupModels()
{
    for (const std::string &nature : Translator::getNatures())
    {
        ui->comboBoxNature->addItem(QString::fromStdString(nature));
    }
    for (const std::string &hiddenPower : Translator::getHiddenPowers())
    {
        ui->comboBoxHiddenPower->addItem(QString::fromStdString(hiddenPower));
    }
    for (const std::string &characteristic : Translator::getCharacteristic())
    {
        ui->comboBoxCharacteristic->addItem(QString::fromStdString(characteristic));
    }

    ui->comboBoxPokemon->setEditable(true);
    ui->comboBoxPokemon->setInsertPolicy(QComboBox::NoInsert);
    ui->comboBoxPokemon->completer()->setCompletionMode(QCompleter::PopupCompletion);

    ui->comboBoxGame->setup({ toInt(Game::Emerald), toInt(Game::RS), toInt(Game::FireRed), toInt(Game::LeafGreen), toInt(Game::DP),
                              toInt(Game::Platinum), toInt(Game::HGSS), toInt(Game::BW), toInt(Game::BW2), toInt(Game::SwSh),
                              toInt(Game::BDSP) });

    addEntry();

    connect(ui->pushButtonAddRow, &QPushButton::clicked, this, &IVCalculator::addEntry);
    connect(ui->pushButtonRemoveRow, &QPushButton::clicked, this, &IVCalculator::removeEntry);
    connect(ui->pushButtonFindIVs, &QPushButton::clicked, this, &IVCalculator::findIVs);
    connect(ui->comboBoxPokemon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &IVCalculator::pokemonIndexChanged);
    connect(ui->comboBoxAltForm, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &IVCalculator::altformIndexChanged);
    connect(ui->comboBoxGame, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &IVCalculator::gameIndexChanged);

    gameIndexChanged(toInt(Game::Emerald));

    QSettings setting;
    if (setting.contains("ivCalculator/geometry"))
    {
        this->restoreGeometry(setting.value("ivCalculator/geometry").toByteArray());
    }
}

void IVCalculator::displayIVs(QLabel *label, const std::vector<u8> &ivs)
{
    QString result;

    if (ivs.empty())
    {
        result = tr("Invalid");
    }
    else
    {
        bool flag = false;
        for (size_t i = 0; i < ivs.size(); i++)
        {
            if (i == 0)
            {
                result += QString::number(ivs[i]);
            }
            else
            {
                if (ivs[i] == ivs[i - 1] + 1)
                {
                    flag = true;

                    //  Check to see if we need to cap here.
                    if (i == ivs.size() - 1)
                    {
                        result += QString("-%1").arg(ivs[i]);
                    }
                }
                else
                {
                    if (flag)
                    {
                        flag = false;
                        result += QString("-%1").arg(ivs[i - 1]);
                        result += QString(", %1").arg(ivs[i]);
                    }
                    else
                    {
                        result += QString(", %1").arg(ivs[i]);
                    }
                }
            }
        }
    }

    label->setText(result);
}

void IVCalculator::addEntry()
{
    auto *level = new QSpinBox(ui->scrollAreaWidgetContents);
    level->setRange(1, 100);

    auto *hp = new QSpinBox(ui->scrollAreaWidgetContents);
    hp->setRange(1, 651);

    auto *atk = new QSpinBox(ui->scrollAreaWidgetContents);
    atk->setRange(1, 437);

    auto *def = new QSpinBox(ui->scrollAreaWidgetContents);
    def->setRange(1, 545);

    auto *spa = new QSpinBox(ui->scrollAreaWidgetContents);
    spa->setRange(1, 420);

    auto *spd = new QSpinBox(ui->scrollAreaWidgetContents);
    spd->setRange(1, 545);

    auto *spe = new QSpinBox(ui->scrollAreaWidgetContents);
    spe->setRange(1, 435);

    rows++;
    ui->gridLayoutEntry->addWidget(level, rows, 0);
    ui->gridLayoutEntry->addWidget(hp, rows, 1);
    ui->gridLayoutEntry->addWidget(atk, rows, 2);
    ui->gridLayoutEntry->addWidget(def, rows, 3);
    ui->gridLayoutEntry->addWidget(spa, rows, 4);
    ui->gridLayoutEntry->addWidget(spd, rows, 5);
    ui->gridLayoutEntry->addWidget(spe, rows, 6);
}

void IVCalculator::removeEntry()
{
    if (rows == 1)
    {
        return;
    }

    for (int column = 0; column < 7; column++)
    {
        QLayoutItem *item = ui->gridLayoutEntry->itemAtPosition(rows, column);
        ui->gridLayoutEntry->removeItem(item);

        delete item->widget();
        delete item;
    }

    rows--;
}

void IVCalculator::findIVs()
{
    std::vector<std::vector<u16>> stats;
    std::vector<u8> levels;

    for (int row = 1; row <= rows; row++)
    {
        QLayoutItem *item = ui->gridLayoutEntry->itemAtPosition(row, 0);
        auto *widget = reinterpret_cast<QSpinBox *>(item->widget());

        levels.emplace_back(widget->value());

        std::vector<u16> stat;
        for (int column = 1; column < 7; column++)
        {
            item = ui->gridLayoutEntry->itemAtPosition(row, column);
            widget = reinterpret_cast<QSpinBox *>(item->widget());
            stat.emplace_back(widget->value());
        }
        stats.emplace_back(stat);
    }

    u8 nature = static_cast<u8>(ui->comboBoxNature->currentIndex());
    u8 hiddenPower = static_cast<u8>(ui->comboBoxHiddenPower->currentIndex() - 1);
    u8 characteristic = static_cast<u8>(ui->comboBoxCharacteristic->currentIndex() - 1);

    Game version = static_cast<Game>(ui->comboBoxGame->getCurrentUInt());
    u16 specie = ui->comboBoxPokemon->currentData().toUInt();
    u8 altform = ui->comboBoxAltForm->currentIndex();
    PersonalInfo info = PersonalLoader::getPersonal(version, specie, altform);

    auto ivs = IVChecker::calculateIVRange(info.getStats(), stats, levels, nature, characteristic, hiddenPower);

    displayIVs(ui->labelHPIVValue, ivs[0]);
    displayIVs(ui->labelAtkIVValue, ivs[1]);
    displayIVs(ui->labelDefIVValue, ivs[2]);
    displayIVs(ui->labelSpAIVValue, ivs[3]);
    displayIVs(ui->labelSpDIVValue, ivs[4]);
    displayIVs(ui->labelSpeIVValue, ivs[5]);
}

void IVCalculator::pokemonIndexChanged(int index)
{
    if (index >= 0)
    {
        Game version = static_cast<Game>(ui->comboBoxGame->getCurrentUInt());
        u16 specie = ui->comboBoxPokemon->currentData().toUInt();

        PersonalInfo base = PersonalLoader::getPersonal(version, specie);
        u8 formCount = base.getFormCount();

        ui->labelAltForm->setVisible(formCount > 1);
        ui->comboBoxAltForm->setVisible(formCount > 1);

        ui->comboBoxAltForm->clear();
        for (u8 i = 0; i < formCount; i++)
        {
            ui->comboBoxAltForm->addItem(QString::number(i));
        }
    }
}

void IVCalculator::altformIndexChanged(int index)
{
    if (index >= 0)
    {
        Game version = static_cast<Game>(ui->comboBoxGame->getCurrentUInt());
        u16 specie = ui->comboBoxPokemon->currentData().toUInt();

        PersonalInfo info = PersonalLoader::getPersonal(version, specie, index);

        std::array<u8, 6> stats = info.getStats();
        ui->labelBaseHPValue->setText(QString::number(stats[0]));
        ui->labelBaseAtkValue->setText(QString::number(stats[1]));
        ui->labelBaseDefValue->setText(QString::number(stats[2]));
        ui->labelBaseSpAValue->setText(QString::number(stats[3]));
        ui->labelBaseSpDValue->setText(QString::number(stats[4]));
        ui->labelBaseSpeValue->setText(QString::number(stats[5]));
    }
}

void IVCalculator::gameIndexChanged(int index)
{
    if (index >= 0)
    {
        Game version = static_cast<Game>(ui->comboBoxGame->getCurrentUInt());

        const PersonalInfo *info = PersonalLoader::getPersonal(version);

        u16 max = 0;
        if ((version & Game::Gen3) != Game::None)
        {
            max = 386;
        }
        else if ((version & Game::Gen4) != Game::None)
        {
            max = 493;
        }
        else if ((version & Game::Gen5) != Game::None)
        {
            max = 649;
        }
        else if ((version & Game::SwSh) != Game::None)
        {
            max = 898;
        }
        else
        {
            max = 493;
        }

        std::vector<u16> species;
        species.reserve(max);
        for (int i = 1; i <= max; i++)
        {
            if (info[i].getPresent())
            {
                species.emplace_back(i);
            }
        }

        ui->comboBoxPokemon->clear();
        for (u16 specie : species)
        {
            ui->comboBoxPokemon->addItem(QString::fromStdString(Translator::getSpecies(specie)), specie);
        }
    }
}
