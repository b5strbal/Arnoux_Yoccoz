/**
 * @file    Foliation.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 14, 2013
 *
 * @brief   This file contains implementation for the Foliation class.
 */

#include "Foliation.h"


//-----------//
// Foliation //
//-----------//


Foliation::Foliation(const std::vector<floating_point_type>& lengths, const Permutation& permutation, floating_point_type twist) :
    m_twistedIntervalExchange(lengths, permutation, twist),
    m_numSeparatrices(2 * m_twistedIntervalExchange.sizeBeforeTwist())
{
}

Foliation::Foliation(const TwistedIntervalExchangeMap& twistedintervalExchange) :
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






//--------------------------------//
// InitArguments_FoliationFromRP2 //
//--------------------------------//


std::vector<floating_point_type> InitArguments_FoliationFromRP2::arg_lengths(0);
Permutation InitArguments_FoliationFromRP2::arg_permutation;

InitArguments_FoliationFromRP2::InitArguments_FoliationFromRP2(const FoliationRP2& foliationRP2){


}




//------------------//
// FoliationFromRP2 //
//------------------//


FoliationFromRP2::FoliationFromRP2(const FoliationRP2& foliationRP2):
    InitArguments_FoliationFromRP2(foliationRP2),
    Foliation(arg_lengths, arg_permutation, 0.5)
{
}




//-----------------------------------//
// InitArguments_FoliationFromSphere //
//-----------------------------------//


std::vector<floating_point_type> InitArguments_FoliationFromSphere::arg_lengths;
Permutation InitArguments_FoliationFromSphere::arg_permutation;
floating_point_type InitArguments_FoliationFromSphere::arg_twist;


InitArguments_FoliationFromSphere::InitArguments_FoliationFromSphere(const FoliationSphere& foliationSphere)
{

}





//---------------------//
// FoliationFromSphere //
//---------------------//


FoliationFromSphere::FoliationFromSphere(const FoliationSphere& foliationSphere) :
    InitArguments_FoliationFromSphere(foliationSphere),
    Foliation(arg_lengths, arg_permutation, arg_twist)
{
}




