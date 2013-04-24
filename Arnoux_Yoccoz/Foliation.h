/**
 * @file    Foliation.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 14, 2013
 *
 * @brief   This file contains the Foliation class.
 */

#ifndef __Arnoux_Yoccoz__Foliation__
#define __Arnoux_Yoccoz__Foliation__

#include <vector>
#include "Mod1Number.h"
#include "TwistedIntervalExchangeMap.h"
#include "PerronFrobenius.h"
#include "FoliationRP2.h"
#include "FoliationSphere.h"






//-----------//
// Foliation //
//-----------//


class Foliation{
    friend class SeparatrixSegment;
    friend class ArcsAroundDivPoints;
    
public:
    Foliation(const std::vector<floating_point_type>& lengths, const Permutation& permutation, floating_point_type twist);
    static Foliation fromFoliationRP2(const FoliationRP2 &foliationRP2);
    static Foliation fromFoliationSphere(const FoliationSphere&foliationSphere);
    Foliation rotateBy(int rotationAmount) const;
    Foliation reflect() const;
    Foliation flipOver() const;
    
   // int numSeparatrices() const { return m_numSeparatrices; }
    int numIntervals() const { return m_numIntervals; }

    friend std::ostream& operator<<(std::ostream& out, const Foliation& f);

protected:
//    typedef std::pair<UnitIntervalPoint, UnitIntervalPoint> interval_t;


// MEMBER VARIABLES
    int m_numIntervals;
    int m_numSeparatrices;
    TwistedIntervalExchangeMap m_twistedIntervalExchange;
    std::vector<Mod1Number> m_allRealDivPoints;
    std::vector<short> m_isTopDivPoint;
    std::vector<Mod1Number> m_topRealDivPoints;
    std::vector<Mod1Number> m_bottomRealDivPoints;
    std::vector<int> m_pairOfTopDivPoints;

private:
    Foliation(const TwistedIntervalExchangeMap&);
    
    void init();

    struct ConnectedPoints {
        Mod1Number topPoint;
        Mod1Number bottomPoint;
    };

    static void generateTopConnectingPairs(const FoliationSphere& foliationSphere, std::vector<ConnectedPoints>& allConnectedPoints);
    static void generateBottomConnectingPairs(const FoliationSphere& foliationSphere, std::vector<ConnectedPoints>& allConnectedPoints);

    
};


Foliation arnouxYoccozFoliation(int genus);








#endif /* defined(__Arnoux_Yoccoz__Foliation__) */
