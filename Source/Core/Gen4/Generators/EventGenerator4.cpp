#include "EventGenerator4.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/States/State.hpp>
#include <Core/RNG/LCRNG.hpp>

EventGenerator4::EventGenerator4(u32 initialAdvances, u32 maxAdvances, u32 delay, u16 species, u8 nature, u8 level, const Profile4 &profile,
                                 const StateFilter4 &filter) :
    Generator(initialAdvances, maxAdvances, delay, Method::None, profile, filter), species(species), nature(nature), level(level)
{
}

std::vector<GeneratorState> EventGenerator4::generate(u32 seed) const
{
    const PersonalInfo *info = PersonalLoader::getPersonal(profile.getVersion(), species);

    PokeRNG rng(seed, initialAdvances + delay);

    std::vector<GeneratorState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        PokeRNG go(rng);

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();

        std::array<u8, 6> ivs;
        ivs[0] = iv1 & 31;
        ivs[1] = (iv1 >> 5) & 31;
        ivs[2] = (iv1 >> 10) & 31;
        ivs[3] = (iv2 >> 5) & 31;
        ivs[4] = (iv2 >> 10) & 31;
        ivs[5] = iv2 & 31;

        if (filter.compareIV(ivs))
        {
            GeneratorState state(initialAdvances + cnt, 0, ivs, 0, 0, level, nature, 0, info);
            states.emplace_back(state);
        }
    }

    return states;
}
