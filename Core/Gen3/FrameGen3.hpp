#ifndef FRAMEGEN3_HPP
#define FRAMEGEN3_HPP
#include <Core/Objects/Encounter.hpp>
#include <Core/Objects/Lead.hpp>
#include <Core/Objects/Method.hpp>
#include <vector>
#include <cstdint>
#include <string>


class FrameGen3
{

private:
    uint32_t HABCDS[6] = { 0, 1, 2, 5, 3, 4 };
    uint32_t ABCDS[5] = { 1, 2, 5, 3, 4 };
    uint32_t ACDS[4] = { 1, 5, 3, 4 };
    std::string natures[25] = { "Hardy", "Lonely", "Brave", "Adamant", "Naughty",
                           "Bold", "Docile", "Relaxed", "Impish", "Lax",
                           "Timid", "Hasty", "Serious", "Jolly", "Naive",
                           "Modest", "Mild", "Quiet", "Bashful", "Rash",
                           "Calm", "Gentle", "Sassy", "Careful", "Quirky" };
    std::string powers[16] = { "Fighting", "Flying", "Poison", "Ground",
                               "Rock", "Bug", "Ghost", "Steel",
                               "Fire", "Water", "Grass", "Electric",
                               "Psychic", "Ice", "Dragon", "Dark" };

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
    uint32_t gender;
    uint32_t nature;
    uint32_t ability;
    uint32_t hidden;
    uint32_t power;
    uint32_t frame;
    bool shiny;
    bool synchable;
    Method MethodType;
    Lead LeadType;
    Encounter EncounterType;

    FrameGen3(uint32_t id, uint32_t sid);

    void setPID(uint32_t pid1, uint32_t pid2);

    void setIVs(uint32_t iv1, uint32_t iv2);

    std::string getNature();

    std::string getPower();

    std::string getFemale125();

    std::string getFemale25();

    std::string getFemale50();

    std::string getFemale75();

    std::string getTime();

    std::string getShiny();

};

#endif // FRAMEGEN3_HPP
