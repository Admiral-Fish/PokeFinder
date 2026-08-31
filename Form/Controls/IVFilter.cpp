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

#include "IVFilter.hpp"
#include "ui_IVFilter.h"
#include <Form/Util/IVCalculator.hpp>
#include <QClipboard>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QRegularExpression>

/**
 * @brief Updates min/max values based on control keys selected
 *
 * @param minBox Spinbox that has the minimum value
 * @param maxBox Spinbox that has the maximum value
 * @param type Control keys
 */
static void changeCompare(QSpinBox *minBox, QSpinBox *maxBox, int type)
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

IVFilter::IVFilter(QWidget *parent) : QWidget(parent), ui(new Ui::IVFilter), orientation(Qt::Vertical)
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
    ui->labelSpD->installEventFilter(this);
    ui->labelSpe->installEventFilter(this);

    auto *copyAction = addAction(tr("Copy IVs to clipboard"));
    auto *pasteAction = addAction(tr("Paste IVs from clipboard"));

    connect(copyAction, &QAction::triggered, this, &IVFilter::setIVsToClipBoard);
    connect(pasteAction, &QAction::triggered, this, &IVFilter::setIVsFromClipBoard);

    connect(ui->spinBoxHPMin, &QSpinBox::valueChanged, this, &IVFilter::ivsChanged);
    connect(ui->spinBoxHPMax, &QSpinBox::valueChanged, this, &IVFilter::ivsChanged);
    connect(ui->spinBoxAtkMin, &QSpinBox::valueChanged, this, &IVFilter::ivsChanged);
    connect(ui->spinBoxAtkMax, &QSpinBox::valueChanged, this, &IVFilter::ivsChanged);
    connect(ui->spinBoxDefMin, &QSpinBox::valueChanged, this, &IVFilter::ivsChanged);
    connect(ui->spinBoxDefMax, &QSpinBox::valueChanged, this, &IVFilter::ivsChanged);
    connect(ui->spinBoxSpAMin, &QSpinBox::valueChanged, this, &IVFilter::ivsChanged);
    connect(ui->spinBoxSpAMax, &QSpinBox::valueChanged, this, &IVFilter::ivsChanged);
    connect(ui->spinBoxSpDMin, &QSpinBox::valueChanged, this, &IVFilter::ivsChanged);
    connect(ui->spinBoxSpDMax, &QSpinBox::valueChanged, this, &IVFilter::ivsChanged);
    connect(ui->spinBoxSpeMin, &QSpinBox::valueChanged, this, &IVFilter::ivsChanged);
    connect(ui->spinBoxSpeMax, &QSpinBox::valueChanged, this, &IVFilter::ivsChanged);
    connect(ui->checkBoxShowStats, &QCheckBox::toggled, this, &IVFilter::showStatsChanged);
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

void IVFilter::copyFrom(const IVFilter *other)
{
    ui->spinBoxHPMin->setValue(other->ui->spinBoxHPMin->value());
    ui->spinBoxAtkMin->setValue(other->ui->spinBoxAtkMin->value());
    ui->spinBoxDefMin->setValue(other->ui->spinBoxDefMin->value());
    ui->spinBoxSpAMin->setValue(other->ui->spinBoxSpAMin->value());
    ui->spinBoxSpDMin->setValue(other->ui->spinBoxSpDMin->value());
    ui->spinBoxSpeMin->setValue(other->ui->spinBoxSpeMin->value());

    ui->spinBoxHPMax->setValue(other->ui->spinBoxHPMax->value());
    ui->spinBoxAtkMax->setValue(other->ui->spinBoxAtkMax->value());
    ui->spinBoxDefMax->setValue(other->ui->spinBoxDefMax->value());
    ui->spinBoxSpAMax->setValue(other->ui->spinBoxSpAMax->value());
    ui->spinBoxSpDMax->setValue(other->ui->spinBoxSpDMax->value());
    ui->spinBoxSpeMax->setValue(other->ui->spinBoxSpeMax->value());

    ui->checkBoxShowStats->setChecked(other->ui->checkBoxShowStats->isChecked());
}

void IVFilter::disableControls()
{
    ui->labelHP->hide();
    ui->spinBoxHPMin->hide();
    ui->spinBoxHPMax->hide();

    ui->labelAtk->hide();
    ui->spinBoxAtkMin->hide();
    ui->spinBoxAtkMax->hide();

    ui->labelDef->hide();
    ui->spinBoxDefMin->hide();
    ui->spinBoxDefMax->hide();

    ui->labelSpA->hide();
    ui->spinBoxSpAMin->hide();
    ui->spinBoxSpAMax->hide();

    ui->labelSpD->hide();
    ui->spinBoxSpDMin->hide();
    ui->spinBoxSpDMax->hide();

    ui->labelSpe->hide();
    ui->spinBoxSpeMin->hide();
    ui->spinBoxSpeMax->hide();

    ui->checkBoxShowStats->hide();
    ui->pushButtonIVCalculator->hide();
}

