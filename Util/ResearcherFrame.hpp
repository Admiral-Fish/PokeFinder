#ifndef RESEARCHERFRAME_HPP
#define RESEARCHERFRAME_HPP

#include <cstdint>
#include <QObject>

typedef uint32_t u32;
typedef uint64_t u64;

class ResearcherFrame
{

public:
    quint64 Custom[10];
    bool RNG64Bit;
    u64 Full64;
    u32 Full32;
    u32 Frame;

    ResearcherFrame(bool rng64Bit, u32 frame);
    inline u32 High32() { return (u32)(Full64 >> 32); }
    inline u32 Low32() { return (u32)(Full64 & 0xFFFFFFFF); }
    inline u32 High16() { return RNG64Bit ? High32() >> 16 : Full32 >> 16; }
    inline u32 Low16() { return RNG64Bit ? High32() & 0xFFFF : Full32 & 0xFFFF; }
    inline u32 Mod25() { return RNG64Bit ? High32() % 25 : High16() % 25; }
    inline u32 Mod100() { return RNG64Bit ? High32() % 100 : High16() % 100; }
    inline u32 Mod3() { return RNG64Bit ? High32() % 3 : High16() % 3; }
    inline u32 Div656() { return High16() / 656; }
    inline u32 HighBit() { return RNG64Bit ? High32() >> 31 : High16() >> 15; }
    inline u32 LowBit() { return RNG64Bit ? High32() & 1 : High16() & 1; }

};

#endif // RESEARCHERFRAME_HPP
