#ifndef IRNGPROVIDER4_HPP
#define IRNGPROVIDER4_HPP

#include <Core/Global.hpp>

class IRNGProvider4
{
public:
    virtual ~IRNGProvider4() = default;
    virtual u8 getChatot(int row) const = 0;
    virtual u8 getCall(int row) const = 0;
};

#endif // IRNGPROVIDER4_HPP
