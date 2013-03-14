/**
 * @file Arc.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 6, 2013
 *
 * @brief   This file contains the Arc class.
 */



#ifndef ArnouxYoccoz_Arc_h
#define ArnouxYoccoz_Arc_h

#include <iostream>
#include <cassert>
#include "CirclePoint.h"


/**
 * @brief   Objects of this class type represent closed arcs on the circle of circumference 1.
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 6, 2013
 * @details The endpoints of an Arc object are CirclePoint objects. Arcs that degenerate to a point are not allowed. That said, Arc objects
 *          of length 0 are still possible: Arc(CirclePoint(0,LEFT), CirclePoint(0)) is for example such an arc. The DistanceBetween
 *          function returns 0 for the distance between the two endpoints.
 *
 *          However, degenerate arcs of length 1 whose two endpoints are the same are allowed.
 *          Note that even though all these degenerate arcs are the same as sets, we do distinguish them by their endpoints.
 *          E.g. the arc covering the whole circle, starting at CirclePoint(0.2, RIGHT) and ending at CirclePoint(0.2, RIGHT) is different
 *          from the arc starting at CirclePoint(0.6) and ending at CirclePoint(0.6).
 *
 *          The conventions for what left/right, negative/positive direction mean is the same as for CirclePoint: we
 *          look at the circle from the outside, so "left" means the clockwise or negative direction, "right" means the counterclockwise or 
 *          positive direction.
 *
 * @see     CirclePoint, ArcsAroundDivPoints, FoliationDisk
 */
class Arc {        
public:    
    /**
     * @brief   Constructor for the endpoints as inputs.
     * @details Notice that the constructor is well-defined even when the two endpoint parameters coincide, because we allow arcs of 
     *          length 1 with considing endpoints but not arcs of length 0 with coinciding endpoints.
     */
    Arc(const CirclePoint& LeftEndpoint, const CirclePoint& RightEndpoint) :
        m_LeftEndpoint(LeftEndpoint),
        m_RightEndpoint(RightEndpoint)
    {}
    
    inline const CirclePoint& GetLeftEndpoint() const{ return m_LeftEndpoint; }    ///< @brief  Returns a reference to the left endpoint.
    inline const CirclePoint& GetRightEndpoint() const { return m_RightEndpoint; } ///< @brief  Returns a reference to the right endpoint.
    inline floating_point_type GetLength() const{
        return m_LeftEndpoint == m_RightEndpoint ? 1 : DistanceBetween(m_LeftEndpoint, m_RightEndpoint);
    }                                                                              ///< @brief  Returns the length of the Arc.
    
    /**
     * @brief   Decides whether a CirclePoint is contained in the Arc.
     */
    inline bool ContainsQ(const CirclePoint& cp) const{
        return m_LeftEndpoint == m_RightEndpoint ? true : IsBetween(m_LeftEndpoint, m_RightEndpoint, cp);
    }
    


    
    /**
     * @brief   Cuts off a portion of the arc. The cutting happens at CuttingPoint, and of the two arcs obtained, the one containing
     *          MarkingPoint is kept, the other is discarded.
     * @param MarkingPoint  The marking point has to be contained in the arc, otherwise it is not possible to choose between the two arcs
     *                      obtained by cutting. By the same reason, if endpoints of the arc coincide, the marking point has to be different
     *                      from the endpoints.
     * @param CuttingPoint  The cutting point may not be contained in the arc, in which case no cutting happens. However, 
     *                      MarkingPoint and CuttingPoint have to be different, otherwise we are unable to choose between the arcs again.
     * @details The code uses asserts for safety to make sure that the parameters satisfy the appropriate requirements, but
     *          as long as this function is only used by the ArcsAroundDivPoints class, the parameters should always be legal. This is
     *          because the endpoints of the arcs there are always generalized CirclePoints (shifted to the LEFT or RIGHT), so are the 
     *          cutting points, but the marking points are always ordinary CirclePoints, and are contained in the arcs.
     */
    void CutOff(const CirclePoint& MarkingPoint, const CirclePoint& CuttingPoint);

    
    /**
     * @brief   Prints out the arc in interval form.
     */
    friend std::ostream& operator<< (std::ostream &Out, const Arc &MyArc){
        Out << "[" << MyArc.m_LeftEndpoint << "," << MyArc.m_RightEndpoint << "]";
        return Out;
    }
        
        
private:
    CirclePoint m_LeftEndpoint;     ///< @brief The left endpoint of the arc.
    CirclePoint m_RightEndpoint;    ///< @brief The right endpoint of the arc.
};





#endif
