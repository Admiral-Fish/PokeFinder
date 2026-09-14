/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "EggSettings.hpp"
#include "ui_EggSettings.h"
#include <Core/Enum/Game.hpp>
#include <Core/Parents/Daycare.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <QAction>
#include <QClipboard>
#include <QContextMenuEvent>
#include <QMenu>
#include <QMessageBox>
#include <QRegularExpression>
#include <algorithm>
#include <vector>

constexpr u16 allowed[]
    = { 1,   4,   7,   10,  13,  16,  19,  21,  23,  27,  29,  32,  37,  41,  43,  46,  48,  50,  52,  54,  56,  58,  60,  63,  66,  69,
        72,  74,  77,  79,  81,  83,  84,  86,  88,  90,  92,  95,  96,  98,  100, 102, 104, 108, 109, 111, 113, 114, 115, 116, 118, 120,
        122, 123, 127, 128, 129, 131, 133, 137, 138, 140, 142, 143, 147, 152, 155, 158, 161, 163, 165, 167, 170, 172, 173, 174, 175, 177,
        179, 183, 185, 187, 190, 191, 193, 194, 198, 200, 202, 203, 204, 206, 207, 209, 211, 213, 214, 215, 216, 218, 220, 222, 223, 225,
        226, 227, 228, 231, 234, 235, 236, 238, 239, 240, 241, 246, 252, 255, 258, 261, 263, 265, 270, 273, 276, 278, 280, 283, 285, 287,
        290, 292, 293, 296, 298, 299, 300, 302, 303, 304, 307, 309, 311, 312, 313, 314, 315, 316, 318, 320, 322, 324, 325, 327, 328, 331,
        333, 335, 336, 337, 338, 339, 341, 343, 345, 347, 349, 351, 352, 353, 355, 357, 358, 359, 360, 361, 363, 366, 369, 370, 371, 374,
        387, 390, 393, 396, 399, 401, 403, 406, 408, 410, 412, 415, 417, 418, 420, 422, 425, 427, 431, 433, 434, 436, 438, 439, 440, 441,
        442, 443, 446, 447, 449, 451, 453, 455, 456, 458, 459, 479, 489, 495, 498, 501, 504, 506, 509, 511, 513, 515, 517, 519, 522, 524,
        527, 529, 531, 532, 535, 538, 539, 540, 543, 546, 548, 550, 551, 554, 556, 557, 559, 561, 562, 564, 566, 568, 570, 572, 574, 577,
        580, 582, 585, 587, 588, 590, 592, 594, 595, 597, 599, 602, 605, 607, 610, 613, 615, 616, 618, 619, 621, 622, 624, 626, 627, 629,
        631, 632, 633, 636, 650, 653, 656, 659, 661, 664, 667, 669, 672, 674, 676, 677, 679, 682, 684, 686, 688, 690, 692, 694, 696, 698,
        701, 702, 703, 704, 707, 708, 710, 712, 714, 722, 725, 728, 731, 734, 736, 739, 741, 742, 744, 746, 747, 749, 751, 753, 755, 757,
        759, 761, 764, 765, 766, 767, 769, 771, 774, 775, 776, 777, 778, 779, 780, 781, 782 };

constexpr u16 noEggsDiscovered5[]
    = { 30,  31,  144, 145, 146, 150, 151, 172, 173, 174, 175, 201, 236, 238, 239, 240, 243, 244, 245, 249, 250, 251,
        298, 360, 377, 378, 379, 380, 381, 382, 383, 384, 385, 386, 406, 433, 438, 439, 440, 446, 447, 458, 480, 481,
        482, 483, 484, 485, 486, 487, 488, 489, 490, 491, 492, 493, 494, 638, 639, 640, 641, 642, 643, 644, 645, 646,
        647, 648, 649 };

