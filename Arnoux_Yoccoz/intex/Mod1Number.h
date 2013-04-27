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
 * @see     ArcsAroundDivPoints, FoliationDisk
 */


class Mod1Number {
public:
    Mod1Number(floating_point_type position = 0, int epsilon = 0);
    void shiftTo(Direction::LeftOrRight side);
    Direction::LeftOrRight side() const;
    Mod1Number& operator+=(const Mod1Number& rhs);
    Mod1Number operator-() const;
    friend bool operator<(const Mod1Number& p1, const Mod1Number& p2);
    friend std::ostream& operator<<(std::ostream& out, const Mod1Number& p);
    operator floating_point_type() const { return m_position; }
protected:
    floating_point_type m_position; // The position of the point.
    int m_epsilon;                  // The infinitesimal shift.
};



floating_point_type distanceBetween(const Mod1Number& p1, const Mod1Number& p2);
Mod1Number operator+(const Mod1Number& p1, const Mod1Number& p2);
Mod1Number operator+(const Mod1Number& p1, const floating_point_type& p2);
Mod1Number operator-(const Mod1Number& p1, const Mod1Number& p2);
Mod1Number operator-(const Mod1Number& p1, const floating_point_type& p2);
bool operator>(const Mod1Number& p1, const Mod1Number& p2);
bool operator==(const Mod1Number& p1, const Mod1Number& p2);
bool operator!=(const Mod1Number& p1, const Mod1Number& p2);
bool operator<=(const Mod1Number& p1, const Mod1Number& p2);
bool operator>=(const Mod1Number& p1, const Mod1Number& p2);



typedef struct{
    Mod1Number leftEndpoint;
    Mod1Number rightEndpoint;
} interval_t;

std::ostream& operator<<(std::ostream& out, const interval_t& interval);



template <typename type1, typename type2>
unsigned int containingInterval(const std::vector<type1>& orderedList, const type2& point){
    int interval = std::upper_bound(orderedList.begin(), orderedList.end(), point) - orderedList.begin() - 1;
    return interval == -1 ? orderedList.size() - 1 : interval;
}



        
#endif
        
        
