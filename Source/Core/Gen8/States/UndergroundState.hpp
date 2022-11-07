#ifndef UNDERGROUNDSTATE_HPP
#define UNDERGROUNDSTATE_HPP

#include <Core/Parents/States/State.hpp>

class PersonalInfo;

class UndergroundState : public State
{
public:
    UndergroundState(u32 advances, u16 specie, u32 pid, u8 shiny, const std::array<u8, 6> &ivs, u8 ability, u8 gender, u8 nature, u8 level,
                     u16 item, const PersonalInfo *info);

    u32 getAdvances() const
    {
        return advances;
    }

    u16 getSpecie() const
    {
        return specie;
    }

    u16 getItem() const
    {
        return item;
    }

protected:
    u32 advances;
    u16 specie;
    u16 item;
};

#endif // UNDERGROUNDSTATE_HPP
