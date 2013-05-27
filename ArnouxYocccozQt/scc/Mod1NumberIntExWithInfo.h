#ifndef MOD1NUMBERINTEXWITHINFO_H
#define MOD1NUMBERINTEXWITHINFO_H

#include "../intex/Mod1NumberIntExchange.h"

namespace balazs{

class Foliation;

class Mod1NumberIntExWithInfo
{
public:
    Mod1NumberIntExWithInfo() : m_foliation(nullptr) {} // Constructs uninitialized obj.
    Mod1NumberIntExWithInfo(const Mod1NumberIntExchange& number, const Foliation* parent);
    Mod1NumberIntExWithInfo shiftedTo(HDirection side) const;

    const Mod1NumberIntExchange& number() const { return m_mod1NumberIntExchange; }
    std::size_t smallContainingInterval() const { return m_smallContainingInterval; }
    const Foliation& foliation() const { return *m_foliation; }
    operator Mod1NumberIntExchange() const { return m_mod1NumberIntExchange; }
private:
    Mod1NumberIntExchange m_mod1NumberIntExchange;
    std::size_t m_smallContainingInterval;
    const Foliation* m_foliation;
};

bool operator< (const Mod1NumberIntExWithInfo& lhs, const Mod1NumberIntExWithInfo& rhs);
bool isTooCloseToADivPoint(const Mod1NumberIntExWithInfo& x);




}

#endif // MOD1NUMBERINTEXWITHINFO_H
