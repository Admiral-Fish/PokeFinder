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

#include "IVFilter.hpp"
#include "ui_IVFilter.h"

IVFilter::IVFilter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IVFilter)
{
    ui->setupUi(this);

    connect(ui->hpLabel, &Label::pressed, this, &IVFilter::changeCompareHP);
    connect(ui->atkLabel, &Label::pressed, this, &IVFilter::changeCompareAtk);
    connect(ui->defLabel, &Label::pressed, this, &IVFilter::changeCompareDef);
    connect(ui->spaLabel, &Label::pressed, this, &IVFilter::changeCompareSpA);
    connect(ui->spdLabel, &Label::pressed, this, &IVFilter::changeCompareSpD);
    connect(ui->speLabel, &Label::pressed, this, &IVFilter::changeCompareSpe);
}

IVFilter::~IVFilter()
{
    delete ui;
}

QVector<quint32> IVFilter::getEvals() const
{
    QVector<quint32> evals =
    {
        static_cast<quint32>(ui->comboBoxHP->currentIndex()), static_cast<quint32>(ui->comboBoxAtk->currentIndex()),
        static_cast<quint32>(ui->comboBoxDef->currentIndex()), static_cast<quint32>(ui->comboBoxSpA->currentIndex()),
        static_cast<quint32>(ui->comboBoxSpD->currentIndex()), static_cast<quint32>(ui->comboBoxSpe->currentIndex())
    };

    return evals;
}

QVector<quint32> IVFilter::getValues() const
{
    QVector<quint32> values =
    {
        static_cast<quint32>(ui->spinBoxHP->value()), static_cast<quint32>(ui->spinBoxAtk->value()),
        static_cast<quint32>(ui->spinBoxDef->value()), static_cast<quint32>(ui->spinBoxSpA->value()),
        static_cast<quint32>(ui->spinBoxSpD->value()), static_cast<quint32>(ui->spinBoxSpe->value())
    };

    return values;
}

QVector<quint32> IVFilter::getLower() const
{
    QVector<quint32> eval = getEvals();
    QVector<quint32> ivs = getValues();

    QVector<quint32> low;

    for (int i = 0; i < 6; i++)
    {
        switch (eval[i])
        {
            case 0:
            case 3:
                low.append(0);
                break;
            case 1:
            case 2:
                low.append(ivs[i]);
                break;
        }
    }

    return low;
}

QVector<quint32> IVFilter::getUpper() const
{
    QVector<quint32> eval = getEvals();
    QVector<quint32> ivs = getValues();

    QVector<quint32> high;

    for (int i = 0; i < 6; i++)
    {
        switch (eval[i])
        {
            case 0:
            case 2:
                high.append(31);
                break;
            case 1:
            case 3:
                high.append(ivs[i]);
                break;
        }
    }

    return high;
}

void IVFilter::clearValues()
{
    changeHP(0, 0);
    changeAtk(0, 0);
    changeDef(0, 0);
    changeSpA(0, 0);
    changeSpD(0, 0);
    changeSpe(0, 0);
}

void IVFilter::setValues(quint32 hp, quint32 atk, quint32 def, quint32 spa, quint32 spd, quint32 spe)
{
    changeHP(hp, 1);
    changeAtk(atk, 1);
    changeDef(def, 1);
    changeSpA(spa, 1);
    changeSpD(spd, 1);
    changeSpe(spe, 1);
}

void IVFilter::changeHP(int val, int index)
{
    ui->spinBoxHP->setValue(val);
    ui->comboBoxHP->setCurrentIndex(index);
}

void IVFilter::changeAtk(int val, int index)
{
    ui->spinBoxAtk->setValue(val);
    ui->comboBoxAtk->setCurrentIndex(index);
}

void IVFilter::changeDef(int val, int index)
{
    ui->spinBoxDef->setValue(val);
    ui->comboBoxDef->setCurrentIndex(index);
}

void IVFilter::changeSpA(int val, int index)
{
    ui->spinBoxSpA->setValue(val);
    ui->comboBoxSpA->setCurrentIndex(index);
}

void IVFilter::changeSpD(int val, int index)
{
    ui->spinBoxSpD->setValue(val);
    ui->comboBoxSpD->setCurrentIndex(index);
}

void IVFilter::changeSpe(int val, int index)
{
    ui->spinBoxSpe->setValue(val);
    ui->comboBoxSpe->setCurrentIndex(index);
}

void IVFilter::on_pushButton31HP_clicked()
{
    changeHP(31, 1);
}

void IVFilter::on_pushButton30HP_clicked()
{
    changeHP(30, 1);
}

void IVFilter::on_pushButtonG30HP_clicked()
{
    changeHP(30, 2);
}

