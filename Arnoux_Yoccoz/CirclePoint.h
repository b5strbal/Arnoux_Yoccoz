/** 
 * @file CirclePoint.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 6, 2013
 *
 * @brief   This file contains the CirclePoint class.
 */


#ifndef ArnouxYoccoz_CirclePoint_h
#define ArnouxYoccoz_CirclePoint_h

#include <iostream>
#include <cmath>
#include "global.h"



/**
 * @brief   A direction on a circle.
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 6, 2013
 * @details We look at circles from outside, which means that LEFT translates to the clockwise direction, and RIGHT to the counterclockwise
 *          direction. Often these directions are used to shift a point on the circle to one direction, usually by an infinitesimally small
 *          amount, while CENTER indicates that there is no shift.
 * @see     CirclePoint
 * @see     FoliationRP2
 */
enum LeftOrRight{
    LEFT,
    RIGHT,
    CENTER
};


/**
 * @brief   Returns the opposite of a LeftOrRight. E.g. Opposite(LEFT) = RIGHT, Opposite(CENTER) = CENTER.
 */
inline LeftOrRight Opposite(LeftOrRight Side){
    switch (Side) {
        case LEFT:
            return RIGHT;
        case RIGHT:
            return LEFT;
        default:
            return CENTER;
    }
}









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
 *          For example CirclePoint(0.2, RIGHT) is just a little to the right from CirclePoint(0.2), and is between CirclePoint(0.2) 
 *          and CirclePoint(0.200000000000001), no matter how close this number is to 2 as long as they are not considered equal by the computer.
 *
 * @see     Arc, ArcsAroundDivPoints, FoliationDisk
 */
class CirclePoint {        
public:
    /**
     * @brief   Constructor for a generalized point
     * @param Position  It can be any real number. It is taken mod 1 to get the position in the interval [0,1).
     * @param Side      If not specified, an ordinaty point is constructed. If LEFT or RIGHT is specified, a generalized point is constructed.
     *
     */
    explicit CirclePoint(floating_point_type Position = 0, LeftOrRight Side = CENTER) : m_Position(FracPart(Position)), m_Side(Side) {}
    
    /**
     * @brief   Returns the position as a number in [0,1), ignoring the small shift for generalized points.
     */
    inline floating_point_type GetPosition() const { return m_Position; }
    
    
    /**
     * @brief   Returns the side towards which the point is shifted to.
     */
    inline LeftOrRight GetSide() const{ return m_Side; }                        

    
    /** 
     * @brief   Returns the antipodal (opposite) of the point. For example, the antipodal of CirclePoint(0.72, RIGHT) is CirclePoint(0.22, RIGHT).
     */
    inline CirclePoint Antipodal() const{ return CirclePoint(m_Position + 0.5, m_Side); }
    
    
    /**
     *  @brief  Returns the shifted version of the point to LEFT, RIGHT, or CENTER (which corresponds to the case of no shift).
     *          E.g. CirclePoint(0.2).ShiftTo(LEFT) is CirclePoint(0.2, LEFT).
     */
    inline CirclePoint ShiftTo(LeftOrRight Side) const{ return CirclePoint(m_Position, Side); }


    
    /**
     * @brief   Returns the distance between two points. 
     * @details It looks for c2 on the RIGHT of c1, so the return value may be bigger than 0.5. Ignored the shifts for generalized points.
     *          E.g. DistaceBetween(CirclePoint(0.8, LEFT), CirclePoint(0.1)) is 0.3.
     */
    friend floating_point_type DistanceBetween(const CirclePoint& c1, const CirclePoint& c2);
    
    
    /**
     * @brief   Decides if a point is between other two.
     * @details Decides if cThird is contained in the closed arc lying on the right side of cLeft and the left side of cRight.
     *          E.g. IsBetween(CirclePoint(0.5), CirclePoint(0.2), CirclePoint(0.2, LEFT or CENTER)) returns true, but
     *          IsBetween(CirclePoint(0.5), CirclePoint(0.2), CirclePoint(0.2, RIGHT)) returns false.
     *  
     *          Although it is not the main use of generalized points, notice, that they can be used to with the IsBetween function to
     *          check containments in open intervals. 
     *          For example, IsBetween(CirclePoint(0.3, RIGHT), CirclePoint(0.6, LEFT), CirclePoint(x)) returns true if and only if
     *          x is in the open interval (0.3, 0.6).
     */
    friend bool IsBetween(const CirclePoint& cLeft, const CirclePoint& cRight, const CirclePoint& cThird);
    

