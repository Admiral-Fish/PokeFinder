#ifndef PROFILEGEN3_HPP
#define PROFILEGEN3_HPP

#include <Core/Objects/Profile.hpp>

class ProfileGen3 : public Profile
{
public:
    ProfileGen3(QString profileName, QString version, uint32_t tid, uint32_t sid, QString language = "", bool deadBattery = false, bool valid = false);

    void saveProfile();

    static ProfileGen3& loadProfile(QString profileName);

    uint32_t tid;
    uint32_t sid;
    bool deadBattery;

};

#endif // PROFILEGEN3_HPP
