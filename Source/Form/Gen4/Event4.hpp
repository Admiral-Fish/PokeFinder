#ifndef EVENT4_HPP
#define EVENT4_HPP

#include <QWidget>

class EventGeneratorModel4;
class EventSearcherModel4;
class Profile4;

namespace Ui
{
    class Event4;
}

class Event4 : public QWidget
{
    Q_OBJECT
signals:
    void profilesModified(int);

public:
    Event4(QWidget *parent = nullptr);

    ~Event4() override;

    void updateProfiles();

private:
    Ui::Event4 *ui;

    EventGeneratorModel4 *generatorModel;
    EventSearcherModel4 *searcherModel;
    Profile4 *currentProfile;
    std::vector<Profile4> profiles;

private slots:
    void generate();

    void search();

    void profileIndexChanged(int index);

    void profileManager();

    /**
     * @brief Opens seed to time. Provides current game version and seed from the selected tableview row.
     */
    void seedToTime();
};

#endif // EVENT4_HPP
