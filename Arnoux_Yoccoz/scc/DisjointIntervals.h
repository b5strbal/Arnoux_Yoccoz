#ifndef DISJOINTINTERVALS_H
#define DISJOINTINTERVALS_H

#include <vector>
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
    DisjointIntervals() {} // Empty set

    //! \brief Constructs an object from an unordered list of points, wrapping around 0 or not.
    //! \warning Whether or not there are two coinciding points or two that are too close to
    //!          each other is not checked.
    DisjointIntervals(const std::vector<Mod1Number>& unsortedPoints, bool wrapsAroundEnds);

    //! Returns the ordered list of endpoints.
    const std::vector<Mod1Number>& endpoints() const { return m_endpoints; }

    //! Decides if there is an interval that wraps around 0.
    bool wrapsAroundEnds() const { return m_wrapsAroundEnds; }

    //! Decides if a point is contained in any of the intervals.
    //! \throws ExceptionContainingIntervalNotUnique    If point is too close to one of the endpoints.
    bool contains(const Mod1Number& point) const;

    //! Returns the total length of the intervals.
    floating_point_type totalLength() const { return m_totalLength; }

    //! Prints the objects to a string.
    friend std::ostream& operator<<(std::ostream& out, const DisjointIntervals& d);
private:
    std::vector<Mod1Number> m_endpoints; // The ordered list of endpoints.
    bool m_wrapsAroundEnds;                     // Whether 0 is in one of the intervals.
    floating_point_type m_totalLength;          // The total length of the intervals.
};


}

#endif // DISJOINTINTERVALS_H