// Parent species to base egg species, sourced from BW2's pms.narc.
constexpr std::array<u16, 650> hatchSpecies5
    = { 0, 1, 1, 1, 4, 4, 4, 7, 7, 7, 10, 10, 10, 13, 13, 13, 16, 16, 16, 19, 19, 21, 21, 23, 23,
        172, 172, 27, 27, 29, 29, 29, 32, 32, 32, 173, 173, 37, 37, 174, 174, 41, 41, 43, 43, 43, 46, 46, 48, 48,
        50, 50, 52, 52, 54, 54, 56, 56, 58, 58, 60, 60, 60, 63, 63, 63, 66, 66, 66, 69, 69, 69, 72, 72, 74,
        74, 74, 77, 77, 79, 79, 81, 81, 83, 84, 84, 86, 86, 88, 88, 90, 90, 92, 92, 92, 95, 96, 96, 98, 98,
        100, 100, 102, 102, 104, 104, 236, 236, 108, 109, 109, 111, 111, 440, 114, 115, 116, 116, 118, 118, 120, 120, 439, 123,
        238, 239, 240, 127, 128, 129, 129, 131, 132, 133, 133, 133, 133, 137, 138, 138, 140, 140, 142, 446, 144, 145, 146, 147,
        147, 147, 150, 151, 152, 152, 152, 155, 155, 155, 158, 158, 158, 161, 161, 163, 163, 165, 165, 167, 167, 41, 170, 170,
        172, 173, 174, 175, 175, 177, 177, 179, 179, 179, 43, 298, 298, 438, 60, 187, 187, 187, 190, 191, 191, 193, 194, 194,
        133, 133, 198, 79, 200, 201, 360, 203, 204, 204, 206, 207, 95, 209, 209, 211, 123, 213, 214, 215, 216, 216, 218, 218,
        220, 220, 222, 223, 223, 225, 458, 227, 228, 228, 116, 231, 231, 137, 234, 235, 236, 236, 238, 239, 240, 241, 440, 243,
        244, 245, 246, 246, 246, 249, 250, 251, 252, 252, 252, 255, 255, 255, 258, 258, 258, 261, 261, 263, 263, 265, 265, 265,
        265, 265, 270, 270, 270, 273, 273, 273, 276, 276, 278, 278, 280, 280, 280, 283, 283, 285, 285, 287, 287, 287, 290, 290,
        290, 293, 293, 293, 296, 296, 298, 299, 300, 300, 302, 303, 304, 304, 304, 307, 307, 309, 309, 311, 312, 313, 314, 406,
        316, 316, 318, 318, 320, 320, 322, 322, 324, 325, 325, 327, 328, 328, 328, 331, 331, 333, 333, 335, 336, 337, 338, 339,
        339, 341, 341, 343, 343, 345, 345, 347, 347, 349, 349, 351, 352, 353, 353, 355, 355, 357, 433, 359, 360, 361, 361, 363,
        363, 363, 366, 366, 366, 369, 370, 371, 371, 371, 374, 374, 374, 377, 378, 379, 380, 381, 382, 383, 384, 385, 386, 387,
        387, 387, 390, 390, 390, 393, 393, 393, 396, 396, 396, 399, 399, 401, 401, 403, 403, 403, 406, 406, 408, 408, 410, 410,
        412, 412, 412, 415, 415, 417, 418, 418, 420, 420, 422, 422, 190, 425, 425, 427, 427, 200, 198, 431, 431, 433, 434, 434,
        436, 436, 438, 439, 440, 441, 442, 443, 443, 443, 446, 447, 447, 449, 449, 451, 451, 453, 453, 455, 456, 456, 458, 459,
        459, 215, 81, 108, 111, 114, 239, 240, 175, 193, 133, 133, 207, 220, 137, 280, 299, 355, 361, 479, 480, 481, 482, 483,
        484, 485, 486, 487, 488, 489, 490, 491, 492, 493, 494, 495, 495, 495, 498, 498, 498, 501, 501, 501, 504, 504, 506, 506,
        506, 509, 509, 511, 511, 513, 513, 515, 515, 517, 517, 519, 519, 519, 522, 522, 524, 524, 524, 527, 527, 529, 529, 531,
        532, 532, 532, 535, 535, 535, 538, 539, 540, 540, 540, 543, 543, 543, 546, 546, 548, 548, 550, 551, 551, 551, 554, 554,
        556, 557, 557, 559, 559, 561, 562, 562, 564, 564, 566, 566, 568, 568, 570, 570, 572, 572, 574, 574, 574, 577, 577, 577,
        580, 580, 582, 582, 582, 585, 585, 587, 588, 588, 590, 590, 592, 592, 594, 595, 595, 597, 597, 599, 599, 599, 602, 602,
        602, 605, 605, 607, 607, 607, 610, 610, 610, 613, 613, 615, 616, 616, 618, 619, 619, 621, 622, 622, 624, 624, 626, 627,
        627, 629, 629, 631, 632, 633, 633, 633, 636, 636, 638, 639, 640, 641, 642, 643, 644, 645, 646, 647, 648, 649 };

