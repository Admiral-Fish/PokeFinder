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

#ifndef RESEARCHER_HPP
#define RESEARCHER_HPP

#include <Core/Util/Global.hpp>
#include <QHash>
#include <QMessageBox>

namespace PokeFinderCore
{
    class ResearcherFrame;
}

namespace PokeFinderModels
{
    class ResearcherModel;
}

namespace PokeFinderForms
{
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
        PokeFinderModels::ResearcherModel *model {};
        QHash<QString, u8> keys;

        void setupModels();
        u64 getCustom(const QString &text, const PokeFinderCore::ResearcherFrame &frame,
            const QVector<PokeFinderCore::ResearcherFrame> &frames);
        void resizeHeader();
        QVector<bool> getHexCheck();
        static inline u64 divide(u64 x, u64 y) { return y == 0 ? 0 : x / y; }
        static inline u64 modulo(u64 x, u64 y) { return x % y; }
        static inline u64 shiftRight(u64 x, u64 y) { return x >> y; }
        static inline u64 shiftLeft(u64 x, u64 y) { return x << y; }
        static inline u64 bitAnd(u64 x, u64 y) { return x & y; }
        static inline u64 bitOr(u64 x, u64 y) { return x | y; }
        static inline u64 bitXor(u64 x, u64 y) { return x ^ y; }
        static inline u64 add(u64 x, u64 y) { return x + y; }
        static inline u64 subtract(u64 x, u64 y) { return x - y; }
        static inline u64 multiply(u64 x, u64 y) { return x * y; }

    private slots:
        void on_pushButtonGenerate32Bit_clicked();
        void on_rngSelection_currentChanged(int index);
        void on_pushButtonSearch_clicked();
        void on_pushButtonNext_clicked();
    };
}

#endif // RESEARCHER_HPP
