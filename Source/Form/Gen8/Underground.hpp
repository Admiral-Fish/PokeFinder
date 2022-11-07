#ifndef UNDERGROUND_HPP
#define UNDERGROUND_HPP

#include <QWidget>

class Profile8;
class EncounterArea8;
class UndergroundModel;

namespace Ui
{
    class Underground;
}

/**
 * @brief Provides settings and filters to RNG underground encounters in Pokemon Brilliant Diamond and Shining Pearl
 */
class Underground : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesModified(int);

public:
    /**
     * @brief Construct a new Underground object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    Underground(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Underground object
     */
    ~Underground() override;

    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::Underground *ui;

    Profile8 *currentProfile;
    std::vector<EncounterArea8> encounters;
    std::vector<Profile8> profiles;
    UndergroundModel *model;

    /**
     * @brief Updates encounter tables and locations
     */
    void updateEncounters();

private slots:
    /**
     * @brief Generates underground encounters from a starting seed
     */
    void generate();

    void storyFlagIndexChanged(int index);

    /**
     * @brief Updates the pokemon listed
     *
     * @param index Location index
     */
    void locationIndexChanged(int index);

    /**
     * @brief Updates displayed information for a profile
     *
     * @param index Profile index
     */
    void profileIndexChanged(int index);

    /**
     * @brief Opens the profile manager
     */
    void profileManager();
};

#endif // UNDERGROUND_HPP
