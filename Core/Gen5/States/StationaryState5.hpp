#ifndef STATIONARYSTATE5_HPP
#define STATIONARYSTATE5_HPP

#include <Core/Gen5/States/State5.hpp>
#include <Core/Parents/States/StationaryState.hpp>

class StationaryState5 : public State5, public StationaryState
{
public:
    StationaryState5() = default;

    explicit StationaryState5(u32 advance) : StationaryState(advance)
    {
    }

    u32 getIVCurrentState() const
    {
        return ivCurrentState;
    }

    void setIVCurrentState(u32 ivCurrentState)
    {
        this->ivCurrentState = ivCurrentState;
    }

private:
    u32 ivCurrentState;
};

#endif // STATIONARYSTATE5_HPP
