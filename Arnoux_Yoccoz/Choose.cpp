#include "Choose.h"
#include <cassert>



Choose::Choose(unsigned int n, unsigned int k) :
m_n(n),
m_k(k),
m_chosenIndices(k)
{
    for (unsigned int i = 0; i < k; i++) {
        m_chosenIndices[i] = i;
    }
}


Choose& Choose::operator++() {
    assert(!isAfterLast());
    int indexToIncrease = m_k - 1;
    while( indexToIncrease >= 0 && m_chosenIndices[indexToIncrease] == m_n - m_k + indexToIncrease)
        indexToIncrease--;
    if ( indexToIncrease < 0){
        m_chosenIndices.clear(); // indicating the element after the last one
    } else {
        m_chosenIndices[indexToIncrease]++;
        for(unsigned int i = indexToIncrease + 1; i < m_k; i++)
            m_chosenIndices[i] = m_chosenIndices[indexToIncrease] + i - indexToIncrease;
    }

    return *this;
}

bool Choose::isAfterLast() const
{
    return m_chosenIndices.empty();
}

unsigned int Choose::operator [](unsigned int i)
{
    return m_chosenIndices[i];
}
