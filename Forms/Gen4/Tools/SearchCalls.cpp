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

#include "SearchCalls.hpp"
#include "ui_SearchCalls.h"
#include <Core/Util/Utilities.hpp>
#include <QSettings>

SearchCalls::SearchCalls(const std::vector<DateTime> &model, const std::vector<bool> &roamers, const std::vector<u8> &routes,
                         QWidget *parent) :
    QDialog(parent), ui(new Ui::SearchCalls)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    this->roamers = roamers;
    this->routes = routes;

    data = model;
    ui->labelPossibleResults->setText(tr("Possible Results: ") + QString::number(model.size()));

    connect(ui->pushButtonE, &QPushButton::clicked, this, &SearchCalls::e);
    connect(ui->pushButtonK, &QPushButton::clicked, this, &SearchCalls::k);
    connect(ui->pushButtonP, &QPushButton::clicked, this, &SearchCalls::p);
    connect(ui->lineEditCalls, &QLineEdit::textChanged, this, &SearchCalls::callsTextChanged);
    connect(ui->radioButtonElm, &QRadioButton::clicked, this, &SearchCalls::elm);
    connect(ui->radioButtonIrwin, &QRadioButton::clicked, this, &SearchCalls::irwin);

    QSettings setting;
    if (setting.contains("searchCalls/geometry"))
    {
        this->restoreGeometry(setting.value("searchCalls/geometry").toByteArray());
    }
}

SearchCalls::~SearchCalls()
{
    QSettings setting;
    setting.setValue("searchCalls/geometry", this->saveGeometry());

    delete ui;
}

std::vector<bool> SearchCalls::possibleResults() const
{
    return possible;
}

void SearchCalls::k()
{
    QString string = ui->lineEditCalls->text();
    string += string.isEmpty() ? "K" : ", K";
    ui->lineEditCalls->setText(string);
}

void SearchCalls::e()
{
    QString string = ui->lineEditCalls->text();
    string += string.isEmpty() ? "E" : ", E";
    ui->lineEditCalls->setText(string);
}

void SearchCalls::p()
{
    QString string = ui->lineEditCalls->text();
    string += string.isEmpty() ? "P" : ", P";
    ui->lineEditCalls->setText(string);
}

void SearchCalls::callsTextChanged(const QString &val)
{
    if (!val.isEmpty())
    {
        QString result = val;
        result.replace(" ", "").replace(",", "");
        int num = 0;

        possible.clear();
        for (const auto &dt : data)
        {
            QString compare = Utilities::getCalls(dt.getSeed(), dt.getInfo());
            if (compare.contains("skipped"))
            {
                int index = compare.indexOf(")", 1);
                compare = compare.mid(index + 3);
            }
            compare.replace(" ", "").replace(",", "");

            bool pass = compare.contains(result);
            possible.push_back(pass);
            if (pass)
            {
                num++;
            }
        }

        ui->labelPossibleResults->setText(tr("Possible Results: ") + QString::number(num));
    }
}

void SearchCalls::elm()
{
    ui->labelKResponse->setText(tr("K - I expect there are some Pokémon in the Kanto region that I don't know. There are probably methods "
                                   "of evolution that I'm not familiar with yet. I should use that perspective and discover what I can!"));
    ui->labelEResponse->setText(tr("E - There are so many different ways that Pokémon evolve, aren't there?! Some "
                                   "Pokémon don't even evolve until they meet certain conditions first!"));
    ui->labelPResponse->setText(tr("P - It seems that Pokémon that have been infected with Pokérus level up "
                                   "better. We're not quite sure why..."));
}

void SearchCalls::irwin()
{
    ui->labelKResponse->setText(tr("K - I'm so glad you called! I was just about to call you, too! I guess we must be a good match!"));
    ui->labelEResponse->setText(tr("E - Hearing about your escapades rocks my soul! It sure does!"));
    ui->labelPResponse->setText(tr("P - How are you? What are you doing? Where are you? How many Badges do you have now? How much money "
                                   "have you saved? How's your mom? Have you got lots of Pokémon? Is it going to be sunny tomorrow? Arrgh, "
                                   "there's so much I want to chat about! This is going nowhere!"));
}
