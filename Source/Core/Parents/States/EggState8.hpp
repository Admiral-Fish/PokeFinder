#ifndef EGGSTATE8_HPP
#define EGGSTATE8_HPP

#include <Core/Parents/States/EggState.hpp>

class EggState8 : public EggState
{
public:
    EggState8() = default;
    explicit EggState8(u32 advance) : EggState(advance)
    {
    }

    u32 getEggSeed() const
    {
        return eggSeed;
    }

    void setEggSeed(u32 val)
    {
        eggSeed = val;
    }

protected:
    u32 eggSeed;
};

#endif // EGGSTATE8_H
