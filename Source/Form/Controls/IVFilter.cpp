/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Form/Util/IVCalculator.hpp>
#include <QMouseEvent>

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

    ui->labelHP->installEventFilter(this);
    ui->labelAtk->installEventFilter(this);
    ui->labelDef->installEventFilter(this);
    ui->labelSpA->installEventFilter(this);
    ui->labelSpA->installEventFilter(this);
    ui->labelSpD->installEventFilter(this);
    ui->labelSpe->installEventFilter(this);

    connect(ui->checkBoxShowStats, &QCheckBox::stateChanged, this, [=](int state) { emit showStatsChanged(state == Qt::Checked); });
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

void IVFilter::setLower(const std::array<u8, 6> ivs)
{
    ui->spinBoxHPMin->setValue(ivs[0]);
    ui->spinBoxAtkMin->setValue(ivs[1]);
    ui->spinBoxDefMin->setValue(ivs[2]);
    ui->spinBoxSpAMin->setValue(ivs[3]);
    ui->spinBoxSpDMin->setValue(ivs[4]);
    ui->spinBoxSpeMin->setValue(ivs[5]);
}

void IVFilter::setUpper(const std::array<u8, 6> ivs)
{
    ui->spinBoxHPMax->setValue(ivs[0]);
    ui->spinBoxAtkMax->setValue(ivs[1]);
    ui->spinBoxDefMax->setValue(ivs[2]);
    ui->spinBoxSpAMax->setValue(ivs[3]);
    ui->spinBoxSpDMax->setValue(ivs[4]);
    ui->spinBoxSpeMax->setValue(ivs[5]);
}

bool IVFilter::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        auto *mouse = reinterpret_cast<QMouseEvent *>(event);
        if (object == ui->labelHP)
        {
            changeCompare(ui->spinBoxHPMin, ui->spinBoxHPMax, mouse->modifiers());
            return true;
        }
        else if (object == ui->labelAtk)
        {
            changeCompare(ui->spinBoxAtkMin, ui->spinBoxAtkMax, mouse->modifiers());
            return true;
        }
        else if (object == ui->labelDef)
        {
            changeCompare(ui->spinBoxDefMin, ui->spinBoxDefMax, mouse->modifiers());
            return true;
        }
        else if (object == ui->labelSpA)
        {
            changeCompare(ui->spinBoxSpAMin, ui->spinBoxSpAMax, mouse->modifiers());
            return true;
        }
        else if (object == ui->labelSpD)
        {
            changeCompare(ui->spinBoxSpDMin, ui->spinBoxSpDMax, mouse->modifiers());
            return true;
        }
        else if (object == ui->labelSpe)
        {
            changeCompare(ui->spinBoxSpeMin, ui->spinBoxSpeMax, mouse->modifiers());
            return true;
        }
    }
    return false;
}

void IVFilter::changeCompare(QSpinBox *minBox, QSpinBox *maxBox, int type)
{
    int min;
    int max;
    if (type == Qt::NoModifier)
    {
        min = 0;
        max = 31;
    }
    else if (type == Qt::ControlModifier)
    {
        min = 31;
        max = 31;
    }
    else if (type == Qt::AltModifier)
    {
        min = 30;
        max = 31;
    }
    else if (type & Qt::ControlModifier && type & Qt::AltModifier)
    {
        min = 0;
        max = 0;
    }
    else
    {
        return;
    }

    minBox->setValue(min);
    maxBox->setValue(max);
}

void IVFilter::openIVCalculator() const
{
    auto *calculator = new IVCalculator();
    connect(calculator, &IVCalculator::ivsCalculated, this, &IVFilter::updateIVs);
    calculator->show();
}

void IVFilter::updateIVs(const std::array<std::vector<u8>, 6> &ivs)
{
    QList<QSpinBox *> minIVs
        = { ui->spinBoxHPMin, ui->spinBoxAtkMin, ui->spinBoxDefMin, ui->spinBoxSpAMin, ui->spinBoxSpDMin, ui->spinBoxSpeMin };
    QList<QSpinBox *> maxIVs
        = { ui->spinBoxHPMax, ui->spinBoxAtkMax, ui->spinBoxDefMax, ui->spinBoxSpAMax, ui->spinBoxSpDMax, ui->spinBoxSpeMax };

    for (size_t i = 0; i < ivs.size(); i++)
    {
        const auto &iv = ivs[i];

        u8 min = 0;
        u8 max = 31;

        // Vector is sorted, grab first/last as min/max
        if (!iv.empty())
        {
            min = iv.front();
            max = iv.back();
        }

        minIVs[i]->setValue(min);
        maxIVs[i]->setValue(max);
    }
}
