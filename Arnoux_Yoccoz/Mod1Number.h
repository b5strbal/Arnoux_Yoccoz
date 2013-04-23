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
#include <sstream>
#include "Matrix.h"



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
    //inline floating_point_type getPosition() const{ return m_position; }
    bool isGeneralized() const { return m_epsilon !=0; }
    static floating_point_type distanceBetween(const Mod1Number& p1, const Mod1Number& p2);
    friend Mod1Number operator+(const Mod1Number& p1, const Mod1Number& p2);
    friend Mod1Number operator-(const Mod1Number& p1, const Mod1Number& p2);
    friend bool operator<(const Mod1Number& p1, const Mod1Number& p2);
    friend bool operator>(const Mod1Number& p1, const Mod1Number& p2);
    friend bool operator==(const Mod1Number& p1, const Mod1Number& p2);
    friend bool operator!=(const Mod1Number& p1, const Mod1Number& p2);
    friend bool operator<=(const Mod1Number& p1, const Mod1Number& p2);
    friend bool operator>=(const Mod1Number& p1, const Mod1Number& p2);
    friend std::ostream& operator<<(std::ostream& out, const Mod1Number& p);
    operator floating_point_type(){ return m_position; }

    static std::string printInterval(const Mod1Number& p1, const Mod1Number& p2);

protected:
    floating_point_type m_position; // The position of the point.
    int m_epsilon;                  // The infinitesimal shift.
};

bool arePointsTooClose(const std::vector<Mod1Number>& points);



        
        
#endif
        
        
