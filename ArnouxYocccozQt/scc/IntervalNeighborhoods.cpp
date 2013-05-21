#include "IntervalNeighborhoods.h"
#include "../fol/Foliation.h"
#include <cassert>

balazs::IntervalNeighborhoods::IntervalNeighborhoods(const Foliation & foliation) :
    m_foliation(foliation),
    m_cuttingPoints(foliation.numSeparatrices())
{
}



void balazs::IntervalNeighborhoods::insertPoint(const Mod1NumberIntExWithInfo &newCuttingPoint){
    assert(&foliation() == &newCuttingPoint.foliation());
 //   assert(newCuttingPoint.number().side() != Direction::CENTER);

    std::size_t indexOfInterval = newCuttingPoint.smallContainingInterval();

    if (m_cuttingPoints[indexOfInterval].isEmpty) {
        m_cuttingPoints[indexOfInterval].first = m_cuttingPoints[indexOfInterval].second = newCuttingPoint;
        m_cuttingPoints[indexOfInterval].isEmpty = false;
    } else if (localLess(newCuttingPoint, m_cuttingPoints[indexOfInterval].first))
        m_cuttingPoints[indexOfInterval].first = newCuttingPoint;
    else if (localLess(m_cuttingPoints[indexOfInterval].second, newCuttingPoint))
        m_cuttingPoints[indexOfInterval].second = newCuttingPoint;
}




// An interval around a divpoint is two sided if the divpoint cuts the interval into
// two intervals, both with length bigger than zero.
//
// The IntervalNeighborhoods corresponding to a SeparatrixSegment have the first cutting
// point which is an infinitesimal shift of one of the divpoints. As a result, the interval
// around that divpoint is one-sided, unless it is long enough to contain another divpoint.
// But if it only contains that one divpoint, then a SeparatrixSegment with endpoint falling
// into that interval can't be good no matter which side that infinitesimal shift is.
//
// The parameter indexOfOneSidedDivPoint tells which divpoint might have the one-sided interval.
//
// This little compication has the benefit that without this it would be possible that
// a separatrix segment of certain depth is considered good when shifted to the left, and
// bad if shifted to the right, or vica versa, and this assymetry would be inconvenient when
// iterating over the good separatrix segments.
//
bool balazs::IntervalNeighborhoods::containsInTwoSidedInterval(const Mod1NumberIntExWithInfo& point) const{
    assert(&foliation() == &point.foliation());

    std::size_t indexOfInterval = point.smallContainingInterval();

    if (m_cuttingPoints[indexOfInterval].isEmpty) {
        return true;
    }

    std::size_t prevIndex = (indexOfInterval + (m_cuttingPoints.size() - 1)) % m_cuttingPoints.size();
    std::size_t nextIndex = (indexOfInterval + 1) % m_cuttingPoints.size();
    bool isLeftDivPointTwoSided = m_cuttingPoints[prevIndex].isEmpty ||
            m_cuttingPoints[prevIndex].second.number() != m_foliation.allDivPoints()[indexOfInterval].shiftedTo(HDirection::Left);

    bool isRightDivPointTwoSided = m_cuttingPoints[nextIndex].isEmpty ||
            m_foliation.allDivPoints()[nextIndex].shiftedTo(HDirection::Right) != m_cuttingPoints[nextIndex].first.number();

    if ((localLess(point, m_cuttingPoints[indexOfInterval].first) && isLeftDivPointTwoSided)
            || (localLess(m_cuttingPoints[indexOfInterval].second, point) && isRightDivPointTwoSided))
    {
        return true;
    }
    return false;
}






bool balazs::IntervalNeighborhoods::containsIntervalThroughADivPoint(const Mod1NumberIntExWithInfo &leftEndPoint,
                                                                  const Mod1NumberIntExWithInfo &rightEndPoint,
                                                                  bool throughTopDivPointQ) const{
    assert(&foliation() == &leftEndPoint.foliation());
    assert(&foliation() == &rightEndPoint.foliation());

    std::size_t leftIndexOfInterval = leftEndPoint.smallContainingInterval();
    std::size_t rightIndexOfInterval = rightEndPoint.smallContainingInterval();


    if (leftIndexOfInterval == rightIndexOfInterval) {
        return false;
    }
    if (!m_cuttingPoints[leftIndexOfInterval].isEmpty &&
            localLess(leftEndPoint,  m_cuttingPoints[leftIndexOfInterval].second)) {
        return false;
    }
    if (!m_cuttingPoints[rightIndexOfInterval].isEmpty &&
            localLess(m_cuttingPoints[rightIndexOfInterval].first, rightEndPoint)) {
        return false;
    }

    for (std::size_t i = (leftIndexOfInterval + 1) % m_foliation.numSeparatrices(); i != rightIndexOfInterval;
         i = (i + 1) % m_foliation.numSeparatrices()) {
        if (!m_cuttingPoints[i].isEmpty) {
            return false;
        }
    }

    for (std::size_t i = rightIndexOfInterval; i != leftIndexOfInterval;
         i = (i + (m_foliation.numSeparatrices() - 1)) % m_foliation.numSeparatrices()) {
        if ((throughTopDivPointQ && m_foliation.isTopDivPoint(i)) ||
            (!throughTopDivPointQ && !m_foliation.isTopDivPoint(i))) {
            return true;
        }
    }

    return false;
}



bool balazs::IntervalNeighborhoods::localLess(const Mod1NumberIntExWithInfo &lhs, const Mod1NumberIntExWithInfo &rhs)
{
    assert(lhs.smallContainingInterval() == rhs.smallContainingInterval());
    if(lhs.smallContainingInterval() < lhs.foliation().numSeparatrices() - 1){
        return lhs < rhs;
    }
    long double lValue = lhs.number();
    long double rValue = rhs.number();
    if(lValue != 0 && rValue != 0){
        return lhs < rhs;
    }
    if(lValue != 0){
        return true;
    }
    if(rValue != 0){
        return false;
    }
    return lhs < rhs;
}










// The union of cutting points in all the IntervalNeighborhoods are taken and the resulting object,
// generally with shorter intervals around the division point are returned.
//
balazs::IntervalNeighborhoods::IntervalNeighborhoods(const std::vector<const IntervalNeighborhoods*>& inbhVector) :
    m_foliation(inbhVector[0]->foliation()),
    m_cuttingPoints(inbhVector[0]->m_cuttingPoints.size())
{
    for(std::size_t i = 1; i < inbhVector.size(); i++){
        assert(&inbhVector[i - 1]->foliation() == &inbhVector[i]->foliation());
    }
    assert(inbhVector.size() >= 2);

    for (std::size_t i = 0; i < m_cuttingPoints.size(); i++) {
        for (auto it = inbhVector.begin(); it != inbhVector.end(); it++) {
            if ((*it)->m_cuttingPoints[i].isEmpty) {
            } else
            if (m_cuttingPoints[i].isEmpty) {
                m_cuttingPoints[i] = (*it)->m_cuttingPoints[i];
            } else {
                m_cuttingPoints[i].first = std::min(m_cuttingPoints[i].first, (*it)->m_cuttingPoints[i].first, localLess);
                m_cuttingPoints[i].second = std::max(m_cuttingPoints[i].second, (*it)->m_cuttingPoints[i].second, localLess);
            }
        }
    }
}





