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

#include "IVFilter.hpp"
#include "ui_IVFilter.h"
#include <Forms/Util/IVCalculator.hpp>
#include <array>

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

    connect(ui->labelHP, &Label::pressed, this, &IVFilter::changeCompareHP);
    connect(ui->labelAtk, &Label::pressed, this, &IVFilter::changeCompareAtk);
    connect(ui->labelDef, &Label::pressed, this, &IVFilter::changeCompareDef);
    connect(ui->labelSpA, &Label::pressed, this, &IVFilter::changeCompareSpA);
    connect(ui->labelSpD, &Label::pressed, this, &IVFilter::changeCompareSpD);
    connect(ui->labelSpe, &Label::pressed, this, &IVFilter::changeCompareSpe);
    connect(ui->pushButtonIVCalculator, &QPushButton::clicked, this, &IVFilter::openIVCalculator);
}

IVFilter::~IVFilter()
{
    delete ui;
}

std::array<u8, 6> IVFilter::getLower() const
{
    std::array<u8, 6> low = { static_cast<u8>(ui->spinBoxHPMin->value()),  static_cast<u8>(ui->spinBoxAtkMin->value()),
                              static_cast<u8>(ui->spinBoxDefMin->value()), static_cast<u8>(ui->spinBoxSpAMin->value()),
                              static_cast<u8>(ui->spinBoxSpDMin->value()), static_cast<u8>(ui->spinBoxSpeMin->value()) };

    return low;
}

std::array<u8, 6> IVFilter::getUpper() const
{
    std::array<u8, 6> high = { static_cast<u8>(ui->spinBoxHPMax->value()),  static_cast<u8>(ui->spinBoxAtkMax->value()),
                               static_cast<u8>(ui->spinBoxDefMax->value()), static_cast<u8>(ui->spinBoxSpAMax->value()),
                               static_cast<u8>(ui->spinBoxSpDMax->value()), static_cast<u8>(ui->spinBoxSpeMax->value()) };

    return high;
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

void IVFilter::openIVCalculator()
{
    auto *calculator = new IVCalculator();
    connect(calculator, &IVCalculator::ivsCalculated, this, &IVFilter::updateIVs);
    calculator->show();
}

void IVFilter::updateIVs(const std::vector<std::vector<u8>> &ivs)
{
    QVector<QSpinBox *> minIVs
        = { ui->spinBoxHPMin, ui->spinBoxAtkMin, ui->spinBoxDefMin, ui->spinBoxSpAMin, ui->spinBoxSpDMin, ui->spinBoxSpeMin };
    QVector<QSpinBox *> maxIVs
        = { ui->spinBoxHPMax, ui->spinBoxAtkMax, ui->spinBoxDefMax, ui->spinBoxSpAMax, ui->spinBoxSpDMax, ui->spinBoxSpeMax };

    for (size_t i = 0; i < ivs.size(); i++)
    {
        std::vector<u8> iv = ivs[i];

        u8 min = 0;
        u8 max = 31;

        // Vector is sorted, grab first/last as min/max
        if (!ivs.empty())
        {
            min = iv.front();
            max = iv.back();
        }

        minIVs[i]->setValue(min);
        maxIVs[i]->setValue(max);
    }
}