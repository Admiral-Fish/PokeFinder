/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

#include "JirachiGeneration.hpp"
#include "ui_JirachiGeneration.h"

JirachiGeneration::JirachiGeneration(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::JirachiGeneration)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    setupModels();

    ui->tableViewGenerator->setModel(s);
    ui->tableViewGenerator->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

JirachiGeneration::~JirachiGeneration()
{
    delete ui;
    delete s;
}

void JirachiGeneration::setupModels()
{
    ui->jirachiGenerationSeed->setValues(0, 32, false);

    s->setHorizontalHeaderLabels(QStringList() << tr("Probable"));
}

void JirachiGeneration::on_pushButtonGenerate_clicked()
{
    s->removeRows(0, s->rowCount());
    u32 seed = ui->jirachiGenerationSeed->text().toUInt(NULL, 16);
    genListOut(seed);
}

void JirachiGeneration::genListOut(u32 seed)
{
    QString genlistout = calcProbable(seed);

    QString result = genlistout.replace("|", " | ");
    s->appendRow(new QStandardItem(result));
}

QString JirachiGeneration::calcProbable(u32 seed)
{
    QString genlistout = QString::number(seed >> 30, 16);
    u32 f = seed >> 30;
    u32 checker[] = { 0, 0, 0, 0};
    u32 compare[] = { 0, 1, 1, 1};
    u32 backseed = seed;
    LCRNG rng = XDRNGR(backseed);
    u32 advance = 8;
    bool xCheck = false;

    for (u32 m = 0; m < 35; m++)
    {
        backseed = rng.nextUInt();
        genlistout = QString::number(backseed >> 30, 16) + "|" + genlistout;

        if (m == (advance - 1))
        {
            f = backseed >> 30;
            genlistout = " M: " + genlistout;
        }
    }

    QString genlistend = genlistout.mid(genlistout.length() - 7);
    int intAdvance = 3;

    if (genlistend.at(0) == QChar('0'))
        intAdvance = 7;
    if (genlistend.at(2) == QChar('0') && intAdvance == 3)
        intAdvance = 5;
    if (genlistend.at(4) == QChar('0') && intAdvance == 3)
        intAdvance = 3;
    if (genlistend.at(6) == QChar('0') && intAdvance == 3)
        intAdvance = 3;
    int mm = genlistout.indexOf(QChar('M'));
    genlistout = genlistout.mid(0, mm + intAdvance) + "T:" + genlistout.mid(mm + intAdvance);
    genlistout = genlistout.mid(2);
    genlistout = genlistout.replace("M:", "");
    genlistout = genlistout.replace(" ", "");
    int index = genlistout.indexOf(QChar(':')) + 1;
    QChar targetNum = genlistout.at(index);
    QString string2Search = genlistout.mid(0, index);
    genlistend = genlistout.mid(index);
    string2Search = flip(string2Search);
    checker[QString(targetNum).toInt(NULL, 10)] = 1;
    for(int x = 0; x < string2Search.length(); x++)
    {
        if (!xCheck)
        {
            if (string2Search.at(x) == QChar('1') || string2Search.at(x) == QChar('2') || string2Search.at(x) == QChar('3'))
            {
                if (string2Search.at(x) == targetNum)
                {
                    if (checker[0] == 1 || checker[1] == 0 || checker[2] == 0 || checker[3] == 0)
                    {
                        string2Search = string2Search.mid(0, x) + "XXX" + string2Search.mid(x);
                        xCheck = true;
                    }
                }
                else
                {
                    checker[QString(targetNum).toInt(NULL, 10)] = 1;
                }
            }
        }
    }

    string2Search = flip(string2Search);
    genlistout = string2Search + genlistend;

    return genlistout;
}

QString JirachiGeneration::flip(QString text)
{
    QString reverse = "";
    for (int i = text.length() - 1; i >= 0; i--)
        reverse += text.mid(i, 1);
    return reverse;
}
