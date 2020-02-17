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

#include "SearchCoinFlips.hpp"
#include "ui_SearchCoinFlips.h"
#include <Core/Util/Utilities.hpp>
#include <QSettings>

SearchCoinFlips::SearchCoinFlips(const QVector<DateTime> &model, QWidget *parent) : QDialog(parent), ui(new Ui::SearchCoinFlips)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    data = model;
    ui->labelPossibleResults->setText(tr("Possible Results: ") + QString::number(model.size()));

    connect(ui->pushButtonHeads, &QPushButton::clicked, this, &SearchCoinFlips::heads);
    connect(ui->pushButtonTails, &QPushButton::clicked, this, &SearchCoinFlips::tails);
    connect(ui->lineEditFlips, &QLineEdit::textChanged, this, &SearchCoinFlips::flipsTextChanged);

    QSettings setting;
    if (setting.contains("searchCoinFlips/geometry"))
    {
        this->restoreGeometry(setting.value("searchCoinFlips/geometry").toByteArray());
    }
}

SearchCoinFlips::~SearchCoinFlips()
{
    QSettings setting;
    setting.setValue("searchCoinFlips/geometry", this->saveGeometry());

    delete ui;
}

QVector<bool> SearchCoinFlips::possibleResults() const
{
    return possible;
}

void SearchCoinFlips::heads()
{
    QString string = ui->lineEditFlips->text();
    string += string.isEmpty() ? "H" : ", H";
    ui->lineEditFlips->setText(string);
}

void SearchCoinFlips::tails()
{
    QString string = ui->lineEditFlips->text();
    string += string.isEmpty() ? "T" : ", T";
    ui->lineEditFlips->setText(string);
}

void SearchCoinFlips::flipsTextChanged(const QString &val)
{
    QStringList results = val.split(",", QString::SkipEmptyParts);
    int num = 0;

    possible.clear();
    for (const auto &dt : data)
    {
        QStringList compare = Utilities::coinFlips(dt.getSeed(), 15).split(",", QString::SkipEmptyParts);

        bool pass = true;
        for (int j = 0; j < results.size(); j++)
        {
            if (results.at(j) != compare.at(j))
            {
                pass = false;
                break;
            }
        }
        possible.append(pass);
        if (pass)
        {
            num++;
        }
    }

    ui->labelPossibleResults->setText(tr("Possible Results: ") + QString::number(num));
}
