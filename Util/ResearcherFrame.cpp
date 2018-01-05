#include "ResearcherFrame.hpp"

ResearcherFrame::ResearcherFrame(bool rng64Bit, u32 frame)
{
    for (int i = 0; i < 10; i++)
    {
        Custom[i] = 0;
    }
    RNG64Bit = rng64Bit;
    Full32 = 0;
    Full64 = 0;
    Frame = frame;
}
