/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef CHECKLIST
#define CHECKLIST

#include <Core/Global.hpp>
#include <QComboBox>

class QStandardItemModel;

/**
 * @brief Provides a combo box of check boxes
 */
class CheckList : public QComboBox
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new CheckList object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    CheckList(QWidget *parent = nullptr);

    /**
     * @brief Determines which of the check boxes are checked
     *
     * @return Vector of true/false to signify which check boxes are checked
     */
    std::vector<bool> getChecked() const;

    /**
     * @brief Determines which of the check boxes are checked
     *
     * @tparam size Size of the array
     *
     * @return Array of true/false to signify which check boxes are checked
     */
    template <int size>
    std::array<bool, size> getCheckedArray() const
    {
        auto checked = getChecked();
        std::array<bool, size> array;
        std::copy(checked.begin(), checked.end(), array.begin());
        return array;
    }

    /**
     * @brief Gets the data of the checked items
     *
     * @return Vector of checked data
     */
    std::vector<u16> getCheckedData() const;

    /**
     * @brief Sets all check boxes to be unchecked
     */
    void resetChecks();

    /**
     * @brief Sets which of the check boxes are checked
     *
     * @param flags Vector detailing which check boxes are to be checked and unchecked
     */
    void setChecks(const std::vector<bool> &flags);

    /**
     * @brief Setups the model of check boxes to be checkable by the user
     *
     * @param items List of items that should be added to the combo box
     */
    void setup(const std::vector<std::string> &items = std::vector<std::string>());

    /**
     * @brief Setups the model of check boxes to be checkable by the user
     *
     * @param items List of items that should be added to the combo box
     * @param data List of data to assign with the items
     */
    void setup(const std::vector<std::string> &items, const std::vector<u16> &data);

protected:
    /**
     * @brief Shows the combo box model when clicked
     *
     * @param object Object that is part of triggered event
     * @param event Contains information about the triggered event
     *
     * @return true Model should be shown
     * @return false Model should not be shown
     */
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    QStandardItemModel *model;

    /**
     * @brief Determines the check state of the check boxes
     *
     * @return Checked if all check boxes are checked, PartiallyChecked if some of the check boxes are checked, and Unchecked otherwise
     */
    Qt::CheckState checkState() const;

private slots:
    /**
     * @brief Updates the text displayed of the combo box based upon which check boxes are checked
     */
    void modelDataChanged();

    /**
     * @brief Updates the checked status of the check box that is selected
     *
     * @param index Index of the check box
     */
    void itemPressed(const QModelIndex &index);
};

#endif // CHECKLIST
