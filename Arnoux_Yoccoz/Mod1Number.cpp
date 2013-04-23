/**
 * @file UnitIntervalPoint.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 6, 2013
 *
 */

#include "Mod1Number.h"
 

// const floating_point_type UnitIntervalPoint::PRECISION = ;



Mod1Number::Mod1Number(floating_point_type position, int epsilon) :
    m_position(fracPart(position)),
    m_epsilon(epsilon)
{
}


Mod1Number operator+(const Mod1Number& p1, const Mod1Number& p2){
    return Mod1Number(p1.m_position + p2.m_position, p1.m_epsilon + p2.m_epsilon);
}

Mod1Number operator-(const Mod1Number& p1, const Mod1Number& p2){
    return Mod1Number(p1.m_position - p2.m_position, p1.m_epsilon - p2.m_epsilon);
}



// Decides if p1 is before p2 on the segment [0,1).
//
// Caution: 0 is not the first element in the ordering. (0, -1), the left shift of zero is less than that, so are
// all (0, -n) points, but we don't use points with higher n.
//
bool operator<(const Mod1Number& p1, const Mod1Number& p2){
    if(p1.m_position < p2.m_position || p1.m_epsilon < p2.m_epsilon)
        return true;
    return false;
}



// Returns the distance between two points. Ignores the infinitesimal shift.
//
// It looks for c2 on the RIGHT of c1, so the return value may be bigger than 0.5.
// E.g. DistaceBetween(UnitIntervalPoint(0.8, LEFT), UnitIntervalPoint(0.1)) is 0.3.
// floating_point_type distanceBetween(const UnitIntervalPoint& p1, const UnitIntervalPoint& p2)
//
floating_point_type Mod1Number::distanceBetween(const Mod1Number& p1, const Mod1Number& p2)
{
    return p1.m_position <= p2.m_position ? p2.m_position - p1.m_position : p2.m_position - p1.m_position + 1;
}



std::ostream& operator<<(std::ostream& out, const Mod1Number& p){
    out << p.m_position;
    if(p.m_epsilon > 0)
        out << "(+" << p.m_epsilon << "eps)";
    else if(p.m_epsilon < 0)
        out << "(" << p.m_epsilon << "eps)";
    return out;
}




std::string Mod1Number::printInterval(const Mod1Number& p1, const Mod1Number& p2){
    std::ostringstream s;
    s << "[" << p1 << "," << p2 << "]";
    return s.str();
}



bool arePointsTooClose(const std::vector<Mod1Number>& points){
    for (auto it = points.begin() + 1; it != points.end(); it++) {
        if (!(*(it - 1) < *it)) {
            return true;
        }
    }
    return false;
}



bool operator==(const Mod1Number &p1, const Mod1Number &p2)
{
    return p1.m_position == p2.m_position && p1.m_epsilon == p2.m_epsilon;
}

bool operator !=(const Mod1Number &p1, const Mod1Number &p2)
{
    return p1 == p2 ? false : true;
}

bool operator <=(const Mod1Number &p1, const Mod1Number &p2)
{
    return p1 < p2 || p1 == p2 ? true : false;
}

bool operator >=(const Mod1Number &p1, const Mod1Number &p2)
{
    return p2 <= p1;
}


bool operator >(const Mod1Number &p1, const Mod1Number &p2)
{
    return p2 < p1;
}


