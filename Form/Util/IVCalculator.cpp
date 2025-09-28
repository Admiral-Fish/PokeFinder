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

#include "IVCalculator.hpp"
#include "ui_IVCalculator.h"
#include <Core/Enum/Game.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Util/IVChecker.hpp>
#include <Core/Util/Translator.hpp>
#include <QSettings>
#include <QSpinBox>

IVCalculator::IVCalculator(QWidget *parent) : QWidget(parent), ui(new Ui::IVCalculator), rows(0)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    ui->comboBoxNature->addItems(Translator::getNatures());
    ui->comboBoxHiddenPower->addItems(Translator::getHiddenPowers());
    ui->comboBoxCharacteristic->addItems(Translator::getCharacteristics());

    ui->comboBoxPokemon->enableAutoComplete();

    ui->comboBoxGame->setup(
        { toInt(Game::Gen3), toInt(Game::Platinum), toInt(Game::HGSS), toInt(Game::BW2), toInt(Game::SwSh), toInt(Game::BDSP) });

    ui->labelNextLevel->setToolTip(tr("Next level may not be completely accurate without specifying a nature"));

    connect(ui->pushButtonAddRow, &QPushButton::clicked, this, &IVCalculator::addEntry);
    connect(ui->pushButtonRemoveRow, &QPushButton::clicked, this, &IVCalculator::removeEntry);
    connect(ui->pushButtonFindIVs, &QPushButton::clicked, this, &IVCalculator::findIVs);
    connect(ui->comboBoxPokemon, &QComboBox::currentIndexChanged, this, &IVCalculator::pokemonIndexChanged);
    connect(ui->comboBoxAltForm, &QComboBox::currentIndexChanged, this, &IVCalculator::altformIndexChanged);
    connect(ui->comboBoxGame, &QComboBox::currentIndexChanged, this, &IVCalculator::gameIndexChanged);

    addEntry();
    gameIndexChanged(0);

    QSettings setting;
    if (setting.contains("ivCalculator/geometry"))
    {
        this->restoreGeometry(setting.value("ivCalculator/geometry").toByteArray());
    }
}

IVCalculator::~IVCalculator()
{
    QSettings setting;
    setting.setValue("ivCalculator/geometry", this->saveGeometry());

    delete ui;
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

                    // Check to see if we need to cap here.
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
                        result += QString("-%1, %2").arg(ivs[i - 1]).arg(ivs[i]);
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
    hp->setRange(1, 651); // Blissey

    auto *atk = new QSpinBox(ui->scrollAreaWidgetContents);
    atk->setRange(1, 437); // Kartana

    auto *def = new QSpinBox(ui->scrollAreaWidgetContents);
    def->setRange(1, 545); // Shuckle

    auto *spa = new QSpinBox(ui->scrollAreaWidgetContents);
    spa->setRange(1, 435); // Deoxys (Attack Form)

    auto *spd = new QSpinBox(ui->scrollAreaWidgetContents);
    spd->setRange(1, 545); // Shuckle

    auto *spe = new QSpinBox(ui->scrollAreaWidgetContents);
    spe->setRange(1, 479); // Regieleki

    rows++;
    ui->gridLayoutEntry->addWidget(level, rows, 0);
    ui->gridLayoutEntry->addWidget(hp, rows, 1);
    ui->gridLayoutEntry->addWidget(atk, rows, 2);
    ui->gridLayoutEntry->addWidget(def, rows, 3);
    ui->gridLayoutEntry->addWidget(spa, rows, 4);
    ui->gridLayoutEntry->addWidget(spd, rows, 5);
    ui->gridLayoutEntry->addWidget(spe, rows, 6);
}

