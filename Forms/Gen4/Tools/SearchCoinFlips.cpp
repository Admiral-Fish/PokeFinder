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

SearchCoinFlips::SearchCoinFlips(const std::vector<SeedTime> &model, QWidget *parent) : QDialog(parent), ui(new Ui::SearchCoinFlips)
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

std::vector<bool> SearchCoinFlips::possibleResults() const
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
    if (!val.isEmpty())
    {
        QString result = val;
        result.replace(" ", "").replace(",", "");
        int num = 0;

        possible.clear();
        for (const auto &dt : data)
        {
            std::string compare = Utilities::coinFlips(dt.getSeed());
            for (size_t i = 0; i < compare.size();)
            {
                auto c = compare[i];
                if (c == ' ' || c == ',')
                {
                    compare.erase(compare.begin() + i);
                }
                else
                {
                    i++;
                }
            }

            bool pass = compare.find(result.toStdString()) != std::string::npos;
            possible.emplace_back(pass);
            if (pass)
            {
                num++;
            }
        }

        ui->labelPossibleResults->setText(tr("Possible Results: ") + QString::number(num));
    }
}
