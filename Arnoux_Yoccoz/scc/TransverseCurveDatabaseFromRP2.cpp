#include "TransverseCurveDatabaseFromRP2.h"

TransverseCurveDatabaseFromRP2::TransverseCurveDatabaseFromRP2(SepSegmentDatabaseFromRP2& sepSegmentDatabaseFromRP2) :
    TransverseCurveDatabase(sepSegmentDatabaseFromRP2)
{

}




/*
void TransverseCurveDatabaseFromRP2::generateTransverseCurvesFromRP2(int maxdepth, int numLeafComponents){
    assert(numLeafComponents % 2 == 0);

    m_sepSegmentDatabase.generateSepSegments(maxdepth);
    SepSegmentCollectionsFromRP2

    std::vector<std::list<SeparatrixSegment>::const_iterator> transverseCurveInput;
    transverseCurveInput.reserve(4);

    for (int index = 0; index < m_numIntervals; index++) {
        if (index < m_separatrixPair[index]) {


            auto itFirstDown = m_goodShiftedSeparatrixSegments[DOWNWARDS][index].begin();
            auto itSecondUp = m_goodShiftedSeparatrixSegments[UPWARDS][m_separatrixPair[index]].begin();
            while (itFirstDown != --m_goodShiftedSeparatrixSegments[DOWNWARDS][index].end() &&
                   itSecondUp != --m_goodShiftedSeparatrixSegments[UPWARDS][m_separatrixPair[index]].end() &&
                   itFirstDown->m_depth <= maxdepth &&
                   itSecondUp->m_depth <= maxdepth) {
                assert(itFirstDown->m_depth == itSecondUp->m_depth);

                auto itFirstUp = m_goodShiftedSeparatrixSegments[UPWARDS][index].begin();
                auto itSecondDown = m_goodShiftedSeparatrixSegments[DOWNWARDS][m_separatrixPair[index]].begin();
                while (itFirstUp != --m_goodShiftedSeparatrixSegments[UPWARDS][index].end() &&
                       itSecondDown != --m_goodShiftedSeparatrixSegments[DOWNWARDS][m_separatrixPair[index]].end() &&
                       itFirstUp->m_depth <= maxdepth &&
                       itSecondDown->m_depth <= maxdepth) {

                    assert(itFirstUp->m_depth == itSecondDown->m_depth);

                    transverseCurveInput = {
                        itFirstDown,
                        itFirstUp,
                        itSecondUp,
                        itSecondDown
                    };

                    std::array<bool, 2> isWrapsAroundZeroGood = whichTransverseCurvesExist(transverseCurveInput);
                    for (short wrapsAroundZero = 0; wrapsAroundZero < 2; wrapsAroundZero++ ) {
                        if (isWrapsAroundZeroGood[wrapsAroundZero]) {
                            auto ret = m_transverseCurves.emplace(*this, transverseCurveInput, wrapsAroundZero);
                            if (ret.second) {
                                m_liftsOfGoodTransverseCurves.insert(ret.first);
                            }
                        }
                    }
                    itFirstUp++;
                    itSecondDown++;
                }
                itFirstDown++;
                itSecondUp++;
            }

        }
    }
}



void TransverseCurveDatabaseFromRP2::printLiftsOfGoodTransverseCurves(int depth){
    generateTransverseCurvesFromRP2(depth);
    for (auto it : m_liftsOfGoodTransverseCurves)
        std::cout << it->print() << "\n\n";
}
*/
