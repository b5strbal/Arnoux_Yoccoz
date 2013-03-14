/**
 * @file ArcsAroundDivPoints.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 6, 2013
 *
 * @brief   This file contains the ArcsAroundDivPoints class.
 */


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
 *
 ******************************************************************************/


#ifndef ArnouxYoccoz_ArcsAroundDivPoints_h
#define ArnouxYoccoz_ArcsAroundDivPoints_h

#include <iostream>
#include <vector>
#include <cassert>
#include "CirclePoint.h"
//#include "Arc.h"

/**
 * @brief   A collection of arcs around a set of points on the circle of circumference 1.
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 6, 2013
 * @details Given a set of CirclePoints on the circle of circumference 1, called the "division points", an ArcAroundDivPoints object
 *          is just a collection of arcs around the division points with some properties:
 *        
 *          1) There is one Arc assigned to each division point, so the number of Arcs and division points are equal.
 *
 *          2) The division points are ordinary CirclePoints.
 *
 *          3) Each division point is contained in the inside of the corresponding Arc.
 *
 *          4) It is true for any pair of Arcs in the collection that they either coincide, or touch each other only at the endpoints,
 *             or they are disjoint.
 *
 *          To better understand their structure, it might be better to understand how we are constructing them. First of all, we construct
 *          an empty object, which means that we initialize the division points, but no Arcs are assigned to them. This represents the idea
 *          that in this case the "arc" belonging to each division point is the whole circle, but since it is not really an arc, and it is 
 *          not represented by the Arc class, we simply handle this situation by not assigning an Arc to the division points.
 *
 *          Then, we start to add "cutting points" to the picture. When a cutting point is added, each Arc that contains it is cut in half
 *          along it, and the smaller Arcs obtained which do not contain any division points are discarded. In other words, we look at 
 *          each division point and their corresponding Arc in order, and we try to cut the Arcs in two by the cutting point and keep the
 *          Arc that contains the division point. Since the cutting points are always generalized CirclePoints, one can see that the above
 *          properties are satisfied.
 *
 *          In particular, when a cutting point is inserted in an empty object, then the whole circle has to be divided along the cutting
 *          point, the Arc of length 1, both endpoints being the cutting point will the Arc that is assigned to each division point.
 *
 * @see     CirclePoint, Arc, FoliationDisk
 */

class Foliation{
public:
    int getNumDivPoints() const { return m_NumDivPoints; }
    
private:
    int m_NumDivPoints;
};


class ArcsAroundDivPoints{
public:
    /**
     * @brief   Constructor of empty object.
     * @param ShiftedDivPoints     The division points have to be regular (not generalized) CirclePoints. 
     *                      Also, there has to be at least one division point.
     */
    ArcsAroundDivPoints(const Foliation* foliation) :
        m_CuttingPoints(foliation->getNumDivPoints(), std::vector<CirclePoint>(0)),
        m_Foliation(foliation)
    {
    }

    /**
     * @brief   Decides whether the object is empty, i.e. there are no cutting points yet.
     */
    //inline bool IsEmpty() const { return m_Arcs.size() == 0; }
    
    /**
     * @brief   Inserts a new cutting point and updates the Arcs.
     * @param NewCuttingPoint   It must not coincide with any of the division points, because in that case case the operation won't be
     *                          well-defined.
     */
    void InsertPoint(const CirclePoint& NewCuttingPoint, int IndexOfInterval);
    
    /**
     * @brief   Decides if a point is contained in any of the Arcs.
     */
    bool ContainsQ(const CirclePoint& cp, int IndexOfInterval) const;
    
    /**
     * @brief   Takes the intersection of two ArcsAroundDivPoints objects.
     * @details The intersection of the two Arcs corresponding to each division point is taken and that is assigned to the division point
     *          it the intersection. The operation is well-defined, because although in general the intersection of two arcs on the circle
     *          might not be a single arc, in our case it can't happen because of the properties the Arcs in an ArcAroundDivPoints object
     *          have to satisfy.
     *
     *          The motivation for this operation is to take the union of two sets of cutting points. A set of cutting points determines
     *          an ArcsAroundDivPoints, another set determines another one, and the union of the two sets determines exactly the 
     *          intersection of the two objects.
     */
    friend ArcsAroundDivPoints Intersect(const ArcsAroundDivPoints& adp1, const ArcsAroundDivPoints& adp2);
    
    /**
     * @brief   Returns true if the an Arc is contained in any of the member Arcs and it passes through the division point contained in the
     *          member Arc.
     */
    bool ContainsArcThroughADivPointQ(const CirclePoint& LeftEndPoint, int LeftIndexOfInterval,
                                      const CirclePoint& RightEndPoint, int RightIndexOfInterval) const;
    
    /**
     * @brief   Prints out the division points and all the Arcs.
     */
    //friend std::ostream& operator<<(std::ostream& Out, const ArcsAroundDivPoints& adp);

private:
    /**
     * @brief   
     */
    
    const Foliation* m_Foliation;
    std::vector<std::vector<CirclePoint>> m_CuttingPoints;
};






/*
 * @brief   Decides if an Arc contains any of the division points.
 */
//bool ContainsADivPoint(const Arc& arc) const;

/*
 * @brief   Decides if any of the member Arcs contains the specified Arc.
 */
//bool ContainsArcQ(const Arc& arc) const;




#endif
