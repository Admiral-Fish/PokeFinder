#ifndef WildPLA_HPP
#define WildPLA_HPP

#include <QWidget>

class Profile8;
class QMenu;
class RaidModel;

namespace Ui
{
    class WildPLA;
}

class WildPLA : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    explicit WildPLA(QWidget *parent = nullptr);
    ~WildPLA() override;
    void updateProfiles();
    bool hasProfiles() const;

private:
    Ui::WildPLA *ui;
    std::vector<Profile8> profiles;
    Profile8 *currentProfile;
    RaidModel *model;
    QMenu *menu;

    void setupModels();

private slots:
    void generate();
    void profilesIndexChanged(int index);
    void tableViewContextMenu(QPoint pos);
    void profileManager();
};

#endif // WildPLA_HPP
