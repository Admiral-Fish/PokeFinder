#ifndef GENERATORGEN3_HPP
#define GENERATORGEN3_HPP
#include <Core/Objects/LCRNG.hpp>
#include <Core/Objects/Method.hpp>
#include <Core/Objects/Encounter.hpp>
#include <Core/Objects/Lead.hpp>
#include <Core/Gen3/FrameGen3.hpp>
#include <vector>
#include <cstdint>

class GeneratorGen3
{

private:
    LCRNG rng = LCRNG();
    std::vector<uint32_t> rngList;
    int iv1;
    int iv2;

    std::vector<FrameGen3> GenerateMethod124();

    std::vector<FrameGen3> GenerateMethodH124();

    std::vector<FrameGen3> GenerateMethodH124Synch();

    std::vector<FrameGen3> GenerateMethodH124CuteCharm();

public:
    Method FrameType = Method1;
    Encounter EncounterType = Stationary;
    Lead LeadType = None;
    uint32_t InitialSeed;
    uint32_t InitialFrame;
    uint32_t MaxResults;
    uint32_t SynchNature;
    uint32_t cuteCharm;

    GeneratorGen3();

    std::vector<FrameGen3> Generate();

};

#endif // GENERATORGEN3_HPP
