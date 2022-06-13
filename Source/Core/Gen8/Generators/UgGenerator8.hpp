#ifndef UGGENERATOR8_HPP
#define UGGENERATOR8_HPP

#include <Core/Enum/Method.hpp>
#include <Core/Gen8/Profile8.hpp>
#include <Core/Gen8/States/UgState.hpp>
#include <Core/Parents/Generators/Generator.hpp>
#include <Core/Parents/PersonalLoader.hpp>

class UgGenerator8 : public Generator
{
public:
    UgGenerator8(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, const StateFilter &filter, bool bonus);
    std::vector<UgState> generate(u64 seed0, u64 seed1, u8 randMarkID, u8 storyFlag, const Profile8 &profile) const;

private:
    bool bonus;
};

#endif // UGGENERATOR8_HPP
