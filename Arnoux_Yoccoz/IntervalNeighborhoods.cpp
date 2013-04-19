#include "IntervalNeighborhoods.h"
#include "Foliation.h"
#include "Modint.h"



IntervalNeighborhoods::IntervalNeighborhoods(const Foliation & foliation) :
    m_cuttingPoints(foliation.m_numSeparatrices),
    m_foliation(foliation)
{
}



// \param  newCuttingPoint     It must not coincide with any of the division points.
void IntervalNeighborhoods::insertPoint(const UnitIntervalPoint& newCuttingPoint, int indexOfInterval){
    if (m_cuttingPoints[indexOfInterval].isEmpty) {
        m_cuttingPoints[indexOfInterval].first = m_cuttingPoints[indexOfInterval].second = newCuttingPoint;
        m_cuttingPoints[indexOfInterval].isEmpty = false;
    } else if (newCuttingPoint < m_cuttingPoints[indexOfInterval].first)
        m_cuttingPoints[indexOfInterval].first = newCuttingPoint;
    else if (m_cuttingPoints[indexOfInterval].second < newCuttingPoint)
        m_cuttingPoints[indexOfInterval].second = newCuttingPoint;
}





bool IntervalNeighborhoods::contains(const UnitIntervalPoint& point, int indexOfInterval) const{
    if (m_cuttingPoints[indexOfInterval].isEmpty) {
        return true;
    }
    if (point < m_cuttingPoints[indexOfInterval].first || m_cuttingPoints[indexOfInterval].second < point)
    {
        return true;
    }
    return false;
}






bool IntervalNeighborhoods::containsIntervalThroughADivPoint(const UnitIntervalPoint& leftEndPoint, int leftIndexOfInterval,
                                                                  const UnitIntervalPoint& rightEndPoint, int rightIndexOfInterval,
                                                                  bool throughTopDivPointQ) const{

    if (leftIndexOfInterval == rightIndexOfInterval) {
        return false;
    }
    if (!m_cuttingPoints[leftIndexOfInterval].isEmpty && leftEndPoint < m_cuttingPoints[leftIndexOfInterval].second) {
        return false;
    }
    if (!m_cuttingPoints[rightIndexOfInterval].isEmpty && m_cuttingPoints[rightIndexOfInterval].first < rightEndPoint) {
        return false;
    }

    for (Modint i(leftIndexOfInterval + 1, m_foliation.m_numSeparatrices); i != rightIndexOfInterval; ++i) {
        if (!m_cuttingPoints[i].isEmpty) {
            return false;
        }
    }

    for (Modint i(leftIndexOfInterval, m_foliation.m_numSeparatrices); i != rightIndexOfInterval; ++i) {
        if ((throughTopDivPointQ && m_foliation.m_isTopDivPoint[i + 1]) ||
            (!throughTopDivPointQ && !m_foliation.m_isTopDivPoint[i + 1])) {
            return true;
        }
    }

    return false;
}



std::string IntervalNeighborhoods::print() const
{
    std::ostringstream s;
    for (unsigned int i = 0; i < m_cuttingPoints.size(); i++){
        if (m_cuttingPoints[i].isEmpty) {
            s << "() ";
        } else
            s << "(" << m_cuttingPoints[i].first << "," << m_cuttingPoints[i].second << ") ";
    }
    return s.str();
}







/**
 * @details The intersection of the two Arcs corresponding to each division point is taken and that is assigned to the division point
 *          it the intersection. The operation is well-defined, because although in general the intersection of two arcs on the circle
 *          might not be a single arc, in our case it can't happen because of the properties the Arcs in an ArcAroundDivPoints object
 *          have to satisfy.
 *
 *          The motivation for this operation is to take the union of two sets of cutting points. A set of cutting points determines
 *          an ArcsAroundDivPoints, another set determines another one, and the union of the two sets determines exactly the
 *          intersection of the two objects.
 */
IntervalNeighborhoods IntervalNeighborhoods::intersect(const std::vector<const IntervalNeighborhoods*>& inbhVector)
{
    assert(inbhVector.size() >= 2);
    //const Foliation& foliation = adpVector[0]->m_foliation;
    IntervalNeighborhoods inbh = *inbhVector[0];
    for (unsigned int i = 0; i < inbh.m_cuttingPoints.size(); i++) {
        for (auto it = inbhVector.begin() + 1; it != inbhVector.end(); it++) {
            if ((*it)->m_cuttingPoints[i].isEmpty) {
            } else
            if (inbh.m_cuttingPoints[i].isEmpty) {
                inbh.m_cuttingPoints[i].first = (*it)->m_cuttingPoints[i].first;
                inbh.m_cuttingPoints[i].second = (*it)->m_cuttingPoints[i].second;
                inbh.m_cuttingPoints[i].isEmpty = false;
            } else {
                inbh.m_cuttingPoints[i].first = std::min(inbh.m_cuttingPoints[i].first, (*it)->m_cuttingPoints[i].first);
                inbh.m_cuttingPoints[i].second = std::max(inbh.m_cuttingPoints[i].second, (*it)->m_cuttingPoints[i].second);
            }
        }
    }
    return adp;
}





 









