/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Forms/QCheckList.hpp>
#include <Forms/QTextBox.hpp>
#include <libPokeFinder/Gen3/Frame3.hpp>
#include <libPokeFinder/Gen3/Generator3.hpp>
#include <libPokeFinder/Gen3/Searcher3.hpp>
#include <libPokeFinder/Objects/FrameCompare.hpp>
#include <libPokeFinder/Objects/Nature.hpp>
#include <libPokeFinder/Objects/Power.hpp>
#include <vector>
#include <QDir>
#include <libPokeFinder/Gen3/Profile3.hpp>
#include <Forms/ProfileManager/ProfileManager3.hpp>
#include <Forms/Researcher.hpp>
#include <Models/Gen3/Stationary3Model.hpp>
#include <Models/Gen3/Wild3Model.hpp>
#include <Models/Gen3/Searcher3Model.hpp>
#include <thread>

using namespace std;
typedef uint32_t u32;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void UpdateView(Searcher3Model *model);

protected:
    void changeEvent(QEvent*);

private:
    Ui::MainWindow *ui;
    QTranslator m_translator;
    QTranslator m_translatorQt;
    QString m_currLang;
    QString m_langPath;
    Researcher *r;

    void LoadLanguage(const QString& rLanguage);
    void CreateLanguageMenu(void);
    void SetupModels();
    void CreateProfileXml();
    void Search3(Searcher3Model *model);

private slots:
    void on_generateStationary3_clicked();
    void SlotLanguageChanged(QAction* action);
    void UpdateProfiles();
    void on_saveProfileStationary3_clicked();
    void on_comboBoxProfiles_currentIndexChanged(int index);
    void on_anyNatureStationary3_clicked();
    void on_anyHiddenPowerStationary3_clicked();
    void on_checkBoxDelayStationary3_clicked();
    void on_actionResearcher_triggered();
    void on_generateWild3_clicked();
    void on_anyHiddenPowerWild3_clicked();
    void on_anyNatureWild3_clicked();
    void on_saveWild3_clicked();
    void on_saveSearcher3_clicked();
    void on_generateSearcher3_clicked();
    void on_anyNatureSearcher3_clicked();
    void on_anyHiddenPowerSearcher3_clicked();
    void UpdateViewSearcher(Searcher3Model *model);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    vector<Profile3> profiles;

};

#endif // MAINWINDOW_H
