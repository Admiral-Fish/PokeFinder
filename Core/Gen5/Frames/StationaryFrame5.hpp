#ifndef STATIONARYFRAME5_HPP
#define STATIONARYFRAME5_HPP

#include <Core/Gen5/Frames/Frame5.hpp>
#include <Core/Parents/Frames/StationaryFrame.hpp>

class StationaryFrame5 : public Frame5, public StationaryFrame
{
public:
    StationaryFrame5() = default;

    explicit StationaryFrame5(u32 frame) : StationaryFrame(frame)
    {
    }

    u32 getIVFrame() const
    {
        return ivFrame;
    }

    void setIVFrame(u32 ivFrame)
    {
        this->ivFrame = ivFrame;
    }

private:
    u32 ivFrame;
};

#endif // STATIONARYFRAME5_HPP
