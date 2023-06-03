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

#include "IDsFilter.hpp"
#include "ui_IDsFilter.h"
#include <Core/Parents/Filters/IDFilter.hpp>
#include <QRegularExpression>

IDsFilter::IDsFilter(QWidget *parent) : QWidget(parent), ui(new Ui::IDsFilter)
{
    ui->setupUi(this);

    ui->radioButtonDisplayTID->setVisible(false);

    connect(ui->buttonGroup, &QButtonGroup::buttonClicked, this, &IDsFilter::buttonGroupButtonClicked);
    connect(ui->plainTextEdit, &QPlainTextEdit::textChanged, this, &IDsFilter::textEditIDsTextChanged);
}

IDsFilter::~IDsFilter()
{
    delete ui;
}

IDFilter IDsFilter::getFilter() const
{
    std::vector<u16> tidFilter;
    std::vector<u16> sidFilter;
    std::vector<u16> tsvFilter;
    std::vector<u32> displayFilter;

    QStringList inputs = ui->plainTextEdit->toPlainText().split('\n');
    if (ui->radioButtonTID->isChecked())
    {
        for (const QString &input : inputs)
        {
            if (!input.isEmpty())
            {
                tidFilter.emplace_back(input.toUShort());
            }
        }
    }
    else if (ui->radioButtonSID->isChecked())
    {
        for (const QString &input : inputs)
        {
            if (!input.isEmpty())
            {
                sidFilter.emplace_back(input.toUShort());
            }
        }
    }
    else if (ui->radioButtonTIDSID->isChecked())
    {
        for (const QString &input : inputs)
        {
            if (!input.isEmpty())
            {
                QStringList ids = input.split('/');
                tidFilter.emplace_back(ids[0].toUShort());
                sidFilter.emplace_back(ids[1].toUShort());
            }
        }
    }
    else if (ui->radioButtonPID->isChecked())
    {
        for (const QString &input : inputs)
        {
            if (!input.isEmpty())
            {
                u32 pid = input.toUInt(nullptr, 16);
                tsvFilter.emplace_back((pid >> 16) ^ (pid & 0xffff));
            }
        }
    }
    else if (ui->radioButtonTSV->isChecked())
    {
        for (const QString &input : inputs)
        {
            if (!input.isEmpty())
            {
                tsvFilter.emplace_back(input.toUShort());
            }
        }
    }
    else
    {
        for (const QString &input : inputs)
        {
            if (!input.isEmpty())
            {
                displayFilter.emplace_back(input.toUInt());
            }
        }
    }

    return IDFilter(tidFilter, sidFilter, tsvFilter, displayFilter);
}

void IDsFilter::enableDisplayTID()
{
    ui->radioButtonDisplayTID->setVisible(true);
}

void IDsFilter::buttonGroupButtonClicked(QAbstractButton *button)
{
    ui->plainTextEdit->clear();
}

void IDsFilter::textEditIDsTextChanged()
{
    QStringList inputs = ui->plainTextEdit->toPlainText().split('\n');
    if (ui->radioButtonTID->isChecked() || ui->radioButtonSID->isChecked())
    {
        QRegularExpression filter("[^0-9]");
        for (QString &input : inputs)
        {
            input.remove(filter);
            if (!input.isEmpty())
            {
                bool flag;
                u16 val = input.toUShort(&flag);
                val = flag ? val : 65535;
                input = QString::number(val);
            }
        }
    }
    else if (ui->radioButtonPID->isChecked())
    {
        QRegularExpression filter("[^0-9a-f]");
        for (QString &input : inputs)
        {
            input.remove(filter);
            if (!input.isEmpty())
            {
                bool flag;
                u32 val = input.toUInt(&flag, 16);
                val = flag ? val : 0xffffffff;
                input = QString::number(val, 16);
            }
        }
    }
    else if (ui->radioButtonTSV->isChecked())
    {
        QRegularExpression filter("[^0-9]");
        for (QString &input : inputs)
        {
            input.remove(filter);
            if (!input.isEmpty())
            {
                bool flag;
                u16 val = input.toUShort(&flag);
                val = (flag && val <= 8191) ? val : 8191;
                input = QString::number(val);
            }
        }
    }
    else if (ui->radioButtonDisplayTID->isChecked())
    {
        QRegularExpression filter("[^0-9]");
        for (QString &input : inputs)
        {
            input.remove(filter);
            if (!input.isEmpty())
            {
                bool flag;
                u32 val = input.toUShort(&flag);
                val = (flag && val <= 999999) ? val : 999999;
                input = QString::number(val);
            }
        }
    }
    else
    {
        QRegularExpression filter = QRegularExpression("[^0-9/]");
        for (QString &input : inputs)
        {
            input.remove(filter);
            // Only allow a single '/' character
            while (input.count('/') > 1)
            {
                input.remove(input.lastIndexOf('/'), 1);
            }

            if (!input.isEmpty())
            {
                QStringList ids = input.split('/');

                bool flag;
                u16 tid = ids[0].toUShort(&flag);
                tid = flag ? tid : 65535;

                if (ids.size() == 1)
                {
                    if (input.contains('/'))
                    {
                        input = QString("%1/").arg(tid);
                    }
                    else
                    {
                        input = QString::number(tid);
                    }
                }
                else if (!ids[1].isEmpty())
                {
                    u16 sid = ids[1].toUShort(&flag);
                    sid = flag ? sid : 65535;
                    input = QString("%1/%2").arg(tid).arg(sid);
                }
            }
        }
    }

    // Block signals so this function doesn't get called recursively
    // We also grab the cursor position and restore as setting the text resets it
    ui->plainTextEdit->blockSignals(true);
    QTextCursor cursor = ui->plainTextEdit->textCursor();
    ui->plainTextEdit->setPlainText(inputs.join('\n'));
    ui->plainTextEdit->setTextCursor(cursor);
    ui->plainTextEdit->blockSignals(false);
}
