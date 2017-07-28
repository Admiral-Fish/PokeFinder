#include "Utilities.hpp"

Utilities::calcGen3Seed(boost::gregorian::date time, uint32_t h, uint32_t m)
{
    boost::gregorian::date start = boost::gregorian::date(1999, December, 10);
    boost::gregorian::date_duration span = time - start;
    uint32_t d = span.days();
    
    uint32_t seed = 1440 * d + 960 * (h / 10) + 60 * (h % 10) + 16 * (m / 10) + m % 10;
    return (seed >> 16) ^ (seed & 0xFFFF);
}


