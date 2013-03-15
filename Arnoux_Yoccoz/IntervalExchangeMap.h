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
#include "global.h"




template <typename Type>
std::ostream& operator<<(std::ostream& Out, const std::vector<Type>& vec);




class Permutation{
public:
    Permutation(){};
    Permutation(const std::vector<int> functionValues);
    inline int size() const{ return static_cast<int>(m_functionValues.size()); }
    Permutation inverse() const;
    int operator[](int index){ return m_functionValues[index]; }
    friend std::ostream& operator<<(std::ostream& Out, const Permutation& permutation);

private:
    std::vector<int> m_functionValues;
};



     
            
            
            
            
            
            
            




class IntervalExchangeMap{
public:
    IntervalExchangeMap(const std::vector<floating_point_type> lengths, const Permutation permutation);
    IntervalExchangeMap(const std::vector<floating_point_type> lengths, const Permutation permutation, floating_point_type twist);

    inline int size() const { return static_cast<int>(m_lengths.size()); }
    
    UnitIntervalPoint applyTo(const UnitIntervalPoint& point);
    UnitIntervalPoint applyInverseTo(const UnitIntervalPoint& point);
    
    friend std::ostream& operator<<(std::ostream& Out, const IntervalExchangeMap intervalExchange);
    
protected:
    template <typename Type>
    static int containingInterval(const std::vector<Type>& separatingPoints, Type point){
        return findInterval(separatingPoints, point, -1, static_cast<int>(separatingPoints.size())  - 1);
    }
    
private:
    std::vector<floating_point_type> m_lengths;
    Permutation m_permutation;
    Permutation m_inversePermutation;
    std::vector<UnitIntervalPoint> m_divPoints;
    std::vector<UnitIntervalPoint> m_divPointsAfterExchange;
    std::vector<floating_point_type> m_translations;

    void assertValidInput(const std::vector<floating_point_type> lengths, const Permutation permutation);
    void setLengthsAndDivPoints(const std::vector<floating_point_type> lengths);
    void init(const std::vector<floating_point_type> lengths, const Permutation permutation);
    
    template <typename Type>
    static int findInterval(const std::vector<Type>& separatingPoints, const Type& point, int start, int end);

};











#endif /* defined(__Arnoux_Yoccoz__IntervalExchangeMap__) */
