/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

IVCalculator::IVCalculator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IVCalculator)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    setupModels();
}

IVCalculator::~IVCalculator()
{
    delete ui;
}

void IVCalculator::setupModels()
{
    QVector<u16> species;
    for (u16 i = 1; i <= 493; i++)
    {
        species.append(i);
    }

    ui->comboBoxPokemon->addItems(Translator::getSpecies(species));
    ui->comboBoxNature->addItems(Nature::getFrameNatures());
    ui->comboBoxHiddenPower->addItems(Power::getPowers());

    ui->textBoxLevel->setValues(1, 100, 10);
    ui->textBoxHP->setValues(1, 800, 10);
    ui->textBoxAtk->setValues(1, 800, 10);
    ui->textBoxDef->setValues(1, 800, 10);
    ui->textBoxSpA->setValues(1, 800, 10);
    ui->textBoxSpD->setValues(1, 800, 10);
    ui->textBoxSpe->setValues(1, 800, 10);
}

void IVCalculator::displayIVs(QLabel *label, QVector<u8> ivs)
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
                        result += "-" + QString::number(ivs[i]);
                    }
                }
                else
                {
                    if (flag)
                    {
                        flag = false;
                        result += "-" + QString::number(ivs[i - 1]);
                        result += ", " + QString::number(ivs[i]);
                    }
                    else
                    {
                        result += ", " + QString::number(ivs[i]);
                    }
                }
            }
        }
    }

    label->setText(result);
}

void IVCalculator::on_pushButtonFindIVs_clicked()
{
    u8 level = ui->textBoxLevel->text().toUShort();
    u8 nature = ui->comboBoxNature->currentIndex();
    int hiddenPower = ui->comboBoxHiddenPower->currentIndex() - 1;
    int characteristic = ui->comboBoxCharacteristic->currentIndex() - 1;

    QVector<u16> stats =
    {
        ui->textBoxHP->text().toUShort(), ui->textBoxAtk->text().toUShort(), ui->textBoxDef->text().toUShort(),
        ui->textBoxSpA->text().toUShort(), ui->textBoxSpD->text().toUShort(), ui->textBoxSpe->text().toUShort()
    };

    bool valid[6] = { false, false, false, false, false, false };
    u8 minIVs[6] = { 31, 31, 31, 31, 31, 31 };
    u8 maxIVs[6] = { 0, 0, 0, 0, 0, 0 };
    QVector<u16> baseStats = { 45, 49, 49, 65, 65, 45 };

    for (u8 iv = 0; iv < 32; iv++)
    {
        double hp = (((2 * baseStats[0] + iv) * level) / 100.0) + level + 10;

        if (static_cast<u16>(hp) == stats[0])
        {
            valid[0] = true;

            if (iv >= maxIVs[0])
            {
                maxIVs[0] = iv;
            }
            if (iv <= minIVs[0])
            {
                minIVs[0] = iv;
            }
        }
    }

    for (int i = 1; i < 6; i++)
    {
        for (u8 iv = 0; iv < 32; iv++)
        {
            double stat = qFloor((((2 * baseStats[i] + iv) * level) / 100.0) + 5) * natureModifier[nature][i];

            if (static_cast<u16>(stat) == stats[i])
            {
                valid[i] = true;

                if (iv >= maxIVs[i])
                {
                    maxIVs[i] = iv;
                }
                if (iv <= minIVs[i])
                {
                    minIVs[i] = iv;
                }
            }
        }
    }

    u8 characteristicHigh = 31;
    QVector<QVector<u8>> possible(6);
    for (int i = 0; i < 6; i++)
    {
        for (u8 iv = minIVs[i]; iv <= maxIVs[i]; iv++)
        {
            if (iv <= characteristicHigh)
            {
                possible[i].append(iv);
            }
        }
    }

    displayIVs(ui->labelHPIVValue, possible[0]);
    displayIVs(ui->labelAtkIVValue, possible[1]);
    displayIVs(ui->labelDefIVValue, possible[2]);
    displayIVs(ui->labelSpAIVValue, possible[3]);
    displayIVs(ui->labelSpDIVValue, possible[4]);
    displayIVs(ui->labelSpeIVValue, possible[5]);
}
