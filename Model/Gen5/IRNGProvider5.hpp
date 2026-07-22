#ifndef IRNGPROVIDER5_HPP
#define IRNGPROVIDER5_HPP

#include <Core/Global.hpp>

/**
 * @brief Provides interfact to query information required for Advance Finder
 */
class IRNGProvider5
{
public:
    /**
     * @brief Destroy the IRNGProvider5 object
     */
    virtual ~IRNGProvider5() = default;

    /**
     * @brief Returns chatot pitch for given \p row
     *
     * @return Row chatot pitch
     */
    virtual u8 getChatot(int row) const = 0;

    /**
     * @brief Returns needle value for given \p row
     *
     * @return Row needle value
     */
    virtual u8 getNeedle(int row) const = 0;
};

#endif // IRNGPROVIDER5_HPP
