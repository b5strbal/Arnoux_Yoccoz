#include "SeparatrixSegment.h"
#include "../fol/Foliation.h"
#include "../intex/ContainingInterval.h"
#include "../intex/HDirection.h"
#include <cassert>


namespace balazs{

const Mod1NumberIntExchange firstIntersection(const Foliation& foliation, const SepSegmentIndex& ssIndex)
{
    if (ssIndex.vDirection == VDirection::Up) {
        std::size_t index;
        switch(ssIndex.hDirection){
        case HDirection::Left:
            index = (ssIndex.singularityIndex + foliation.numIntervals() - 1) % foliation.numIntervals();
            index = foliation.intExchange().permutationWithMinimalTwist()[index];
            index = (index + 1) % foliation.numIntervals();
            return foliation.bottomDivPoints()[index].shiftedTo(HDirection::Left);

        case HDirection::Right:
            index = foliation.intExchange().permutationWithMinimalTwist()[ssIndex.singularityIndex];
            return foliation.bottomDivPoints()[index].shiftedTo(HDirection::Right);

        default:
            assert(false);
        }

    } else
        return foliation.topDivPoints()[ssIndex.singularityIndex].shiftedTo(ssIndex.hDirection);
}

}



balazs::SeparatrixSegment::SeparatrixSegment(const Foliation& foliation, const SepSegmentIndex &ssIndex) :
    m_foliation(foliation),
    m_startingSingularity(ssIndex.singularityIndex),
    m_depth(1),
    m_endpoint(firstIntersection(foliation, ssIndex), &foliation),
    m_intervalNeighborhoods(foliation),
    m_intervalIntersectionCount(std::vector<std::size_t>(foliation.numIntervals(), 0)),
    m_vDirection(ssIndex.vDirection),
    m_reachedSaddleConnection(false)
{
    if(m_vDirection == VDirection::Down){
        m_intervalIntersectionCount[containingInterval(m_foliation.topDivPoints(), m_endpoint)]++;
    }
}





void balazs::SeparatrixSegment::lengthen()
{
    if(m_vDirection == VDirection::Up){
        m_intervalIntersectionCount[containingInterval(m_foliation.topDivPoints(), m_endpoint)]++;
    }
    m_intervalNeighborhoods.insertPoint(m_endpoint);

    m_depth++;
    Mod1NumberIntExchange newPoint = (m_vDirection == VDirection::Up) ?
                m_foliation.intExchange().applyTo(m_endpoint.number()) :
                m_foliation.intExchange().applyInverseTo(m_endpoint.number());
    m_endpoint = Mod1NumberIntExWithInfo(newPoint, &m_foliation);

    if(m_vDirection == VDirection::Down){
        m_intervalIntersectionCount[containingInterval(m_foliation.topDivPoints(), m_endpoint)]++;
    }
    if(m_endpoint.isTooCloseToADivPoint()){
        m_reachedSaddleConnection = true;
    }
}






bool balazs::SeparatrixSegment::isGood() const
{
    return m_intervalNeighborhoods.containsInTwoSidedInterval(m_endpoint);
}




