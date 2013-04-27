#include "DisjointIntervals.h"


//------------------------------//
// Foliation::DisjointIntervals //
//------------------------------//



DisjointIntervals::DisjointIntervals(const std::vector<Mod1Number>& unsortedPoints, bool wrapsAroundEnds) :
    m_endpoints(unsortedPoints),
    m_wrapsAroundEnds(wrapsAroundEnds),
    m_totalLength(0)
{
    assert(unsortedPoints.size() % 2 == 0);
    std::sort(m_endpoints.begin(), m_endpoints.end());
    for (unsigned int i = 0; i < m_endpoints.size(); i += 2) {
        m_totalLength += distanceBetween(m_endpoints[i], m_endpoints[i + 1]);
    }
    m_totalLength = wrapsAroundEnds ? 1 - m_totalLength : m_totalLength;
}


bool DisjointIntervals::contains(const Mod1Number& point) const {
    assert(point.side() == Direction::CENTER);
    int containingIntervalIndex = containingInterval(m_endpoints, point);

    if ((containingIntervalIndex % 2 == 0 && !m_wrapsAroundEnds) ||
            (containingIntervalIndex % 2 == 1 && m_wrapsAroundEnds)) {
        return true;
    }
    return false;
}





std::ostream& operator<<(std::ostream& out, const DisjointIntervals& d){
    for (unsigned int i = d.m_wrapsAroundEnds ? 1 : 0; i < d.m_endpoints.size() - 2; i += 2) {
        out << interval_t({d.m_endpoints[i], d.m_endpoints[i + 1]}) << " ";
    }
    if (d.m_wrapsAroundEnds) {
        out << interval_t({d.m_endpoints.back(), d.m_endpoints.front()});
    } else
        out << interval_t({d.m_endpoints[d.m_endpoints.size() - 2], d.m_endpoints.back()});
    return out;
}
