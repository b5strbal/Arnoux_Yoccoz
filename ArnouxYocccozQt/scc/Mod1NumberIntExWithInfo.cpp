#include "Mod1NumberIntExWithInfo.h"
#include "../fol/Foliation.h"


balazs::Mod1NumberIntExWithInfo::Mod1NumberIntExWithInfo(const Mod1NumberIntExchange &number, const Foliation *parent) :
    m_mod1NumberIntExchange(number),
    m_foliation(parent)
{
    std::cout << parent->intExchange().signature() << "\n";
    std::cout << number.signature() << "\n";
    qDebug() << m_foliation->allDivPoints();

    assert(parent->intExchange().signature() == number.signature());
    m_smallContainingInterval = containingInterval(m_foliation->allDivPoints(), number);
}


balazs::Mod1NumberIntExWithInfo balazs::Mod1NumberIntExWithInfo::shiftedTo(Direction::LeftOrRight side) const{
    return Mod1NumberIntExWithInfo(m_mod1NumberIntExchange.shiftedTo(side), m_foliation);
}

bool balazs::Mod1NumberIntExWithInfo::isTooCloseToADivPoint() const
{
    std::size_t nextInterval = integerMod(m_smallContainingInterval + 1, m_foliation->numSeparatrices());
    return distanceBetween(m_foliation->allDivPoints()[m_smallContainingInterval], m_mod1NumberIntExchange) < PRECISION ||
           distanceBetween(m_mod1NumberIntExchange, m_foliation->allDivPoints()[nextInterval]) < PRECISION;
}



bool balazs::operator< (const Mod1NumberIntExWithInfo& lhs, const Mod1NumberIntExWithInfo& rhs){
    return lhs.number() < rhs.number();
}

//bool operator< (const Mod1Number& lhs, const Mod1NumberIntExWithInfo& rhs);


//bool operator< (const Mod1NumberIntExWithInfo& lhs, const Mod1Number& rhs);
