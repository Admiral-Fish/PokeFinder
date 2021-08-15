#ifndef HIDDENGROTTOSEARCHER_HPP
#define HIDDENGROTTOSEARCHER_HPP

#include <Core/Gen5/Generators/HiddenGrottoGenerator.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/States/HiddenGrottoState.hpp>
#include <Core/Gen5/States/SearcherState5.hpp>
#include <Core/Util/DateTime.hpp>
#include <Core/Util/Global.hpp>
#include <atomic>
#include <mutex>

class HiddenGrottoSearcher
{
public:
    HiddenGrottoSearcher() = default;
    explicit HiddenGrottoSearcher(const Profile5 &profile);
    void startSearch(const HiddenGrottoGenerator &generator, int threads, Date start, const Date &end);
    void cancelSearch();
    std::vector<SearcherState5<HiddenGrottoState>> getResults();
    int getProgress() const;

private:
    Profile5 profile;

    bool searching;
    std::atomic<int> progress;
    std::vector<SearcherState5<HiddenGrottoState>> results;
    std::mutex mutex;

    void search(HiddenGrottoGenerator generator, const Date &start, const Date &end);
};

#endif // HIDDENGROTTOSEARCHER_HPP
