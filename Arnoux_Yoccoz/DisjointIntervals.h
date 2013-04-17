#ifndef DISJOINTINTERVALS_H
#define DISJOINTINTERVALS_H

#include <vector>
#include "UnitIntervalPoint.h"

class DisjointIntervals{
public:
    DisjointIntervals() {};
    DisjointIntervals(const std::vector<UnitIntervalPoint>& unsortedPoints, bool wrapsAroundZero);
    const std::vector<UnitIntervalPoint>& points() const { return m_points; }
    bool wrapsAroundZero() const { return m_wrapsAroundZero; }
    bool containsQ(const UnitIntervalPoint& point) const;
    floating_point_type totalLength() const { return m_totalLength; }
    std::string print() const;
private:
    std::vector<UnitIntervalPoint> m_points;
    bool m_wrapsAroundZero;
    floating_point_type m_totalLength;
};
#endif // DISJOINTINTERVALS_H
