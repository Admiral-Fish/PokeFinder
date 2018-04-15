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

#include "Researcher.hpp"
#include "ui_Researcher.h"

Researcher::Researcher(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Researcher)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

void Researcher::changeEvent(QEvent *event)
{
    if (event != NULL)
    {
        switch (event->type())
        {
            case QEvent::LanguageChange:
                ui->retranslateUi(this);
                translate();
                break;
            default:
                break;
        }
    }
}

Researcher::~Researcher()
{
    delete ui;
    delete model;
}

void Researcher::on_pushButtonGenerate32Bit_clicked()
{
    bool rng64Bit = ui->rngSelection->currentIndex() == 1;

    model->clear();
    model->setFlag(rng64Bit);

    u64 seed = ui->textBoxSeed->text().toULongLong(NULL, 16);
    u32 maxFrames = ui->textBoxMaxFrames->text().toUInt(NULL, 10);
    u32 startingFrame = ui->textBoxStartingFrame->text().toUInt(NULL, 10);

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
                    if (maxFrames > 227 || startingFrame > 227 || (startingFrame + maxFrames > 227))
                    {
                        QMessageBox error;
                        error.setText(tr("Please enter a search range lower then 228"));
                        error.exec();
                        return;
                    }
                    rng = new MersenneTwisterFast(seed, maxFrames);
                    break;
            }
        }
        else
        {
            u32 add = ui->textBoxAdd32Bit->text().toUInt(NULL, 16);
            u32 mult = ui->textBoxMult32Bit->text().toUInt(NULL, 16);
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
            u64 add = ui->textBoxAdd64Bit->text().toUInt(NULL, 16);
            u64 mult = ui->textBoxMult64Bit->text().toUInt(NULL, 16);
            rng64 = new LCRNG64(add, mult, seed);
        }
    }
    else
    {
        u32 status[4] = { ui->textBoxStatus0->text().toUInt(NULL, 16), ui->textBoxStatus1->text().toUInt(NULL, 16),
                          ui->textBoxStatus2->text().toUInt(NULL, 16), ui->textBoxStatus3->text().toUInt(NULL, 16)
                        };
        rng = new TinyMT(status);
    }

    Calculator calc;

    calc["/"] = &Researcher::divide;
    calc["%"] = &Researcher::modulo;
    calc[">>"] = &Researcher::shiftRight;
    calc["<<"] = &Researcher::shiftLeft;
    calc["&"] = &Researcher::bitAnd;
    calc["|"] = &Researcher::bitOr;
    calc["^"] = &Researcher::bitXor;
    calc["+"] = &Researcher::add;
    calc["-"] = &Researcher::subtract;
    calc["*"] = &Researcher::multiply;

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

    Calculators[0] = calc[ui->comboBoxOperator1->currentText()];
    Calculators[1] = calc[ui->comboBoxOperator2->currentText()];
    Calculators[2] = calc[ui->comboBoxOperator3->currentText()];
    Calculators[3] = calc[ui->comboBoxOperator4->currentText()];
    Calculators[4] = calc[ui->comboBoxOperator5->currentText()];
    Calculators[5] = calc[ui->comboBoxOperator6->currentText()];
    Calculators[6] = calc[ui->comboBoxOperator7->currentText()];
    Calculators[7] = calc[ui->comboBoxOperator8->currentText()];
    Calculators[8] = calc[ui->comboBoxOperator9->currentText()];
    Calculators[9] = calc[ui->comboBoxOperator10->currentText()];

    vector<ResearcherFrame> frames;

    if (rng64Bit)
    {
        rng64->advanceFrames(startingFrame - 1);
    }
    else
    {
        rng->advanceFrames(startingFrame - 1);
    }

    QString textL[10] = { ui->comboBoxLValue1->currentText(), ui->comboBoxLValue2->currentText(),
                          ui->comboBoxLValue3->currentText(), ui->comboBoxLValue4->currentText(),
                          ui->comboBoxLValue5->currentText(), ui->comboBoxLValue6->currentText(),
                          ui->comboBoxLValue7->currentText(), ui->comboBoxLValue8->currentText(),
                          ui->comboBoxLValue9->currentText(), ui->comboBoxLValue10->currentText()
                        };

    QString textR[10] = { tr("None"), ui->comboBoxRValue2->currentText(),
                          ui->comboBoxRValue3->currentText(), ui->comboBoxRValue4->currentText(),
                          ui->comboBoxRValue5->currentText(), ui->comboBoxRValue6->currentText(),
                          ui->comboBoxRValue7->currentText(), ui->comboBoxRValue8->currentText(),
                          ui->comboBoxRValue9->currentText(), ui->comboBoxRValue10->currentText()
                        };

    for (u32 i = startingFrame; i < maxFrames + startingFrame; i++)
    {
        ResearcherFrame frame = ResearcherFrame(rng64Bit, i);
        if (rng64Bit)
        {
            frame.full64 = rng64->nextULong();
        }
        else
        {
            frame.full32 = rng->nextUInt();
        }

        for (int j = 0; j < 10; j++)
        {
            if (calcCustom[j])
            {
                u64 temp = getCustom(textL[j], frame, frames);

                if (textR[j] != tr("None"))
                    customRValue[j] = getCustom(textR[j], frame, frames);

                frame.custom[j] = Calculators[j](temp, customRValue[j]);
            }
        }
        frames.push_back(frame);
    }

    model->setHex(getHexCheck());
    model->setModel(frames);

    delete rng;
    delete rng64;
}

