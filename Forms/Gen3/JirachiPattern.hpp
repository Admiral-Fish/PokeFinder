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

#ifndef JIRACHIPATTERN_HPP
#define JIRACHIPATTERN_HPP

#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QStringList>
#include <QString>
#include <QVector>
#include <QChar>
#include <PokeFinderCore/RNG/LCRNG.hpp>

typedef uint32_t u32;

namespace Ui
{
    class JirachiPattern;
}

class JirachiPattern : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent *);

private:
    Ui::JirachiPattern *ui;
    QStandardItemModel *model = new QStandardItemModel(this);
    QVector<u32> data;

    void setupModels();
    void generate(u32 seed);
    QStringList getPatterns(u32 seed);
    QString getTarget(QString in, int index);

private slots:
    void on_pushButtonGenerate_clicked();

public:
    explicit JirachiPattern(QWidget *parent = nullptr);
    ~JirachiPattern();

};

#endif // JIRACHIPATTERN_HPP
