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

#ifndef STATIONARY5_HPP
#define STATIONARY5_HPP

#include <QMenu>
#include <QMutex>
#include <QFileDialog>
#include <QSettings>
#include <QThread>
#include <QTimer>
#include <Core/Gen5/Generator5.hpp>
//#include <Core/Gen5/IVSearcher5.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Gen5/ProfileManager5.hpp>
//#include <Models/Gen5/Searcher5Model.hpp>
//#include <Models/Gen5/Stationary5Model.hpp>

#include <Core/Util/Lead.hpp>
#include <Core/Util/Nature.hpp>
#include <Core/Util/Power.hpp>
#include <Core/Parents/FrameCompare.hpp>

namespace Ui
{
    class Stationary5;
}

class Stationary5 : public QWidget
{
    Q_OBJECT

signals:
    void alertProfiles(int);

public:
    explicit Stationary5(QWidget *parent = nullptr);
    ~Stationary5() override;
    void updateProfiles();

private:
    Ui::Stationary5 *ui;
    //Searcher5Model *searcherModel{};
    //Stationary5Model *generatorModel{};
    QVector<Profile5> profiles;
    QMenu *generatorMenu{};
    QMenu *searcherMenu{};

    void setupModels();
    void updateView(const QVector<Frame5> &frames, int progress);

private slots:
    void refreshProfiles();
    void on_pushButtonGenerate_clicked();
    void on_pushButtonSearch_clicked();
    void on_comboBoxProfiles_currentIndexChanged(int index);
    void on_pushButtonGeneratorLead_clicked();
    void on_tableViewGenerator_customContextMenuRequested(const QPoint &pos);
    void on_tableViewSearcher_customContextMenuRequested(const QPoint &pos);
    void on_pushButtonProfileManager_clicked();

};

#endif // STATIONARY5_HPP
