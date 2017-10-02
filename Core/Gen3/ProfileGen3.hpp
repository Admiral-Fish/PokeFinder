#ifndef PROFILEGEN3_HPP
#define PROFILEGEN3_HPP

#include <Core/Objects/Profile.hpp>
#include <QList>
#include <QStandardItemModel>

class ProfileGen3 : public Profile
{
public:
    ProfileGen3(QString profileName, int version, uint32_t tid, uint32_t sid, int language = 0, bool deadBattery = false, bool valid = false);

    void saveProfile();

    void deleteProfile();

    static ProfileGen3& loadProfile(QString profileName);

    static std::vector<QList<QStandardItem *>>& loadProfiles();

    static std::vector<ProfileGen3>& loadProfileList();

    static QString getVersion(int i);

    static QString getLanguage(int i);

    static int getVersionIndex(QString s);

    static int getLanguageIndex(QString s);

    uint32_t tid;
    uint32_t sid;
    bool deadBattery;

};

#endif // PROFILEGEN3_HPP
