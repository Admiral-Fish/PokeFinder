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

#include "JirachiAdvancer.hpp"
#include "ui_JirachiAdvancer.h"
#include <QSettings>
#include <QStandardItemModel>

JirachiAdvancer::JirachiAdvancer(QWidget *parent) : QWidget(parent), ui(new Ui::JirachiAdvancer)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);
    ui->currentSeed->setValues(InputType::Seed32Bit);
    ui->targetSeed->setValues(InputType::Seed32Bit);
    ui->maxFrame->setValues(InputType::Advance32Bit);
    ui->bruteforceRange->setValues(InputType::Advance32Bit);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &JirachiAdvancer::run);
    connect(ui->checkBoxMin, &QAbstractButton::clicked, this, &JirachiAdvancer::minClicked);
}

JirachiAdvancer::~JirachiAdvancer()
{
    QSettings setting;
    setting.setValue("jirachiAdvancer/geometry", this->saveGeometry());

    delete ui;
}

void JirachiAdvancer::minClicked(bool toggled)
{
    ui->bruteforceRange->setEnabled(!toggled);
    ui->bruteforceLabel->setEnabled(!toggled);
}

void JirachiAdvancer::run()
{
    ui->listWidgetInfo->clear();
    u32 currentSeed = ui->currentSeed->getUInt();

    if (!validateMenu(currentSeed))
    {
        ui->listWidgetInfo->addItem("Current seed is invalid");
        return;
    }
    else
    {
        u32 targetSeed = ui->targetSeed->getUInt();
        if (!validateJirachi(targetSeed))
        {
            ui->listWidgetInfo->addItem("Target seed is invalid");
            return;
        }
        else
        {
            u32 maxFrame = ui->maxFrame->getULong();
            u32 targetFrame = calculateFrame(currentSeed,targetSeed);
            if (targetFrame > maxFrame)
            {
                ui->listWidgetInfo->addItem("Frame range is greater than max frame");
                return;
            }
            else
            {
                if (ui->checkBoxMin->isChecked())
                {
                    std::vector<u8> path;
                    u32 bruteForce = targetFrame;
                    while (path.empty())
                    {
                        ui->bruteforceRange->setText(QString::number(bruteForce));
                        path = calculateActions(currentSeed, targetFrame, bruteForce);
                        if (!path.empty())
                        {
                            int number = 1;
                            if (path[0] != 255)
                            {
                                for (u8 action : path)
                                {
                                    if (action == 0)
                                    {
                                        ui->listWidgetInfo->addItem(QString::number(number) + ": Reload Menu");
                                    }
                                    else if (action == 1)
                                    {
                                        ui->listWidgetInfo->addItem(QString::number(number) + ": Reject Jirachi");
                                    }
                                    else
                                    {
                                        ui->listWidgetInfo->addItem(QString::number(number) + ": Exit Special cutscene");
                                    }
                                    number++;
                                }
                            }
                            ui->listWidgetInfo->addItem(QString::number(number) + ": Accept Jirachi");
                        }
                        else
                        {
                            bruteForce--;
                            if (bruteForce <= 15)
                            {
                                ui->listWidgetInfo->addItem("Target seed is unreachable");
                                return;
                            }
                        }
                    }
                }
                else
                {
                    u32 bruteForce = ui->bruteforceRange->getUInt();
                    std::vector<u8> path = calculateActions(currentSeed, targetFrame, bruteForce);
                    if (path.empty())
                    {
                        ui->listWidgetInfo->addItem("Target seed is unreachable");
                    }
                    else
                    {
                        int number = 1;
                        if (path[0] != 255)
                        {
                            for (u8 action : path)
                            {
                                if (action == 0)
                                {
                                    ui->listWidgetInfo->addItem(QString::number(number) + ": Reload Menu");
                                }
                                else if (action == 1)
                                {
                                    ui->listWidgetInfo->addItem(QString::number(number) + ": Reject Jirachi");
                                }
                                else
                                {
                                    ui->listWidgetInfo->addItem(QString::number(number) + ": Exit Special cutscene");
                                }
                                number++;
                            }
                        }
                        ui->listWidgetInfo->addItem(QString::number(number) + ": Accept Jirachi");
                    }
                }

            }
        }
    }
}

// Working backwards this validates if a Jirachi seed is obtainable
// There are 3 different patterns for this (6/7/8 advances) plus menu checking
bool JirachiAdvancer::validateJirachi(u32 seed)
{
    XDRNGR rng(seed);

    uint16_t num1 = rng.nextUShort();
    uint16_t num2 = rng.nextUShort();
    uint16_t num3 = rng.nextUShort();

    rng.advance(3);
    if (num1 <= 0x4000) // 6 advances
    {
        if (validateMenu(rng.getSeed()))
        {
            return true;
        }
    }

    rng.advance(1);
    if (num2 > 0x4000 && num1 <= 0x547a) // 7 advances
    {
        if (validateMenu(rng.getSeed()))
        {
            return true;
        }
    }

    rng.advance(1);
    if (num3 > 0x4000 && num2 > 0x547a) // 8 advances
    {
        if (validateMenu(rng.getSeed()))
        {
            return true;
        }
    }

    return false;
}

