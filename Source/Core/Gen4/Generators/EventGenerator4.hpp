#ifndef EVENTGENERATOR4_HPP
#define EVENTGENERATOR4_HPP

#include <Core/Gen4/Filters/StateFilter4.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Parents/Generators/Generator.hpp>

class GeneratorState;

class EventGenerator4 : public Generator<Profile4, StateFilter4>
{
public:
    EventGenerator4(u32 initialAdvances, u32 maxAdvances, u32 delay, u16 species, u8 nature, u8 level, const Profile4 &profile,
                    const StateFilter4 &filter);

    std::vector<GeneratorState> generate(u32 seed) const;

private:
    u16 species;
    u8 nature;
    u8 level;
};

#endif // EVENTGENERATOR4_HPP
