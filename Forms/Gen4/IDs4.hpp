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

#ifndef IDS4_HPP
#define IDS4_HPP

#include <QStandardItemModel>
#include <QWidget>
#include <Core/Util/Global.hpp>

namespace PokeFinderForms
{

    namespace Ui
    {
        class IDs4;
    }

    class IDs4 : public QWidget
    {
        Q_OBJECT

    signals:
        void updateProgress();

    public:
        explicit IDs4(QWidget *parent = nullptr);
        ~IDs4() override;

    private:
        Ui::IDs4 *ui;
        QStandardItemModel *shinyPID{};
        QStandardItemModel *tidSID{};
        QStandardItemModel *seedFinder{};

        void setupModels();

    private slots:
        void updateProgressShinyPID(const QVector<QList<QStandardItem *>> &frames, int progress);
        void updateProgressTIDSID(const QVector<QList<QStandardItem *>> &frames, int progress);
        void on_pushButtonShinyPIDSearch_clicked();
        void on_pushButtonTIDSIDSearch_clicked();
        void on_pushButtonSeedFinderSearch_clicked();

    };

}

#endif // IDS4_HPP
