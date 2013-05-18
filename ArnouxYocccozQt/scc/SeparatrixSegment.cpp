#include "SeparatrixSegment.h"
#include "../fol/Foliation.h"
#include "../intex/ContainingInterval.h"
#include "../intex/HDirection.h"
#include <cassert>

balazs::SeparatrixSegment::SeparatrixSegment(const Foliation& foliation, std::size_t startingSingularity, VDirection direction) :
    m_foliation(foliation),
    m_startingSingularity(startingSingularity),
    m_depth(1),
    m_endpoint(foliation.firstIntersection(startingSingularity, direction), &foliation),
    m_intervalNeighborhoods(foliation),
    m_intervalIntersectionCount(std::vector<std::size_t>(foliation.numIntervals(), 0)),
    m_vDirection(direction),
    m_reachedSaddleConnection(false)
{
}








void balazs::SeparatrixSegment::lengthen()
{
    assert(isCentered());
    if(m_depth > 1){
        m_intervalIntersectionCount[containingInterval(m_foliation.topDivPoints(), m_endpoint)]++;
        m_intervalNeighborhoods.insertPoint(m_endpoint);
    }
    m_depth++;
    Mod1NumberIntExchange newPoint = (m_vDirection == VDirection::Up) ?
                m_foliation.intExchange().applyTo(m_endpoint.number()) :
                m_foliation.intExchange().applyInverseTo(m_endpoint.number());
    m_endpoint = Mod1NumberIntExWithInfo(newPoint, &m_foliation);

    if(m_endpoint.isTooCloseToADivPoint()){
        m_reachedSaddleConnection = true;
    }
}






bool balazs::SeparatrixSegment::isGood() const
{
    //assert(!isCentered());
//    if(m_direction == VDirection::Down && m_depth == 12){
//        int a;
//        a = a + 1;
//    }
    return m_intervalNeighborhoods.containsInTwoSidedInterval(m_endpoint);
}

void balazs::SeparatrixSegment::shiftTo(HDirection side)
{
    assert(isCentered());
    m_endpoint = m_endpoint.shiftedTo(side);

    if(m_depth > 1){
        // inserting the first intersection that we have previously omitted
        Mod1NumberIntExWithInfo pointToInsert(m_foliation.firstIntersection(m_startingSingularity,
                                                                        m_vDirection).shiftedTo(side), &m_foliation);
        m_intervalNeighborhoods.insertPoint(pointToInsert);
        m_intervalIntersectionCount[containingInterval(m_foliation.topDivPoints(), pointToInsert)]++;
    }

}


