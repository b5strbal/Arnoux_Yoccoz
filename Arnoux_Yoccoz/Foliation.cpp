/**
 * @file    Foliation.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 14, 2013
 *
 * @brief   This file contains implementation for the Foliation class.
 */

#include "Foliation.h"





Foliation::Foliation(const std::vector<floating_point_type>& lengths, const Permutation& permutation, floating_point_type twist) :
    m_intervalExchange(lengths, permutation, twist),
    m_numSeparatrices(2 * static_cast<int>(lengths.size())),
    m_shiftedSeparatrixPoints(0)
{
    for (int i = 0; i < m_intervalExchange.size(); i++) {
        if (m_intervalExchange.m_lengths[i] < 3 * PRECISION) {
            throw std::runtime_error("The lengths parameters of the foliation are too small.");
        }
    }
    
    m_shiftedSeparatrixPoints.push_back(PRECISION);
    int firstindex = 1;
    int secondindex = 1;
    while (firstindex + secondindex < m_numSeparatrices + 2) {
        if (secondindex == m_numSeparatrices/2 + 1 || (firstindex < m_numSeparatrices/2 + 1 &&
                    m_intervalExchange.m_divPoints[firstindex] < m_intervalExchange.m_divPointsAfterExchange[secondindex])) {
            if (m_intervalExchange.m_permutation[firstindex] == 0) {
                firstindex++;
            } else {
                m_shiftedSeparatrixPoints.push_back(m_intervalExchange.m_divPoints[firstindex] - PRECISION);
                m_shiftedSeparatrixPoints.push_back(m_intervalExchange.m_divPoints[firstindex] + PRECISION);
                firstindex++;
            }
        } else {
            m_shiftedSeparatrixPoints.push_back(m_intervalExchange.m_divPointsAfterExchange[secondindex] - PRECISION);
            m_shiftedSeparatrixPoints.push_back(m_intervalExchange.m_divPointsAfterExchange[secondindex] + PRECISION);
            secondindex++;
        }
    }
    m_shiftedSeparatrixPoints.push_back(1 - PRECISION);
}