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

#include "Researcher.hpp"
#include "ui_Researcher.h"

Researcher::Researcher(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Researcher)
{
    ui->setupUi(this);
    setupModel();

    keys["64Bit"] = 0;
    keys["32Bit"] = 1;
    keys["32Bit High"] = 2;
    keys["32Bit Low"] = 3;
    keys["16Bit High"] = 4;
    keys["16Bit Low"] = 5;
    keys["Custom1"] = 6;
    keys["Custom2"] = 7;
    keys["Custom3"] = 8;
    keys["Custom4"] = 9;
    keys["Custom5"] = 10;
    keys["Custom6"] = 11;
    keys["Custom7"] = 12;
    keys["Custom8"] = 13;
    keys["Custom9"] = 14;
    keys["Custom10"] = 15;
    keys["Previous1"] = 16;
    keys["Previous2"] = 17;
    keys["Previous3"] = 18;
    keys["Previous4"] = 19;
    keys["Previous5"] = 20;
    keys["Previous6"] = 21;
    keys["Previous7"] = 22;
    keys["Previous8"] = 23;
    keys["Previous9"] = 24;
    keys["Previous10"] = 25;
}

Researcher::~Researcher()
{
    delete ui;
}

u64 Divide(u64 x, u64 y) { return y == 0 ? 0 : x / y; }

u64 Modulo(u64 x, u64 y) { return x % y; }

u64 ShiftRight(u64 x, u64 y) { return x >> y; }

u64 ShiftLeft(u64 x, u64 y) { return x << y; }

u64 And(u64 x, u64 y) { return x & y; }

u64 Or(u64 x, u64 y) { return x | y; }

u64 Xor(u64 x, u64 y) { return x ^ y; }

u64 Add(u64 x, u64 y) { return x + y; }

u64 Subtract(u64 x, u64 y) { return x - y; }

u64 Multiply(u64 x, u64 y) { return x * y; }

void Researcher::setupModel()
{
    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({ tr("Frame"), tr("32Bit"), tr("16Bit High"), tr("16BitLow"),
                                       tr("Custom1"), tr("Custom2"), tr("Custom3"), tr("Custom4"),
                                       tr("Custom5"), tr("Custom6"), tr("Custom7"), tr("Custom8"),
                                       tr("Custom9"), tr("Custom10"), tr("%3"), tr("% 25"),
                                       tr("%100"), tr("/656"), tr("LBit"), tr("HBit") });
    ui->tableView->setModel(model);
}

