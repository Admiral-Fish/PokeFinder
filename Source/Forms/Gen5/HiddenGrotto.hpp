#ifndef HIDDENGROTTO_HPP
#define HIDDENGROTTO_HPP

#include <Core/Gen5/Generators/HiddenGrottoGenerator.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <QMenu>

class HiddenGrottoSearcherModel5;

namespace Ui {
class HiddenGrotto;
}

class HiddenGrotto : public QWidget
{
    Q_OBJECT

signals:
    void alertProfiles(int);

public:
    explicit HiddenGrotto(QWidget *parent = nullptr);
    ~HiddenGrotto() override;
    void updateProfiles();
    bool hasProfiles() const;

private:
    Ui::HiddenGrotto *ui;
    HiddenGrottoSearcherModel5 *searcherModel = nullptr;
    std::vector<Profile5> profiles;
    Profile5 currentProfile;
    QMenu *searcherMenu = nullptr;

    void setupModels();

private slots:
    void search();
    void profileIndexChanged(int index);
    void tableViewSearcherContextMenu(QPoint pos);
    void profileManager();
};

#endif // HIDDENGROTTO_HPP
