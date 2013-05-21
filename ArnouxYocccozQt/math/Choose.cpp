#include "Choose.h"
#include <cassert>
#include <cmath>


balazs::Choose::Choose(std::size_t n, std::size_t k) :
m_n(n),
m_k(k)
{
    m_chosenIndices.reserve(k);
    for (std::size_t i = 0; i < k; i++) {
        m_chosenIndices.push_back(i);
    }
}


balazs::Choose& balazs::Choose::operator++() {
    assert(!isAfterLast());
    int indexToIncrease = m_k - 1;
    while( indexToIncrease >= 0 && m_chosenIndices[indexToIncrease] == m_n - m_k + indexToIncrease)
        indexToIncrease--;
    if ( indexToIncrease < 0){
        m_chosenIndices.clear(); // indicating the element after the last one
    } else {
        m_chosenIndices[indexToIncrease]++;
        for(std::size_t i = indexToIncrease + 1; i < m_k; i++)
            m_chosenIndices[i] = m_chosenIndices[indexToIncrease] + i - indexToIncrease;
    }

    return *this;
}

bool balazs::Choose::isAfterLast() const
{
    return m_chosenIndices.empty();
}

std::size_t balazs::Choose::operator [](std::size_t i) const
{
    return m_chosenIndices[i];
}




std::size_t balazs::choose(std::size_t n, std::size_t k)
{
    assert(k > 0);
    if(k > n) return 0;

    long double retval = 1;
    for(std::size_t i = 0; i < k; i++){
        retval *= n - i;
        retval /= i + 1;
    }
    return floor(retval + 0.5);
}
