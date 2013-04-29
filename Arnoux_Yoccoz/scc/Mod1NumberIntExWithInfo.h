#ifndef MOD1NUMBERINTEXWITHINFO_H
#define MOD1NUMBERINTEXWITHINFO_H

#include "../intex/Mod1NumberIntExchange.h"
#include "../fol/Foliation.h"

namespace balazs{

class Mod1NumberIntExWithInfo
{
public:
    Mod1NumberIntExWithInfo() : m_foliation(nullptr) {} // Constructs uninitialized obj.
    Mod1NumberIntExWithInfo(const Mod1NumberIntExchange& number, const Foliation* parent);
    Mod1NumberIntExWithInfo shiftedTo(Direction::LeftOrRight side) const;
    bool isTooCloseToADivPoint() const;

    const Mod1NumberIntExchange number() const { return m_mod1NumberIntExchange; }
    unsigned int smallContainingInterval() const { return m_smallContainingInterval; }
    const Foliation& foliation() const { return *m_foliation; }
    operator Mod1NumberIntExchange() const { return m_mod1NumberIntExchange; }
private:
    Mod1NumberIntExchange m_mod1NumberIntExchange;
    unsigned int m_smallContainingInterval;
    const Foliation* m_foliation;
};

bool operator< (const Mod1NumberIntExWithInfo& lhs, const Mod1NumberIntExWithInfo& rhs);
//bool operator< (const Mod1Number& lhs, const Mod1NumberIntExWithInfo& rhs);
//bool operator< (const Mod1NumberIntExWithInfo& lhs, const Mod1Number& rhs);



}

#endif // MOD1NUMBERINTEXWITHINFO_H
