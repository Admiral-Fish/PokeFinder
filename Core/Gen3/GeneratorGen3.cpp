#include "GeneratorGen3.hpp"

GeneratorGen3::GeneratorGen3()
{
    MaxResults = 100000;
    InitialFrame = 1;
    InitialSeed = 0;
    tid = 12345;
    sid = 54321;
}

GeneratorGen3::GeneratorGen3(uint32_t MaxResults, uint32_t InitialFrame, uint32_t InitialSeed, uint32_t tid, uint32_t sid)
{
    this->MaxResults = MaxResults;
    this->InitialFrame = InitialFrame;
    this->InitialSeed = InitialSeed;
    this->tid = tid;
    this->sid = sid;
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
            rng.setpokeRNG();
            return GenerateMethod124();
        case MethodH1:
        case MethodH2:
        case MethodH4:
            rng.setpokeRNG();
            switch (LeadType)
            {
                case None:
                    return GenerateMethodH124();
                case Synchronize:
                    return GenerateMethodH124Synch();
                case CuteCharm:
                    return GenerateMethodH124CuteCharm();
            }
        case XDColo:
            rng.setxdRNG();
            return GenerateMethodXDColo();
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
        FrameGen3 frame = FrameGen3(tid, sid);
        frame.setPID(rngList[1], rngList[0]);
        frame.setIVs(rngList[iv1], rngList[iv2]);
        frame.frame = cnt;
        frames.push_back(frame);
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

std::vector<FrameGen3> GeneratorGen3::GenerateMethodXDColo()
{
    std::vector<FrameGen3> frames;
    for (int i = 0; i < 5; i++)
        rngList.push_back(rng.next16Bit());

    uint32_t max = InitialFrame + MaxResults;
    for (uint32_t cnt = InitialFrame; cnt < max; cnt++, rngList.erase(rngList.begin()), rngList.push_back(rng.next16Bit()))
    {
        FrameGen3 frame = FrameGen3(tid, sid);
        frame.setPID(rngList[3], rngList[4]);
        frame.setIVs(rngList[0], rngList[1]);
        frame.frame = cnt;
        frames.push_back(frame);
    }
    return frames;
}
