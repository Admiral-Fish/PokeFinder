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

#ifndef STATIONARY3_H
#define STATIONARY3_H

#include <QMenu>
#include <Core/Gen3/Profile3.hpp>
#include <Models/Gen3/Searcher3Model.hpp>
#include <Models/Gen3/Stationary3Model.hpp>

namespace Ui
{
    class Stationary3;
}

class Stationary3 : public QWidget
{
    Q_OBJECT

signals:
    void alertProfiles(int);

public:
    explicit Stationary3(QWidget *parent = nullptr);
    ~Stationary3() override;
    void updateProfiles();

private:
    Ui::Stationary3 *ui;
    Searcher3Model *searcherModel{};
    Stationary3Model *generatorModel{};
    QVector<Profile3> profiles;
    QMenu *generatorMenu{};
    QMenu *searcherMenu{};
    QModelIndex lastIndex;
    QModelIndex targetFrame;

    void setupModels();
    void updateView(const QVector<Frame3> &frames, int progress);

public slots:
    void moveResults(const QString &seed, const QString &method, u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe);

private slots:
    void refreshProfiles();
    void on_comboBoxProfiles_currentIndexChanged(int index);
    void on_pushButtonGenerate_clicked();
    void on_pushButtonSearch_clicked();
    void on_tableViewGenerator_customContextMenuRequested(const QPoint &pos);
    void on_tableViewSearcher_customContextMenuRequested(const QPoint &pos);
    void setTargetFrameGenerator();
    void jumpToTargetGenerator();
    void centerFramesAndSetTargetGenerator(u32 centerFrames);
    void seedToTime();
    void copySeedToClipboard();
    void on_pushButtonProfileManager_clicked();

};

#endif // STATIONARY3_H
