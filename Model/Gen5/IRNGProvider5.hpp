#ifndef IRNGPROVIDER5_HPP
#define IRNGPROVIDER5_HPP

#include <Core/Global.hpp>

class IRNGProvider5
{
public:
    virtual ~IRNGProvider5() = default;
    virtual u8 getChatot(int row) const = 0;
    virtual u8 getNeedle(int row) const = 0;
};

class IRNGDreamProvider
{
public:
    virtual ~IRNGDreamProvider() = default;
    virtual u8 getNeedle(int row) const = 0;
};

#endif // IRNGPROVIDER5_HPP
