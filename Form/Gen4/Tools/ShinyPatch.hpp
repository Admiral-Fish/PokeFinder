/*
 * This file is part of PokéFinder
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef SHINYPATCH_HPP
#define SHINYPATCH_HPP

#include <Core/Gen4/Tools/ShinyPatchCalculator.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <QWidget>

class QPushButton;
class QTimer;
class ShinyPatchModel;

namespace Ui
{
    class ShinyPatch;
}

/** @brief Finds D/P/Pt radar activations that produce shiny grass on a supplied terrain grid. */
class ShinyPatch final : public QWidget
{
    Q_OBJECT
public:
    explicit ShinyPatch(QWidget *parent = nullptr);
    ~ShinyPatch() override;

    /** @brief Prefills an initial seed from a wild search result. */
    void setSeed(u32 seed, u32 maximumAdvances);

private:
    Ui::ShinyPatch *ui;
    ShinyPatchModel *model;
    QTimer *timer;
    std::array<QPushButton *, 81> tiles {};
    ShinyPatchCalculator::Grass grass {};
    PokeRNG rng { 0 };
    u64 firstAdvance = 0;
    u64 nextAdvance = 0;
    u64 lastAdvance = 0;
    u16 chain = 0;
    bool captured = false;
    bool shinyOnly = true;

    void generate();
    void processBatch();
    void finish(const QString &message);
    void setGrass(bool enabled);
};

#endif // SHINYPATCH_HPP
