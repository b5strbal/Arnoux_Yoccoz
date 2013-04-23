#include "FoliationFromRP2.h"




//------------------//
// FoliationFromRP2 //
//------------------//


FoliationFromRP2::FoliationFromRP2(const FoliationRP2& foliationRP2):
    FoliationFromSphere(foliationRP2)
{
    m_separatrixPair.resize(m_numIntervals);
    for (int i = 0 ; i < m_numIntervals; i++) {
        Mod1Number pair = m_twistedIntervalExchange.applyTo(Mod1Number(m_topRealDivPoints[i].getPosition(), 1)) - 0.5;
        for (int j = 0; j < m_numIntervals; j++) {
            if (distanceBetween(m_topRealDivPoints[j], pair) < Mod1Number::PRECISION || distanceBetween(pair, m_topRealDivPoints[j]) < Mod1Number::PRECISION) {
                m_separatrixPair[i] = j;
                break;
            }
        }
    }
}





