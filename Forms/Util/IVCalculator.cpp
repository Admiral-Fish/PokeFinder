/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Parents/PersonalInfo.hpp>
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
    ui->comboBoxNature->addItems(Translator::getNatures());
    ui->comboBoxHiddenPower->addItems(Translator::getHiddenPowers());
    ui->comboBoxCharacteristic->addItems(Translator::getCharacteristic());

    ui->comboBoxPokemon->setEditable(true);
    ui->comboBoxPokemon->setInsertPolicy(QComboBox::NoInsert);
    ui->comboBoxPokemon->completer()->setCompletionMode(QCompleter::PopupCompletion);

    addEntry();

    connect(ui->pushButtonAddRow, &QPushButton::clicked, this, &IVCalculator::addEntry);
    connect(ui->pushButtonRemoveRow, &QPushButton::clicked, this, &IVCalculator::removeEntry);
    connect(ui->pushButtonFindIVs, &QPushButton::clicked, this, &IVCalculator::findIVs);
    connect(ui->comboBoxPokemon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &IVCalculator::pokemonIndexChanged);
    connect(ui->comboBoxAltForm, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &IVCalculator::altformIndexChanged);
    connect(ui->comboBoxGeneration, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &IVCalculator::generationIndexChanged);

    generationIndexChanged(0);

    QSettings setting;
    if (setting.contains("ivCalculator/geometry"))
    {
        this->restoreGeometry(setting.value("ivCalculator/geometry").toByteArray());
    }
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

PersonalInfo IVCalculator::getPersonalInfo(const PersonalInfo &base)
{
    u8 form = static_cast<u8>(ui->comboBoxAltForm->currentIndex());
    u16 formIndex = base.getFormStatIndex();

    if (form == 0 || formIndex == 0)
    {
        return base;
    }

    return personalInfo.at(formIndex + form - 1);
}

void IVCalculator::addEntry()
{
    QSpinBox *level = new QSpinBox(ui->scrollAreaWidgetContents);
    level->setRange(1, 100);

    QSpinBox *hp = new QSpinBox(ui->scrollAreaWidgetContents);
    hp->setRange(1, 651);

    QSpinBox *atk = new QSpinBox(ui->scrollAreaWidgetContents);
    atk->setRange(1, 437);

    QSpinBox *def = new QSpinBox(ui->scrollAreaWidgetContents);
    def->setRange(1, 545);

    QSpinBox *spa = new QSpinBox(ui->scrollAreaWidgetContents);
    spa->setRange(1, 420);

    QSpinBox *spd = new QSpinBox(ui->scrollAreaWidgetContents);
    spd->setRange(1, 545);

    QSpinBox *spe = new QSpinBox(ui->scrollAreaWidgetContents);
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
    QVector<QVector<u16>> stats;
    QVector<u8> levels;

    for (int row = 1; row < rows; row++)
    {
        QLayoutItem *item = ui->gridLayoutEntry->itemAtPosition(row, 0);
        QSpinBox *widget = reinterpret_cast<QSpinBox *>(item->widget());

        levels.append(widget->value());

        QVector<u16> stat;
        for (int column = 1; column < 7; column++)
        {
            item = ui->gridLayoutEntry->itemAtPosition(row, column);
            widget = reinterpret_cast<QSpinBox *>(item->widget());
            stat.append(widget->value());
        }
        stats.append(stat);
    }

    u8 nature = static_cast<u8>(ui->comboBoxNature->currentIndex());
    u8 hiddenPower = static_cast<u8>(ui->comboBoxHiddenPower->currentIndex() - 1);
    u8 characteristic = static_cast<u8>(ui->comboBoxCharacteristic->currentIndex() - 1);
    auto base = personalInfo.at(ui->comboBoxPokemon->currentIndex() + 1);

    auto ivs = IVChecker::calculateIVRange(getPersonalInfo(base).getBaseStats(), stats, levels, nature, characteristic, hiddenPower);

    displayIVs(ui->labelHPIVValue, ivs.at(0));
    displayIVs(ui->labelAtkIVValue, ivs.at(1));
    displayIVs(ui->labelDefIVValue, ivs.at(2));
    displayIVs(ui->labelSpAIVValue, ivs.at(3));
    displayIVs(ui->labelSpDIVValue, ivs.at(4));
    displayIVs(ui->labelSpeIVValue, ivs.at(5));
}

void IVCalculator::pokemonIndexChanged(int index)
{
    if (index >= 0 && !personalInfo.isEmpty())
    {
        PersonalInfo base = personalInfo.at(index + 1);
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
        u16 specie = static_cast<u16>(ui->comboBoxPokemon->currentIndex());

        auto base = personalInfo.at(specie + 1);
        auto info = getPersonalInfo(base);

        QVector<u8> stats = info.getBaseStats();
        ui->labelBaseHPValue->setText(QString::number(stats[0]));
        ui->labelBaseAtkValue->setText(QString::number(stats[1]));
        ui->labelBaseDefValue->setText(QString::number(stats[2]));
        ui->labelBaseSpAValue->setText(QString::number(stats[3]));
        ui->labelBaseSpDValue->setText(QString::number(stats[4]));
        ui->labelBaseSpeValue->setText(QString::number(stats[5]));
    }
}

void IVCalculator::generationIndexChanged(int index)
{
    if (index >= 0)
    {
        u16 max = 0;
        if (index == 0)
        {
            personalInfo = PersonalInfo::loadPersonal(3);
            max = 386;
        }
        else if (index == 1)
        {
            personalInfo = PersonalInfo::loadPersonal(4);
            max = 493;
        }
        else if (index == 2)
        {
            personalInfo = PersonalInfo::loadPersonal(5);
            max = 649;
        }

        QVector<u16> species;
        for (u16 i = 1; i <= max; i++)
        {
            species.append(i);
        }

        ui->comboBoxPokemon->clear();
        ui->comboBoxPokemon->addItems(Translator::getSpecies(species));
    }
}
