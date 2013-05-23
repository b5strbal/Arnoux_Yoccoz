/**
 * @file UnitIntervalPoint.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 6, 2013
 *
 * @brief   This file contains the UnitIntervalPoint class.
 */


#ifndef ArnouxYoccoz_UnitIntervalPoint_h
#define ArnouxYoccoz_UnitIntervalPoint_h

#include "HDirection.h"

namespace balazs{


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
 * @see     ArcsAroundDivPoints, FoliationDisk
 */



// Calculates the fractional part of a real number. E.g. fracPart(2.3) = 0.3, fracPart(-1.8) = 0.2.
long double fracPart(long double x);
const long double PRECISION = 0.00000000001L;


class Mod1Number {
public:
    Mod1Number(long double position = 0, int epsilon = 0);
    virtual ~Mod1Number() = default;
    Mod1Number shiftedTo(HDirection side) const;
    HDirection side() const;
    Mod1Number& operator+=(const Mod1Number& rhs);
    Mod1Number operator-() const;
    friend bool operator<(const Mod1Number& p1, const Mod1Number& p2);
    operator long double() const { return m_position; }
private:
    long double m_position; // The position of the point.
    int m_epsilon;                  // The infinitesimal shift.
};


bool isBetween(const Mod1Number& leftPoint, const Mod1Number& rightPoint, const Mod1Number& middlePoint);
long double distanceBetween(const Mod1Number& p1, const Mod1Number& p2);
Mod1Number operator+(const Mod1Number& p1, const Mod1Number& p2);
Mod1Number operator+(const Mod1Number& p1, const long double& p2);
Mod1Number operator-(const Mod1Number& p1, const Mod1Number& p2);
Mod1Number operator-(const Mod1Number& p1, const long double& p2);
bool operator>(const Mod1Number& p1, const Mod1Number& p2);
bool operator==(const Mod1Number& p1, const Mod1Number& p2);
bool operator!=(const Mod1Number& p1, const Mod1Number& p2);
bool operator<=(const Mod1Number& p1, const Mod1Number& p2);
bool operator>=(const Mod1Number& p1, const Mod1Number& p2);









}


        
#endif
        
        