std::array<u8, 6> IVFilter::getMaxIVs() const
{
    std::array<u8, 6> high = { static_cast<u8>(ui->spinBoxHPMax->value()),  static_cast<u8>(ui->spinBoxAtkMax->value()),
                               static_cast<u8>(ui->spinBoxDefMax->value()), static_cast<u8>(ui->spinBoxSpAMax->value()),
                               static_cast<u8>(ui->spinBoxSpDMax->value()), static_cast<u8>(ui->spinBoxSpeMax->value()) };
    return high;
}

std::array<u8, 6> IVFilter::getMinIVs() const
{
    std::array<u8, 6> low = { static_cast<u8>(ui->spinBoxHPMin->value()),  static_cast<u8>(ui->spinBoxAtkMin->value()),
                              static_cast<u8>(ui->spinBoxDefMin->value()), static_cast<u8>(ui->spinBoxSpAMin->value()),
                              static_cast<u8>(ui->spinBoxSpDMin->value()), static_cast<u8>(ui->spinBoxSpeMin->value()) };
    return low;
}

bool IVFilter::isValid() const
{
    if (ui->spinBoxHPMin->value() > ui->spinBoxHPMax->value())
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid filter settings"), tr("HP minimum is greater than maximum"));
        msg.exec();
        return false;
    }

    if (ui->spinBoxAtkMin->value() > ui->spinBoxAtkMax->value())
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid filter settings"), tr("Atk minimum is greater than maximum"));
        msg.exec();
        return false;
    }

    if (ui->spinBoxDefMin->value() > ui->spinBoxDefMax->value())
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid filter settings"), tr("Def minimum is greater than maximum"));
        msg.exec();
        return false;
    }

    if (ui->spinBoxSpAMin->value() > ui->spinBoxSpAMax->value())
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid filter settings"), tr("SpA minimum is greater than maximum"));
        msg.exec();
        return false;
    }

    if (ui->spinBoxSpDMin->value() > ui->spinBoxSpDMax->value())
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid filter settings"), tr("SpD minimum is greater than maximum"));
        msg.exec();
        return false;
    }

    if (ui->spinBoxSpeMin->value() > ui->spinBoxSpeMax->value())
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid filter settings"), tr("Spe minimum is greater than maximum"));
        msg.exec();
        return false;
    }

    return true;
}

void IVFilter::setOrientation(Qt::Orientation orientation)
{
    if (this->orientation == orientation)
    {
        return;
    }
    this->orientation = orientation;

    ui->gridLayout->removeWidget(ui->labelHP);
    ui->gridLayout->removeWidget(ui->spinBoxHPMin);
    ui->gridLayout->removeWidget(ui->spinBoxHPMax);

    ui->gridLayout->removeWidget(ui->labelAtk);
    ui->gridLayout->removeWidget(ui->spinBoxAtkMin);
    ui->gridLayout->removeWidget(ui->spinBoxAtkMax);

    ui->gridLayout->removeWidget(ui->labelDef);
    ui->gridLayout->removeWidget(ui->spinBoxDefMin);
    ui->gridLayout->removeWidget(ui->spinBoxDefMax);

    ui->gridLayout->removeWidget(ui->labelSpA);
    ui->gridLayout->removeWidget(ui->spinBoxSpAMin);
    ui->gridLayout->removeWidget(ui->spinBoxSpAMax);

    ui->gridLayout->removeWidget(ui->labelSpD);
    ui->gridLayout->removeWidget(ui->spinBoxSpDMin);
    ui->gridLayout->removeWidget(ui->spinBoxSpDMax);

    ui->gridLayout->removeWidget(ui->labelSpe);
    ui->gridLayout->removeWidget(ui->spinBoxSpeMin);
    ui->gridLayout->removeWidget(ui->spinBoxSpeMax);

    ui->gridLayout->removeWidget(ui->checkBoxShowStats);
    ui->gridLayout->removeWidget(ui->pushButtonIVCalculator);

    if (orientation == Qt::Vertical)
    {
        ui->checkBoxShowStats->show();

        ui->gridLayout->addWidget(ui->labelHP, 0, 0);
        ui->gridLayout->addWidget(ui->spinBoxHPMin, 0, 1);
        ui->gridLayout->addWidget(ui->spinBoxHPMax, 0, 2);

        ui->gridLayout->addWidget(ui->labelAtk, 1, 0);
        ui->gridLayout->addWidget(ui->spinBoxAtkMin, 1, 1);
        ui->gridLayout->addWidget(ui->spinBoxAtkMax, 1, 2);

        ui->gridLayout->addWidget(ui->labelDef, 2, 0);
        ui->gridLayout->addWidget(ui->spinBoxDefMin, 2, 1);
        ui->gridLayout->addWidget(ui->spinBoxDefMax, 2, 2);

        ui->gridLayout->addWidget(ui->labelSpA, 3, 0);
        ui->gridLayout->addWidget(ui->spinBoxSpAMin, 3, 1);
        ui->gridLayout->addWidget(ui->spinBoxSpAMax, 3, 2);

        ui->gridLayout->addWidget(ui->labelSpD, 4, 0);
        ui->gridLayout->addWidget(ui->spinBoxSpDMin, 4, 1);
        ui->gridLayout->addWidget(ui->spinBoxSpDMax, 4, 2);

        ui->gridLayout->addWidget(ui->labelSpe, 5, 0);
        ui->gridLayout->addWidget(ui->spinBoxSpeMin, 5, 1);
        ui->gridLayout->addWidget(ui->spinBoxSpeMax, 5, 2);

        ui->gridLayout->addWidget(ui->checkBoxShowStats, 6, 0, 1, 3);
        ui->gridLayout->addWidget(ui->pushButtonIVCalculator, 7, 0, 1, 3);
    }
    else
    {
        ui->checkBoxShowStats->hide();

        ui->gridLayout->addWidget(ui->labelHP, 0, 0);
        ui->gridLayout->addWidget(ui->spinBoxHPMin, 1, 0);
        ui->gridLayout->addWidget(ui->spinBoxHPMax, 2, 0);

        ui->gridLayout->addWidget(ui->labelAtk, 0, 1);
        ui->gridLayout->addWidget(ui->spinBoxAtkMin, 1, 1);
        ui->gridLayout->addWidget(ui->spinBoxAtkMax, 2, 1);

        ui->gridLayout->addWidget(ui->labelDef, 0, 2);
        ui->gridLayout->addWidget(ui->spinBoxDefMin, 1, 2);
        ui->gridLayout->addWidget(ui->spinBoxDefMax, 2, 2);

        ui->gridLayout->addWidget(ui->labelSpA, 0, 3);
        ui->gridLayout->addWidget(ui->spinBoxSpAMin, 1, 3);
        ui->gridLayout->addWidget(ui->spinBoxSpAMax, 2, 3);

        ui->gridLayout->addWidget(ui->labelSpD, 0, 4);
        ui->gridLayout->addWidget(ui->spinBoxSpDMin, 1, 4);
        ui->gridLayout->addWidget(ui->spinBoxSpDMax, 2, 4);

        ui->gridLayout->addWidget(ui->labelSpe, 0, 5);
        ui->gridLayout->addWidget(ui->spinBoxSpeMin, 1, 5);
        ui->gridLayout->addWidget(ui->spinBoxSpeMax, 2, 5);

        ui->gridLayout->addWidget(ui->pushButtonIVCalculator, 1, 6, 2, 1);
    }

    ui->gridLayout->invalidate();
}

