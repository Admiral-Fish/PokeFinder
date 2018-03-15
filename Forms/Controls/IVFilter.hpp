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

#ifndef IVFILTER_HPP
#define IVFILTER_HPP

#include <QWidget>
#include <vector>

typedef uint32_t u32;
using std::vector;

namespace Ui
{
    class IVFilter;
}

class IVFilter : public QWidget
{
    Q_OBJECT

public:
    explicit IVFilter(QWidget *parent = 0);
    ~IVFilter();
    vector<u32> getEvals();
    vector<u32> getValues();
    vector<u32> getLower();
    vector<u32> getUpper();
    void clearValues();
    void setValues(u32 hp, u32 atk, u32 def, u32 spa, u32 spd, u32 spe);

private slots:
    void on_pushButton31HP_clicked();
    void on_pushButton30HP_clicked();
    void on_pushButtonG30HP_clicked();
    void on_pushButtonClearHP_clicked();
    void on_pushButton31Atk_clicked();
    void on_pushButton30Atk_clicked();
    void on_pushButtonG30Atk_clicked();
    void on_pushButtonClearAtk_clicked();
    void on_pushButton31Def_clicked();
    void on_pushButton30Def_clicked();
    void on_pushButtonG30Def_clicked();
    void on_pushButtonClearDef_clicked();
    void on_pushButton31SpA_clicked();
    void on_pushButton30SpA_clicked();
    void on_pushButtonG30SpA_clicked();
    void on_pushButtonClearSpA_clicked();
    void on_pushButton31SpD_clicked();
    void on_pushButton30SpD_clicked();
    void on_pushButtonG30SpD_clicked();
    void on_pushButtonClearSpD_clicked();
    void on_pushButton31Spe_clicked();
    void on_pushButton30Spe_clicked();
    void on_pushButtonG30Spe_clicked();
    void on_pushButtonClearSpe_clicked();

private:
    Ui::IVFilter *ui;

};

#endif // IVFILTER_HPP
