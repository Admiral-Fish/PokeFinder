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

#include "IVCalculator.hpp"
#include "ui_IVCalculator.h"
#include <Core/Parents/Pokemon.hpp>
#include <Core/Util/Characteristic.hpp>
#include <Core/Util/IVChecker.hpp>
#include <Core/Util/Nature.hpp>
#include <Core/Util/Power.hpp>
#include <Core/Util/Translator.hpp>
#include <QSettings>

namespace PokeFinderForms
{
    IVCalculator::IVCalculator(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::IVCalculator)
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
        characteristics = PokeFinderCore::Characteristic::getCharacteristics();

        on_comboBoxGeneration_currentIndexChanged(0);
        ui->comboBoxNature->addItems(PokeFinderCore::Nature::getFrameNatures());
        ui->comboBoxHiddenPower->addItems(PokeFinderCore::Power::getPowers());
        ui->comboBoxCharacteristic->addItems(PokeFinderCore::Translator::getCharacteristic());

        QSettings setting;
        if (setting.contains("ivCalculator/geometry"))
            this->restoreGeometry(setting.value("ivCalculator/geometry").toByteArray());
    }

    void IVCalculator::displayIVs(QLabel *label, const QVector<u8> &ivs)
    {
        QString result;

        if (ivs.isEmpty())
        {
            result = tr("Invalid");
        }
        else
        {
            bool flag = false;
            for (int i = 0; i < ivs.size(); i++)
            {
                if (i == 0)
                {
                    result += QString::number(ivs.at(i));
                }
                else
                {
                    if (ivs.at(i) == ivs.at(i - 1) + 1)
                    {
                        flag = true;

                        //  Check to see if we need to cap here.
                        if (i == ivs.size() - 1)
                        {
                            result += QString("-%1").arg(ivs.at(i));
                        }
                    }
                    else
                    {
                        if (flag)
                        {
                            flag = false;
                            result += QString("-%1").arg(ivs.at(i - 1));
                            result += QString(", %1").arg(ivs.at(i));
                        }
                        else
                        {
                            result += QString(", %1").arg(ivs.at(i));
                        }
                    }
                }
            }
        }

        label->setText(result);
    }

    void IVCalculator::on_pushButtonFindIVs_clicked()
    {
        QVector<u16> stats = { static_cast<u16>(ui->spinBoxHP->value()), static_cast<u16>(ui->spinBoxAtk->value()),
            static_cast<u16>(ui->spinBoxDef->value()), static_cast<u16>(ui->spinBoxSpA->value()),
            static_cast<u16>(ui->spinBoxSpD->value()), static_cast<u16>(ui->spinBoxSpe->value()) };

        u8 level = ui->spinBoxLevel->value();
        u8 nature = ui->comboBoxNature->currentIndex();
        int hiddenPower = ui->comboBoxHiddenPower->currentIndex() - 1;
        PokeFinderCore::Characteristic characteristic = characteristics.at(ui->comboBoxCharacteristic->currentIndex());

        PokeFinderCore::IVChecker ivCheck;
        auto possible = ivCheck.calculateIVs(
            pokemon.at(ui->comboBoxPokemon->currentIndex() + 1), stats, level, nature, characteristic, hiddenPower);

        displayIVs(ui->labelHPIVValue, possible.at(0));
        displayIVs(ui->labelAtkIVValue, possible.at(1));
        displayIVs(ui->labelDefIVValue, possible.at(2));
        displayIVs(ui->labelSpAIVValue, possible.at(3));
        displayIVs(ui->labelSpDIVValue, possible.at(4));
        displayIVs(ui->labelSpeIVValue, possible.at(5));
    }

    void IVCalculator::on_comboBoxPokemon_currentIndexChanged(int index)
    {
        if (index >= 0 && !pokemon.isEmpty())
        {
            PokeFinderCore::Pokemon poke = pokemon.at(index + 1);

            ui->labelBaseHPValue->setText(QString::number(poke.getBaseHP()));
            ui->labelBaseAtkValue->setText(QString::number(poke.getBaseAtk()));
            ui->labelBaseDefValue->setText(QString::number(poke.getBaseDef()));
            ui->labelBaseSpAValue->setText(QString::number(poke.getBaseSpA()));
            ui->labelBaseSpDValue->setText(QString::number(poke.getBaseSpD()));
            ui->labelBaseSpeValue->setText(QString::number(poke.getBaseSpe()));
        }
    }

    void IVCalculator::on_comboBoxGeneration_currentIndexChanged(int index)
    {
        if (index >= 0)
        {
            u16 max = 0;
            if (index == 0)
            {
                pokemon = PokeFinderCore::Pokemon::loadPersonal(3);
                max = 386;
            }
            else if (index == 1)
            {
                pokemon = PokeFinderCore::Pokemon::loadPersonal(4);
                max = 493;
            }

            QVector<u16> species;
            for (u16 i = 1; i <= max; i++)
            {
                species.append(i);
            }

            ui->comboBoxPokemon->clear();
            ui->comboBoxPokemon->addItems(PokeFinderCore::Translator::getSpecies(species));
        }
    }
}
