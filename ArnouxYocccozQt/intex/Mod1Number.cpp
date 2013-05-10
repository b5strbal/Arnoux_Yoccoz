/**
 * @file UnitIntervalPoint.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 6, 2013
 *
 */

#include "Mod1Number.h"
 

// const floating_point_type UnitIntervalPoint::PRECISION = ;



balazs::Mod1Number::Mod1Number(floating_point_type position, int epsilon) :
    m_position(fracPart(position)),
    m_epsilon(epsilon)
{
}

balazs::Mod1Number balazs::Mod1Number::shiftedTo(Direction::LeftOrRight side) const
{
    switch(side){
    case Direction::LEFT:
        return Mod1Number(m_position, -1);
        break;
    case Direction::RIGHT:
        return Mod1Number(m_position, 1);
        break;
    case Direction::CENTER:
        return Mod1Number(m_position, 0);
        break;
    }
}

balazs::Direction::LeftOrRight balazs::Mod1Number::side() const
{
    if(m_epsilon < 0)
        return Direction::LEFT;
    if(m_epsilon > 0)
        return Direction::RIGHT;
    return Direction::CENTER;
}

balazs::Mod1Number &balazs::Mod1Number::operator +=(const Mod1Number &rhs)
{
    m_position += rhs.m_position;
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
    if(p1.m_position < p2.m_position || p1.m_epsilon < p2.m_epsilon)
        return true;
    return false;
}





std::ostream& balazs::operator<<(std::ostream& out, const Mod1Number& p){
    out << p.m_position;
    if(p.m_epsilon > 0)
        out << "(+" << p.m_epsilon << "eps)";
    else if(p.m_epsilon < 0)
        out << "(" << p.m_epsilon << "eps)";
    return out;
}




std::ostream& balazs::operator<<(std::ostream& out, const interval_t& interval){
    out << "[" << interval.leftEndpoint << "," << interval.rightEndpoint << "]";
    return out;
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


balazs::Mod1Number balazs::operator +(const Mod1Number &p1, const floating_point_type &p2)
{
    return p1 + Mod1Number(p2);
}


balazs::Mod1Number balazs::operator -(const Mod1Number &p1, const floating_point_type &p2)
{
    return p1 - Mod1Number(p2);
}


// Returns the distance between two points. Ignores the infinitesimal shift.
//
// It looks for c2 on the RIGHT of c1, so the return value may be bigger than 0.5.
// E.g. DistaceBetween(UnitIntervalPoint(0.8, LEFT), UnitIntervalPoint(0.1)) is 0.3.
// floating_point_type distanceBetween(const UnitIntervalPoint& p1, const UnitIntervalPoint& p2)
//
balazs::floating_point_type balazs::distanceBetween(const Mod1Number& p1, const Mod1Number& p2)
{
    return static_cast<floating_point_type>(p2 - p1);
}

