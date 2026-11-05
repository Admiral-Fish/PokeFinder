/*
 * This file is part of PokéFinder
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "ShinyPatch.hpp"
#include "ui_ShinyPatch.h"
#include <Model/Gen4/ShinyPatchModel.hpp>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>
#include <algorithm>

ShinyPatch::ShinyPatch(QWidget *parent) : QWidget(parent), ui(new Ui::ShinyPatch), timer(new QTimer(this))
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);
    model = new ShinyPatchModel(ui->tableView);
    ui->tableView->setModel(model);
    ui->textBoxSeed->setValues(InputType::Seed32Bit);
    ui->textBoxStart->setValues(InputType::Advance32Bit);
    ui->textBoxEnd->setValues(InputType::Advance32Bit);

    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            auto *tile = new QPushButton(ui->groupBoxTerrain);
            tiles[y * 9 + x] = tile;
            tile->setFixedSize(27, 27);
            tile->setCheckable(true);
            tile->setToolTip(tr("X: %1, Y: %2").arg(x - 4).arg(y - 4));
            tile->setAccessibleName(tile->toolTip());
            tile->setObjectName(QString("tile_%1_%2").arg(x).arg(y));
            tile->setStyleSheet("QPushButton:checked { background-color: #548b38; color: white; }");
            if (x == 4 && y == 4)
            {
                tile->setText(tr("P"));
                tile->setEnabled(false);
            }
            ui->gridLayoutTerrain->addWidget(tile, y, x);
        }
    }
    setGrass(true);

    connect(ui->pushButtonAllGrass, &QPushButton::clicked, this, [this] { setGrass(true); });
    connect(ui->pushButtonNoGrass, &QPushButton::clicked, this, [this] { setGrass(false); });
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &ShinyPatch::generate);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this,
            [this] { finish(tr("Cancelled after %1 advances; %2 rows.").arg(nextAdvance - firstAdvance).arg(model->rowCount())); });
    connect(timer, &QTimer::timeout, this, &ShinyPatch::processBatch);

    QSettings settings;
    if (settings.contains("shinyPatch/geometry"))
    {
        restoreGeometry(settings.value("shinyPatch/geometry").toByteArray());
    }
}

ShinyPatch::~ShinyPatch()
{
    QSettings settings;
    settings.setValue("shinyPatch/geometry", saveGeometry());
    delete ui;
}

void ShinyPatch::setSeed(u32 seed, u32 maximumAdvances)
{
    ui->textBoxSeed->setText(QString::number(seed, 16).toUpper().rightJustified(8, '0'));
    ui->textBoxEnd->setText(QString::number(maximumAdvances));
}

void ShinyPatch::setGrass(bool enabled)
{
    for (size_t i = 0; i < tiles.size(); i++)
    {
        tiles[i]->setChecked(i != 40 && enabled);
    }
}

void ShinyPatch::generate()
{
    if (ui->textBoxSeed->text().isEmpty() || ui->textBoxStart->text().isEmpty() || ui->textBoxEnd->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Invalid input"), tr("Enter a seed and an advance range."));
        return;
    }
    firstAdvance = ui->textBoxStart->getUInt();
    lastAdvance = ui->textBoxEnd->getUInt();
    if (firstAdvance > lastAdvance)
    {
        QMessageBox::warning(this, tr("Invalid range"), tr("Start advances must not exceed end advances."));
        return;
    }
    for (size_t i = 0; i < tiles.size(); i++)
    {
        grass[i] = tiles[i]->isChecked();
    }
    if (std::none_of(grass.begin(), grass.end(), [](bool active) { return active; }))
    {
        QMessageBox::warning(this, tr("Invalid terrain"), tr("Select at least one eligible grass tile."));
        return;
    }

    chain = ui->spinBoxChain->value();
    captured = ui->comboBoxActivation->currentIndex() == 1;
    shinyOnly = ui->checkBoxShinyOnly->isChecked();
    rng = PokeRNG(ui->textBoxSeed->getUInt(), firstAdvance);
    nextAdvance = firstAdvance;
    model->clearModel();
    ui->groupBoxSettings->setEnabled(false);
    ui->groupBoxTerrain->setEnabled(false);
    ui->pushButtonGenerate->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);
    ui->progressBar->setValue(0);
    ui->labelStatus->setText(tr("Searching..."));
    timer->start(0);
}

void ShinyPatch::processBatch()
{
    // Bounded batches keep the window responsive and allow cancellation even for a full u32 range.
    constexpr int rowLimit = 100000;
    std::vector<ShinyPatchRow> rows;
    for (int i = 0; i < 512 && nextAdvance <= lastAdvance; i++, nextAdvance++, rng.next())
    {
        auto state = ShinyPatchCalculator::calculate(rng.getSeed(), chain, captured, grass);
        for (const auto &patch : state.patches)
        {
            if (!shinyOnly || patch.shiny)
            {
                rows.push_back({ static_cast<u32>(nextAdvance), rng.getSeed(), patch });
            }
        }
        if (model->rowCount() + rows.size() >= rowLimit)
        {
            rows.resize(rowLimit - model->rowCount());
            model->addItems(rows);
            finish(tr("Stopped at %1 rows. Narrow the advance range to see more results.").arg(rowLimit));
            return;
        }
    }
    model->addItems(rows);
    ui->progressBar->setValue(100 * (nextAdvance - firstAdvance) / (lastAdvance - firstAdvance + 1));
    if (nextAdvance > lastAdvance)
    {
        finish(tr("Completed %1 advances; %2 rows.").arg(lastAdvance - firstAdvance + 1).arg(model->rowCount()));
    }
}

void ShinyPatch::finish(const QString &message)
{
    timer->stop();
    ui->groupBoxSettings->setEnabled(true);
    ui->groupBoxTerrain->setEnabled(true);
    ui->pushButtonGenerate->setEnabled(true);
    ui->pushButtonCancel->setEnabled(false);
    ui->labelStatus->setText(message);
}
