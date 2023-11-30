/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Util/Translator.hpp>

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

EggSettings::EggSettings(QWidget *parent) : QWidget(parent), ui(new Ui::EggSettings)
{
    ui->setupUi(this);

    for (const std::string &nature : Translator::getNatures())
    {
        ui->comboBoxParentANature->addItem(QString::fromStdString(nature));
        ui->comboBoxParentBNature->addItem(QString::fromStdString(nature));
    }

    for (const std::string &gender : Translator::getGenders())
    {
        ui->comboBoxParentAGender->addItem(QString::fromStdString(gender));
        ui->comboBoxParentBGender->addItem(QString::fromStdString(gender));
    }
    ui->comboBoxParentAGender->addItem(QString::fromStdString(Translator::getSpecie(132)));
    ui->comboBoxParentBGender->addItem(QString::fromStdString(Translator::getSpecie(132)));

    ui->comboBoxParentAAbility->addItem("1");
    ui->comboBoxParentAAbility->addItem("2");

    ui->comboBoxParentBAbility->addItem("1");
    ui->comboBoxParentBAbility->addItem("2");

    ui->comboBoxParentAItem->addItem(tr("None"), 0);
    ui->comboBoxParentAItem->addItem(tr("Everstone"), 1);

    ui->comboBoxParentBItem->addItem(tr("None"), 0);
    ui->comboBoxParentBItem->addItem(tr("Everstone"), 1);

    ui->comboBoxEggSpecie->enableAutoComplete();

    connect(ui->checkBoxShowInheritance, &QCheckBox::stateChanged, this,
            [=](int state) { emit showInheritanceChanged(state == Qt::Checked); });
}

EggSettings::~EggSettings()
{
    delete ui;
}

bool EggSettings::compatibleParents() const
{
    u8 parent1 = ui->comboBoxParentAGender->currentIndex();
    u8 parent2 = ui->comboBoxParentBGender->currentIndex();

    // Male/Female
    if ((parent1 == 0 && parent2 == 1) || (parent1 == 1 && parent2 == 0))
    {
        return true;
    }

    // Ditto/Female
    if ((parent1 == 3 && parent2 == 1) || (parent1 == 1 && parent2 == 3))
    {
        return true;
    }

    // Male/Ditto
    if ((parent1 == 0 && parent2 == 3) || (parent1 == 3 && parent2 == 0))
    {
        return true;
    }

    // Genderless/Ditto
    if ((parent1 == 2 && parent2 == 3) || (parent1 == 3 && parent2 == 2))
    {
        return true;
    }

    return false;
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

    std::array<u8, 2> parentItem = { ui->comboBoxParentAItem->getCurrentUChar(), ui->comboBoxParentBItem->getCurrentUChar() };

    std::array<u8, 2> parentNature
        = { static_cast<u8>(ui->comboBoxParentANature->currentIndex()), static_cast<u8>(ui->comboBoxParentBNature->currentIndex()) };

    u16 specie = ui->comboBoxEggSpecie->getCurrentUShort();
    bool masuda = ui->checkBoxMasuda->isChecked();

    return Daycare(parentIVs, parentAbility, parentGender, parentItem, parentNature, specie, masuda);
}

bool EggSettings::reorderParents()
{
    u8 parent1 = ui->comboBoxParentAGender->currentIndex();
    u8 parent2 = ui->comboBoxParentBGender->currentIndex();

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
        ui->comboBoxParentAGender->setCurrentIndex(daycare.getParentGender(1));
        ui->comboBoxParentAItem->setCurrentIndex(ui->comboBoxParentAItem->findData(daycare.getParentItem(1)));
        ui->comboBoxParentANature->setCurrentIndex(daycare.getParentNature(1));

        ui->spinBoxParentBHP->setValue(daycare.getParentIV(0, 0));
        ui->spinBoxParentBAtk->setValue(daycare.getParentIV(0, 1));
        ui->spinBoxParentBDef->setValue(daycare.getParentIV(0, 2));
        ui->spinBoxParentBSpA->setValue(daycare.getParentIV(0, 3));
        ui->spinBoxParentBSpD->setValue(daycare.getParentIV(0, 4));
        ui->spinBoxParentBSpe->setValue(daycare.getParentIV(0, 5));
        ui->comboBoxParentBAbility->setCurrentIndex(daycare.getParentAbility(0));
        ui->comboBoxParentBGender->setCurrentIndex(daycare.getParentGender(0));
        ui->comboBoxParentBItem->setCurrentIndex(ui->comboBoxParentBItem->findData(daycare.getParentItem(0)));
        ui->comboBoxParentBNature->setCurrentIndex(daycare.getParentNature(0));
    }

    return flag;
}

void EggSettings::setup(Game game)
{
    u16 max = 0;
    if ((game & Game::Gen3) != Game::None)
    {
        bool flag = (game & Game::Emerald) != Game::None;

        ui->labelAbility->setVisible(false);
        ui->labelItem->setVisible(flag);
        ui->labelNature->setVisible(flag);

        ui->comboBoxParentAAbility->setVisible(false);
        ui->comboBoxParentBAbility->setVisible(false);
        ui->comboBoxParentAItem->setVisible(flag);
        ui->comboBoxParentBItem->setVisible(flag);
        ui->comboBoxParentANature->setVisible(flag);
        ui->comboBoxParentBNature->setVisible(flag);

        max = 386;
        ui->checkBoxMasuda->setVisible(false);
    }
    else if ((game & Game::Gen4) != Game::None)
    {
        ui->labelAbility->setVisible(false);
        ui->labelItem->setVisible(false);
        ui->labelNature->setVisible(false);

        ui->comboBoxParentAAbility->setVisible(false);
        ui->comboBoxParentBAbility->setVisible(false);
        ui->comboBoxParentAItem->setVisible(false);
        ui->comboBoxParentBItem->setVisible(false);
        ui->comboBoxParentANature->setVisible(false);
        ui->comboBoxParentBNature->setVisible(false);

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

    ui->comboBoxEggSpecie->clear();
    for (u16 i : allowed)
    {
        if (i > max)
        {
            break;
        }
        ui->comboBoxEggSpecie->addItem(QString::fromStdString(Translator::getSpecie(i)), i);
    }
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

    ui->comboBoxEggSpecie->setCurrentIndex(other->ui->comboBoxEggSpecie->currentIndex());

    ui->checkBoxMasuda->setCheckState(other->ui->checkBoxMasuda->checkState());
    ui->checkBoxShowInheritance->setCheckState(other->ui->checkBoxShowInheritance->checkState());
}
