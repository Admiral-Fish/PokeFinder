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

#include "SearchCoinFlips.hpp"
#include "ui_SearchCoinFlips.h"
#include <Core/Gen4/SeedTime4.hpp>
#include <Core/Util/Utilities.hpp>
#include <QSettings>

SearchCoinFlips::SearchCoinFlips(const std::vector<SeedTimeCalibrate4> &data, QWidget *parent) :
    QDialog(parent), ui(new Ui::SearchCoinFlips), data(data)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    ui->labelPossibleResults->setText(tr("Possible Results: %1").arg(data.size()));

    connect(ui->pushButtonHeads, &QPushButton::clicked, this, &SearchCoinFlips::heads);
    connect(ui->pushButtonTails, &QPushButton::clicked, this, &SearchCoinFlips::tails);
    connect(ui->lineEditFlips, &QLineEdit::textChanged, this, &SearchCoinFlips::flipsTextChanged);
    connect(ui->pushButtonOkay, &QPushButton::clicked, this, &SearchCoinFlips::accept);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &SearchCoinFlips::reject);

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

std::vector<bool> SearchCoinFlips::getResults() const
{
    return possible;
}

void SearchCoinFlips::flipsTextChanged(const QString &text)
{
    if (!text.isEmpty())
    {
        std::string result = text.toStdString();
        std::erase_if(result, [](char c) { return c == ' ' || c == ','; });
        int num = 0;

        possible.clear();
        for (const auto &dt : data)
        {
            std::string compare = dt.getSequence();
            std::erase_if(compare, [](char c) { return c == ' ' || c == ','; });

            bool pass = compare.find(result) != std::string::npos;
            possible.emplace_back(pass);
            if (pass)
            {
                num++;
            }
        }

        ui->labelPossibleResults->setText(tr("Possible Results: %1").arg(num));
    }
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
