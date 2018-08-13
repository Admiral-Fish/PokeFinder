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
#include <QLabel>
#include <QVector>

namespace Ui
{
    class IVFilter;
}

class IVFilter : public QWidget
{
    Q_OBJECT

protected:
    void changeEvent(QEvent *);

private:
    Ui::IVFilter *ui;

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
    void changeCompareHP(int type);
    void changeCompareAtk(int type);
    void changeCompareDef(int type);
    void changeCompareSpA(int type);
    void changeCompareSpD(int type);
    void changeCompareSpe(int type);

public:
    explicit IVFilter(QWidget *parent = nullptr);
    ~IVFilter();
    QVector<quint32> getEvals();
    QVector<quint32> getValues();
    QVector<quint32> getLower();
    QVector<quint32> getUpper();
    void clearValues();
    void setValues(quint32 hp, quint32 atk, quint32 def, quint32 spa, quint32 spd, quint32 spe);

};

#endif // IVFILTER_HPP
