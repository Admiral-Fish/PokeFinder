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
#include <libPokeFinder/RNG/IRNG.hpp>
#include <libPokeFinder/RNG/IRNG64.hpp>
#include <libPokeFinder/RNG/LCRNG.hpp>
#include <libPokeFinder/RNG/LCRNG64.hpp>
#include <libPokeFinder/RNG/MTRNG.hpp>
#include <libPokeFinder/RNG/SFMT.hpp>
#include <libPokeFinder/RNG/TinyMT.hpp>

using namespace std;
typedef uint32_t u32;
typedef uint64_t u64;
typedef u64 (*func)(u64, u64);
typedef unordered_map<string, func> Calculator;

class ResearcherFrame
{

private:


public:
    quint64 Custom[10];
    bool RNG64Bit;
    u64 Full64;
    u32 Full32;
    u32 Frame;

    ResearcherFrame(bool rng64Bit, u32 frame);
    inline u32 High32() { return (u32)(Full64 >> 32); }
    inline u32 Low32() { return (u32)(Full64 & 0xFFFFFFFF); }
    inline u32 High16() { return RNG64Bit ? High32() >> 16 : Full32 >> 16; }
    inline u32 Low16() { return RNG64Bit ? High32() & 0xFFFF : Full32 & 0xFFFF; }
    inline u32 Mod25() { return RNG64Bit ? High32() % 25 : High16() % 25; }
    inline u32 Mod100() { return RNG64Bit ? High32() % 100 : High16() % 100; }
    inline u32 Mod3() { return RNG64Bit ? High32() % 3 : High16() % 3; }
    inline u32 Div656() { return High16() / 656; }
    inline u32 HighBit() { return RNG64Bit ? High32() >> 31 : High16() >> 15; }
    inline u32 LowBit() { return RNG64Bit ? High32() & 1 : High16() & 1; }

};

namespace Ui {
class Researcher;
}

class Researcher : public QMainWindow
{
    Q_OBJECT

private:
    void setupModel();
    u64 GetCustom(string text, ResearcherFrame frame, vector<ResearcherFrame> frames);
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