void Researcher::on_pushButtonGenerate32Bit_clicked()
{
    u64 seed = ui->lineEditSeed->text().toULongLong(NULL, 16);
    u32 maxFrames = ui->lineEditMaxFrames->text().toUInt(NULL, 10);
    u32 startingFrame = ui->lineEditStartingFrame->text().toUInt(NULL, 10);

    IRNG *rng = NULL;
    IRNG64 *rng64 = NULL;

    if (ui->rngSelection->currentIndex() != 1 && (seed > 0xffffffff))
        seed >>= 32;

    if (ui->rngSelection->currentIndex() == 0)
    {
        if (ui->radioButtonCommon32Bit->isChecked())
        {
            switch (ui->comboBoxRNG32Bit->currentIndex())
            {
                case 0:
                    rng = new PokeRNG(seed);
                    break;
                case 1:
                    rng = new PokeRNGR(seed);
                    break;
                case 2:
                    rng = new XDRNG(seed);
                    break;
                case 3:
                    rng = new XDRNGR(seed);
                    break;
                case 4:
                    rng = new ARNG(seed);
                    break;
                case 5:
                    rng = new ARNGR(seed);
                    break;
                case 6:
                    rng = new MersenneTwister(seed);
                    break;
                case 7:
                    rng = new MersenneTwisterUntempered(seed);
                    break;
                case 8:
                    if (maxFrames > 227)
                    {
                        QMessageBox error;
                        error.setText("Please enter a max frame lower then 228");
                        error.exec();
                        return;
                    }
                    else
                        rng = new MersenneTwisterFast(seed, maxFrames);
                    break;
            }
        }
        else
        {
            u32 add = ui->lineEditAdd32Bit->text().toUInt(NULL, 16);
            u32 mult = ui->lineEditMult32Bit->text().toUInt(NULL, 16);
            rng = new LCRNG(add, mult, seed);
        }
    }
    else if (ui->rngSelection->currentIndex() == 1)
    {
        if (ui->radioButtonCommon64Bit->isChecked())
        {
            switch (ui->comboBoxRNG64Bit->currentIndex())
            {
                case 0:
                    rng64 = new BWRNG(seed);
                    break;
                case 1:
                    rng64 = new BWRNGR(seed);
                    break;
                case 2:
                    if (seed > 0xffffffff)
                        seed >>= 32;
                    rng64 = new SFMT(seed);
                    break;
            }
        }
        else
        {
            u64 add = ui->lineEditAdd64Bit->text().toUInt(NULL, 16);
            u64 mult = ui->lineEditMult64Bit->text().toUInt(NULL, 16);
            rng64 = new LCRNG64(add, mult, seed);
        }
    }
    else
    {
        u32 status[4] = { ui->lineEditStatus0->text().toUInt(NULL, 16), ui->lineEditStatus1->text().toUInt(NULL, 16),
                          ui->lineEditStatus2->text().toUInt(NULL, 16), ui->lineEditStatus3->text().toUInt(NULL, 16) };
        rng = new TinyMT(status);
    }

    Calculator calc;
    calc["/"] = &Divide;
    calc["%"] = &Modulo;
    calc[">>"] = &ShiftRight;
    calc["<<"] = &ShiftLeft;
    calc["&"] = &And;
    calc["|"] = &Or;
    calc["^"] = &Xor;
    calc["+"] = &Add;
    calc["-"] = &Subtract;
    calc["*"] = &Multiply;

    bool calcCustom[10];
    u64 customRValue[10];
    bool pass[10];
    u64(*Calculators[10])(u64, u64);

    calcCustom[0] = ui->lineEditRValue1->text() != "";
    calcCustom[1] = ui->lineEditRValue2->text() != "" || ui->comboBoxRValue2->currentIndex() != 0;
    calcCustom[2] = ui->lineEditRValue3->text() != "" || ui->comboBoxRValue3->currentIndex() != 0;
    calcCustom[3] = ui->lineEditRValue4->text() != "" || ui->comboBoxRValue4->currentIndex() != 0;
    calcCustom[4] = ui->lineEditRValue5->text() != "" || ui->comboBoxRValue5->currentIndex() != 0;
    calcCustom[5] = ui->lineEditRValue6->text() != "" || ui->comboBoxRValue6->currentIndex() != 0;
    calcCustom[6] = ui->lineEditRValue7->text() != "" || ui->comboBoxRValue7->currentIndex() != 0;
    calcCustom[7] = ui->lineEditRValue8->text() != "" || ui->comboBoxRValue8->currentIndex() != 0;
    calcCustom[8] = ui->lineEditRValue9->text() != "" || ui->comboBoxRValue9->currentIndex() != 0;
    calcCustom[9] = ui->lineEditRValue10->text() != "" || ui->comboBoxRValue10->currentIndex() != 0;

    customRValue[0] = ui->checkBoxHex1->isChecked()
                      ? ui->lineEditRValue1->text().toULongLong(&pass[0], 16)
                      : ui->lineEditRValue1->text().toULongLong(&pass[0], 10);
    customRValue[1] = ui->checkBoxHex2->isChecked()
                      ? ui->lineEditRValue2->text().toULongLong(&pass[1], 16)
                      : ui->lineEditRValue2->text().toULongLong(&pass[1], 10);
    customRValue[2] = ui->checkBoxHex3->isChecked()
                      ? ui->lineEditRValue3->text().toULongLong(&pass[2], 16)
                      : ui->lineEditRValue3->text().toULongLong(&pass[2], 10);
    customRValue[3] = ui->checkBoxHex4->isChecked()
                      ? ui->lineEditRValue4->text().toULongLong(&pass[3], 16)
                      : ui->lineEditRValue4->text().toULongLong(&pass[3], 10);
    customRValue[4] = ui->checkBoxHex5->isChecked()
                      ? ui->lineEditRValue5->text().toULongLong(&pass[4], 16)
                      : ui->lineEditRValue5->text().toULongLong(&pass[4], 10);
    customRValue[5] = ui->checkBoxHex6->isChecked()
                      ? ui->lineEditRValue6->text().toULongLong(&pass[5], 16)
                      : ui->lineEditRValue6->text().toULongLong(&pass[5], 10);
    customRValue[6] = ui->checkBoxHex7->isChecked()
                      ? ui->lineEditRValue7->text().toULongLong(&pass[6], 16)
                      : ui->lineEditRValue7->text().toULongLong(&pass[6], 10);
    customRValue[7] = ui->checkBoxHex8->isChecked()
                      ? ui->lineEditRValue8->text().toULongLong(&pass[7], 16)
                      : ui->lineEditRValue8->text().toULongLong(&pass[7], 10);
    customRValue[8] = ui->checkBoxHex9->isChecked()
                      ? ui->lineEditRValue9->text().toULongLong(&pass[8], 16)
                      : ui->lineEditRValue9->text().toULongLong(&pass[8], 10);
    customRValue[9] = ui->checkBoxHex10->isChecked()
                      ? ui->lineEditRValue10->text().toULongLong(&pass[9], 16)
                      : ui->lineEditRValue10->text().toULongLong(&pass[9], 10);

    for (int i = 0; i < 10; i++)
    {
        if (calcCustom[i] && !pass[i])
        {
            QMessageBox error;
            error.setText(tr("You must check the Hex box in order to use Hex values."));
            error.exec();
            return;
        }
    }

    Calculators[0] = calc[ui->comboBoxOperator1->currentText().toStdString()];
    Calculators[1] = calc[ui->comboBoxOperator2->currentText().toStdString()];
    Calculators[2] = calc[ui->comboBoxOperator3->currentText().toStdString()];
    Calculators[3] = calc[ui->comboBoxOperator4->currentText().toStdString()];
    Calculators[4] = calc[ui->comboBoxOperator5->currentText().toStdString()];
    Calculators[5] = calc[ui->comboBoxOperator6->currentText().toStdString()];
    Calculators[6] = calc[ui->comboBoxOperator7->currentText().toStdString()];
    Calculators[7] = calc[ui->comboBoxOperator8->currentText().toStdString()];
    Calculators[8] = calc[ui->comboBoxOperator9->currentText().toStdString()];
    Calculators[9] = calc[ui->comboBoxOperator10->currentText().toStdString()];

    vector<ResearcherFrame> frames;
    bool rng64Bit = ui->rngSelection->currentIndex() == 1;

    if (rng64Bit)
    {
        rng64->AdvanceFrames(startingFrame - 1);
    }
    else
    {
        rng->AdvanceFrames(startingFrame - 1);
    }

    string textL[10] = { ui->comboBoxLValue1->currentText().toStdString(), ui->comboBoxLValue2->currentText().toStdString(),
                          ui->comboBoxLValue3->currentText().toStdString(), ui->comboBoxLValue4->currentText().toStdString(),
                          ui->comboBoxLValue5->currentText().toStdString(), ui->comboBoxLValue6->currentText().toStdString(),
                          ui->comboBoxLValue7->currentText().toStdString(), ui->comboBoxLValue8->currentText().toStdString(),
                          ui->comboBoxLValue9->currentText().toStdString(), ui->comboBoxLValue10->currentText().toStdString() };

    string textR[10] = { "None", ui->comboBoxRValue2->currentText().toStdString(),
                          ui->comboBoxRValue3->currentText().toStdString(), ui->comboBoxRValue4->currentText().toStdString(),
                          ui->comboBoxRValue5->currentText().toStdString(), ui->comboBoxRValue6->currentText().toStdString(),
                          ui->comboBoxRValue7->currentText().toStdString(), ui->comboBoxRValue8->currentText().toStdString(),
                          ui->comboBoxRValue9->currentText().toStdString(), ui->comboBoxRValue10->currentText().toStdString() };

    for (u32 i = startingFrame; i < maxFrames + startingFrame; i++)
    {
        ResearcherFrame frame = ResearcherFrame(rng64Bit, i);
        if (rng64Bit)
        {
            frame.Full64 = rng64->Nextulong();
        }
        else
        {
            frame.Full32 = rng->Nextuint();
        }

        for (int j = 0; j < 10; j++)
        {
            if (calcCustom[j])
            {
                u64 temp = GetCustom(textL[j], frame, frames);

                if (textR[j] != "None")
                    customRValue[j] = GetCustom(textR[j], frame, frames);

                frame.Custom[j] = Calculators[j](temp, customRValue[j]);
            }
        }
        frames.push_back(frame);
    }

    QStandardItemModel *model = new QStandardItemModel(this);
    if (rng64Bit)
    {
        model->setHorizontalHeaderLabels({ tr("Frame"), tr("64 Bit"), tr("32Bit High"), tr("32Bit Low"),
                                           tr("16Bit High"), tr("16BitLow"), tr("Custom1"), tr("Custom2"),
                                           tr("Custom3"), tr("Custom4"), tr("Custom5"), tr("Custom6"),
                                           tr("Custom7"), tr("Custom8"), tr("Custom9"), tr("Custom10"),
                                           tr("%3"), tr("% 25"), tr("%100"), tr("/656"),  tr("LBit"),
                                           tr("HBit") });
    }
    else
    {
        model->setHorizontalHeaderLabels({ tr("Frame"), tr("32Bit"), tr("16Bit High"), tr("16BitLow"),
                                           tr("Custom1"), tr("Custom2"), tr("Custom3"), tr("Custom4"),
                                           tr("Custom5"), tr("Custom6"), tr("Custom7"), tr("Custom8"),
                                           tr("Custom9"), tr("Custom10"), tr("%3"), tr("% 25"), tr("%100"),
                                           tr("/656"), tr("LBit"), tr("HBit") });
    }

    for (u32 k = 0; k < maxFrames; k++)
        model->appendRow(frames[k].GetRow());

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();

    delete rng;
    delete rng64;
}