void IVCalculator::altformIndexChanged(int index)
{
    if (index >= 0)
    {
        Game version = ui->comboBoxGame->getEnum<Game>();
        u16 specie = ui->comboBoxPokemon->getCurrentUShort();

        const PersonalInfo *info = PersonalLoader::getPersonal(version, specie, index);

        std::array<u8, 6> stats = info->getStats();
        ui->labelBaseHPValue->setText(QString::number(stats[0]));
        ui->labelBaseAtkValue->setText(QString::number(stats[1]));
        ui->labelBaseDefValue->setText(QString::number(stats[2]));
        ui->labelBaseSpAValue->setText(QString::number(stats[3]));
        ui->labelBaseSpDValue->setText(QString::number(stats[4]));
        ui->labelBaseSpeValue->setText(QString::number(stats[5]));
    }
}

void IVCalculator::findIVs()
{
    std::vector<std::array<u16, 6>> stats;
    std::vector<u8> levels;

    for (int row = 1; row <= rows; row++)
    {
        QLayoutItem *item = ui->gridLayoutEntry->itemAtPosition(row, 0);
        auto *widget = reinterpret_cast<QSpinBox *>(item->widget());

        levels.emplace_back(widget->value());

        std::array<u16, 6> stat;
        for (int column = 1; column < 7; column++)
        {
            item = ui->gridLayoutEntry->itemAtPosition(row, column);
            widget = reinterpret_cast<QSpinBox *>(item->widget());
            stat[column - 1] = widget->value();
        }
        stats.emplace_back(stat);
    }

    u8 nature = static_cast<u8>(ui->comboBoxNature->currentIndex() - 1);
    u8 hiddenPower = static_cast<u8>(ui->comboBoxHiddenPower->currentIndex() - 1);
    u8 characteristic = static_cast<u8>(ui->comboBoxCharacteristic->currentIndex() - 1);

    Game version = ui->comboBoxGame->getEnum<Game>();
    u16 specie = ui->comboBoxPokemon->getCurrentUShort();
    u8 altform = ui->comboBoxAltForm->currentIndex();
    const PersonalInfo *info = PersonalLoader::getPersonal(version, specie, altform);

    auto ivs = IVChecker::calculateIVRange(info->getStats(), stats, levels, nature, characteristic, hiddenPower);

    displayIVs(ui->labelHPIVValue, ivs[0]);
    displayIVs(ui->labelAtkIVValue, ivs[1]);
    displayIVs(ui->labelDefIVValue, ivs[2]);
    displayIVs(ui->labelSpAIVValue, ivs[3]);
    displayIVs(ui->labelSpDIVValue, ivs[4]);
    displayIVs(ui->labelSpeIVValue, ivs[5]);

    auto nextLevel = IVChecker::nextLevel(info->getStats(), ivs, levels.back(), nature);
    ui->labelNextLevelValue->setText(QString("%1, %2, %3, %4, %5, %6")
                                         .arg(nextLevel[0])
                                         .arg(nextLevel[1])
                                         .arg(nextLevel[2])
                                         .arg(nextLevel[3])
                                         .arg(nextLevel[4])
                                         .arg(nextLevel[5]));

    emit ivsCalculated(ivs);
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
            ui->comboBoxPokemon->addItem(QString::fromStdString(Translator::getSpecie(specie)), specie);
        }
    }
}

void IVCalculator::pokemonIndexChanged(int index)
{
    if (index >= 0)
    {
        Game version = ui->comboBoxGame->getEnum<Game>();
        u16 specie = ui->comboBoxPokemon->getCurrentUShort();

        const PersonalInfo *info = PersonalLoader::getPersonal(version, specie);
        u8 formCount = info->getFormCount();

        ui->labelAltForm->setVisible(formCount > 1);
        ui->comboBoxAltForm->setVisible(formCount > 1);

        ui->comboBoxAltForm->clear();
        for (u8 i = 0; i < formCount; i++)
        {
            auto const &form = Translator::getForm(specie, i);
            if (form.empty())
            {
                ui->comboBoxAltForm->addItem(QString::number(i));
            }
            else
            {
                ui->comboBoxAltForm->addItem(QString::fromStdString(form));
            }
        }
    }
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
