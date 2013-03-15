/** 
 * @file UnitIntervalPoint.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 6, 2013
 *
 * @brief   This file contains the UnitIntervalPoint class.
 */


#ifndef ArnouxYoccoz_UnitIntervalPoint_h
#define ArnouxYoccoz_UnitIntervalPoint_h

#include <iostream>
#include <cmath>
#include <cassert>
#include "global.h"





/**
 * @brief   A generalized point of the circle of circumference 1.
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 6, 2013
 * 
 * @details Take the interval [0,1], and glue the endpoints together to get a circle of circumference one. Points on this circle can be
 *          represented by the floating point number 0 <= m_Position < 1. Since the computer can't handle exact numbers, we actually
 *          only represent a discrete subset of it.
 *
 *          To have a little more flexibility, we want to have the notion of a point that is shifted by a very small amount from this 
 *          discrete set to either direction such that the shifted point is still closer to the origial point than closest point in the discrete
 *          set in that direction. These generalized points are represented by the private members 0 <= m_Position < 1, and a m_Side of type 
 *          LeftOrRight, which is either LEFT, CENTER or RIGHT. For the ordinary points discussed above, m_Side is CENTER.
 *
 *          For example UnitIntervalPoint(0.2, RIGHT) is just a little to the right from UnitIntervalPoint(0.2), and is between UnitIntervalPoint(0.2) 
 *          and UnitIntervalPoint(0.200000000000001), no matter how close this number is to 2 as long as they are not considered equal by the computer.
 *
 * @see     Arc, ArcsAroundDivPoints, FoliationDisk
 */
class UnitIntervalPoint {        
public:
    /**
     * @brief   
     */
    UnitIntervalPoint(floating_point_type position = 0, int epsilon = 0) : m_position(position), m_epsilon(epsilon) {
        assert(0 <= position && position < 1);
    }
    
    /**
     *
     */
    inline floating_point_type getPosition() const{ return m_position; }
    
    /**
     *  @brief  
     */
    inline UnitIntervalPoint ShiftToRight() const{ return UnitIntervalPoint(m_position, m_epsilon + 1); }


    
    /**
     * @brief   Returns the distance between two points. 
     * @details It looks for c2 on the RIGHT of c1, so the return value may be bigger than 0.5. Ignored the shifts for generalized points.
     *          E.g. DistaceBetween(UnitIntervalPoint(0.8, LEFT), UnitIntervalPoint(0.1)) is 0.3.
     */
    friend floating_point_type DistanceBetween(const UnitIntervalPoint& c1, const UnitIntervalPoint& c2);
    
    
    /**
     * @brief   Decides if a point is between other two.
     * @details Decides if cThird is contained in the closed arc lying on the right side of cLeft and the left side of cRight.
     *          E.g. IsBetween(UnitIntervalPoint(0.5), UnitIntervalPoint(0.2), UnitIntervalPoint(0.2, LEFT or CENTER)) returns true, but
     *          IsBetween(UnitIntervalPoint(0.5), UnitIntervalPoint(0.2), UnitIntervalPoint(0.2, RIGHT)) returns false.
     *  
     *          Although it is not the main use of generalized points, notice, that they can be used to with the IsBetween function to
     *          check containments in open intervals. 
     *          For example, IsBetween(UnitIntervalPoint(0.3, RIGHT), UnitIntervalPoint(0.6, LEFT), UnitIntervalPoint(x)) returns true if and only if
     *          x is in the open interval (0.3, 0.6).
     */
    // friend bool IsBetween(const UnitIntervalPoint& cLeft, const UnitIntervalPoint& cRight, const UnitIntervalPoint& cThird);
    

    /**
     * @brief   
     */
    friend UnitIntervalPoint operator+(const UnitIntervalPoint& p1, floating_point_type distance);
    
    /**
     * @brief   
     */
    friend UnitIntervalPoint operator-(const UnitIntervalPoint& p1, floating_point_type distance);
    
    
    /**
     * @brief   Decides if two points are equal. To be equal, the points have to have the same position and shift to the same side.
     */
    inline friend bool operator==(const UnitIntervalPoint& p1, const UnitIntervalPoint& p2){
        return p1.m_position == p2.m_position && p1.m_epsilon == p2.m_epsilon;
    }
    
    /**
     * @brief   Decides if two points are not equal.
     */
    inline friend bool operator!=(const UnitIntervalPoint& c1, const UnitIntervalPoint& c2){ return c1 == c2 ? false : true; }
    
    /**
     * @brief   Prints out a UnitIntervalPoint.
     */
    friend std::ostream& operator<<(std::ostream& Out, const UnitIntervalPoint& p){
        Out << p.m_position;
        if (p.m_epsilon > 0)
            Out << "(+" << p.m_epsilon << "eps)";
        else if (p.m_epsilon < 0)
        Out << "(" << p.m_epsilon << "eps)";        
        return Out;
    }
    
    
private:
    /**
     * @brief   The position of the point is described by this number which lies in the interval [0,1).
     */
    floating_point_type m_position;
    /**
     * @brief   If its value is CENTER, the point is an ordinary point, otherwise it is a generalized point shifted in the appropriate 
     *          direction by a very small amount.
     */
    int m_epsilon;
    
    
    
    
    
    /**
     * @brief   Calculates the fractional part of a real number. E.g. FracPart(2.3) = 0.3, FracPart(-1.8) = 0.2.
     */
    inline static floating_point_type FracPart(floating_point_type x){ return x - floor(x); }
    
    /**
     * @brief   Decides whether c1 is before c2 on the segment [0,1), now ignoring that the endpoints of this interval were identified.
     * @details This determines a total ordering of the points. The first element is UnitIntervalPoint(0, LEFT), which is technically the
     *          last point in [0,1), so in that sense that description was not accurate. But other than that the order is what is expected:
     *          UnitIntervalPoint(0, LEFT), UnitIntervalPoint(0), UnitIntervalPoint(0, RIGHT), ..., UnitIntervalPoint(0.99999999999, RIGHT) or whatever the
     *          last floating point number in [0,1) is in the given floating point type.
     *
     *          The purpose of this function is simply to make the code for IsBetween, and DistanceBetween simpler.
     */
    inline friend bool operator<(const UnitIntervalPoint& c1, const UnitIntervalPoint& c2);
    
    /**
     * @brief   Same as operator<, but allows equality, too.
     */
    inline friend bool operator<=(const UnitIntervalPoint& c1, const UnitIntervalPoint& c2){ return c1 < c2 || c1 == c2 ? true : false;}

    /// @endcond
};


        
        
        
        


    








#endif