void Researcher::on_pushButtonSearch_clicked()
{
    if (model->rowCount() == 0)
        return;

    QString string = ui->comboBoxSearch->currentText();
    u64 result = ui->textBoxSearch->text().toULongLong(NULL, 16);

    QModelIndex end = model->search(string, result, 0);
    if (end.isValid())
    {
        ui->tableView->setCurrentIndex(end);
        ui->tableView->scrollTo(end);
        ui->tableView->setFocus();
    }
    else
    {
        QMessageBox error;
        error.setText(tr("No result found"));
        error.exec();
        return;
    }
}

void Researcher::on_pushButtonNext_clicked()
{
    if (model->rowCount() == 0)
        return;

    QString string = ui->comboBoxSearch->currentText();
    QModelIndex start = ui->tableView->currentIndex();
    u64 result = ui->textBoxSearch->text().toULongLong(NULL, 16);

    if (!start.isValid())
        return;

    QModelIndex end = model->search(string, result, start.row() + 1);
    if (end.isValid())
    {
        ui->tableView->setCurrentIndex(end);
        ui->tableView->scrollTo(end);
        ui->tableView->setFocus();
    }
    else
    {
        QMessageBox error;
        error.setText(tr("No result found"));
        error.exec();
        return;
    }
}

u64 Researcher::getCustom(QString text, ResearcherFrame frame, vector<ResearcherFrame> frames)
{
    switch (keys[text])
    {
        case 0:
            return frame.full64;
        case 1:
            return frame.full32;
        case 2:
            return frame.high32();
        case 3:
            return frame.low32();
        case 4:
            return frame.high16();
        case 5:
            return frame.low16();
        case 6:
            return frame.custom[0];
        case 7:
            return frame.custom[1];
        case 8:
            return frame.custom[2];
        case 9:
            return frame.custom[3];
        case 10:
            return frame.custom[4];
        case 11:
            return frame.custom[5];
        case 12:
            return frame.custom[6];
        case 13:
            return frame.custom[7];
        case 14:
            return frame.custom[8];
        case 15:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].custom[0];
        case 16:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].custom[1];
        case 17:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].custom[2];
        case 18:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].custom[3];
        case 19:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].custom[4];
        case 20:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].custom[5];
        case 21:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].custom[6];
        case 22:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].custom[7];
        case 23:
            return frames.size() == 0 ? 0 : frames[frames.size() - 1].custom[8];
        default:
            return 0;
    }
}

