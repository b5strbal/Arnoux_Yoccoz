#include "Mod1NumberIntExWithInfo.h"
#include "../fol/Foliation.h"
#include <cassert>

balazs::Mod1NumberIntExWithInfo::Mod1NumberIntExWithInfo(const Mod1NumberIntExchange &number, const Foliation *parent) :
    m_mod1NumberIntExchange(number),
    m_foliation(parent)
{
    assert(&parent->intExchange() == number.parent());
    m_smallContainingInterval = containingInterval(m_foliation->allDivPoints(), number);
}


balazs::Mod1NumberIntExWithInfo balazs::Mod1NumberIntExWithInfo::shiftedTo(HDirection side) const{
    return Mod1NumberIntExWithInfo(m_mod1NumberIntExchange.shiftedTo(side), m_foliation);
}

bool balazs::isTooCloseToADivPoint(const Mod1NumberIntExWithInfo& x)
{
    std::size_t nextInterval = (x.smallContainingInterval() + 1) % x.foliation().numSeparatrices();
    return distanceBetween(x.foliation().allDivPoints()[x.smallContainingInterval()], x.number()) < Mod1Number::precision ||
           distanceBetween(x.number(), x.foliation().allDivPoints()[nextInterval]) < Mod1Number::precision;
}



bool balazs::operator< (const Mod1NumberIntExWithInfo& lhs, const Mod1NumberIntExWithInfo& rhs){
    return lhs.number() < rhs.number();
}


