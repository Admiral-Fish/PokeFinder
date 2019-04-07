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

#ifndef STATIONARY4_HPP
#define STATIONARY4_HPP

#include <QMenu>
#include <QMutex>
#include <QFileDialog>
#include <QSettings>
#include <QThread>
#include <QTimer>
#include <Core/Gen4/Generator4.hpp>
#include <Core/Gen4/IVSearcher4.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Gen4/ProfileManager4.hpp>
#include <Forms/Gen4/SeedtoTime4.hpp>
#include <Models/Gen4/Searcher4Model.hpp>
#include <Models/Gen4/Stationary4Model.hpp>

namespace Ui
{
    class Stationary4;
}

class Stationary4 : public QWidget
{
    Q_OBJECT

signals:
    void alertProfiles(int);

public:
    explicit Stationary4(QWidget *parent = nullptr);
    ~Stationary4() override;
    void updateProfiles();

private:
    Ui::Stationary4 *ui;
    Searcher4Model *searcherModel{};
    Stationary4Model *generatorModel{};
    QVector<Profile4> profiles;
    QMenu *generatorMenu{};
    QMenu *searcherMenu{};

    void setupModels();
    void updateView(const QVector<Frame4> &frames, int progress);

private slots:
    void refreshProfiles();
    void on_pushButtonGenerate_clicked();
    void on_pushButtonSearch_clicked();
    void on_comboBoxProfiles_currentIndexChanged(int index);
    void on_pushButtonGeneratorLead_clicked();
    void seedToTime();
    void on_tableViewGenerator_customContextMenuRequested(const QPoint &pos);
    void on_tableViewSearcher_customContextMenuRequested(const QPoint &pos);
    void on_pushButtonProfileManager_clicked();

};

#endif // STATIONARY4_HPP
