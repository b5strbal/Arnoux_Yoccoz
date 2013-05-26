#ifndef INTERVALNEIGHBORHOODS_H
#define INTERVALNEIGHBORHOODS_H

#include "Mod1NumberIntExWithInfo.h"

namespace balazs{



/*!
 * \brief   A collection of intervals around a set of division points on the unit interval.
 * \author  Balazs Strenner, strenner@math.wisc.edu
 * \date    April 18, 2013
 *
 * The endpoints of the unit interval are considered to be identified, so one can equally imagine that as a
 * circle of circumference 1.
 *
 * Given a set of UnitIntervalPoints on the unit interval, called the "division points", an IntervalNeighborhoods
 * object is a collection of intervals around the division points with the following properties:
 *
 * 1) There is one interval assigned to each division point, so the number of intervals and division points
 *    are equal.
 * 2) The division points are ordinary UnitIntervalPoints (i.e. not generalized ones).
 * 3) Each division point is contained in the inside of the corresponding interval.
 * 4) It is true for any pair of intervals in the collection that they either coincide,
 *    or touch each other only at the endpoints, or they are disjoint.
 *
 * These objects naturally appear in the algorithm of finding so-called "good separatrix segments" for Foliations.
 * We will constuct IntervalNeighborhoods objects iteratively from an empty object: at construction, but no
 * intervals are assigned to the division points. This represents the idea that in this case the interval
 * belonging to each division point is the whole circle, but since it is not really an interval, we simply handle
 * this situation by not assigning intervals at all.
 *
 * Then, we start to add "cutting points" to the picture. When a cutting point is added, each interval that contains
 * it is cut in half along it, and the smaller intervals obtained which do not contain any division points are
 * discarded. After a while we are going to have a lot of cutting points that separate the unit interval into
 * small intervals, but only keep the intervals around the division points, and only keep the cutting points that
 * are endpoints of these intervals.
 *
 * Internally the division points are actually not even stored. So any time a point is inserted or provided as an
 * argument, the index of the interval between the division points has to be provided. The method of these class
 * don't check if the point is in the interval, the caller has to make sure it is correct. The reason for this design
 * is that there are points used often and this way it is sufficient to calculate the containing interval only once,
 * instead of the IntervalNeighborhoods class calculating it over and over.
 *
 */
class IntervalNeighborhoods{
public:
    IntervalNeighborhoods(const Foliation & foliation); // empty object
    IntervalNeighborhoods(const std::vector<const IntervalNeighborhoods*>& inbhVector);
  //  IntervalNeighborhoods(const IntervalNeighborhoods&) = delete;
    IntervalNeighborhoods& operator=(const IntervalNeighborhoods&) = delete;

    const Foliation& foliation() const { return m_foliation; }
    void insertPoint(const Mod1NumberIntExWithInfo& newCuttingPoint);

    // Decides if the point is contained in an intervals which is two sided.
    bool contains(const Mod1NumberIntExWithInfo& point) const;

    // Returns true if the interval between two points is contained in any of the member intervals and passes
    // through either a TopDivPoint or BottomDivPoint, whichever is specified.
    bool containsIntervalThroughADivPoint(const Mod1NumberIntExWithInfo& leftEndPoint,
                                      const Mod1NumberIntExWithInfo& rightEndPoint,
                                      bool throughTopDivPointQ) const;

private:
    static bool localLess(const Mod1NumberIntExWithInfo& lhs, const Mod1NumberIntExWithInfo& rhs);

    struct CuttingPoints{
        Mod1NumberIntExWithInfo first;
        Mod1NumberIntExWithInfo second;
        bool isEmpty;
    public:
        CuttingPoints() : first(), second(), isEmpty(true) {}
    };


    const Foliation& m_foliation;
    std::vector<CuttingPoints> m_cuttingPoints;
    /* We store the cutting points grouped according to which interval they are in. So m_cuttingpoints has size
     * of the number of division points, and each element stores the left and right cutting point in the interval.
     * If there is no cutting point yet in an interval, CuttingPoints::isEmpty is set to true, otherwise it is false.
     * If there is one cutting point in the interval, then CuttingPoints::first and CuttingPoints::second concide.
     * So actually we store the complement of the union of the interval neighborhoods by listing the gaps in
     * each interval between the division points.
     */
};




}



#endif
