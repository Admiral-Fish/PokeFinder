#ifndef IRNGPROVIDER4_HPP
#define IRNGPROVIDER4_HPP

#include <Core/Global.hpp>

/**
 * @brief Provides interfact to query information required for Advance Finder
 */
class IRNGProvider4
{
public:
    /**
     * @brief Destroy the IRNGProvider4 object
     */
    virtual ~IRNGProvider4() = default;

    /**
     * @brief Returns Elm/Irwin call for given \p row
     *
     * @return Row Elm/Irwin call
     */
    virtual u8 getCall(int row) const = 0;

    /**
     * @brief Returns chatot pitch for given \p row
     *
     * @return Row chatot pitch
     */
    virtual u8 getChatot(int row) const = 0;
};

#endif // IRNGPROVIDER4_HPP
