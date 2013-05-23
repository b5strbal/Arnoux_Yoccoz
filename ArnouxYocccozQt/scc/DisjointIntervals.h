#ifndef DISJOINTINTERVALS_H
#define DISJOINTINTERVALS_H

#include "../intex/Mod1NumberIntExchange.h"

namespace balazs{


/*!
 * \brief The DisjointIntervals class stores a union or disjoint subintervals of the unit interval.
 * \author Balazs Strenner, strenner@math.wisc.edu
 * \date April 19, 2013
 *
 * The endpoints of the unit interval are identified, so there might be an interval that wraps
 * around the ends. So intervals can be described by their endpoints and whether there is an interval
 * that wraps around the ends or not.
 *
 */

class DisjointIntervals{
public:
  //  DisjointIntervals() {} // Empty set

    DisjointIntervals(const std::vector<Mod1NumberIntExchange>& unsortedPoints, bool wrapsAroundEnds);
    DisjointIntervals(const DisjointIntervals&) = delete;
    DisjointIntervals& operator=(const DisjointIntervals&) = delete;

    const std::vector<Mod1NumberIntExchange>& endpoints() const { return m_endpoints; }
    bool wrapsAroundEnds() const { return m_wrapsAroundEnds; }
    bool contains(const Mod1Number& point) const;
    Mod1NumberIntExchange totalLength() const { return m_totalLength; }

private:
    std::vector<Mod1NumberIntExchange> m_endpoints; // The ordered list of endpoints.
    bool m_wrapsAroundEnds;              // Whether 0 is in one of the intervals.
    Mod1NumberIntExchange m_totalLength;           // The total length of the intervals.
};


}

#endif // DISJOINTINTERVALS_H
