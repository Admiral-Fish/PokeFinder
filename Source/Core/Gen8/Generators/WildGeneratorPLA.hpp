#ifndef WildGeneratorPLA_HPP
#define WildGeneratorPLA_HPP

#include <Core/Parents/Generators/Generator.hpp>

class WildGeneratorPLA : Generator
{
public:
    WildGeneratorPLA(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, const StateFilter &filter, u8 rolls,
                     bool isAlpha);
    std::vector<State> generate(u64 seed) const;

private:
    u32 rolls;
    bool isAlpha;
};

#endif // WildGeneratorPLA_HPP
