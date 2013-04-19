#ifndef DISJOINTINTERVALS_H
#define DISJOINTINTERVALS_H

#include <vector>
#include "UnitIntervalPoint.h"

/*!
 * \brief The DisjointIntervals class stores a union or disjoint subintervals of the unit interval.
 * \author Balazs Strenner, strenner@math.wisc.edu
 * \date April 19, 2013
 * \see UnitIntervalPoint
 *
 * The endpoints of the unit interval are identified, so there might be an interval that wraps
 * around 0. So intervals can be described by their endpoints and whether there is an interval
 * that wraps around zero or not. If 0 is the endpoint of one of the intervals, it is somewhat
 * ambiguous what is means, but in our use the endpoints never coincide with division points
 * of a Foliation, and since 0 is always a division point, this is not really an issue.
 *
 */

class DisjointIntervals{
public:
    //! Default constructor, can be thought of the constructor of an empty set.
    DisjointIntervals() {};

    //! \brief Constructs an object from an unordered list of points, wrapping around 0 or not.
    //! \warning Whether or not there are two coinciding points or two that are too close to
    //!          each other is not checked.
    DisjointIntervals(const std::vector<UnitIntervalPoint>& unsortedPoints, bool wrapsAroundZero);

    //! Returns the ordered list of endpoints.
    const std::vector<UnitIntervalPoint>& endpoints() const { return m_endpoints; }

    //! Decides if there is an interval that wraps around 0.
    bool wrapsAroundZero() const { return m_wrapsAroundZero; }

    //! Decides if a point is contained in any of the intervals.
    //! \throws ExceptionContainingIntervalNotUnique    If point is too close to one of the endpoints.
    bool contains(const UnitIntervalPoint& point) const;

    //! Returns the total length of the intervals.
    floating_point_type totalLength() const { return m_totalLength; }

    //! Prints the objects to a string.
    std::string print() const;
private:
    std::vector<UnitIntervalPoint> m_endpoints; // The ordered list of endpoints.
    bool m_wrapsAroundZero;                     // Whether 0 is in one of the intervals.
    floating_point_type m_totalLength;          // The total length of the intervals.
};
#endif // DISJOINTINTERVALS_H