u64 Researcher::GetCustom(string text, ResearcherFrame frame, vector<ResearcherFrame> frames)
{
    switch (keys[text])
    {
        case 0:
            return frame.Full64;
        case 1:
            return frame.Full32;
        case 2:
            return frame.High32();
        case 3:
            return frame.Low32();
        case 4:
            return frame.High16();
        case 5:
            return frame.Low16();
        case 6:
            return frame.Custom[0];
        case 7:
            return frame.Custom[1];
        case 8:
            return frame.Custom[2];
        case 9:
            return frame.Custom[3];
        case 10:
            return frame.Custom[4];
        case 11:
            return frame.Custom[5];
        case 12:
            return frame.Custom[6];
        case 13:
            return frame.Custom[7];
        case 14:
            return frame.Custom[8];
        case 15:
            return frame.Custom[9];
        case 16:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].Custom[0];
        case 17:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].Custom[1];
        case 18:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].Custom[2];
        case 19:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].Custom[3];
        case 20:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].Custom[4];
        case 21:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].Custom[5];
        case 22:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].Custom[6];
        case 23:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].Custom[7];
        case 24:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].Custom[8];
        case 25:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].Custom[9];
        default:
            return 0;
    }
}

ResearcherFrame::ResearcherFrame(bool rng64Bit, u32 frame)
{
    for (int i = 0; i < 10; i++)
    {
        Custom[i] = 0;
    }
    RNG64Bit = rng64Bit;
    Full32 = 0;
    Full64 = 0;
    Frame = frame;
}


