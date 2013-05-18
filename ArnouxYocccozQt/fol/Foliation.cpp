/**
 * @file    Foliation.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 14, 2013
 *
 * @brief   This file contains implementation for the Foliation class.
 */

#include "Foliation.h"
#include "../math/PerronFrobenius.h"



balazs::Foliation::Foliation(const std::vector<long double>& lengths, const Permutation& permutation, long double twist) :
    m_twistedIntervalExchange(lengths, permutation, twist)
{
    init();
}


balazs::Foliation::Foliation(const Foliation& foliation, int rotationAmount, const rotate_tag&)
    :   m_twistedIntervalExchange(foliation.m_twistedIntervalExchange, rotationAmount, rotate_tag())
{
    init();
}

balazs::Foliation::Foliation(const Foliation& foliation, const reverse_tag&)
    :   m_twistedIntervalExchange(foliation.m_twistedIntervalExchange, reverse_tag())
{
    init();
}

balazs::Foliation::Foliation(const Foliation& foliation, const flip_over_tag&)
    :   m_twistedIntervalExchange(TwistedIntervalExchangeMap(foliation.m_twistedIntervalExchange, invert_tag()), reverse_tag())
{
    init();
}

balazs::Foliation::Foliation(int genus)
    :   m_twistedIntervalExchange(arnouxYoccozLengths(genus), arnouxYoccozPermutation(genus), 0.5)
{
    init();
}




int balazs::Foliation::eulerChar() const
{
    int retval = 0;
    for(auto vec : m_singularities){
        retval -= vec.size() - 1;
    }
    return retval;
}

std::vector<std::size_t> balazs::Foliation::singularityTypeOfAbelianDiff() const
{
    std::vector<std::size_t> retval;
    for(auto vec : m_singularities){
        retval.push_back(vec.size() - 1);
    }
    std::sort(retval.begin(), retval.end(), std::greater<std::size_t>());
    return retval;
}




std::vector<std::size_t> balazs::Foliation::singularityTypeProngs() const
{
    std::vector<std::size_t> retval = singularityTypeOfAbelianDiff();
    for(auto &x : retval){
        x = (x + 1) * 2;
    }
    return retval;
}





bool balazs::Foliation::isTopDivPoint(int divPointIndex) const{
    return m_allDivPoints[divPointIndex].twistCoeff() == 0;
}




const balazs::Mod1NumberIntExchange &balazs::Foliation::firstIntersection(int singularityIndex, VDirection direction) const
{
    if (direction == VDirection::Up) {
        return bottomDivPoints()[m_twistedIntervalExchange.permutationWithMinimalTwist()[singularityIndex]];
    } else
        return topDivPoints()[singularityIndex];
}








void balazs::Foliation::initSingularities()
{
    std::size_t size = m_twistedIntervalExchange.size();
    m_indexOfSingularity.resize(size);

    std::vector<char> alreadyLookedAt(size, false);
    for(std::size_t i = 0; i < size; i++){
        if(!alreadyLookedAt[i]){
            m_singularities.push_back(std::vector<std::size_t>(0));

            std::size_t j = i;
            do {
                m_singularities.back().push_back(j);
                m_indexOfSingularity[j] = m_singularities.size() - 1;
                alreadyLookedAt[j] = true;


                j = (j + (size - 1)) % size;
                j = m_twistedIntervalExchange.permutationWithMinimalTwist()[j];
                j = (j + 1) % size;
                j = m_twistedIntervalExchange.inversePermutationWithMinimalTwist()[j];
            } while(j != i);
        }
    }
}



void balazs::Foliation::init()
{
    m_allDivPoints.reserve(numSeparatrices());
    std::merge(topDivPoints().begin(), topDivPoints().end(),
               bottomDivPoints().begin(), bottomDivPoints().end(), std::back_inserter(m_allDivPoints));
 //   qDebug() << dynamic_cast<Mod1Number&>(m_allDivPoints[0]) << "ssss";
    for(std::size_t i = 1; i < numSeparatrices(); i++){
        if(distanceBetween(m_allDivPoints[i - 1], m_allDivPoints[i]) < PRECISION){
            throw std::runtime_error("The foliation has a saddle connection.");
        }
    }

    initSingularities();
}













balazs::Permutation balazs::arnouxYoccozPermutation(int genus)
{
    std::vector<std::size_t> permutationInput(2 * genus);
    for (int i = 0; i < genus; i++) {
        permutationInput[2 * i] = 2 * i + 1;
        permutationInput[2 * i + 1] = 2 * i;
    }
    return Permutation(permutationInput);
}


std::vector<long double> balazs::arnouxYoccozLengths(int genus)
{
    std::vector<long double> lengths(2 * genus);
    long double shrinkingNumber = 1/arnouxYoccozStretchFactor(genus);
    long double currentLength = shrinkingNumber;
    for (int i = 0; i < genus; i++) {
        lengths[2 * i] = lengths[2 * i + 1] = currentLength;
        currentLength *= shrinkingNumber;
    }
    return lengths;
}

