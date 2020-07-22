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

    u32 getIVState() const
    {
        return ivState;
    }

    void setIVState(u32 ivState)
    {
        this->ivState = ivState;
    }

private:
    u32 ivState;
};

#endif // STATIONARYSTATE5_HPP