EggSettings::EggSettings(QWidget *parent) : QWidget(parent), ui(new Ui::EggSettings)
{
    ui->setupUi(this);

    ui->comboBoxParentANature->addItems(Translator::getNatures());
    ui->comboBoxParentBNature->addItems(Translator::getNatures());

    for (const std::string &gender : Translator::getGenders())
    {
        ui->comboBoxParentAGender->addItem(QString::fromStdString(gender));
        ui->comboBoxParentBGender->addItem(QString::fromStdString(gender));
    }
    ui->comboBoxParentAGender->addItem(QString::fromStdString(Translator::getSpecie(132)));
    ui->comboBoxParentBGender->addItem(QString::fromStdString(Translator::getSpecie(132)));

    ui->comboBoxParentAAbility->addItem("0");
    ui->comboBoxParentAAbility->addItem("1");

    ui->comboBoxParentBAbility->addItem("0");
    ui->comboBoxParentBAbility->addItem("1");

    ui->comboBoxParentAItem->addItem(tr("None"), 0);
    ui->comboBoxParentAItem->addItem(tr("Everstone"), 1);

    ui->comboBoxParentBItem->addItem(tr("None"), 0);
    ui->comboBoxParentBItem->addItem(tr("Everstone"), 1);

    ui->comboBoxParentASpecie->enableAutoComplete();
    ui->comboBoxParentBSpecie->enableAutoComplete();

    auto *copyAction = addAction(tr("Copy IVs to clipboard"));
    auto *pasteAction = addAction(tr("Paste IVs from clipboard"));

    connect(copyAction, &QAction::triggered, this, &EggSettings::setIVsToClipBoard);
    connect(pasteAction, &QAction::triggered, this, &EggSettings::setIVsFromClipBoard);

    connect(ui->checkBoxShowInheritance, &QCheckBox::toggled, this, &EggSettings::showInheritanceChanged);
}

EggSettings::~EggSettings()
{
    delete ui;
}

void EggSettings::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu::exec(actions(), event->globalPos(), nullptr, this);
}

bool EggSettings::isValid(bool hiddenAbility) const
{
    // Gender compatability
    u8 parent1 = ui->comboBoxParentAGender->currentIndex();
    u8 parent2 = ui->comboBoxParentBGender->currentIndex();
    if ((game & Game::Gen5) != Game::None)
    {
        parent1 = ui->comboBoxParentASpecie->getCurrentUShort() == 132 ? 3 : parent1;
        parent2 = ui->comboBoxParentBSpecie->getCurrentUShort() == 132 ? 3 : parent2;
    }
    if (!((parent1 == 0 && parent2 == 1) || (parent1 == 1 && parent2 == 0) || // Male/Female
          (parent1 == 3 && parent2 == 1) || (parent1 == 1 && parent2 == 3) || // Ditto/Female
          (parent1 == 0 && parent2 == 3) || (parent1 == 3 && parent2 == 0) || // Male/Ditto
          (parent1 == 2 && parent2 == 3) || (parent1 == 3 && parent2 == 2))) // Genderless/Ditto
    {
        QMessageBox box(QMessageBox::Warning, tr("Incompatible Parents"), tr("Gender of selected parents are not compatible for breeding"));
        box.exec();
        return false;
    }

    // Hidden ability compatability
    if (hiddenAbility)
    {
        u8 ability1 = ui->comboBoxParentAAbility->currentIndex();
        u8 ability2 = ui->comboBoxParentBAbility->currentIndex();

        bool hiddenAbilityCompatible = (parent1 == 0 && parent2 == 1 && ability2 == 2) || (parent1 == 1 && ability1 == 2 && parent2 == 0);
        if ((game & Game::Gen8) != Game::None) 
        {
            hiddenAbilityCompatible |= (parent1 == 3 && ability2 == 2) || (ability1 == 2 && parent2 == 3);
        }

        if (!hiddenAbilityCompatible)
        {
            QMessageBox box(QMessageBox::Warning, tr("Incompatible Parents"), tr("Parents incompatible for breeding Hidden Ability!"));
            box.exec();
            return false;
        }
    }

    return true;
}

