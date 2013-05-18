#include "DisjointIntervals.h"
#include "../intex/ContainingInterval.h"

#include <cassert>



balazs::DisjointIntervals::DisjointIntervals(const std::vector<Mod1Number>& unsortedPoints, bool wrapsAroundEnds) :
    m_endpoints(unsortedPoints),
    m_wrapsAroundEnds(wrapsAroundEnds),
    m_totalLength(0)
{
    assert(unsortedPoints.size() % 2 == 0);
    std::sort(m_endpoints.begin(), m_endpoints.end());
    for (std::size_t i = 0; i < m_endpoints.size(); i += 2) {
        m_totalLength += distanceBetween(m_endpoints[i], m_endpoints[i + 1]);
    }
    m_totalLength = wrapsAroundEnds ? 1 - m_totalLength : m_totalLength;
}


bool balazs::DisjointIntervals::contains(const Mod1Number& point) const {
    assert(point.side() == HDirection::Center);
    int containingIntervalIndex = containingInterval(m_endpoints, point);

    if ((containingIntervalIndex % 2 == 0 && !m_wrapsAroundEnds) ||
            (containingIntervalIndex % 2 == 1 && m_wrapsAroundEnds)) {
        return true;
    }
    return false;
}






