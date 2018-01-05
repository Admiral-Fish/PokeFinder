/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

#include <QMainWindow>
#include <QMessageBox>
#include <QStandardItemModel>
#include <Forms/QTextBox.hpp>
#include <vector>
#include <unordered_map>
#include <libPokeFinder/RNG/LCRNG.hpp>
#include <libPokeFinder/RNG/LCRNG64.hpp>
#include <libPokeFinder/RNG/MTRNG.hpp>
#include <libPokeFinder/RNG/SFMT.hpp>
#include <libPokeFinder/RNG/TinyMT.hpp>
#include <Models/ResearcherModel.hpp>
#include <Util/ResearcherFrame.hpp>

using namespace std;
typedef uint32_t u32;
typedef uint64_t u64;
typedef u64 (*func)(u64, u64);
typedef unordered_map<string, func> Calculator;

namespace Ui {
class Researcher;
}

class Researcher : public QMainWindow
{
    Q_OBJECT

private:
    u64 GetCustom(string text, ResearcherFrame frame, vector<ResearcherFrame> frames);
    static inline u64 Divide(u64 x, u64 y) { return y == 0 ? 0 : x / y; }
    static inline u64 Modulo(u64 x, u64 y) { return x % y; }
    static inline u64 ShiftRight(u64 x, u64 y) { return x >> y; }
    static inline u64 ShiftLeft(u64 x, u64 y) { return x << y; }
    static inline u64 And(u64 x, u64 y) { return x & y; }
    static inline u64 Or(u64 x, u64 y) { return x | y; }
    static inline u64 Xor(u64 x, u64 y) { return x ^ y; }
    static inline u64 Add(u64 x, u64 y) { return x + y; }
    static inline u64 Subtract(u64 x, u64 y) { return x - y; }
    static inline u64 Multiply(u64 x, u64 y) { return x * y; }

    unordered_map<string, int> keys;

public:
    explicit Researcher(QWidget *parent = 0);
    ~Researcher();

private slots:
    void on_pushButtonGenerate32Bit_clicked();
    void on_rngSelection_currentChanged(int index);

private:
    Ui::Researcher *ui;

};

#endif // RESEARCHER_HPP
