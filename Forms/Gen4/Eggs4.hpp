/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef EGGS4_HPP
#define EGGS4_HPP

#include <QMenu>
#include <Core/Gen4/Profile4.hpp>
#include <Models/Gen4/Egg4Model.hpp>

namespace Ui
{
    class Eggs4;
}

class Eggs4 : public QWidget
{
    Q_OBJECT

signals:
    void alertProfiles(int);

public:
    explicit Eggs4(QWidget *parent = nullptr);
    ~Eggs4() override;
    void updateProfiles();

private:
    Ui::Eggs4 *ui;
    QVector<Profile4> profiles;
    Egg4GeneratorModel *generatorModel{};
    Egg4SearcherModel *searcherModel{};
    QMenu *searcherMenu{};

    void setupModels();

private slots:
    void updateProgress(const QVector<Frame4> &frames, int progress);
    void refreshProfiles();
    void on_pushButtonGenerate_clicked();
    void on_pushButtonSearch_clicked();
    void on_comboBoxProfiles_currentIndexChanged(int index);
    void on_tableViewSearcher_customContextMenuRequested(const QPoint &pos);
    void seedToTime();
    void on_pushButtonProfileManager_clicked();

};

#endif // EGGS4_HPP
