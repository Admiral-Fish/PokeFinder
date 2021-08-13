#ifndef HIDDENGROTTO_HPP
#define HIDDENGROTTO_HPP

#include <Core/Gen5/Generators/HiddenGrottoGenerator.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <QMenu>

class HiddenGrottoGeneratorModel5;
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
    HiddenGrottoGeneratorModel5 *generatorModel = nullptr;
    HiddenGrottoSearcherModel5 *searcherModel = nullptr;
    std::vector<Profile5> profiles;
    Profile5 currentProfile;
    QMenu *generatorMenu = nullptr;
    QMenu *searcherMenu = nullptr;

    void setupModels();

private slots:
    void generate();
    void search();
    void profileIndexChanged(int index);
    void tableViewGeneratorContextMenu(QPoint pos);
    void tableViewSearcherContextMenu(QPoint pos);
    void profileManager();
};

#endif // HIDDENGROTTO_HPP
