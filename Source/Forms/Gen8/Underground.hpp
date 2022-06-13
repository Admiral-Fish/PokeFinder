#ifndef UNDERGROUND_H
#define UNDERGROUND_H

#include <QWidget>

class EncounterArea8;
class Profile8;
class UndergroundModel;
class QMenu;

namespace Ui
{
    class Underground;
}

class Underground : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    explicit Underground(QWidget *parent = nullptr);
    ~Underground() override;
    void updateProfiles();
    bool hasProfiles() const;

private:
    Ui::Underground *ui;
    std::vector<Profile8> profiles;
    Profile8 *currentProfile;
    UndergroundModel *model;
    QMenu *menu;
    std::vector<EncounterArea8> encounters;

    void setupModels();
    void updateLocations();
    void updatePokemon();

private slots:
    void generate();
    void profilesIndexChanged(int index);
    void locationIndexChanged(int index);
    void tableViewContextMenu(QPoint pos);
    void profileManager();
};

#endif // UNDERGROUND_H