void EggSettings::copyFrom(const EggSettings *other)
{
    ui->spinBoxParentAHP->setValue(other->ui->spinBoxParentAHP->value());
    ui->spinBoxParentAAtk->setValue(other->ui->spinBoxParentAAtk->value());
    ui->spinBoxParentADef->setValue(other->ui->spinBoxParentADef->value());
    ui->spinBoxParentASpA->setValue(other->ui->spinBoxParentASpA->value());
    ui->spinBoxParentASpD->setValue(other->ui->spinBoxParentASpD->value());
    ui->spinBoxParentASpe->setValue(other->ui->spinBoxParentASpe->value());

    ui->spinBoxParentBHP->setValue(other->ui->spinBoxParentBHP->value());
    ui->spinBoxParentBAtk->setValue(other->ui->spinBoxParentBAtk->value());
    ui->spinBoxParentBDef->setValue(other->ui->spinBoxParentBDef->value());
    ui->spinBoxParentBSpA->setValue(other->ui->spinBoxParentBSpA->value());
    ui->spinBoxParentBSpD->setValue(other->ui->spinBoxParentBSpD->value());
    ui->spinBoxParentBSpe->setValue(other->ui->spinBoxParentBSpe->value());

    ui->comboBoxParentAAbility->setCurrentIndex(other->ui->comboBoxParentAAbility->currentIndex());
    ui->comboBoxParentBAbility->setCurrentIndex(other->ui->comboBoxParentBAbility->currentIndex());

    ui->comboBoxParentAGender->setCurrentIndex(other->ui->comboBoxParentAGender->currentIndex());
    ui->comboBoxParentBGender->setCurrentIndex(other->ui->comboBoxParentBGender->currentIndex());

    ui->comboBoxParentAItem->setCurrentIndex(other->ui->comboBoxParentAItem->currentIndex());
    ui->comboBoxParentBItem->setCurrentIndex(other->ui->comboBoxParentBItem->currentIndex());

    ui->comboBoxParentANature->setCurrentIndex(other->ui->comboBoxParentANature->currentIndex());
    ui->comboBoxParentBNature->setCurrentIndex(other->ui->comboBoxParentBNature->currentIndex());

    ui->comboBoxParentASpecie->setCurrentIndex(other->ui->comboBoxParentASpecie->currentIndex());
    ui->comboBoxParentBSpecie->setCurrentIndex(other->ui->comboBoxParentBSpecie->currentIndex());
    ui->comboBoxOriginalTrainer->setCurrentIndex(other->ui->comboBoxOriginalTrainer->currentIndex());

    ui->checkBoxMasuda->setCheckState(other->ui->checkBoxMasuda->checkState());
    ui->checkBoxShowInheritance->setCheckState(other->ui->checkBoxShowInheritance->checkState());
}