    /**
     * @brief   Shifts the point c into the positive direction (to the RIGHT) by Distance. Preserves the small shift for generalized points.
     */
    inline friend CirclePoint operator+(const CirclePoint& c, floating_point_type Distance){ return CirclePoint(c.m_Position + Distance, c.m_Side);}
    
    /**
     * @brief   Shifts the point c into the negative direction (to the LEFT) by Distance. Preserves the small shift for generalized points.
     */
    inline friend CirclePoint operator-(const CirclePoint& c,floating_point_type Distance){ return c + (-Distance); }
    
    /**
     * @brief   Takes the negative of a point in a natural way. E.g. -CirclePoint(0.3, RIGHT) is CirclePoint(0.7, LEFT).
     * @details Geometrically, the effect is a reflection of the point c to the line passing through 0 and the center of the circle. 
     */
    inline friend CirclePoint operator-(const CirclePoint& c){ return CirclePoint(-c.m_Position, Opposite(c.m_Side)); }
    
    /**
     * @brief   Decides if two points are equal. To be equal, the points have to have the same position and shift to the same side.
     */
    inline friend bool operator==(const CirclePoint& c1, const CirclePoint& c2){ return c1.m_Position == c2.m_Position && c1.m_Side == c2.m_Side; }
    
    /**
     * @brief   Decides if two points are not equal.
     */
    inline friend bool operator!=(const CirclePoint& c1, const CirclePoint& c2){ return c1 == c2 ? false : true; }
    
    /**
     * @brief   Prints out a CirclePoint.
     */
    friend std::ostream& operator<<(std::ostream& Out, const CirclePoint& c){
        Out << c.m_Position;
        switch(c.m_Side){
            case RIGHT: Out << "(+)";
                break;
            case LEFT: Out << "(-)";
                break;
            case CENTER:
                break;
        }
        return Out;
    }
    
    
private:
    /**
     * @brief   The position of the point is described by this number which lies in the interval [0,1).
     */
    floating_point_type m_Position;
    /**
     * @brief   If its value is CENTER, the point is an ordinary point, otherwise it is a generalized point shifted in the appropriate 
     *          direction by a very small amount.
     */
    LeftOrRight m_Side;
    
    
    
    
    
    /// @cond
    /**
     * @brief   Calculates the fractional part of a real number. E.g. FracPart(2.3) = 0.3, FracPart(-1.8) = 0.2.
     */
    inline static floating_point_type FracPart(floating_point_type RealNumber){ return RealNumber - floor(RealNumber); }
    
    /**
     * @brief   Decides whether c1 is before c2 on the segment [0,1), now ignoring that the endpoints of this interval were identified.
     * @details This determines a total ordering of the points. The first element is CirclePoint(0, LEFT), which is technically the
     *          last point in [0,1), so in that sense that description was not accurate. But other than that the order is what is expected:
     *          CirclePoint(0, LEFT), CirclePoint(0), CirclePoint(0, RIGHT), ..., CirclePoint(0.99999999999, RIGHT) or whatever the
     *          last floating point number in [0,1) is in the given floating point type.
     *
     *          The purpose of this function is simply to make the code for IsBetween, and DistanceBetween simpler.
     */
    inline friend bool operator<(const CirclePoint& c1, const CirclePoint& c2);
    
    /**
     * @brief   Same as operator<, but allows equality, too.
     */
    inline friend bool operator<=(const CirclePoint& c1, const CirclePoint& c2){ return c1 < c2 || c1 == c2 ? true : false;}

    /// @endcond
};


        
        
        
        


    








#endif
