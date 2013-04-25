#include "TransverseCurveDatabase.h"
#include "Modint.h"
#include "SepSegmentCollection.h"
#include "Choose.h"

TransverseCurveDatabase::TransverseCurveDatabase(SepSegmentDatabase &sepSegmentDatabase) :
    m_sepSegmentDatabase(sepSegmentDatabase)
{
}


std::array<bool, 2> TransverseCurveDatabase::whichTransverseCurvesExist(const SepSegmentCollection& segments){

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

    std::array<bool, 2> isCandidateForWrapsAroundEnds = {{true, true}};
    std::vector<std::pair<Mod1Number, int>> endpointsAndIndices;

    endpointsAndIndices.reserve(segments.size());
    for (unsigned int i = 0; i < segments.size(); i++){
        endpointsAndIndices.emplace_back(segments[i]->endpoint(), i);
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

    std::vector<Mod1Number> endpoints;
    endpoints.reserve(endpointsAndIndices.size());
    for (auto& x : endpointsAndIndices) {
        endpoints.push_back(x.first);
    }


    for (short wrapsAroundEnds = 0; wrapsAroundEnds < 2; wrapsAroundEnds++){
        Modint index(0, static_cast<int>(segments.size()));
        unsigned int length = 0;
        do {
            if ((wrapsAroundEnds && index % 2 == 1) || (!wrapsAroundEnds && index % 2 == 0)) {
                ++index;
            } else
                --index;
            int pair = endpointsAndIndices[index].second % 2 == 0 ? endpointsAndIndices[index].second + 1 :
            endpointsAndIndices[index].second - 1;
            auto it = std::lower_bound(endpoints.begin(), endpoints.end(), segments[pair]->endpoint());
            index = Modint(it - endpoints.begin(), segments.size());
            length += 2;
        } while (index != 0);
        if (length < segments.size()) {
            isCandidateForWrapsAroundEnds[wrapsAroundEnds] = false;
        }
    }


    // checking that the curve is minimal (can't be simplified trivially) and that it is simple

    std::vector<const IntervalNeighborhoods*> inhVector;
    inhVector.reserve(segments.size());
    for (unsigned int i = 0; i < segments.size(); i++) {
        inhVector.push_back(&segments[i]->m_intervalNeighborhoods);
    }
    IntervalNeighborhoods inhIntersection = IntervalNeighborhoods::intersect(inhVector);

    for (short wrapsAroundEnds = 0; wrapsAroundEnds < 2; wrapsAroundEnds++){
        if (isCandidateForWrapsAroundEnds[wrapsAroundEnds]) {
            for (unsigned int i = wrapsAroundEnds ? 1 : 0; i < segments.size(); i += 2) {
                int next = (i + 1) % segments.size();
                bool throughTopDivPoint = segments[endpointsAndIndices[i].second]->direction() == Direction::DOWN &&
                        segments[endpointsAndIndices[next].second]->direction() == Direction::DOWN ? true : false;


                if (!inhIntersection.containsIntervalThroughADivPoint(endpointsAndIndices[i].first,
                                                                  segments[endpointsAndIndices[i].second]->m_smallContainingInterval,
                                                                  endpointsAndIndices[next].first,
                                                                  segments[endpointsAndIndices[next].second]->m_smallContainingInterval,
                                                                  throughTopDivPoint))
                {
                    isCandidateForWrapsAroundEnds[wrapsAroundEnds] = false;
                }
            }
        }
    }

    return isCandidateForWrapsAroundEnds;
}



void TransverseCurveDatabase::applyToStoredTransverseCurves(void (*function)(const TransverseCurve&)){
   assert(function != nullptr);
   for ( auto curve : m_transverseCurves )
        function(curve);
}



void TransverseCurveDatabase::generateTransverseCurves(int maxdepth, int numLeafComponents, void (*function)(const TransverseCurve&)){
    //m_sepSegmentDatabase.generateSepSegments(maxdepth);
    SepSegmentCollections collections(m_sepSegmentDatabase, maxdepth,
                                      SepSegmentCollections::Mode::SEGMENTS_SHIFTED_TO_RIGHT, numLeafComponents);

    for (const SepSegmentCollection& sepSegmentCollection : collections) {
        std::array<bool, 2> isWrapsAroundEndsGood = whichTransverseCurvesExist(sepSegmentCollection);
        for (short wrapsAroundEnds = 0; wrapsAroundEnds < 2; wrapsAroundEnds++ ) {
            if (isWrapsAroundEndsGood[wrapsAroundEnds]) {
                auto ret = m_transverseCurves.emplace(m_sepSegmentDatabase.m_foliation, sepSegmentCollection, wrapsAroundEnds).first;
                if (function != nullptr) {
                    function(*ret);
                }
            }
        }
    }
}








