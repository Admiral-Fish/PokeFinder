/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "SearchElmCalls.hpp"
#include "ui_SearchElmCalls.h"

SearchElmCalls::SearchElmCalls(vector<DateTime> model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchElmCalls)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    data = model;
    ui->labelPossibleResults->setText(tr("Possible Results: ") + QString::number(model.size()));
}

SearchElmCalls::~SearchElmCalls()
{
    delete ui;
}

vector<bool> SearchElmCalls::possibleResults()
{
    return possible;
}

void SearchElmCalls::on_pushButtonK_clicked()
{
    QString string = ui->lineEditCalls->text();

    if (string.isEmpty())
        string = "K";
    else
        string += ", K";

    ui->lineEditCalls->setText(string);
}

void SearchElmCalls::on_pushButtonE_clicked()
{
    QString string = ui->lineEditCalls->text();

    if (string.isEmpty())
        string = "E";
    else
        string += ", E";

    ui->lineEditCalls->setText(string);
}

void SearchElmCalls::on_pushButtonP_clicked()
{
    QString string = ui->lineEditCalls->text();

    if (string.isEmpty())
        string = "P";
    else
        string += ", P";

    ui->lineEditCalls->setText(string);
}

void SearchElmCalls::on_lineEditCalls_textChanged(const QString &arg1)
{
    QStringList results = arg1.split(",", QString::SkipEmptyParts);

    int num = 0;

    possible.clear();
    for (int i = 0; i < data.size(); i++)
    {
        QStringList compare = Utilities::elmCalls(data[i].getSeed(), 15).split(",", QString::SkipEmptyParts);

        bool pass = true;
        for (int j = 0; j < results.size(); j++)
        {
            if (results[j] != compare[j])
            {
                pass = false;
                break;
            }
        }
        possible.push_back(pass);
        if (pass)
            num++;
    }

    ui->labelPossibleResults->setText(tr("Possible Results: ") + QString::number(num));
}

void SearchElmCalls::on_buttonBox_accepted()
{
    done(QDialog::Accepted);
}

void SearchElmCalls::on_buttonBox_rejected()
{
    done(QDialog::Rejected);
}

void SearchElmCalls::on_radioButtonElm_clicked()
{
    ui->labelKResponse->setText(tr("K - I expect there are some Pokémon in the Kanto region that I don't know. There are probably methods of evolution that I'm not familiar with yet. I should use that perspective and discover what I can!"));
    ui->labelEResponse->setText(tr("E - There are so many different ways that Pokémon evolve, aren't there?! Some Pokémon don't even evolve until they meet certain conditions first!"));
    ui->labelPResponse->setText(tr("P - It seems that Pokémon that have been infected with Pokérus level up better. We're not quite sure why..."));
}

void SearchElmCalls::on_radioButtonIrwin_clicked()
{
    ui->labelKResponse->setText(tr("K - I'm so glad you called! I was just about to call you, too! I guess we must be a good match!"));
    ui->labelEResponse->setText(tr("E - Hearing about your escapades rocks my soul! It sure does!"));
    ui->labelPResponse->setText(tr("P - How are you? What are you doing? Where are you? How many Badges do you have now? How much money have you saved? How's your mom? Have you got lots of Pokémon? Is it going to be sunny tomorrow? Arrgh, there's so much I want to chat about! This is going nowhere!"));
}
