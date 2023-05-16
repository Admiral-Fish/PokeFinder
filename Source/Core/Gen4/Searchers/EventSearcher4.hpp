#ifndef EVENTSEARCHER4_HPP
#define EVENTSEARCHER4_HPP

#include <Core/Gen4/Filters/StateFilter4.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Parents/Searchers/StaticSearcher.hpp>
#include <mutex>

class EventSearcher4 : public StaticSearcher<Profile4, StateFilter4>
{
public:
    EventSearcher4(u32 minAdvance, u32 maxAdvance, u32 minDelay, u32 maxDelay, const Profile4 &profile, const StateFilter4 &filter);

    void cancelSearch();

    int getProgress() const;

    std::vector<SearcherState4> getResults();

    void startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max, u16 species, u8 nature, u8 level);

private:
    std::mutex mutex;
    std::vector<SearcherState4> results;
    int progress;
    u32 maxAdvance;
    u32 minAdvance;
    u32 maxDelay;
    u32 minDelay;
    bool searching;

    std::vector<SearcherState4> search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u16 species, u8 nature, u8 level) const;

    std::vector<SearcherState4> searchInitialSeeds(const std::vector<SearcherState4> &results) const;

    std::vector<SearcherState4> searchWondercardIVs(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u16 species, u8 nature, u8 level) const;
};

#endif // EVENTSEARCHER4_HPP
