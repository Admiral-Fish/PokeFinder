/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef IVFILTER_HPP
#define IVFILTER_HPP

#include <Core/Util/Global.hpp>
#include <QWidget>

namespace Ui
{
    class IVFilter;
}

class IVFilter : public QWidget
{
    Q_OBJECT
public:
    explicit IVFilter(QWidget *parent = nullptr);
    ~IVFilter() override;
    std::array<u8, 6> getLower() const;
    std::array<u8, 6> getUpper() const;

private:
    Ui::IVFilter *ui;

    void changeHP(int min, int max);
    void changeAtk(int min, int max);
    void changeDef(int min, int max);
    void changeSpA(int min, int max);
    void changeSpD(int min, int max);
    void changeSpe(int min, int max);

private slots:
    void changeCompareHP(int type);
    void changeCompareAtk(int type);
    void changeCompareDef(int type);
    void changeCompareSpA(int type);
    void changeCompareSpD(int type);
    void changeCompareSpe(int type);
};

#endif // IVFILTER_HPP
