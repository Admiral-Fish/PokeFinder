#include "UndergroundState.hpp"
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Util/Nature.hpp>

constexpr int order[6] = { 0, 1, 2, 5, 3, 4 };

UndergroundState::UndergroundState(u32 advances, u16 specie, u32 pid, u8 shiny, const std::array<u8, 6> &ivs, u8 ability, u8 gender,
                                   u8 nature, u16 item, const PersonalInfo *info) :
    GeneratorState(advances)
{
    this->pid = pid;
    this->shiny = shiny;
    this->specie = specie;
    this->ivs = ivs;
    for (int i = 0; i < 6; i++)
    {
        u16 stat = ((2 * info->getStat(i) + ivs[i]) * level) / 100;
        if (i == 0)
        {
            stats[i] = stat + level + 10;
        }
        else
        {
            stats[i] = Nature::computeStat(stat + 5, nature, i);
        }
    }

    this->ability = ability;
    abilityIndex = info->getAbility(ability);
    this->gender = gender;
    this->nature = nature;
    this->item = item;
}
