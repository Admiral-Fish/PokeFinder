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

#include "SearchCalls.hpp"
#include "ui_SearchCalls.h"

SearchCalls::SearchCalls(const QVector<DateTime> &model, const QVector<bool> &roamers, const QVector<u32> &routes, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchCalls)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    this->roamers = roamers;
    this->routes = routes;

    data = model;
    ui->labelPossibleResults->setText(tr("Possible Results: ") + QString::number(model.size()));
}

SearchCalls::~SearchCalls()
{
    delete ui;
}

QVector<bool> SearchCalls::possibleResults()
{
    return possible;
}

void SearchCalls::on_pushButtonK_clicked()
{
    QString string = ui->lineEditCalls->text();
    string += string.isEmpty() ? "K" : ", K";
    ui->lineEditCalls->setText(string);
}

void SearchCalls::on_pushButtonE_clicked()
{
    QString string = ui->lineEditCalls->text();
    string += string.isEmpty() ? "E" : "< E";
    ui->lineEditCalls->setText(string);
}

void SearchCalls::on_pushButtonP_clicked()
{
    QString string = ui->lineEditCalls->text();
    string += string.isEmpty() ? "P" : ", P";
    ui->lineEditCalls->setText(string);
}

void SearchCalls::on_lineEditCalls_textChanged(const QString &val)
{
    QStringList results = val.split(",", QString::SkipEmptyParts);
    int num = 0;

    possible.clear();
    for (auto &i : data)
    {
        QString str = Utilities::getCalls(i.getSeed(), 15, i.getInfo());

        if (str.contains("skipped"))
        {
            int index = str.indexOf(")", 1);
            str = str.mid(index + 3);
        }

        QStringList compare = str.split(",", QString::SkipEmptyParts);

        bool pass = true;
        for (int j = 0; j < results.size(); j++)
        {
            if (results[j] != compare[j])
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

void SearchCalls::on_pushButtonOkay_clicked()
{
    done(QDialog::Accepted);
}

void SearchCalls::on_pushButtonCancel_clicked()
{
    done(QDialog::Rejected);
}

void SearchCalls::on_radioButtonElm_clicked()
{
    ui->labelKResponse->setText(tr("K - I expect there are some Pokémon in the Kanto region that I don't know. There are probably methods of evolution that I'm not familiar with yet. I should use that perspective and discover what I can!"));
    ui->labelEResponse->setText(tr("E - There are so many different ways that Pokémon evolve, aren't there?! Some Pokémon don't even evolve until they meet certain conditions first!"));
    ui->labelPResponse->setText(tr("P - It seems that Pokémon that have been infected with Pokérus level up better. We're not quite sure why..."));
}

void SearchCalls::on_radioButtonIrwin_clicked()
{
    ui->labelKResponse->setText(tr("K - I'm so glad you called! I was just about to call you, too! I guess we must be a good match!"));
    ui->labelEResponse->setText(tr("E - Hearing about your escapades rocks my soul! It sure does!"));
    ui->labelPResponse->setText(tr("P - How are you? What are you doing? Where are you? How many Badges do you have now? How much money have you saved? How's your mom? Have you got lots of Pokémon? Is it going to be sunny tomorrow? Arrgh, there's so much I want to chat about! This is going nowhere!"));
}
