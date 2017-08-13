#ifndef FRAMEGEN3_HPP
#define FRAMEGEN3_HPP
#include <Core/Objects/Encounter.hpp>
#include <Core/Objects/Lead.hpp>
#include <Core/Objects/Method.hpp>
#include <vector>
#include <cstdint>


class FrameGen3
{

private:
    uint32_t HABCDS[6] = { 0, 1, 2, 5, 3, 4 };
    uint32_t ABCDS[5] = { 1, 2, 5, 3, 4 };
    uint32_t ACDS[4] = { 1, 5, 3, 4 };

public:
    uint32_t seed;
    uint32_t pid;
    uint32_t tid;
    uint32_t sid;
    uint32_t dv1;
    uint32_t dv2;
    uint32_t inh1;
    uint32_t inh2;
    uint32_t inh3;
    uint32_t par1;
    uint32_t par2;
    uint32_t par3;
    uint32_t hp;
    uint32_t atk;
    uint32_t def;
    uint32_t spa;
    uint32_t spd;
    uint32_t spe;
    bool shiny;
    bool synchable;
    Method MethodType;
    Lead LeadType;
    Encounter EncounterType;

    FrameGen3();

};

#endif // FRAMEGEN3_HPP