Daycare EggSettings::getDaycare() const
{
    std::array<std::array<u8, 6>, 2> parentIVs
        = { { { static_cast<u8>(ui->spinBoxParentAHP->value()), static_cast<u8>(ui->spinBoxParentAAtk->value()),
                static_cast<u8>(ui->spinBoxParentADef->value()), static_cast<u8>(ui->spinBoxParentASpA->value()),
                static_cast<u8>(ui->spinBoxParentASpD->value()), static_cast<u8>(ui->spinBoxParentASpe->value()) },
              { static_cast<u8>(ui->spinBoxParentBHP->value()), static_cast<u8>(ui->spinBoxParentBAtk->value()),
                static_cast<u8>(ui->spinBoxParentBDef->value()), static_cast<u8>(ui->spinBoxParentBSpA->value()),
                static_cast<u8>(ui->spinBoxParentBSpD->value()), static_cast<u8>(ui->spinBoxParentBSpe->value()) } } };

    std::array<u8, 2> parentAbility
        = { static_cast<u8>(ui->comboBoxParentAAbility->currentIndex()), static_cast<u8>(ui->comboBoxParentBAbility->currentIndex()) };

    std::array<u8, 2> parentGender
        = { static_cast<u8>(ui->comboBoxParentAGender->currentIndex()), static_cast<u8>(ui->comboBoxParentBGender->currentIndex()) };
    if ((game & Game::Gen5) != Game::None)
    {
        parentGender[0] = ui->comboBoxParentASpecie->getCurrentUShort() == 132 ? 3 : parentGender[0];
        parentGender[1] = ui->comboBoxParentBSpecie->getCurrentUShort() == 132 ? 3 : parentGender[1];
    }

    std::array<u8, 2> parentItem = { ui->comboBoxParentAItem->getCurrentUChar(), ui->comboBoxParentBItem->getCurrentUChar() };

    std::array<u8, 2> parentNature
        = { static_cast<u8>(ui->comboBoxParentANature->currentIndex()), static_cast<u8>(ui->comboBoxParentBNature->currentIndex()) };

    std::array<u16, 2> parentSpecies
        = { ui->comboBoxParentASpecie->getCurrentUShort(), ui->comboBoxParentBSpecie->getCurrentUShort() };
    u16 parentSpecie = parentGender[1] == 3 ? parentSpecies[0] : parentSpecies[1];
    u16 specie = (game & Game::Gen5) != Game::None ? hatchSpecies5[parentSpecie] : parentSpecie;
    bool masuda = ui->checkBoxMasuda->isChecked();

    return Daycare(parentIVs, parentAbility, parentGender, parentItem, parentNature, specie, masuda, parentSpecies,
                   ui->comboBoxOriginalTrainer->currentIndex() == 1);
}

bool EggSettings::reorderParents()
{
    u8 parent1 = ui->comboBoxParentAGender->currentIndex();
    u8 parent2 = ui->comboBoxParentBGender->currentIndex();
    if ((game & Game::Gen5) != Game::None)
    {
        parent1 = ui->comboBoxParentASpecie->getCurrentUShort() == 132 ? 3 : parent1;
        parent2 = ui->comboBoxParentBSpecie->getCurrentUShort() == 132 ? 3 : parent2;
    }

    // Female/Male -> Male/Female
    bool flag = parent1 == 1 && parent2 == 0;

    // Female/Ditto -> Ditto/Female
    flag |= parent1 == 1 && parent2 == 3;

    // Ditto/Male -> Male/Ditto
    flag |= parent1 == 3 && parent2 == 0;

    // Ditto/Genderless -> Genderless/Ditto
    flag |= parent1 == 3 && parent2 == 2;

    if (flag)
    {
        Daycare daycare = getDaycare();

        ui->spinBoxParentAHP->setValue(daycare.getParentIV(1, 0));
        ui->spinBoxParentAAtk->setValue(daycare.getParentIV(1, 1));
        ui->spinBoxParentADef->setValue(daycare.getParentIV(1, 2));
        ui->spinBoxParentASpA->setValue(daycare.getParentIV(1, 3));
        ui->spinBoxParentASpD->setValue(daycare.getParentIV(1, 4));
        ui->spinBoxParentASpe->setValue(daycare.getParentIV(1, 5));
        ui->comboBoxParentAAbility->setCurrentIndex(daycare.getParentAbility(1));
        u8 parentAGender = daycare.getParentGender(1);
        ui->comboBoxParentAGender->setCurrentIndex(parentAGender == 3 ? 2 : parentAGender);
        ui->comboBoxParentAItem->setCurrentIndex(ui->comboBoxParentAItem->findData(daycare.getParentItem(1)));
        ui->comboBoxParentANature->setCurrentIndex(daycare.getParentNature(1));
        ui->comboBoxParentASpecie->setCurrentIndex(ui->comboBoxParentASpecie->findData(daycare.getParentSpecie(1)));

        ui->spinBoxParentBHP->setValue(daycare.getParentIV(0, 0));
        ui->spinBoxParentBAtk->setValue(daycare.getParentIV(0, 1));
        ui->spinBoxParentBDef->setValue(daycare.getParentIV(0, 2));
        ui->spinBoxParentBSpA->setValue(daycare.getParentIV(0, 3));
        ui->spinBoxParentBSpD->setValue(daycare.getParentIV(0, 4));
        ui->spinBoxParentBSpe->setValue(daycare.getParentIV(0, 5));
        ui->comboBoxParentBAbility->setCurrentIndex(daycare.getParentAbility(0));
        u8 parentBGender = daycare.getParentGender(0);
        ui->comboBoxParentBGender->setCurrentIndex(parentBGender == 3 ? 2 : parentBGender);
        ui->comboBoxParentBItem->setCurrentIndex(ui->comboBoxParentBItem->findData(daycare.getParentItem(0)));
        ui->comboBoxParentBNature->setCurrentIndex(daycare.getParentNature(0));
        ui->comboBoxParentBSpecie->setCurrentIndex(ui->comboBoxParentBSpecie->findData(daycare.getParentSpecie(0)));
    }

    return flag;
}

