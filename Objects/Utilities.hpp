#ifndef UTILITIES_HPP
#define UTILITIES_HPP
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <cstdint>

class Utilities
{
    
public:
    
    static uint32_t calcGen3Seed(boost::gregorian::date time, uint32_t h, uint32_t m);

};

#endif // UTILITIES_HPP
