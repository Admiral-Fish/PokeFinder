/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef PROFILE5_HPP
#define PROFILE5_HPP

#include <Core/Parents/Profile.hpp>
#include <vector>

enum class DSType : u8;
enum class Language : u8;

/**
 * @brief Provides additional storage specific to Gen5
 */
class Profile5 : public Profile
{

public:
    /**
     * @brief Construct a new Profile5 object
     *
     * @param name Profile name
     * @param version Game version
     * @param tid Trainer ID
     * @param sid Secret ID
     * @param mac MAC address
     * @param keypresses Vector of keypress values
     * @param vcount VCount value
     * @param gxstat GxStat value
     * @param vframe VFrame value
     * @param skipLR Whether LR buttons should be skipped
     * @param timer0Min Minimum Timer0 value
     * @param timer0Max Maximum Timer0 value
     * @param softReset Whether soft resetting is utilized
     * @param memoryLink Whether memory link is activated
     * @param shinyCharm Whether shiny charm is obtained
     * @param dsType DS type for the profile
     * @param language Language type of the profile
     */
    Profile5(const std::string &name, Game version, u16 tid, u16 sid, u64 mac, const std::vector<bool> &keypresses, u8 vcount, u8 gxstat,
             u8 vframe, bool skipLR, u16 timer0Min, u16 timer0Max, bool softReset, bool memoryLink, bool shinyCharm, DSType dsType,
             Language language);

    /**
     * @brief Returns the DS type of the profile
     *
     * @return DS type
     */
    DSType getDSType() const;

    /**
     * @brief Returns the string representation of the DS type
     *
     * @return DS type string
     */
    std::string getDSTypeString() const;

    /**
     * @brief Returns the GxStat value of the profile
     *
     * @return GxStat value
     */
    u8 getGxStat() const;

    /**
     * @brief Returns the keypresses selected of the profile
     *
     * @return Vector of keypress where true indicates that value is used and false means that value is not used
     */
    std::vector<bool> getKeypresses() const;

    /**
     * @brief Returns the string representation of the keypresses
     *
     * @return Keypresses string
     */
    std::string getKeypressesString() const;

    /**
     * @brief Returns language value of the profile
     *
     * @return Language value
     */
    Language getLanguage() const;

    /**
     * @brief Returns the string representation of the language
     *
     * @return Language string
     */
    std::string getLanguageString() const;

    /**
     * @brief Returns the MAC address of the profile
     *
     * @return MAC address
     */
    u64 getMac() const;

    /**
     * @brief Returns if memory link is activated or not
     *
     * @return true Memory link is activated
     * @return false Memory link is not activated
     */
    bool getMemoryLink() const;

    /**
     * @brief Returns if shiny charm is obtained
     *
     * @return true Shiny charm is obtained
     * @return false Shiny charm is not obtained
     */
    bool getShinyCharm() const;

    /**
     * @brief Returns if the LR buttons should be skipped
     *
     * @return true LR buttons should be skipped
     * @return false LR buttons should not be skipped
     */
    bool getSkipLR() const;

    /**
     * @brief Returns if soft resetting is used
     *
     * @return true Soft resetting is used
     * @return false Soft resetting is not used
     */
    bool getSoftReset() const;

    /**
     * @brief Returns the maximum Timer0 value
     *
     * @return Maximum Timer0 value
     */
    u16 getTimer0Max() const;

    /**
     * @brief Returns the minimum Timer0 value
     *
     * @return Minimum Timer0 value
     */
    u16 getTimer0Min() const;

    /**
     * @brief Returns the VCount value
     *
     * @return VCount value
     */
    u8 getVCount() const;

    /**
     * @brief Returns the VFrame value
     *
     * @return VFrame value
     */
    u8 getVFrame() const;

    /**
     * @brief Checks if two profiles are equal
     *
     * @param other Profile to compare
     *
     * @return true Profiles are equal
     * @return false Profiles are not equal
     */
    bool operator==(const Profile5 &other) const;

    /**
     * @brief Checks if two profiles are not equal
     *
     * @param other Profile to compare
     *
     * @return true Profiles are not equal
     * @return false Profiles are equal
     */
    bool operator!=(const Profile5 &other) const;

private:
    bool memoryLink;
    bool shinyCharm;
    bool skipLR;
    bool softReset;
    DSType dsType;
    Language language;
    u8 gxstat;
    u8 vcount;
    u8 vframe;
    u16 timer0Max;
    u16 timer0Min;
    u64 mac;
    std::vector<bool> keypresses; // 4 index; 0: None, 1: One, 2: Two, 3: Three
};

#endif // PROFILE5_HPP
