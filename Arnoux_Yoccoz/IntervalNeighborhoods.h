

/*
 
    MOTIVATION: This class is the heart of all calculations. The use of this class is primarily
                finding simple closed curves on RP^2 or on the sphere that are transverse to a given
                measured foliation.
 
                The basic idea is that every simple closed curve has be homotoped so that it is a chain
                of arcs which are alternatingly either leaf segments or transverse arcs. The leaf segments 
                can be homotoped in the transverse direction of the foliation until they hit a singularity,
                so the leaf segments can be assumed to be separatrix segments. The the basic idea of finding
                simple closed curves is to consider separatrix segments of different lengths and try to connect
                their endpoints transversely without intersecting the curve itself. (These closed curves are not
                transverse yet and can't always be made transverse, but it is easy to check which one correspond to 
                transverse curves.)
 
                These transverse arcs that will connect the separatrix segments will be taken near and parallel to
                the boundary circle in the case of RP^2 and near the equator in the case of the sphere.
                For simplicity, assume now that the endpoint of the separatrices are always on the boundary circle or
                on the equator, in the future called only the Circle.
 
                So for a start, we can say that two endpoints of separatrix segments can be connected if at least 
                one of the two arcs between the two endpoints on the Circle don't intersect either of the
                separatrix segments. For checking this we would have to store all the intersection of the
                separatrix segments with the Circle, and check if the two endpoints are in the same subarc. 
                
                There are more problems with this approach. First, it is slow. If we don't order the intersections,
                then every the time needed to search for being in the same arc changes linearly with the number 
                of intersections, which is quite a lot of a separatrix segment of length 1 million for instance.
                So it is better to sort the intersection points. Then adding a new intersection point or checking 
                if two points are in the same arcs takes log(#intersections) time, which is much better, but it
                turns out, that storing all the intersection points is unnecessary.
 
                That brings us to the second problem with the above approach, namely that we would get each simple 
                closed curves many times. Why is that? Think about the simple case when our simple closed curve
                has only two parts, a separatrix segment, containing a singularity, and a transverse arc of the Circle,
                that connects its two endpoints. That transverse arcs can be homotoped by moving its endpoint on
                the same leaves into either direction as long as we don't hit a singularity. So we could possibly 
                push the transverse arc through the Circle multiple times before hitting a singularity. Each stages 
                along the way when the transverse arc is on the Circle gives a different representation of the
                simple closed curve. So we can restrict ourselves to connecting transverse arcs that contain
                a division point (the first intersection of a separatrix to the Circle). This means that the only
                separatrix segments that are used to build simple closed curves are those whose endpoint can be
                connected on the Circle to a division point without intersecting itself.
 
                Because of that, we don't need to store all the intersection points of a separatrix segments with the
                Circle, just the ones that are closest to the division points. More precisely: the intersection
                points divide the circle into arcs, and for each division point we only have to remember the arc that 
                contains that division point. (Actually two arcs if an intersection point coinsides with the division
                point.) 
 
                In terms of both storage and running time this is much better than previously. Making a separatrix
                segment one longer, and updating data takes only constant times, because only constant number of
                arcs are stored. Moreover, most of the separatrix segments we don't even have to store at all. We
                only have to remember the good ones, and assemble simple closed curves from them in certain conbinations
                later on. The good news is that the number of good separatrix segments grows only logarithmically 
                with their length (more precisely the number of intersections with the Circle). For the Arnoux-Yoccoz
                foliation on RP^2, there are only about 150 good separatrix segments with length less than 1 million.
 
 *
 ******************************************************************************/


#ifndef INTERVALNEIGHBORHOODS_H
#define INTERVALNEIGHBORHOODS_H

#include "Foliation.h"
//#include "Mod1.h"

class Foliation;

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
 * \see   UnitIntervalPoint, Foliation
 */
class IntervalNeighborhoods{
public:
    IntervalNeighborhoods(const Foliation & foliation); // empty object
    void insertPoint(const Mod1Number& newCuttingPoint, int indexOfInterval);

    // Decides if the point is contained in an intervals which is two sided.
    bool containsInTwoSidedInterval(const Mod1Number& point, unsigned int indexOfInterval, unsigned int indexOfOneSidedDivPoint) const;

    //! \brief Returns true if the interval between two points is contained in any of the member intervals and passes
    //!        through either a TopDivPoint or BottomDivPoint, whichever is specified.
    //! \see   Foliation
    bool containsIntervalThroughADivPoint(const Mod1Number& leftEndPoint, unsigned int leftIndexOfInterval,
                                      const Mod1Number& rightEndPoint, unsigned int rightIndexOfInterval,
                                      bool throughTopDivPointQ) const;

    //! Prints out the object into a string.
    std::string print() const;

    //! Takes the intersection of IntervalNeighborhoods objects.
    static IntervalNeighborhoods intersect(const std::vector<const IntervalNeighborhoods*>& inbhVector);




private:
    struct CuttingPoints{
        Mod1Number first;
        Mod1Number second;
        bool isEmpty = true;
    };

    /* We store the cutting points grouped according to which interval they are in. So m_cuttingpoints has size
     * of the number of division points, and each element stores the left and right cutting point in the interval.
     * If there is no cutting point yet in an interval, CuttingPoints::isEmpty is set to true, otherwise it is false.
     * If there is one cutting point in the interval, then CuttingPoints::first and CuttingPoints::second concide.
     * So actually we store the complement of the union of the interval neighborhoods by listing the gaps in
     * each interval between the division points.
     */
    std::vector<CuttingPoints> m_cuttingPoints;
    const Foliation& m_foliation;
};







#endif
