/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef RESEARCHER_HPP
#define RESEARCHER_HPP

#include <Core/Util/Global.hpp>
#include <QHash>
#include <QMessageBox>

class ResearcherFrame;
class ResearcherModel;

namespace Ui
{
    class Researcher;
}

class Researcher : public QWidget
{
    Q_OBJECT
public:
    explicit Researcher(QWidget *parent = nullptr);
    ~Researcher() override;

private:
    Ui::Researcher *ui;
    ResearcherModel *model = nullptr;
    QHash<QString, u8> keys;

    void setupModels();
    u64 getCustom(const QString &text, const ResearcherFrame &frame, const QVector<ResearcherFrame> &frames);
    void resizeHeader();
    QVector<bool> getHexCheck();

private slots:
    void generate();
    void selectionIndexChanged(int index);
    void search();
    void next();
};

#endif // RESEARCHER_HPP
