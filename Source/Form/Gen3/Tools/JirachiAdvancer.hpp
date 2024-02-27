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

#ifndef JIRACHIADVANCER_HPP
#define JIRACHIADVANCER_HPP

#include <QDialog>

namespace Ui
{
    class JirachiAdvancer;
}

/**
 * @brief Calculates possible advancement path to get target Jirachi
 */
class JirachiAdvancer : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new JirachiAdvancer object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    JirachiAdvancer(QWidget *parent = nullptr);

    /**
     * @brief Destroy the JirachiAdvancer object
     */
    ~JirachiAdvancer();

private:
    Ui::JirachiAdvancer *ui;

private slots:
    /**
     * @brief Computes advancement path to get target Jirachi
     */
    void generate();
};

#endif // JIRACHIADVANCER_HPP
