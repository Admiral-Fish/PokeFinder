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
#include <QAction>
#include <QClipboard>
#include <QMenu>
#include <QMessageBox>
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

    auto *copyAction = new QAction(tr("Copy to clipboard"), this);
    addAction(copyAction);

    auto *pasteAction = new QAction(tr("Paste from clipboard"), this);
    addAction(pasteAction);

    connect(copyAction, &QAction::triggered, this, &IVFilter::setIVsToClipBoard);
    connect(pasteAction, &QAction::triggered, this, &IVFilter::setIVsFromClipBoard);

    connect(ui->checkBoxShowStats, &QCheckBox::stateChanged, this, [=](int state) { emit showStatsChanged(state == Qt::Checked); });
    connect(ui->pushButtonIVCalculator, &QPushButton::clicked, this, &IVFilter::openIVCalculator);
}

IVFilter::~IVFilter()
{
    delete ui;
}

void IVFilter::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu::exec(actions(), event->globalPos(), nullptr, this);
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

void IVFilter::setIVsToClipBoard()
{
    QString ivs = QString("%1/%2/%3/%4/%5/%6-%7/%8/%9/%10/%11/%12")
                      .arg(ui->spinBoxHPMin->value(), 2, 10, QChar('0'))
                      .arg(ui->spinBoxAtkMin->value(), 2, 10, QChar('0'))
                      .arg(ui->spinBoxDefMin->value(), 2, 10, QChar('0'))
                      .arg(ui->spinBoxSpAMin->value(), 2, 10, QChar('0'))
                      .arg(ui->spinBoxSpDMin->value(), 2, 10, QChar('0'))
                      .arg(ui->spinBoxSpeMin->value(), 2, 10, QChar('0'))
                      .arg(ui->spinBoxHPMax->value(), 2, 10, QChar('0'))
                      .arg(ui->spinBoxAtkMax->value(), 2, 10, QChar('0'))
                      .arg(ui->spinBoxDefMax->value(), 2, 10, QChar('0'))
                      .arg(ui->spinBoxSpAMax->value(), 2, 10, QChar('0'))
                      .arg(ui->spinBoxSpDMax->value(), 2, 10, QChar('0'))
                      .arg(ui->spinBoxSpeMax->value(), 2, 10, QChar('0'));

    QApplication::clipboard()->setText(ivs);
}

void IVFilter::setIVsFromClipBoard()
{
    QString text = QApplication::clipboard()->text();
    if (text.length() != 35)
    {
        QMessageBox box(QMessageBox::Warning, tr("Invalid Clipboard Length"),
                        tr("Pasting IVs expects a string that is 35 characters in length."));
        box.exec();
        return;
    }

    QStringList halves = text.split("-");
    if (halves.length() != 2)
    {
        QMessageBox box(QMessageBox::Warning, tr("Invalid Format"), tr("The clipboard text did not match the expected format."));
        box.exec();
        return;
    }

    QStringList minimums = halves[0].split("/");
    if (minimums.length() != 6)
    {
        QMessageBox box(QMessageBox::Warning, tr("Minimum IVs"), tr("The clipboard text did not contain exactly 6 minimum IVs."));
        box.exec();
        return;
    }

    QStringList maximums = halves[1].split("/");
    if (maximums.length() != 6)
    {
        QMessageBox box(QMessageBox::Warning, tr("Maximum IVs"), tr("The clipboard text did not contain exactly 6 maximum IVs."));
        box.exec();
        return;
    }

    ui->spinBoxHPMin->setValue(minimums[0].toInt());
    ui->spinBoxAtkMin->setValue(minimums[1].toInt());
    ui->spinBoxDefMin->setValue(minimums[2].toInt());
    ui->spinBoxSpAMin->setValue(minimums[3].toInt());
    ui->spinBoxSpDMin->setValue(minimums[4].toInt());
    ui->spinBoxSpeMin->setValue(minimums[5].toInt());

    ui->spinBoxHPMax->setValue(maximums[0].toInt());
    ui->spinBoxAtkMax->setValue(maximums[1].toInt());
    ui->spinBoxDefMax->setValue(maximums[2].toInt());
    ui->spinBoxSpAMax->setValue(maximums[3].toInt());
    ui->spinBoxSpDMax->setValue(maximums[4].toInt());
    ui->spinBoxSpeMax->setValue(maximums[5].toInt());
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
