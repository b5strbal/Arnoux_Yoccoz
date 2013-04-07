/**
 * @file    Foliation.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 14, 2013
 *
 * @brief   This file contains implementation for the Foliation class.
 */

#include "Foliation.h"


//-------------------------//
// InitArguments_Foliation //
//-------------------------//


std::vector<floating_point_type> InitArguments_Foliation::arg_lengths;
Permutation InitArguments_Foliation::arg_permutation;
floating_point_type InitArguments_Foliation::arg_twist;


InitArguments_Foliation::InitArguments_Foliation(const FoliationSphere& foliationSphere)
{
    
    std::vector<ConnectedPoints> allConnectedPoints;
    allConnectedPoints.reserve(foliationSphere.topFoliation().numSeparatrices() + foliationSphere.bottomFoliation().numSeparatrices());
    
    generateTopConnectingPairs(foliationSphere, allConnectedPoints);
    generateBottomConnectingPairs(foliationSphere, allConnectedPoints);
    
    
    std::sort(allConnectedPoints.begin(), allConnectedPoints.end(), [] (const ConnectedPoints& cp1, const ConnectedPoints& cp2)
              {
                  return cp1.topPoint < cp2.topPoint;
              } );
    std::vector<ConnectedPoints>& allConnectedPointsSortedByTop = allConnectedPoints;
    std::vector<UnitIntervalPoint> allBottomPoints;
    allBottomPoints.reserve(allConnectedPoints.size());
    
    for (ConnectedPoints cp : allConnectedPoints) {
        allBottomPoints.push_back(cp.bottomPoint);
    }
    
    std::sort(allBottomPoints.begin(), allBottomPoints.end());
    
    arg_lengths.resize(allConnectedPoints.size());
    for (int i = 0; i < allConnectedPoints.size() - 1; i++) {
        arg_lengths[i] = distanceBetween( allConnectedPointsSortedByTop[i].topPoint, allConnectedPointsSortedByTop[i + 1].topPoint);
    }
    arg_lengths[allConnectedPoints.size() - 1] = 1 - allConnectedPointsSortedByTop[allConnectedPoints.size() - 1].topPoint.getPosition();
    
    std::vector<int> permutationInput(allConnectedPoints.size());
    for (int i = 0; i < allConnectedPoints.size(); i++) {
        auto it = std::lower_bound(allBottomPoints.begin(), allBottomPoints.end(), allConnectedPointsSortedByTop[i].bottomPoint);
        permutationInput[i] = static_cast<int>(it - allBottomPoints.begin()) ;
    }
    arg_permutation = Permutation(permutationInput);
    
    arg_twist = allBottomPoints[0].getPosition();
}






void InitArguments_Foliation::generateTopConnectingPairs(const FoliationSphere& foliationSphere,
                                                      std::vector<ConnectedPoints>& allConnectedPoints)
{    
    int numSeparatrices = foliationSphere.topFoliation().intervalPairing().size();
    for (int i = 0; i < numSeparatrices; i++) {
        if (foliationSphere.topFoliation().intervalPairing().permutation()[i] != modAdd(i, -1, numSeparatrices) ) {
            // otherwise the current separatrix emanates from a 1-pronged singularity which is not important
            
            ConnectedPoints newConnectedPoints;
            newConnectedPoints.topPoint = foliationSphere.topFoliation().intervalPairing().divPoints()[i];
            
            int indexOfConnectedSeparatrix = modAdd(foliationSphere.topFoliation().intervalPairing().permutation()[i], 1, numSeparatrices);
            UnitIntervalPoint middlePoint = foliationSphere.topFoliation().intervalPairing().divPoints()[indexOfConnectedSeparatrix];
            
            newConnectedPoints.bottomPoint = foliationSphere.bottomFoliation().intervalPairing().applyTo(middlePoint - foliationSphere.twist()) + foliationSphere.twist();


            allConnectedPoints.push_back( newConnectedPoints );
        }
    }
}



