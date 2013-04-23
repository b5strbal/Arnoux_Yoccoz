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
//#include "Mod1Number.h"
#include "WeighedTree.h"
#include "global.h"
#include "Permutation.h"
#include "Mod1NumberIntExchange.h"






            
class IntervalExchangeBase{
public:
    inline int size() const { return static_cast<int>(m_lengths.size()); }
    virtual Mod1Number applyTo(const Mod1Number& point) const = 0;
    virtual Mod1NumberIntExchange applyTo(const Mod1NumberIntExchange &point) const = 0;
    virtual Mod1Number applyInverseTo(const Mod1Number& point) const = 0;
    virtual Mod1NumberIntExchange applyInverseTo(const Mod1NumberIntExchange &point) const = 0;
    const std::vector<Mod1NumberIntExchange>& lengths() const { return m_lengths; }
    const Permutation& permutation() const { return m_permutation; }
    const Permutation& inversePermutation() const { return m_inversePermutation; }
    const std::vector<Mod1NumberIntExchange>& divPoints() const { return m_divPoints; }
    const std::vector<Mod1NumberIntExchange>& divPointsAfterExchange() const { return m_divPointsAfterExchange; }
    friend std::ostream& operator<<(std::ostream& out, const IntervalExchangeBase& exchange);

protected:
    std::vector<Mod1NumberIntExchange> m_lengths;
    Permutation m_permutation;
    Permutation m_inversePermutation;
    std::vector<Mod1NumberIntExchange> m_divPoints;
    std::vector<Mod1NumberIntExchange> m_divPointsAfterExchange;

    IntervalExchangeBase() : IntervalExchangeBase({1}, Permutation()) {}
    IntervalExchangeBase(const std::vector<floating_point_type>& lengths, const Permutation& permutation);
};


            
            
            




class IntervalExchangeMap : public IntervalExchangeBase{
public:
    IntervalExchangeMap() : IntervalExchangeBase() {}
    IntervalExchangeMap(const std::vector<floating_point_type>& lengths, const Permutation& permutation);


    virtual Mod1Number applyTo(const Mod1Number& point) const;
    virtual Mod1NumberIntExchange applyTo(const Mod1NumberIntExchange &point) const;
    virtual Mod1Number applyInverseTo(const Mod1Number& point) const;
    virtual Mod1NumberIntExchange applyInverseTo(const Mod1NumberIntExchange &point) const;
    const std::vector<floating_point_type>& translations() const { return m_translations; }
    

private:
    std::vector<floating_point_type> m_translations;
};














class TwistedIntervalExchangeMap{
    friend class Foliation;
    friend class FoliationFromRP2;
public:
    TwistedIntervalExchangeMap(const std::vector<floating_point_type>& lengths, const Permutation& permutation, floating_point_type twist);
    TwistedIntervalExchangeMap rotateBy(int rotationAmount) const;
    TwistedIntervalExchangeMap reflect() const;
    TwistedIntervalExchangeMap invert() const;
    Mod1Number applyTo(const Mod1Number& p) const { return m_intervalExchangeAfterTwist.applyTo(p); }
    Mod1Number applyInverseTo(const Mod1Number& p) const { return m_intervalExchangeAfterTwist.applyInverseTo(p); }

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










class IntervalExchangeFoliationDisk : public IntervalExchangeBase

{
public:
    static IntervalExchangeFoliationDisk fromWeighedTree(const WeighedTree& wt);
    
    virtual Mod1Number applyTo(const Mod1Number& point) const;
    virtual Mod1NumberIntExchange applyTo(const Mod1NumberIntExchange& point) const;
    virtual Mod1Number applyInverseTo(const Mod1Number& point) const { return applyTo(point); }
    virtual Mod1NumberIntExchange applyInverseTo(const Mod1NumberIntExchange& point) const;
private:
    IntervalExchangeFoliationDisk(const std::vector<floating_point_type>& lengths, const Permutation& permutation);

    static void fillInLengthsAndPairing(std::vector<floating_point_type>& lengths,
                                 std::vector<unsigned int> &pairing,
                                 int StartingIndex,
                                 WeighedTree::Node* pNode);

};












#endif /* defined(__Arnoux_Yoccoz__IntervalExchangeMap__) */
