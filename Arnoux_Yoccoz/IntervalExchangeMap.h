/**
 * @file    IntervalExchangeMap.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 14, 2013
 *
 * @brief   This file contains the IntervalExchangeMap class.
 */

#ifndef __Arnoux_Yoccoz__IntervalExchangeMap__
#define __Arnoux_Yoccoz__IntervalExchangeMap__

#include <iostream>
#include <vector>
#include <cassert>
#include "UnitIntervalPoint.h"
#include "WeighedTree.h"
#include "global.h"









class Permutation{
public:
    Permutation(){};
    Permutation(const std::vector<int> functionValues);
    inline int size() const{ return static_cast<int>(m_functionValues.size()); }
    Permutation inverse() const;
    int operator[](int index){ return m_functionValues[index]; }
    friend std::ostream& operator<<(std::ostream& Out, const Permutation& permutation);

protected:
    std::vector<int> m_functionValues;
};





class FoliationDiskPairing : public Permutation{
public:
    FoliationDiskPairing(const std::vector<int> functionValues);
private:
    void assertRangeIsClosedUnderPair(const std::vector<int>& functionValues, int begin, int end);
};






     
            
            
class IntervalExchangeBase{
public:
    inline int size() const { return static_cast<int>(m_lengths.size()); }
protected:
    std::vector<floating_point_type> m_lengths;
    Permutation m_permutation;
    Permutation m_inversePermutation;
    std::vector<UnitIntervalPoint> m_divPoints;
    std::vector<UnitIntervalPoint> m_divPointsAfterExchange;
    std::vector<floating_point_type> m_translations;

    IntervalExchangeBase(const std::vector<floating_point_type>& lengths, const Permutation& permutation);
    IntervalExchangeBase(const WeighedTree& wt);
    
    void init(const std::vector<floating_point_type>& lengths, const Permutation& permutation);
    
private:

};

            
            
            




class IntervalExchangeMap : public IntervalExchangeBase{
    friend class Foliation;
    
public:
    IntervalExchangeMap(const std::vector<floating_point_type>& lengths, const Permutation& permutation);
    IntervalExchangeMap(const std::vector<floating_point_type>& lengths, const Permutation& permutation, floating_point_type twist);

    UnitIntervalPoint applyTo(const UnitIntervalPoint& point);
    UnitIntervalPoint applyInverseTo(const UnitIntervalPoint& point);
    
    friend std::ostream& operator<<(std::ostream& Out, const IntervalExchangeMap intervalExchange);
};





class IntervalExchangeFoliationDisk : public IntervalExchangeBase{
public:
    IntervalExchangeFoliationDisk(const WeighedTree& wt);
    
    UnitIntervalPoint applyTo(const UnitIntervalPoint& point);
    
    bool isTooCloseToDivPoint(const UnitIntervalPoint& point);

    friend std::ostream& operator<<(std::ostream& Out, const IntervalExchangeFoliationDisk intervalExchange);

};












#endif /* defined(__Arnoux_Yoccoz__IntervalExchangeMap__) */