void InitArguments_Foliation::generateBottomConnectingPairs(const FoliationSphere& foliationSphere,
                                                            std::vector<ConnectedPoints>& allConnectedPoints)
{
    int numSeparatrices = foliationSphere.bottomFoliation().intervalPairing().size();
    for (int i = 0; i < numSeparatrices; i++) {
        if (foliationSphere.bottomFoliation().intervalPairing().permutation()[i] != modAdd(i, -1, numSeparatrices) ) {
            
            ConnectedPoints newConnectedPoints;
            newConnectedPoints.bottomPoint = foliationSphere.bottomFoliation().intervalPairing().divPoints()[i] + foliationSphere.twist();
            
            int indexOfConnectedSeparatrix = modAdd(foliationSphere.bottomFoliation().intervalPairing().permutation()[i], 1, numSeparatrices);
            UnitIntervalPoint middlePoint = foliationSphere.topFoliation().intervalPairing().divPoints()[indexOfConnectedSeparatrix] + foliationSphere.twist();
            
            newConnectedPoints.topPoint = foliationSphere.topFoliation().intervalPairing().applyTo(middlePoint);

            allConnectedPoints.push_back( newConnectedPoints );
        }
    }

}



//-----------//
// Foliation //
//-----------//


Foliation::Foliation(const std::vector<floating_point_type>& lengths, const Permutation& permutation, floating_point_type twist) :
    InitArguments_Foliation(),
    m_twistedIntervalExchange(lengths, permutation, twist),
    m_numSeparatrices(2 * m_twistedIntervalExchange.sizeBeforeTwist())
{
}

Foliation::Foliation(const FoliationRP2& foliationRP2) :
    Foliation(FoliationSphere(foliationRP2))
{
}

Foliation::Foliation(const FoliationSphere& foliationSphere) :
    InitArguments_Foliation(foliationSphere),
    m_twistedIntervalExchange(arg_lengths, arg_permutation, arg_twist),
    m_numSeparatrices(2 * m_twistedIntervalExchange.sizeBeforeTwist()){
}



Foliation::Foliation(const TwistedIntervalExchangeMap& twistedintervalExchange) :
    InitArguments_Foliation(),
    m_twistedIntervalExchange(twistedintervalExchange),
    m_numSeparatrices(2 * m_twistedIntervalExchange.sizeBeforeTwist())
{
}




Foliation Foliation::rotateBy(int rotationAmount) const{
    return Foliation(m_twistedIntervalExchange.rotateBy(rotationAmount));
}


Foliation Foliation::reflect() const{
    return Foliation(m_twistedIntervalExchange.reflect());
}


Foliation Foliation::flipOver() const{
    return Foliation(m_twistedIntervalExchange.invert().reflect());
}



std::ostream& operator<<(std::ostream& Out, Foliation f){
    std::cout << f.m_twistedIntervalExchange;
    return Out;
}




Foliation arnouxYoccozFoliation(int genus){
    std::vector<int> permutationInput(2 * genus);
    std::vector<floating_point_type> lengths(2 * genus);
    floating_point_type shrinkingNumber = 1/arnouxYoccozStretchFactor(genus);
    floating_point_type currentLength = shrinkingNumber;
    for (int i = 0; i < genus; i++) {
        permutationInput[2 * i] = 2 * i + 1;
        permutationInput[2 * i + 1] = 2 * i;
        lengths[2 * i] = lengths[2 * i + 1] = currentLength;
        currentLength *= shrinkingNumber;
    }
    try {
        return Foliation(lengths, Permutation(permutationInput), 0.5);
    } catch (const std::exception& ex) {
        throw std::runtime_error("Machine precision is not enough to handle such a high genus Arnoux-Yoccox surface.");
    }
}








//------------------//
// FoliationFromRP2 //
//------------------//


FoliationFromRP2::FoliationFromRP2(const FoliationRP2& foliationRP2):
    Foliation(foliationRP2)
{
}






//---------------------//
// FoliationFromSphere //
//---------------------//


FoliationFromSphere::FoliationFromSphere(const FoliationSphere& foliationSphere) :
    Foliation(foliationSphere)
{
}