void EggSettings::setup(Game game)
{
    this->game = game;

    bool gen5 = (game & Game::Gen5) != Game::None;
    if (gen5)
    {
        ui->comboBoxParentAGender->removeItem(3);
        ui->comboBoxParentBGender->removeItem(3);
    }
    ui->labelEggSpecie->setText(gen5 ? tr("Species") : tr("Egg Species"));
    ui->comboBoxParentBSpecie->setVisible(gen5);
    ui->labelOriginalTrainer->setVisible(gen5);
    ui->comboBoxOriginalTrainer->setVisible(gen5);

    u16 max = 0;
    if ((game & Game::Gen3) != Game::None)
    {
        bool flag = (game & Game::Emerald) != Game::None;

        ui->labelAbility->hide();
        ui->labelItem->setVisible(flag);
        ui->labelNature->setVisible(flag);

        ui->comboBoxParentAAbility->hide();
        ui->comboBoxParentBAbility->hide();
        ui->comboBoxParentAItem->setVisible(flag);
        ui->comboBoxParentBItem->setVisible(flag);
        ui->comboBoxParentANature->setVisible(flag);
        ui->comboBoxParentBNature->setVisible(flag);

        max = 386;
        ui->checkBoxMasuda->hide();
    }
    else if ((game & Game::Gen4) != Game::None)
    {
        ui->labelAbility->hide();
        ui->labelItem->hide();
        ui->labelNature->hide();

        ui->comboBoxParentAAbility->hide();
        ui->comboBoxParentBAbility->hide();
        ui->comboBoxParentAItem->hide();
        ui->comboBoxParentBItem->hide();
        ui->comboBoxParentANature->hide();
        ui->comboBoxParentBNature->hide();

        max = 493;
    }
    else if ((game & Game::Gen5) != Game::None)
    {
        ui->comboBoxParentAItem->addItem(tr("Power Weight"), 2);
        ui->comboBoxParentAItem->addItem(tr("Power Bracer"), 3);
        ui->comboBoxParentAItem->addItem(tr("Power Belt"), 4);
        ui->comboBoxParentAItem->addItem(tr("Power Lens"), 5);
        ui->comboBoxParentAItem->addItem(tr("Power Band"), 6);
        ui->comboBoxParentAItem->addItem(tr("Power Anklet"), 7);

        ui->comboBoxParentBItem->addItem(tr("Power Weight"), 2);
        ui->comboBoxParentBItem->addItem(tr("Power Bracer"), 3);
        ui->comboBoxParentBItem->addItem(tr("Power Belt"), 4);
        ui->comboBoxParentBItem->addItem(tr("Power Lens"), 5);
        ui->comboBoxParentBItem->addItem(tr("Power Band"), 6);
        ui->comboBoxParentBItem->addItem(tr("Power Anklet"), 7);

        ui->comboBoxParentAAbility->addItem("H");
        ui->comboBoxParentBAbility->addItem("H");

        max = 649;
    }
    else if ((game & Game::BDSP) != Game::None)
    {
        ui->comboBoxParentAItem->addItem(tr("Destiny Knot"), 8);
        ui->comboBoxParentBItem->addItem(tr("Destiny Knot"), 8);

        ui->comboBoxParentAAbility->addItem("H");
        ui->comboBoxParentBAbility->addItem("H");

        max = 493;
    }

    ui->comboBoxParentASpecie->clear();
    ui->comboBoxParentBSpecie->clear();
    if (gen5)
    {
        const PersonalInfo *info = PersonalLoader::getPersonal(game);
        std::vector<std::pair<QString, u16>> species;
        for (u16 specie = 1; specie <= max; specie++)
        {
            if (info[specie].getPresent() && !std::ranges::binary_search(noEggsDiscovered5, specie))
            {
                species.emplace_back(QString::fromStdString(Translator::getSpecie(specie)), specie);
            }
        }
        std::ranges::sort(species, [](const auto &left, const auto &right) {
            return QString::localeAwareCompare(left.first, right.first) < 0;
        });
        for (const auto &[name, specie] : species)
        {
            ui->comboBoxParentASpecie->addItem(name, specie);
            ui->comboBoxParentBSpecie->addItem(name, specie);
        }
    }
    else
    {
        for (u16 specie : allowed)
        {
            if (specie > max)
            {
                break;
            }
            ui->comboBoxParentASpecie->addItem(QString::fromStdString(Translator::getSpecie(specie)), specie);
        }
    }
}