void Researcher::setupModels()
{
    ui->tableView->setModel(model);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->textBoxStartingFrame->setValues(1, 0, true);
    ui->textBoxMaxFrames->setValues(1, 0, true);
    ui->textBoxSeed->setValues(0, 0, false);
    ui->textBoxSearch->setValues(0, 0, false);

    ui->textBoxMult32Bit->setValues(0, 32, false);
    ui->textBoxAdd32Bit->setValues(0, 32, false);

    ui->textBoxMult64Bit->setValues(0, 0, false);
    ui->textBoxAdd64Bit->setValues(0, 0, false);

    ui->textBoxStatus3->setValues(0, 32, false);
    ui->textBoxStatus2->setValues(0, 32, false);
    ui->textBoxStatus1->setValues(0, 32, false);
    ui->textBoxStatus0->setValues(0, 32, false);

    ui->lineEditRValue1->setValues(0, 32, false);
    ui->lineEditRValue2->setValues(0, 32, false);
    ui->lineEditRValue3->setValues(0, 32, false);
    ui->lineEditRValue4->setValues(0, 32, false);
    ui->lineEditRValue5->setValues(0, 32, false);
    ui->lineEditRValue6->setValues(0, 32, false);
    ui->lineEditRValue7->setValues(0, 32, false);
    ui->lineEditRValue8->setValues(0, 32, false);
    ui->lineEditRValue9->setValues(0, 32, false);
    ui->lineEditRValue10->setValues(0, 32, false);

    translate();
}

void Researcher::translate()
{
    keys.clear();
    keys[tr("64Bit")] = 0;
    keys[tr("32Bit")] = 1;
    keys[tr("32Bit High")] = 2;
    keys[tr("32Bit Low")] = 3;
    keys[tr("16Bit High")] = 4;
    keys[tr("16Bit Low")] = 5;
    keys[tr("Custom 1")] = 6;
    keys[tr("Custom 2")] = 7;
    keys[tr("Custom 3")] = 8;
    keys[tr("Custom 4")] = 9;
    keys[tr("Custom 5")] = 10;
    keys[tr("Custom 6")] = 11;
    keys[tr("Custom 7")] = 12;
    keys[tr("Custom 8")] = 13;
    keys[tr("Custom 9")] = 14;
    keys[tr("Previous 1")] = 15;
    keys[tr("Previous 2")] = 16;
    keys[tr("Previous 3")] = 17;
    keys[tr("Previous 4")] = 18;
    keys[tr("Previous 5")] = 19;
    keys[tr("Previous 6")] = 20;
    keys[tr("Previous 7")] = 21;
    keys[tr("Previous 8")] = 22;
    keys[tr("Previous 9")] = 23;
}

vector<bool> Researcher::getHexCheck()
{
    vector<bool> hex;

    hex.push_back(ui->checkBoxHex1->isChecked());
    hex.push_back(ui->checkBoxHex2->isChecked());
    hex.push_back(ui->checkBoxHex3->isChecked());
    hex.push_back(ui->checkBoxHex4->isChecked());
    hex.push_back(ui->checkBoxHex5->isChecked());
    hex.push_back(ui->checkBoxHex6->isChecked());
    hex.push_back(ui->checkBoxHex7->isChecked());
    hex.push_back(ui->checkBoxHex8->isChecked());
    hex.push_back(ui->checkBoxHex9->isChecked());
    hex.push_back(ui->checkBoxHex10->isChecked());

    return hex;
}

void Researcher::on_rngSelection_currentChanged(int index)
{
    ui->textBoxSeed->setVisible(index != 2);
    ui->label_14->setVisible(index != 2);
    ui->comboBoxSearch->clear();
    QStringList items = index != 1 ? QStringList() << tr("32Bit") << tr("16Bit High") << tr("16Bit Low") :
                        QStringList() << tr("64Bit") << tr("32Bit High") << tr("32Bit Low") << tr("16Bit High") << tr("16Bit Low") ;
    ui->comboBoxSearch->addItems(items);
}
