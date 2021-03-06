/**
 * @file    Foliation.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 14, 2013
 *
 * @brief   This file contains implementation for the Foliation class.
 */

#include "Foliation.h"
#include "../math/PerronFrobenius.h"
#include "../scc/SmallFoliation.h"
#include <random>

static std::default_random_engine generator(static_cast<int>(time(NULL)));


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

balazs::Foliation::Foliation(int genus, const arnoux_yoccoz_tag &)
    :   Foliation(arnouxYoccozLengths(genus), arnouxYoccozPermutation(genus), 0.5)
{
}

std::vector<long double> randomLengths(std::size_t size){
    std::uniform_real_distribution<long double> realDistribution(0.00000000001, 1);

    std::vector<long double> retval;

    for(std::size_t i = 0; i < size; i++){
        retval.push_back(realDistribution(generator));
    }
    return retval;
}

long double randomTwist(){
    std::uniform_real_distribution<long double> realDistribution(0.00000000001, 1);

    return realDistribution(generator);
}


balazs::Foliation::Foliation(int numIntervals, const balazs::random_tag &)
    :   Foliation(randomLengths(numIntervals), Permutation(numIntervals), randomTwist())
{
}

balazs::Foliation::Foliation(const SmallFoliation &smallFoliation)
    :   Foliation(smallFoliation.normalizedLengths(),
                  smallFoliation.permutation(),
                  smallFoliation.normalizedTwist())
{
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
        if(vec.size() > 1){ // otherwise it is a removable singularity
            retval.push_back(vec.size() - 1);
        }
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












void balazs::Foliation::initIndexOfSingularity()
{
    m_indexOfSingularity.reserve(numIntervals());
    for(std::size_t i = 0; i < numIntervals(); i++){
        int j = 0;
        while(std::find(m_singularities[j].begin(), m_singularities[j].end(), i) == m_singularities[j].end()){
            j++;
        }
        m_indexOfSingularity.push_back(j);
    }
}



void balazs::Foliation::init()
{
    m_allDivPoints.reserve(numSeparatrices());
    std::merge(topDivPoints().begin(), topDivPoints().end(),
               bottomDivPoints().begin(), bottomDivPoints().end(), std::back_inserter(m_allDivPoints));

    if(arePointsTooClose(m_allDivPoints)){
        throw std::runtime_error("The foliation has a saddle connection.");
    }

    m_singularities = partition(singularityPermutation(m_twistedIntervalExchange));

    initIndexOfSingularity();
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
    long double currentLength = shrinkingNumber / 2;
    for (int i = 0; i < genus; i++) {
        lengths[2 * i] = lengths[2 * i + 1] = currentLength;
        currentLength *= shrinkingNumber;
    }
    return lengths;
}


bool balazs::isEqual(const balazs::Foliation &f1, const balazs::Foliation &f2, long double allowedError)
{
    // checking permutation
    if(f1.intExchange().permutationWithMinimalTwist() != f2.intExchange().permutationWithMinimalTwist()){
        return false;
    }

    // checking lengths
    for(std::size_t i = 0; i < f1.numIntervals(); i++){
        if(fabs(static_cast<double>(f1.intExchange().lengths()[i]) -
                static_cast<double>(f2.intExchange().lengths()[i])) > allowedError){
            return false;
        }
    }

    // checking twist
    if(fabs(static_cast<double>(f1.intExchange().divPointsAfterExchange()[0]) -
            static_cast<double>(f2.intExchange().divPointsAfterExchange()[0])) > allowedError){
        return false;
    }

    return true;
}
