#include "ArcsAroundDivPoints.h"
#include "Foliation.h"
#include "Modint.h"

//-------------------------------//
// Foliation::ArcAroundDivPoints //
//-------------------------------//


ArcsAroundDivPoints::ArcsAroundDivPoints(const Foliation & foliation) :
    m_cuttingPoints(foliation.m_numSeparatrices),
    m_foliation(foliation)
{
}




void ArcsAroundDivPoints::InsertPoint(const UnitIntervalPoint& NewCuttingPoint, int IndexOfInterval){
    if (m_cuttingPoints[IndexOfInterval].isEmpty) {
        m_cuttingPoints[IndexOfInterval].first = m_cuttingPoints[IndexOfInterval].second = NewCuttingPoint;
        m_cuttingPoints[IndexOfInterval].isEmpty = false;
    } else if (NewCuttingPoint < m_cuttingPoints[IndexOfInterval].first)
        m_cuttingPoints[IndexOfInterval].first = NewCuttingPoint;
    else if (m_cuttingPoints[IndexOfInterval].second < NewCuttingPoint)
        m_cuttingPoints[IndexOfInterval].second = NewCuttingPoint;
}





bool ArcsAroundDivPoints::ContainsQ(const UnitIntervalPoint& c, int IndexOfInterval) const{
    if (m_cuttingPoints[IndexOfInterval].isEmpty) {
        return true;
    }
    if (c < m_cuttingPoints[IndexOfInterval].first || m_cuttingPoints[IndexOfInterval].second < c)
    {
        return true;
    }
    return false;
}






bool ArcsAroundDivPoints::ContainsArcThroughADivPointQ(const UnitIntervalPoint& LeftEndPoint, int LeftIndexOfInterval,
                                                                  const UnitIntervalPoint& RightEndPoint, int RightIndexOfInterval,
                                                                  bool throughTopDivPointQ) const{

    if (LeftIndexOfInterval == RightIndexOfInterval) {
        return false;
    }
    if (!m_cuttingPoints[LeftIndexOfInterval].isEmpty && LeftEndPoint < m_cuttingPoints[LeftIndexOfInterval].second) {
        return false;
    }
    if (!m_cuttingPoints[RightIndexOfInterval].isEmpty && m_cuttingPoints[RightIndexOfInterval].first < RightEndPoint) {
        return false;
    }

    for (Modint i(LeftIndexOfInterval + 1, m_foliation.m_numSeparatrices); i != RightIndexOfInterval; ++i) {
        if (!m_cuttingPoints[i].isEmpty) {
            return false;
        }
    }

    for (Modint i(LeftIndexOfInterval, m_foliation.m_numSeparatrices); i != RightIndexOfInterval; ++i) {
        if ((throughTopDivPointQ && m_foliation.m_isTopDivPoint[i + 1]) ||
            (!throughTopDivPointQ && !m_foliation.m_isTopDivPoint[i + 1])) {
            return true;
        }
    }

    return false;
}



std::string ArcsAroundDivPoints::print() const
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
ArcsAroundDivPoints intersect(const std::vector<const ArcsAroundDivPoints*>& adpVector)
{
    assert(adpVector.size() >= 2);
    //const Foliation& foliation = adpVector[0]->m_foliation;
    ArcsAroundDivPoints adp = *adpVector[0];
    for (unsigned int i = 0; i < adp.m_cuttingPoints.size(); i++) {
        for (auto it = adpVector.begin() + 1; it != adpVector.end(); it++) {
            if ((*it)->m_cuttingPoints[i].isEmpty) {
            } else
            if (adp.m_cuttingPoints[i].isEmpty) {
                adp.m_cuttingPoints[i].first = (*it)->m_cuttingPoints[i].first;
                adp.m_cuttingPoints[i].second = (*it)->m_cuttingPoints[i].second;
                adp.m_cuttingPoints[i].isEmpty = false;
            } else {
                adp.m_cuttingPoints[i].first = std::min(adp.m_cuttingPoints[i].first, (*it)->m_cuttingPoints[i].first);
                adp.m_cuttingPoints[i].second = std::max(adp.m_cuttingPoints[i].second, (*it)->m_cuttingPoints[i].second);
            }
        }
    }
    return adp;
}





 









