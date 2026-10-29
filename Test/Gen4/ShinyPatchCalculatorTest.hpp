/*
 * This file is part of PokéFinder
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef SHINYPATCHCALCULATORTEST_HPP
#define SHINYPATCHCALCULATORTEST_HPP

#include <QObject>

class ShinyPatchCalculatorTest : public QObject
{
    Q_OBJECT
private slots:
    void rejected312();
    void candidate466();
    void innerRing();
    void captureBonus();
    void zeroChain();
    void inactivePatches();
    void shinyThreshold_data();
    void shinyThreshold();
    void ringPositions();
};

#endif // SHINYPATCHCALCULATORTEST_HPP