void IVFilter::on_pushButtonClearHP_clicked()
{
    changeHP(0, 0);
}

void IVFilter::on_pushButton31Atk_clicked()
{
    changeAtk(31, 1);
}

void IVFilter::on_pushButton30Atk_clicked()
{
    changeAtk(30, 1);
}

void IVFilter::on_pushButtonG30Atk_clicked()
{
    changeAtk(30, 2);
}

void IVFilter::on_pushButtonClearAtk_clicked()
{
    changeAtk(0, 0);
}

void IVFilter::on_pushButton31Def_clicked()
{
    changeDef(31, 1);
}

void IVFilter::on_pushButton30Def_clicked()
{
    changeDef(30, 1);
}

void IVFilter::on_pushButtonG30Def_clicked()
{
    changeDef(30, 2);
}

void IVFilter::on_pushButtonClearDef_clicked()
{
    changeDef(0, 0);
}

void IVFilter::on_pushButton31SpA_clicked()
{
    changeSpA(31, 1);
}

void IVFilter::on_pushButton30SpA_clicked()
{
    changeSpA(30, 1);
}

void IVFilter::on_pushButtonG30SpA_clicked()
{
    changeSpA(30, 2);
}

void IVFilter::on_pushButtonClearSpA_clicked()
{
    changeSpA(0, 0);
}

void IVFilter::on_pushButton31SpD_clicked()
{
    changeSpD(31, 1);
}

void IVFilter::on_pushButton30SpD_clicked()
{
    changeSpD(30, 1);
}

void IVFilter::on_pushButtonG30SpD_clicked()
{
    changeSpD(30, 2);
}

void IVFilter::on_pushButtonClearSpD_clicked()
{
    changeSpD(0, 0);
}

void IVFilter::on_pushButton31Spe_clicked()
{
    changeSpe(31, 1);
}

void IVFilter::on_pushButton30Spe_clicked()
{
    changeSpe(30, 1);
}

void IVFilter::on_pushButtonG30Spe_clicked()
{
    changeSpe(30, 2);
}

void IVFilter::on_pushButtonClearSpe_clicked()
{
    changeSpe(0, 0);
}

void IVFilter::changeCompareHP(int type)
{
    if (type == Qt::NoModifier)
    {
        ui->comboBoxHP->setCurrentIndex(0);
    }
    else if (type == Qt::ControlModifier)
    {
        ui->comboBoxHP->setCurrentIndex(1);
    }
    else if (type == Qt::AltModifier)
    {
        ui->comboBoxHP->setCurrentIndex(2);
    }
}

void IVFilter::changeCompareAtk(int type)
{
    if (type == Qt::NoModifier)
    {
        ui->comboBoxAtk->setCurrentIndex(0);
    }
    else if (type == Qt::ControlModifier)
    {
        ui->comboBoxAtk->setCurrentIndex(1);
    }
    else if (type == Qt::AltModifier)
    {
        ui->comboBoxAtk->setCurrentIndex(2);
    }
}

void IVFilter::changeCompareDef(int type)
{
    if (type == Qt::NoModifier)
    {
        ui->comboBoxDef->setCurrentIndex(0);
    }
    else if (type == Qt::ControlModifier)
    {
        ui->comboBoxDef->setCurrentIndex(1);
    }
    else if (type == Qt::AltModifier)
    {
        ui->comboBoxDef->setCurrentIndex(2);
    }
}

void IVFilter::changeCompareSpA(int type)
{
    if (type == Qt::NoModifier)
    {
        ui->comboBoxSpA->setCurrentIndex(0);
    }
    else if (type == Qt::ControlModifier)
    {
        ui->comboBoxSpA->setCurrentIndex(1);
    }
    else if (type == Qt::AltModifier)
    {
        ui->comboBoxSpA->setCurrentIndex(2);
    }
}

void IVFilter::changeCompareSpD(int type)
{
    if (type == Qt::NoModifier)
    {
        ui->comboBoxSpD->setCurrentIndex(0);
    }
    else if (type == Qt::ControlModifier)
    {
        ui->comboBoxSpD->setCurrentIndex(1);
    }
    else if (type == Qt::AltModifier)
    {
        ui->comboBoxSpD->setCurrentIndex(2);
    }
}

void IVFilter::changeCompareSpe(int type)
{
    if (type == Qt::NoModifier)
    {
        ui->comboBoxSpe->setCurrentIndex(0);
    }
    else if (type == Qt::ControlModifier)
    {
        ui->comboBoxSpe->setCurrentIndex(1);
    }
    else if (type == Qt::AltModifier)
    {
        ui->comboBoxSpe->setCurrentIndex(2);
    }
}
