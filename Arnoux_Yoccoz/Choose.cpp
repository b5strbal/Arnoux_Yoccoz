#include "Choose.h"
#include <cassert>



Choose::Choose(int n, int k) :
m_n(n),
m_k(k),
m_chosenIndices(k)
{
    for (int i = 0; i < k; i++) {
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
        for(int i = indexToIncrease + 1; i < m_k; i++)
            m_chosenIndices[i] = m_chosenIndices[indexToIncrease] + i - indexToIncrease;
    }

    return *this;
}

