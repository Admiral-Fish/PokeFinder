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

#include "IVFilter.hpp"
#include "ui_IVFilter.h"

IVFilter::IVFilter(QWidget *parent) : QWidget(parent), ui(new Ui::IVFilter)
{
    ui->setupUi(this);

    QStringList tips = { tr("Click to clear"), tr("Click holding ctrl to set 31"), tr("Click holding alt to set 30-31"),
                         tr("Click holding ctrl+alt to set 0") };

    QString tip = tips.join('\n');
    ui->labelHP->setToolTip(tip);
    ui->labelAtk->setToolTip(tip);
    ui->labelDef->setToolTip(tip);
    ui->labelSpA->setToolTip(tip);
    ui->labelSpD->setToolTip(tip);
    ui->labelSpe->setToolTip(tip);
}

IVFilter::~IVFilter()
{
    delete ui;
}

QVector<u8> IVFilter::getLower() const
{
    QVector<u8> low = { static_cast<u8>(ui->spinBoxHPMin->value()),  static_cast<u8>(ui->spinBoxAtkMin->value()),
                        static_cast<u8>(ui->spinBoxDefMin->value()), static_cast<u8>(ui->spinBoxSpAMin->value()),
                        static_cast<u8>(ui->spinBoxSpDMin->value()), static_cast<u8>(ui->spinBoxSpeMin->value()) };

    return low;
}

QVector<u8> IVFilter::getUpper() const
{
    QVector<u8> high = { static_cast<u8>(ui->spinBoxHPMax->value()),  static_cast<u8>(ui->spinBoxAtkMax->value()),
                         static_cast<u8>(ui->spinBoxDefMax->value()), static_cast<u8>(ui->spinBoxSpAMax->value()),
                         static_cast<u8>(ui->spinBoxSpDMax->value()), static_cast<u8>(ui->spinBoxSpeMax->value()) };

    return high;
}

void IVFilter::clearValues()
{
    changeHP(0, 31);
    changeAtk(0, 31);
    changeDef(0, 31);
    changeSpA(0, 31);
    changeSpD(0, 31);
    changeSpe(0, 31);
}

void IVFilter::setValues(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe)
{
    changeHP(hp, hp);
    changeAtk(atk, atk);
    changeDef(def, def);
    changeSpA(spa, spa);
    changeSpD(spd, spd);
    changeSpe(spe, spe);
}

void IVFilter::changeHP(int min, int max)
{
    ui->spinBoxHPMin->setValue(min);
    ui->spinBoxHPMax->setValue(max);
}

void IVFilter::changeAtk(int min, int max)
{
    ui->spinBoxAtkMin->setValue(min);
    ui->spinBoxAtkMax->setValue(max);
}

void IVFilter::changeDef(int min, int max)
{
    ui->spinBoxDefMin->setValue(min);
    ui->spinBoxDefMax->setValue(max);
}

void IVFilter::changeSpA(int min, int max)
{
    ui->spinBoxSpAMin->setValue(min);
    ui->spinBoxSpAMax->setValue(max);
}

void IVFilter::changeSpD(int min, int max)
{
    ui->spinBoxSpDMin->setValue(min);
    ui->spinBoxSpDMax->setValue(max);
}

void IVFilter::changeSpe(int min, int max)
{
    ui->spinBoxSpeMin->setValue(min);
    ui->spinBoxSpeMax->setValue(max);
}

void IVFilter::changeCompareHP(int type)
{
    if (type == Qt::NoModifier)
    {
        changeHP(0, 31);
    }
    else if (type == Qt::ControlModifier)
    {
        changeHP(31, 31);
    }
    else if (type == Qt::AltModifier)
    {
        changeHP(30, 31);
    }
    else if (type & Qt::ControlModifier && type & Qt::AltModifier)
    {
        changeHP(0, 0);
    }
}

void IVFilter::changeCompareAtk(int type)
{
    if (type == Qt::NoModifier)
    {
        changeAtk(0, 31);
    }
    else if (type == Qt::ControlModifier)
    {
        changeAtk(31, 31);
    }
    else if (type == Qt::AltModifier)
    {
        changeAtk(30, 31);
    }
    else if (type & Qt::ControlModifier && type & Qt::AltModifier)
    {
        changeAtk(0, 0);
    }
}

void IVFilter::changeCompareDef(int type)
{
    if (type == Qt::NoModifier)
    {
        changeDef(0, 31);
    }
    else if (type == Qt::ControlModifier)
    {
        changeDef(31, 31);
    }
    else if (type == Qt::AltModifier)
    {
        changeDef(30, 31);
    }
    else if (type & Qt::ControlModifier && type & Qt::AltModifier)
    {
        changeDef(0, 0);
    }
}

void IVFilter::changeCompareSpA(int type)
{
    if (type == Qt::NoModifier)
    {
        changeSpA(0, 31);
    }
    else if (type == Qt::ControlModifier)
    {
        changeSpA(31, 31);
    }
    else if (type == Qt::AltModifier)
    {
        changeSpA(30, 31);
    }
    else if (type & Qt::ControlModifier && type & Qt::AltModifier)
    {
        changeSpA(0, 0);
    }
}

void IVFilter::changeCompareSpD(int type)
{
    if (type == Qt::NoModifier)
    {
        changeSpD(0, 31);
    }
    else if (type == Qt::ControlModifier)
    {
        changeSpD(31, 31);
    }
    else if (type == Qt::AltModifier)
    {
        changeSpD(30, 31);
    }
    else if (type & Qt::ControlModifier && type & Qt::AltModifier)
    {
        changeSpD(0, 0);
    }
}

void IVFilter::changeCompareSpe(int type)
{
    if (type == Qt::NoModifier)
    {
        changeSpe(0, 31);
    }
    else if (type == Qt::ControlModifier)
    {
        changeSpe(31, 31);
    }
    else if (type == Qt::AltModifier)
    {
        changeSpe(30, 31);
    }
    else if (type & Qt::ControlModifier && type & Qt::AltModifier)
    {
        changeSpe(0, 0);
    }
}
