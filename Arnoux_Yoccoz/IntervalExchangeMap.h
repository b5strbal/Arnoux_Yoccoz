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
#include "Permutation.h"







            
class IntervalExchangeBase{
public:
    inline int size() const { return static_cast<int>(m_lengths.size()); }
    virtual UnitIntervalPoint applyTo(const UnitIntervalPoint& point) const = 0;
    virtual UnitIntervalPoint applyInverseTo(const UnitIntervalPoint& point) const = 0;
    const std::vector<floating_point_type>& lengths() const { return m_lengths; }
    const Permutation& permutation() const { return m_permutation; }
    const Permutation& inversePermutation() const { return m_inversePermutation; }
    const std::vector<UnitIntervalPoint>& divPoints() const { return m_divPoints; }
    const std::vector<UnitIntervalPoint>& divPointsAfterExchange() const { return m_divPointsAfterExchange; }
    std::string print() const;

protected:
    std::vector<floating_point_type> m_lengths;
    Permutation m_permutation;
    Permutation m_inversePermutation;
    std::vector<UnitIntervalPoint> m_divPoints;
    std::vector<UnitIntervalPoint> m_divPointsAfterExchange;

    IntervalExchangeBase(const std::vector<floating_point_type>& lengths, const Permutation& permutation);
};


            
            
            




class IntervalExchangeMap : public IntervalExchangeBase{
    //friend class InitArguments_TwistedIntervalExchangeMap;
public:
    IntervalExchangeMap(const std::vector<floating_point_type>& lengths, const Permutation& permutation);

    // Notice: even from a generalized UnitIntervalPoint, applyTo and applyInverseTo create a non-generalized UnitIntervalPoint!
    // So the shift is lost. In general, this is what we want.
    UnitIntervalPoint applyTo(const UnitIntervalPoint& point) const;
    UnitIntervalPoint applyInverseTo(const UnitIntervalPoint& point) const;
    const std::vector<floating_point_type>& translations() const { return m_translations; }
    

private:
    std::vector<floating_point_type> m_translations;
};








class InitArguments_TwistedIntervalExchangeMap{
    friend class TwistedIntervalExchangeMap;
    
    InitArguments_TwistedIntervalExchangeMap(const std::vector<floating_point_type>& lengths,
                                             const Permutation& permutation,
                                             floating_point_type twist);

    static std::vector<floating_point_type> arg_lengths;
    static Permutation arg_permutation;
    static std::vector<floating_point_type> arg_normalizedOriginalLengths;
};



class TwistedIntervalExchangeMap : private InitArguments_TwistedIntervalExchangeMap{
    friend class Foliation;
    friend class FoliationFromRP2;
public:
    TwistedIntervalExchangeMap(const std::vector<floating_point_type>& lengths, const Permutation& permutation, floating_point_type twist);
    TwistedIntervalExchangeMap rotateBy(int rotationAmount) const;
    TwistedIntervalExchangeMap reflect() const;
    TwistedIntervalExchangeMap invert() const;
    UnitIntervalPoint applyTo(const UnitIntervalPoint& p) const { return m_intervalExchangeAfterTwist.applyTo(p); }
    UnitIntervalPoint applyInverseTo(const UnitIntervalPoint& p) const { return m_intervalExchangeAfterTwist.applyInverseTo(p); }

    friend std::ostream& operator<<(std::ostream& Out, const TwistedIntervalExchangeMap& twistedIntervalExchange);
private:
    IntervalExchangeMap m_intervalExchangeAfterTwist;
    std::vector<floating_point_type> m_originalLengths;
    Permutation m_originalPermutation;
    floating_point_type m_twist;
    int m_indexOfFakeDivPoint;

    
    int sizeBeforeTwist() const{ return m_originalPermutation.size(); }
    int sizeAfterTwist() const{ return m_intervalExchangeAfterTwist.size(); }
};







class InitArguments_IntervalExchangeFoliationDisk{
    friend class IntervalExchangeFoliationDisk;
    
    InitArguments_IntervalExchangeFoliationDisk(const WeighedTree& wt);
    void fillInLengthsAndPairing(std::vector<floating_point_type>& lengths,
                                 std::vector<unsigned int> &pairing, int StartingIndex, WeighedTree::Node* pNode) const;
    
    static std::vector<floating_point_type> arg_lengths;
    static Permutation arg_permutation;
};





class IntervalExchangeFoliationDisk : private InitArguments_IntervalExchangeFoliationDisk,
                                      public IntervalExchangeBase
{
public:
    IntervalExchangeFoliationDisk(const WeighedTree& wt);
    
    UnitIntervalPoint applyTo(const UnitIntervalPoint& point) const;
    UnitIntervalPoint applyInverseTo(const UnitIntervalPoint& point) const { return applyTo(point); }

    

};












#endif /* defined(__Arnoux_Yoccoz__IntervalExchangeMap__) */
