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

#ifndef CHECKLIST
#define CHECKLIST

#include <Core/Global.hpp>
#include <QComboBox>

class QListWidget;

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
     * @brief Add item to the combobox
     *
     * @param string Item that should be added to the combo box
     * @param data Data to assign to the item
     */
    void addItem(const QString &string, const QVariant &data = QVariant());

    /**
     * @brief Add item to the combobox
     *
     * @param string Item that should be added to the combo box
     * @param data Data to assign to the item
     */
    void addItem(const std::string &string, u16 data);

    /**
     * @brief Adds items to the combobox
     *
     * @param strings List of items that should be added to the combo box
     */
    void addItems(const std::vector<std::string> &strings);

    /**
     * @brief Adds items to the combobox
     *
     * @param strings List of items that should be added to the combo box
     * @param data List of data to assign with the items
     */
    void addItems(const std::vector<std::string> &strings, std::vector<u16> &data);

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
    template <size_t size>
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
     * @brief Hides popup window if we aren't clicked on the combobox
     */
    void hidePopup() override;

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
     * @brief Sets which of the check boxes are checked
     *
     * @tparam size Size of the array
     *
     * @param flags Array detailing which check boxes are to be checked and unchecked
     */
    template <size_t size>
    void setChecks(const std::array<bool, size> &flags)
    {
        std::vector<bool> checked(flags.begin(), flags.end());
        setChecks(checked);
    }

    /**
     * @brief Sets text for a combo box item
     *
     * @param index Item to update
     * @param text Text to update with
     */
    void setItemText(int index, const QString &text);

private:
    QLineEdit *lineEdit;
    QListWidget *listWidget;

    /**
     * @brief Determines the check state of the check boxes
     *
     * @return Checked if all check boxes are checked, PartiallyChecked if some of the check boxes are checked, and Unchecked otherwise
     */
    Qt::CheckState checkState() const;

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

    /**
     * @brief Unused event
     *
     * @param event Contains keypress event information
     */
    void keyPressEvent(QKeyEvent *event) override;

    /**
     * @brief Unused event
     *
     * @param event Contains wheel event information
     */
    void wheelEvent(QWheelEvent *event) override;

private slots:
    /**
     * @brief Updates the checked status of the check box that is selected
     *
     * @param index Index of the check box
     */
    void itemPressed(int index);

    /**
     * @brief Updates the text displayed of the combo box based upon which check boxes are checked
     */
    void modelDataChanged();
};

#endif // CHECKLIST