// Working backwards from a seed check if the menu sequence will end on said seed
// Menu will advance the prng until it collects a 1, 2, and 3
bool JirachiAdvancer::validateMenu(u32 seed)
{
    u8 mask = 0;
    u8 target = seed >> 30;

    // Impossible to stop 0
    if (target == 0)
    {
        return false;
    }
    else
    {
        mask |= 1 << target;
    }

    auto rng = XDRNGR(seed);
    while ((mask & 14) != 14)
    {
        u8 num = rng.next() >> 30;

        // Basically this check means that while rolling for 1, 2, and 3
        // We hit our original target meaning that we can't land on the target
        if (num == target)
        {
            return false;
        }

        mask |= 1 << num;
    }

    return true;
}

// Advances prng of menu as described in validateMenu
void JirachiAdvancer::advanceMenu(XDRNG &rng, u32 &count)
{
    u8 mask = 0;
    while ((mask & 14) != 14)
    {
        u8 num = rng.next() >> 30;
        count++;

        mask |= 1 << num;
    }
}

// Advances prng of jirachi flying on screen
void JirachiAdvancer::advanceJirachi(XDRNG &rng, u32 &count)
{
    rng.advance(4);
    count += 4;

    bool flag = false;
    for (u16 thresh : { 0x4000, 0x547a })
    {
        count++;
        if (rng.nextUShort() <= thresh)
        {
            flag = true;
            break;
        }
    }

    rng.advance(flag ? 1 : 2);
    count += flag ? 1 : 2;
}

// Advances prng of special cutscene playing
void JirachiAdvancer::advanceCutscene(XDRNG &rng, u32 &count)
{
    rng.advance(1);
    count++;
}

// Advances prng of title screen reloading
void JirachiAdvancer::advanceTitleScreen(XDRNG &rng, u32 &count)
{
    rng.advance(1);
    count++;
}

// Determines how far apart two prng states are
u32 JirachiAdvancer::calculateFrame(u32 currentSeed, u32 targetSeed)
{
    XDRNG rng(currentSeed);
    u32 count = 0;

    while (rng.getSeed() != targetSeed)
    {
        rng.next();
        count++;
    }

    return count;
}

// Attempts to calculate an action plan to get to a target seed given a current seed
// For computational reasons current and target are only allowed to be 5000 frames apart
std::vector<u8> JirachiAdvancer::calculateActions(u32 currentSeed, u32 targetFrame, u32 bruteForce)
{
    // Not possible, fail early
    if (targetFrame < 6)
    {
        return {};
    }

    // Special handling for if we only need to accept
    // Jirachi can only advance 6, 7, or 8 frames so bound the check
    if (targetFrame >= 6 && targetFrame <= 8)
    {
        XDRNG rng(currentSeed);
        u32 count = 0;
        advanceJirachi(rng, count);

        if (count == targetFrame)
        {
            return { 255 };
        }
    }

    XDRNG menu(currentSeed);
    u32 menuFrame = 0;
    int menuCount = 0;

    // Use menu advances to get to a brute forcable range
    while (targetFrame - menuFrame > bruteForce)
    {
        menuCount++;
        advanceMenu(menu, menuFrame);
    }

    // Now that we are within our brute force range we brute force search
    // We have 3 ways to advance frames
    for (int i = 1;; i++)
    {
        // This variable handles checking if all of the possibilities of the current search size exceed the target seed
        // This is preferred to guessing what value of 'i' that is
        bool done = true;

        std::vector<u8> searchActions(static_cast<size_t>(i), 0);
        while (true)
        {
            u32 searchFrame = menuFrame;
            XDRNG rng(menu.getSeed());

            bool flag = false;
            for (u8 action : searchActions)
            {
                if (action == 0) // Reload menu
                {
                    advanceMenu(rng, searchFrame);
                }
                else if (action == 1) // Reject jirachi
                {
                    advanceJirachi(rng, searchFrame);
                    advanceTitleScreen(rng, searchFrame);
                    advanceMenu(rng, searchFrame);
                }
                else // Special cutscene
                {
                    advanceCutscene(rng, searchFrame);
                    advanceTitleScreen(rng, searchFrame);
                    advanceMenu(rng, searchFrame);
                }

                // Make sure didn't go over frame
                // Add 6 since that is the minimum an accept can advance
                if ((searchFrame + 6) >= targetFrame)
                {
                    flag = true;
                    break;
                }
            }

            // Verify we didn't go over target seed
            if (!flag)
            {
                done = false;

                // Advance the frames of accepting the jirachi
                advanceJirachi(rng, searchFrame);

                // If we land on target seed then return the actions to get to it
                if (searchFrame == targetFrame)
                {
                    // Vector is constructed in the way that the initial menu advances are already set
                    std::vector<u8> actions(static_cast<size_t>(menuCount) + searchActions.size(), 0);

                    // Copy over the search actions
                    std::copy(searchActions.begin(), searchActions.end(), actions.begin() + menuCount);

                    return actions;
                }
            }

            // Exit loop once all possibilities have been attempted
            if (std::count(searchActions.begin(), searchActions.end(), 2) == i)
            {
                break;
            }

            // This loop handles setting and iterating through the possible search actions
            for (size_t j = 0; j < searchActions.size(); j++)
            {
                if (searchActions.at(j) == 2)
                {
                    searchActions[j] = 0;

                    if (j != searchActions.size() - 1)
                    {
                        searchActions[j + 1]++;
                    }
                }
                else
                {
                    searchActions[j]++;
                }
            }
        }

        if (done)
        {
            break;
        }
    }

    // If we get to this point then it is extremely unlikely to get to the the target seed from the current seed
    return {};
}
