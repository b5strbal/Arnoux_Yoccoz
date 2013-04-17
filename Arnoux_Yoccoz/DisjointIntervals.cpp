#include "disjointintervals.h"


//------------------------------//
// Foliation::DisjointIntervals //
//------------------------------//



DisjointIntervals::DisjointIntervals(const std::vector<UnitIntervalPoint>& unsortedPoints, bool wrapsAroundZero) :
m_points(unsortedPoints),
m_wrapsAroundZero(wrapsAroundZero),
m_totalLength(0)
{
    assert(unsortedPoints.size() % 2 == 0);
    std::sort(m_points.begin(), m_points.end());
    for (unsigned int i = 0; i < m_points.size(); i += 2) {
        m_totalLength += distanceBetween(m_points[i], m_points[i + 1]);
    }
    m_totalLength = wrapsAroundZero ? 1 - m_totalLength : m_totalLength;
}


bool DisjointIntervals::containsQ(const UnitIntervalPoint& point) const {
    int containingIntervalIndex = containingInterval(m_points, point);
    if (containingIntervalIndex == CONTAINING_INTERVAL_NOT_UNIQUE) {
        throw std::runtime_error("DisjointIntervals::containsQ : Containing interval is not unique.");
    }
    if ((containingIntervalIndex % 2 == 0 && !m_wrapsAroundZero) || (containingIntervalIndex % 2 == 1 && m_wrapsAroundZero)) {
        return true;
    }
    return false;
}





std::string DisjointIntervals::print() const{
    std::ostringstream s;
    for (unsigned int i = m_wrapsAroundZero ? 1 : 0; i < m_points.size() - 2; i += 2) {
        s << printInterval(m_points[i], m_points[i + 1]) << " ";
    }
    if (m_wrapsAroundZero) {
        s << printInterval(m_points.back(), m_points.front());
    } else
        s << printInterval(m_points[m_points.size() - 2], m_points.back());
    return s.str();
}