QList<QStandardItem *> ResearcherFrame::GetRow()
{
    QList<QStandardItem *> row;
    row.append(new QStandardItem(QString::number(Frame)));
    if (RNG64Bit)
    {
        row.append(new QStandardItem(QString::number(Full64, 16).toUpper().rightJustified(16,'0')));
        row.append(new QStandardItem(QString::number(High32(), 16).toUpper().rightJustified(8,'0')));
        row.append(new QStandardItem(QString::number(Low32(), 16).toUpper().rightJustified(8,'0')));
        row.append(new QStandardItem(QString::number(High16(), 16).toUpper().rightJustified(4,'0')));
        row.append(new QStandardItem(QString::number(Low16(), 16).toUpper().rightJustified(4,'0')));
    }
    else
    {
        row.append(new QStandardItem(QString::number(Full32, 16).toUpper().rightJustified(8,'0')));
        row.append(new QStandardItem(QString::number(High16(), 16).toUpper().rightJustified(4,'0')));
        row.append(new QStandardItem(QString::number(Low16(), 16).toUpper().rightJustified(4,'0')));
    }

    for (int i = 0; i < 10; i++)
    {
        row.append(new QStandardItem(QString::number(Custom[i])));
    }

    row.append(new QStandardItem(QString::number(Mod3())));
    row.append(new QStandardItem(QString::number(Mod25())));
    row.append(new QStandardItem(QString::number(Mod100())));
    row.append(new QStandardItem(QString::number(Div656())));
    row.append(new QStandardItem(QString::number(LowBit())));
    row.append(new QStandardItem(QString::number(HighBit())));

    for(int i = 0; i < row.count(); i++)
    {
        row.at(i)->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    }

    return row;
}


void Researcher::on_rngSelection_currentChanged(int index)
{
    ui->lineEditSeed->setVisible(index != 2);
    ui->label_14->setVisible(index != 2);
}
