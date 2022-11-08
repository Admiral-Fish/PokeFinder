#ifndef UNDERGROUNDGENERATOR_HPP
#define UNDERGROUNDGENERATOR_HPP

#include <Core/Enum/Lead.hpp>
#include <Core/Gen8/Filters/StateFilter8.hpp>
#include <Core/Parents/Generators/Generator.hpp>

class EncounterArea8;

class UndergroundGenerator : public Generator<UndergroundStateFilter>
{
public:
    UndergroundGenerator(u32 initialAdvances, u32 maxAdvances, u32 offset, u16 tid, u16 sid, Game version, Lead lead, u8 randMarkId,
                         u8 storyFlag, bool bonus, const UndergroundStateFilter &filter);

    std::vector<UndergroundState> generate(u64 seed0, u64 seed1) const;

protected:
    Lead lead;
    u8 randMarkId;
    u8 storyFlag;
    bool bonus;
};

#endif // UNDERGROUNDGENERATOR_HPP
