#include "FrameGen3.hpp"

FrameGen3::FrameGen3(uint32_t id, uint32_t sid)
{
    tid = id;
    this->sid = sid;
}

void FrameGen3::setPID(uint32_t pid1, uint32_t pid2)
{
    pid = (pid1 << 16) | pid2;
    nature = pid % 25;
    gender = pid & 255;
    ability = pid & 1;
    if ((pid1 ^ pid2 ^ tid ^ sid) < 8)
        shiny = true;
    else
        shiny = false;
}

void FrameGen3::setIVs(uint32_t iv1, uint32_t iv2)
{
    hp = iv1 & 0x1f;
    atk = (iv1 >> 5) & 0x1f;
    def = (iv1 >> 10) & 0x1f;
    spa = (iv2 >> 5) & 0x1f;
    spd = (iv2 >> 10) & 0x1f;
    spe = iv2 & 0x1f;
    hidden = ((((hp & 1) + 2 * (atk & 1) + 4 * (def & 1) + 8 * (spe & 1) + 16 * (spa & 1) + 32 * (spd & 1)) * 15) / 63);
    power = (30 + ((((hp >> 1) & 1) + 2 * ((atk >> 1) & 1) + 4 * ((def >> 1) & 1) + 8 * ((spe >> 1) & 1) + 16 * ((spa >> 1) & 1) + 32 * ((spd >> 1) & 1)) * 40 / 63));
}

std::string FrameGen3::getNature()
{
    return natures[nature];
}

std::string FrameGen3::getPower()
{
    return powers[hidden];
}

std::string FrameGen3::getFemale125()
{
    return (gender >= 31) ? "M" : "F";
}

std::string FrameGen3::getFemale25()
{
    return (gender >= 63) ? "M" : "F";
}

std::string FrameGen3::getFemale50()
{
    return (gender >= 127) ? "M" : "F";
}

std::string FrameGen3::getFemale75()
{
    return (gender >= 191) ? "M" : "F";
}

std::string FrameGen3::getTime()
{
    int32_t minutes = frame / 3600;
    int32_t seconds = (frame - (3600 * minutes)) / 60;
    int32_t milliseconds = ((frame % 60) * 100) / 60;
    std::string ret = std::to_string(minutes) + ":" + std::to_string(seconds) + "." + std::to_string(milliseconds);
    return ret;
}

std::string FrameGen3::getShiny()
{
    return shiny ? "!!!" : "";
}
