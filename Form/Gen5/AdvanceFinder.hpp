/*
 * This file is part of PokeFinder
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

#ifndef ADVANCEFINDER_HPP
#define ADVANCEFINDER_HPP

#include <Core/Global.hpp>
#include <QDialog>
#include <QVector>
#include <functional>
#include <vector>

class EggGeneratorModel5;
class EggGeneratorModel4;
class EventGeneratorModel5;
class EventGeneratorModel4;
class HiddenGrottoGeneratorModel5;
class HiddenGrottoSlotGeneratorModel5;
class QLabel;
class QGroupBox;
class QLineEdit;
class QPushButton;
class QAbstractItemModel;
class QRadioButton;
class QString;
class StaticGeneratorModel4;
class StaticGeneratorModel5;
class QTableView;
class QTableWidget;
class WildGeneratorModel4;
class WildGeneratorModel5;

/**
 * @brief Provides a dialog to find advances based on Chatot pitch or save needle sequences
 */
class AdvanceFinder : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new AdvanceFinder object
     *
     * @param generatorModel The wild generator model to search through
     * @param parent Parent widget, which takes memory ownership
     */
    AdvanceFinder(WildGeneratorModel5 *generatorModel, QWidget *parent = nullptr);
    AdvanceFinder(WildGeneratorModel5 *generatorModel, QTableView *sourceTableView, QWidget *parent = nullptr);
    AdvanceFinder(WildGeneratorModel4 *generatorModel, QWidget *parent = nullptr);
    AdvanceFinder(WildGeneratorModel4 *generatorModel, QTableView *sourceTableView, QWidget *parent = nullptr);
    AdvanceFinder(StaticGeneratorModel5 *generatorModel, QWidget *parent = nullptr);
    AdvanceFinder(StaticGeneratorModel5 *generatorModel, QTableView *sourceTableView, QWidget *parent = nullptr);
    AdvanceFinder(StaticGeneratorModel4 *generatorModel, QWidget *parent = nullptr);
    AdvanceFinder(StaticGeneratorModel4 *generatorModel, QTableView *sourceTableView, QWidget *parent = nullptr);
    AdvanceFinder(EventGeneratorModel5 *generatorModel, QWidget *parent = nullptr);
    AdvanceFinder(EventGeneratorModel5 *generatorModel, QTableView *sourceTableView, QWidget *parent = nullptr);
    AdvanceFinder(EventGeneratorModel4 *generatorModel, QWidget *parent = nullptr);
    AdvanceFinder(EventGeneratorModel4 *generatorModel, QTableView *sourceTableView, QWidget *parent = nullptr);
    AdvanceFinder(EggGeneratorModel5 *generatorModel, QWidget *parent = nullptr);
    AdvanceFinder(EggGeneratorModel5 *generatorModel, QTableView *sourceTableView, QWidget *parent = nullptr);
    AdvanceFinder(EggGeneratorModel4 *generatorModel, QWidget *parent = nullptr);
    AdvanceFinder(EggGeneratorModel4 *generatorModel, QTableView *sourceTableView, QWidget *parent = nullptr);
    AdvanceFinder(HiddenGrottoSlotGeneratorModel5 *generatorModel, QWidget *parent = nullptr);
    AdvanceFinder(HiddenGrottoSlotGeneratorModel5 *generatorModel, QTableView *sourceTableView, QWidget *parent = nullptr);
    AdvanceFinder(HiddenGrottoGeneratorModel5 *generatorModel, QWidget *parent = nullptr);
    AdvanceFinder(HiddenGrottoGeneratorModel5 *generatorModel, QTableView *sourceTableView, QWidget *parent = nullptr);

    /**
     * @brief Destroy the AdvanceFinder object
     */
    ~AdvanceFinder() override;

private:
    using RowCountGetter = std::function<size_t()>;
    using ValueGetter = std::function<u8(size_t)>;

    QAbstractItemModel *generatorModel;
    QTableView *sourceTableView;
    RowCountGetter rowCountGetter;
    ValueGetter chatotGetter;
    ValueGetter needleGetter;
    QRadioButton *radioButtonChatot;
    QRadioButton *radioButtonNeedle;
    QGroupBox *groupBoxChatot;
    QGroupBox *groupBoxNeedle;
    QLineEdit *lineEditSequence;
    QLabel *labelPossibleResults;
    QPushButton *buttonJump;
    QTableWidget *tableWidgetResults;
    QString chatotSequence;
    QString needleSequence;
    bool switchingMode;
    std::vector<int> previewRows;

    AdvanceFinder(QAbstractItemModel *generatorModel, RowCountGetter rowCountGetter, ValueGetter chatotGetter, ValueGetter needleGetter,
                  QTableView *sourceTableView, QWidget *parent);

    /**
     * @brief Adds a token to the search sequence
     *
     * @param token Token text to append
     */
    void appendToken(const QString &token);

    /**
     * @brief Removes the last token from the search sequence
     */
    void removeToken();

    /**
     * @brief Stores current sequence text for the active mode
     *
     * @param text Current sequence text
     */
    void sequenceTextChanged(const QString &text);

    /**
     * @brief Selects and scrolls to the currently previewed source advance
     */
    void jumpToAdvance();

    /**
     * @brief Searches for advances matching the current sequence
     */
    void search();

    /**
     * @brief Parses the current text as Chatot pitch tokens
     *
     * @param valid Whether every token parsed successfully
     *
     * @return Sequence of valid values for each token
     */
    std::vector<QVector<u8>> getChatotSequence(bool &valid) const;

    /**
     * @brief Parses the current text as save needle tokens
     *
     * @param valid Whether every token parsed successfully
     *
     * @return Sequence of valid values for each token
     */
    std::vector<QVector<u8>> getNeedleSequence(bool &valid) const;

    /**
     * @brief Updates the search result table
     *
     * @param matches Matching starting rows from the generator model
     * @param sequenceSize Number of advances in the matched sequence
     */
    void displayResults(const std::vector<size_t> &matches, size_t sequenceSize);

private slots:
    /**
     * @brief Handles search mode toggle
     */
    void onModeChanged();
};

#endif // ADVANCEFINDER_HPP