void EggSettings::setOriginalTrainerVisible(bool visible)
{
    ui->labelOriginalTrainer->setVisible(visible);
    ui->comboBoxOriginalTrainer->setVisible(visible);
}

void EggSettings::setIVsFromClipBoard()
{
    QRegularExpression re("(\\d{1,2})/(\\d{1,2})/(\\d{1,2})/(\\d{1,2})/(\\d{1,2})/(\\d{1,2})-(\\d{1,2})/(\\d{1,2})/(\\d{1,2})/(\\d{1,2})/"
                          "(\\d{1,2})/(\\d{1,2})");

    QString text = QApplication::clipboard()->text();
    QRegularExpressionMatch match = re.match(text);
    if (!match.hasMatch())
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid Format"), tr("The clipboard text did not match the expected format."));
        msg.exec();
        return;
    }

    ui->spinBoxParentAHP->setValue(match.captured(1).toInt());
    ui->spinBoxParentAAtk->setValue(match.captured(2).toInt());
    ui->spinBoxParentADef->setValue(match.captured(3).toInt());
    ui->spinBoxParentASpA->setValue(match.captured(4).toInt());
    ui->spinBoxParentASpD->setValue(match.captured(5).toInt());
    ui->spinBoxParentASpe->setValue(match.captured(6).toInt());

    ui->spinBoxParentBHP->setValue(match.captured(7).toInt());
    ui->spinBoxParentBAtk->setValue(match.captured(8).toInt());
    ui->spinBoxParentBDef->setValue(match.captured(9).toInt());
    ui->spinBoxParentBSpA->setValue(match.captured(10).toInt());
    ui->spinBoxParentBSpD->setValue(match.captured(11).toInt());
    ui->spinBoxParentBSpe->setValue(match.captured(12).toInt());
}

void EggSettings::setIVsToClipBoard()
{
    QString ivs = QString("%1/%2/%3/%4/%5/%6-%7/%8/%9/%10/%11/%12")
                      .arg(ui->spinBoxParentAHP->value())
                      .arg(ui->spinBoxParentAAtk->value())
                      .arg(ui->spinBoxParentADef->value())
                      .arg(ui->spinBoxParentASpA->value())
                      .arg(ui->spinBoxParentASpD->value())
                      .arg(ui->spinBoxParentASpe->value())
                      .arg(ui->spinBoxParentBHP->value())
                      .arg(ui->spinBoxParentBAtk->value())
                      .arg(ui->spinBoxParentBDef->value())
                      .arg(ui->spinBoxParentBSpA->value())
                      .arg(ui->spinBoxParentBSpD->value())
                      .arg(ui->spinBoxParentBSpe->value());

    QApplication::clipboard()->setText(ivs);
}
