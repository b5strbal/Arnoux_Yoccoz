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
        assert(goodSegmentIndices[i]->m_startingSingularity == goodSegmentIndices[i + 1]->m_startingSingularity);
        assert(singularities[goodSegmentIndices[i]->m_startingSingularity] == 0);
        singularities[goodSegmentIndices[i]->m_startingSingularity] = 1;
        assert(goodSegmentIndices[i]->m_direction != goodSegmentIndices[i + 1]->m_direction);
    }
*/

    std::array<bool, 2> isCandidateForWrapsAroundZero = {{true, true}};
    std::vector<std::pair<UnitIntervalPoint, int>> endpointsAndIndices;

    endpointsAndIndices.reserve(segments.size());
    for (unsigned int i = 0; i < segments.size(); i++){
        endpointsAndIndices.emplace_back(segments[i].m_endpoint, i);
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
            auto it = std::lower_bound(endpoints.begin(), endpoints.end(), segments[pair].m_endpoint);
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
                bool throughTopDivPoint = segments[endpointsAndIndices[i].second].m_direction == DOWNWARDS &&
                        segments[endpointsAndIndices[next].second].m_direction == DOWNWARDS ? true : false;


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

