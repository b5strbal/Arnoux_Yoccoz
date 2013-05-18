/**
 * @file UnitIntervalPoint.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 6, 2013
 *
 */

#include "Mod1Number.h"
#include <cmath>


long double balazs::fracPart(long double x){ return x - floor(x); }



balazs::Mod1Number::Mod1Number(long double position, int epsilon) :
    m_position(fracPart(position)),
    m_epsilon(epsilon)
{
}

balazs::Mod1Number balazs::Mod1Number::shiftedTo(HDirection side) const
{
    switch(side){
    case HDirection::Left:
        return Mod1Number(m_position, -1);
        break;
    case HDirection::Right:
        return Mod1Number(m_position, 1);
        break;
    case HDirection::Center:
        return Mod1Number(m_position, 0);
        break;
    }
}

balazs::HDirection balazs::Mod1Number::side() const
{
    if(m_epsilon < 0)
        return HDirection::Left;
    if(m_epsilon > 0)
        return HDirection::Right;
    return HDirection::Center;
}

balazs::Mod1Number &balazs::Mod1Number::operator +=(const Mod1Number &rhs)
{
    m_position += rhs.m_position;
    m_position = fracPart(m_position);
    m_epsilon += rhs.m_epsilon;
    return *this;
}

balazs::Mod1Number balazs::Mod1Number::operator -() const
{
    return Mod1Number(-m_position, -m_epsilon);
}




// Decides if p1 is before p2 on the segment [0,1).
//
// Caution: 0 is not the first element in the ordering. (0, -1), the left shift of zero is less than that, so are
// all (0, -n) points, but we don't use points with higher n.
//
bool balazs::operator<(const Mod1Number& p1, const Mod1Number& p2){
    if(p1.m_position < p2.m_position)
        return true;
    if(p1.m_position > p2.m_position)
        return false;
    if(p1.m_epsilon < p2.m_epsilon)
        return true;
    return false;
}











bool balazs::operator==(const Mod1Number &p1, const Mod1Number &p2)
{
    return (p1 < p2 || p2 < p1) ? false : true;
}

bool balazs::operator !=(const Mod1Number &p1, const Mod1Number &p2)
{
    return p1 == p2 ? false : true;
}

bool balazs::operator <=(const Mod1Number &p1, const Mod1Number &p2)
{
    return p1 < p2 || p1 == p2 ? true : false;
}

bool balazs::operator >=(const Mod1Number &p1, const Mod1Number &p2)
{
    return p2 <= p1;
}


bool balazs::operator >(const Mod1Number &p1, const Mod1Number &p2)
{
    return p2 < p1;
}



balazs::Mod1Number balazs::operator+(const Mod1Number& p1, const Mod1Number& p2){
    Mod1Number sum = p1;
    sum += p2;
    return sum;
}

balazs::Mod1Number balazs::operator-(const Mod1Number& p1, const Mod1Number& p2){
    return p1 + (-p2);
}


balazs::Mod1Number balazs::operator +(const Mod1Number &p1, const long double &p2)
{
    return p1 + Mod1Number(p2);
}


balazs::Mod1Number balazs::operator -(const Mod1Number &p1, const long double &p2)
{
    return p1 - Mod1Number(p2);
}


// Returns the distance between two points. Ignores the infinitesimal shift.
//
// It looks for c2 on the RIGHT of c1, so the return value may be bigger than 0.5.
// E.g. DistaceBetween(UnitIntervalPoint(0.8, LEFT), UnitIntervalPoint(0.1)) is 0.3.
// long double distanceBetween(const UnitIntervalPoint& p1, const UnitIntervalPoint& p2)
//
long double balazs::distanceBetween(const Mod1Number& p1, const Mod1Number& p2)
{
    return static_cast<long double>(p2 - p1);
}

