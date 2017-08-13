#include "GeneratorGen3.hpp"

GeneratorGen3::GeneratorGen3()
{
    MaxResults = 100000;
    InitialFrame = 1;
    InitialSeed = 0;
}

std::vector<FrameGen3> GeneratorGen3::Generate()
{
    rngList.clear();

    rng.setSeed(InitialSeed);
    rng.advanceFrames(InitialFrame - 1);

    if (FrameType == Method1 || FrameType == MethodH1)
    {
        iv1 = 2;
        iv2 = 3;
    }
    else if (FrameType == Method2 || FrameType == MethodH2)
    {
        iv1 = 3;
        iv2 = 4;
    }
    else if (FrameType == Method4 || FrameType == MethodH4)
    {
        iv1 = 2;
        iv2 = 4;
    }

    switch (FrameType)
    {
        case Method1:
        case Method2:
        case Method4:
            return GenerateMethod124();
            break;
        case MethodH1:
        case MethodH2:
        case MethodH4:
            switch (LeadType)
            {
                case None:
                    return GenerateMethodH124();
                    break;
                case Synchronize:
                    return GenerateMethodH124Synch();
                    break;
                case CuteCharm:
                    return GenerateMethodH124CuteCharm();
                    break;
            }
            break;
    }
}

std::vector<FrameGen3> GeneratorGen3::GenerateMethod124()
{
    std::vector<FrameGen3> frames;
    for (int i = 0; i < 5; i++)
        rngList.push_back(rng.next16Bit());

    uint32_t max = InitialFrame + MaxResults;
    for (uint32_t cnt = InitialFrame; cnt < max; cnt++, rngList.erase(rngList.begin()), rngList.push_back(rng.next16Bit()))
    {

    }
    return frames;
}

std::vector<FrameGen3> GeneratorGen3::GenerateMethodH124()
{
    std::vector<FrameGen3> frames;
    for (int i = 0; i < 200; i++)
        rngList.push_back(rng.next16Bit());

    uint32_t max = InitialFrame + MaxResults;
    for (uint32_t cnt = InitialFrame; cnt < max; cnt++, rngList.erase(rngList.begin()), rngList.push_back(rng.next16Bit()))
    {

    }
    return frames;
}

std::vector<FrameGen3> GeneratorGen3::GenerateMethodH124Synch()
{
    std::vector<FrameGen3> frames;
    for (int i = 0; i < 200; i++)
        rngList.push_back(rng.next16Bit());

    uint32_t max = InitialFrame + MaxResults;
    for (uint32_t cnt = InitialFrame; cnt < max; cnt++, rngList.erase(rngList.begin()), rngList.push_back(rng.next16Bit()))
    {

    }
    return frames;
}

std::vector<FrameGen3> GeneratorGen3::GenerateMethodH124CuteCharm()
{
    std::vector<FrameGen3> frames;
    for (int i = 0; i < 200; i++)
        rngList.push_back(rng.next16Bit());

    uint32_t max = InitialFrame + MaxResults;
    for (uint32_t cnt = InitialFrame; cnt < max; cnt++, rngList.erase(rngList.begin()), rngList.push_back(rng.next16Bit()))
    {

    }
    return frames;
}
