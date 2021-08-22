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

#ifndef JIRACHICHECKSUM_HPP
#define JIRACHICHECKSUM_HPP

#include <Core/Util/Global.hpp>
#include <QStandardItemModel>
#include <QWidget>

class QStandardItemModel;

namespace Ui
{
    class JirachiChecksum;
}

class JirachiChecksum : public QWidget
{
    Q_OBJECT
public:
    explicit JirachiChecksum(QWidget *parent = nullptr);
    void on_Change();
    u32 nameConvert(QString name) const;
    u32 optionlessTimelessSeed() const;
    void generate() const;
    void generateJirachi(QList<QStandardItem *> *row, u16 seed) const;
    ~JirachiChecksum() override;

private slots:
// TODO: change this to use connect()
    void on_wingullCheck_toggled(bool checked);
    void on_wurmpleRadio_toggled(bool checked);
    void on_zigzagoonCheck_toggled(bool checked);
    void on_mudkipRadio_clicked();
    void on_torchicRadio_clicked();
    void on_treeckoRadio_clicked();
    void on_xxRadio_clicked();
    void on_xnRadio_clicked();
    void on_nnRadio_clicked();
    void on_laRadio_clicked();
    void on_lrRadio_clicked();
    void on_naRadio_clicked();
    void on_stereoRadio_clicked();
    void on_monRadio_clicked();
    void on_setRadio_clicked();
    void on_shiftRadio_clicked();
    void on_offRadio_clicked();
    void on_onRadio_clicked();
    void on_fastRadio_clicked();
    void on_midRadio_clicked();
    void on_slowRadio_clicked();
    void on_femaleRadio_clicked();
    void on_maleRadio_clicked();
    void on_framesTB_textChanged(const QString &arg1);
    void on_secondsTB_textChanged(const QString &arg1);
    void on_minutesTB_textChanged(const QString &arg1);
    void on_frameStyleTB_textChanged(const QString &arg1);
    void on_sidTB_textChanged(const QString &arg1);
    void on_tidTB_textChanged(const QString &arg1);
    void on_nameTB_textChanged(const QString &arg1);

private:
    Ui::JirachiChecksum *ui;
    QStandardItemModel *model = nullptr;
private slots:
};

#endif // JIRACHICHECKSUM_HPP
