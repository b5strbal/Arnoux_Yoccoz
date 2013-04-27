#include "IntervalNeighborhoods.h"
#include "Foliation.h"
#include "Modint.h"



IntervalNeighborhoods::IntervalNeighborhoods(const Foliation & foliation) :
    m_cuttingPoints(foliation.numSeparatrices()),
    m_foliation(foliation)
{
}



// \param  newCuttingPoint     It must not coincide with any of the division points.
void IntervalNeighborhoods::insertPoint(const Mod1Number& newCuttingPoint, int indexOfInterval){
    assert(newCuttingPoint.side() != Direction::CENTER);
    if (m_cuttingPoints[indexOfInterval].isEmpty) {
        m_cuttingPoints[indexOfInterval].first = m_cuttingPoints[indexOfInterval].second = newCuttingPoint;
        m_cuttingPoints[indexOfInterval].isEmpty = false;
    } else if (newCuttingPoint < m_cuttingPoints[indexOfInterval].first)
        m_cuttingPoints[indexOfInterval].first = newCuttingPoint;
    else if (m_cuttingPoints[indexOfInterval].second < newCuttingPoint)
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
bool IntervalNeighborhoods::containsInTwoSidedInterval(const Mod1Number& point, unsigned int indexOfInterval,
                                                       unsigned int indexOfOneSidedDivPoint) const{
    if (m_cuttingPoints[indexOfInterval].isEmpty) {
        return true;
    }
    unsigned int nextIndex = Modint(indexOfInterval + 1, m_cuttingPoints.size());

    if ((point < m_cuttingPoints[indexOfInterval].first && indexOfInterval != indexOfOneSidedDivPoint)
            || (m_cuttingPoints[indexOfInterval].second < point && nextIndex != indexOfOneSidedDivPoint))
    {
        return true;
    }
    return false;
}






bool IntervalNeighborhoods::containsIntervalThroughADivPoint(const Mod1Number& leftEndPoint, unsigned int leftIndexOfInterval,
                                                                  const Mod1Number& rightEndPoint, unsigned int rightIndexOfInterval,
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

    for (Modint i(leftIndexOfInterval + 1, m_foliation.numSeparatrices()); i != rightIndexOfInterval; ++i) {
        if (!m_cuttingPoints[i].isEmpty) {
            return false;
        }
    }

    for (Modint i(leftIndexOfInterval, m_foliation.numSeparatrices()); i != rightIndexOfInterval; ++i) {
        if ((throughTopDivPointQ && m_foliation.isTopDivPoint(i + 1)) ||
            (!throughTopDivPointQ && !m_foliation.isTopDivPoint(i + 1))) {
            return true;
        }
    }

    return false;
}



std::ostream & operator<<(std::ostream &out, const IntervalNeighborhoods& inh)
{
    for (unsigned int i = 0; i < inh.m_cuttingPoints.size(); i++){
        if (inh.m_cuttingPoints[i].isEmpty) {
            out << "() ";
        } else
            out << "(" << inh.m_cuttingPoints[i].first << "," << inh.m_cuttingPoints[i].second << ") ";
    }
    return out;
}







/*! The union of cutting points in all the IntervalNeighborhoods are taken and the resulting object,
 * generally with shorter intervals around the division point are returned.
 *
 * \param inbhVector    The IntervalNeighborhoods specified as arguments must be compatible, e.g. belong to the
 *                      same set of division points. But this is never a problem because one only calls this
 *                      function for IntervalNeighborhoods constructed from the same Foliation. *
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
    return inbh;
}





 