bool IVFilter::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        auto *mouse = reinterpret_cast<QMouseEvent *>(event);
        if (mouse->button() == Qt::LeftButton)
        {
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
    }
    return false;
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

void IVFilter::setIVsFromClipBoard()
{
    QRegularExpression re("(\\d{1,2})/(\\d{1,2})/(\\d{1,2})/(\\d{1,2})/(\\d{1,2})/(\\d{1,2})-(\\d{1,2})/(\\d{1,2})/(\\d{1,2})/(\\d{1,2})/"
                          "(\\d{1,2})/(\\d{1,2})");

    QString text = QApplication::clipboard()->text();
    QRegularExpressionMatch match = re.match(text);
    if (!match.hasMatch())
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid Format"), tr("The clipboard text did not match the expected format."));
        msg.exec();
        return;
    }

    ui->spinBoxHPMin->setValue(match.captured(1).toInt());
    ui->spinBoxAtkMin->setValue(match.captured(2).toInt());
    ui->spinBoxDefMin->setValue(match.captured(3).toInt());
    ui->spinBoxSpAMin->setValue(match.captured(4).toInt());
    ui->spinBoxSpDMin->setValue(match.captured(5).toInt());
    ui->spinBoxSpeMin->setValue(match.captured(6).toInt());

    ui->spinBoxHPMax->setValue(match.captured(7).toInt());
    ui->spinBoxAtkMax->setValue(match.captured(8).toInt());
    ui->spinBoxDefMax->setValue(match.captured(9).toInt());
    ui->spinBoxSpAMax->setValue(match.captured(10).toInt());
    ui->spinBoxSpDMax->setValue(match.captured(11).toInt());
    ui->spinBoxSpeMax->setValue(match.captured(12).toInt());
}

void IVFilter::setIVsToClipBoard()
{
    QString ivs = QString("%1/%2/%3/%4/%5/%6-%7/%8/%9/%10/%11/%12")
                      .arg(ui->spinBoxHPMin->value())
                      .arg(ui->spinBoxAtkMin->value())
                      .arg(ui->spinBoxDefMin->value())
                      .arg(ui->spinBoxSpAMin->value())
                      .arg(ui->spinBoxSpDMin->value())
                      .arg(ui->spinBoxSpeMin->value())
                      .arg(ui->spinBoxHPMax->value())
                      .arg(ui->spinBoxAtkMax->value())
                      .arg(ui->spinBoxDefMax->value())
                      .arg(ui->spinBoxSpAMax->value())
                      .arg(ui->spinBoxSpDMax->value())
                      .arg(ui->spinBoxSpeMax->value());

    QApplication::clipboard()->setText(ivs);
}
