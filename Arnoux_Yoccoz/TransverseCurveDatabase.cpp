#include "TransverseCurveDatabase.h"
#include "Modint.h"
#include "SepSegmentCollection.h"
#include "Choose.h"

TransverseCurveDatabase::TransverseCurveDatabase(SepSegmentDatabase &sepSegmentDatabase) :
    m_sepSegmentDatabase(sepSegmentDatabase)
{
}


std::array<bool, 2> TransverseCurveDatabase::whichTransverseCurvesExist(const SepSegmentCollectionBase& segments){

/*
    assert(goodSegmentIndices.size() % 2 == 0);
    assert(goodSegmentIndices.size() >= 2);
    std::vector<short> singularities(m_sepSegmentFinder.numIntervals(), 0);
    for (unsigned int i = 0; i < goodSegmentIndices.size(); i += 2){
        assert(goodSegmentIndices[i]->startingSingularity() == goodSegmentIndices[i + 1]->startingSingularity());
        assert(singularities[goodSegmentIndices[i]->startingSingularity()] == 0);
        singularities[goodSegmentIndices[i]->startingSingularity()] = 1;
        assert(goodSegmentIndices[i]->direction() != goodSegmentIndices[i + 1]->direction());
    }
*/

    std::array<bool, 2> isCandidateForWrapsAroundZero = {{true, true}};
    std::vector<std::pair<UnitIntervalPoint, int>> endpointsAndIndices;

    endpointsAndIndices.reserve(segments.size());
    for (unsigned int i = 0; i < segments.size(); i++){
        endpointsAndIndices.emplace_back(segments[i].endpoint(), i);
    }
    std::sort(endpointsAndIndices.begin(), endpointsAndIndices.end());
    for (auto it = endpointsAndIndices.begin() + 1; it != endpointsAndIndices.end(); it++) {
        if (!((it - 1)->first < it->first)) {
            throw std::runtime_error("Some points are so close that we can't distinguish them.");
        }
    }




    // checking that the curve is transverse
    // this part does not depend on wrapsAroundZero
    {
        std::vector<short> isEndpointIndexOdd(segments.size());
        for (unsigned int i = 0; i < endpointsAndIndices.size(); i++) {
            isEndpointIndexOdd[endpointsAndIndices[i].second] = (i % 2);
        }
        for (unsigned int i = 0; i < endpointsAndIndices.size(); i += 2) {
            if (isEndpointIndexOdd[i] == isEndpointIndexOdd[i + 1]) {
                return {{false, false}};
            }
        }
    }

    // checking that the curve is connected

    std::vector<UnitIntervalPoint> endpoints;
    endpoints.reserve(endpointsAndIndices.size());
    for (auto& x : endpointsAndIndices) {
        endpoints.push_back(x.first);
    }


    for (short wrapsAroundZero = 0; wrapsAroundZero < 2; wrapsAroundZero++){
        Modint index(0, static_cast<int>(segments.size()));
        unsigned int length = 0;
        do {
            if ((wrapsAroundZero && index % 2 == 1) || (!wrapsAroundZero && index % 2 == 0)) {
                ++index;
            } else
                --index;
            int pair = endpointsAndIndices[index].second % 2 == 0 ? endpointsAndIndices[index].second + 1 :
            endpointsAndIndices[index].second - 1;
            auto it = std::lower_bound(endpoints.begin(), endpoints.end(), segments[pair].endpoint());
            index = Modint(it - endpoints.begin(), segments.size());
            length += 2;
        } while (index != 0);
        if (length < segments.size()) {
            isCandidateForWrapsAroundZero[wrapsAroundZero] = false;
        }
    }


    // checking that the curve is minimal (can't be simplified trivially) and that it is simple

    std::vector<const ArcsAroundDivPoints*> adpVector;
    adpVector.reserve(segments.size());
    for (unsigned int i = 0; i < segments.size(); i++) {
        adpVector.push_back(&segments[i].m_arcsAroundDivPoints);
    }
    ArcsAroundDivPoints adpIntersection = intersect(adpVector);

    for (short wrapsAroundZero = 0; wrapsAroundZero < 2; wrapsAroundZero++){
        if (isCandidateForWrapsAroundZero[wrapsAroundZero]) {
            for (unsigned int i = wrapsAroundZero ? 1 : 0; i < segments.size(); i += 2) {
                int next = (i + 1) % segments.size();
                bool throughTopDivPoint = segments[endpointsAndIndices[i].second].direction() == DOWNWARDS &&
                        segments[endpointsAndIndices[next].second].direction() == DOWNWARDS ? true : false;


                if (!adpIntersection.ContainsArcThroughADivPointQ(endpointsAndIndices[i].first,
                                                                  segments[endpointsAndIndices[i].second].m_smallContainingInterval,
                                                                  endpointsAndIndices[next].first,
                                                                  segments[endpointsAndIndices[next].second].m_smallContainingInterval,
                                                                  throughTopDivPoint))
                {
                    isCandidateForWrapsAroundZero[wrapsAroundZero] = false;
                }
            }
        }
    }

    return isCandidateForWrapsAroundZero;
}



void TransverseCurveDatabase::applyToStoredTransverseCurves(void (*function)(const TransverseCurve&)){
   assert(function != nullptr);
   for ( auto curve : m_transverseCurves )
        function(curve);
}



void TransverseCurveDatabase::generateTransverseCurves(int maxdepth, int numLeafComponents, void (*function)(const TransverseCurve&)){
    m_sepSegmentDatabase.generateSepSegments(maxdepth);

        for (auto indices = Choose(m_sepSegmentDatabase.numIntervals(), numLeafComponents); !indices.isAfterLast(); ++indices) {
            //  std::cout << static_cast<std::vector<int>>(indices) << "\n";
            for (SepSegmentCollectionDefault segments(m_sepSegmentDatabase, indices, maxdepth); !segments.isAfterLast(); segments.advance()) {

                std::array<bool, 2> isWrapsAroundZeroGood = whichTransverseCurvesExist(segments);
                for (short wrapsAroundZero = 0; wrapsAroundZero < 2; wrapsAroundZero++ ) {
                    if (isWrapsAroundZeroGood[wrapsAroundZero]) {
                        auto ret = m_transverseCurves.emplace(m_sepSegmentDatabase.m_foliation, segments, wrapsAroundZero).first;
                        if (function != nullptr) {
                            function(*ret);
                        }
                    }
                }
            }
        }
}




///////////////////////



TransverseCurveDatabaseFromRP2::TransverseCurveDatabaseFromRP2(SepSegmentDatabaseFromRP2& sepSegmentDatabaseFromRP2) :
    TransverseCurveDatabase(sepSegmentDatabaseFromRP2)
{

}





void TransverseCurveDatabaseFromRP2::generateTransverseCurvesFromRP2(int maxdepth, int numLeafComponents){
    assert(numLeafComponents % 2 == 0);

    m_sepSegmentDatabase.generateSepSegments(maxdepth);
    SepSegmentCollectionFromRP2

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
    generateLiftsOfGoodTransverseCurves(depth);
    for (auto it : m_liftsOfGoodTransverseCurves)
        std::cout << it->print() << "\n\n";
}
