#include "TransverseCurveDatabase.h"
#include "../fol/Foliation.h"
#include "SepSegmentCollectionList.h"
#include "SeparatrixSegment.h"
#include "SepSegmentDatabase.h"

balazs::TransverseCurveDatabase::TransverseCurveDatabase(SepSegmentDatabase &sepSegmentDatabase) :
    m_sepSegmentDatabase(sepSegmentDatabase)
{
}

const balazs::Foliation& balazs::TransverseCurveDatabase::foliation() const
{
    return m_sepSegmentDatabase.foliation();
}


std::array<bool, 2> balazs::TransverseCurveDatabase::whichTransverseCurvesExist(const SepSegmentCollection& segments){

    assert(segments.size() % 2 == 0);
    assert(segments.size() >= 2);
    std::vector<char> singularities(foliation().numIntervals(), 0);
    for (std::size_t i = 0; i < segments.size(); i += 2){
        assert(segments[i]->startingSingularity() == segments[i + 1]->startingSingularity());
        assert(singularities[segments[i]->startingSingularity()] == 0);
        singularities[segments[i]->startingSingularity()] = 1;
        assert(segments[i]->direction() != segments[i + 1]->direction() ||
               segments[i]->side() != segments[i + 1]->side());
    }


    std::array<bool, 2> isCandidateForWrapsAroundEnds = {{true, true}};
    std::vector<std::pair<Mod1NumberIntExWithInfo, int>> endpointsAndIndices;

    endpointsAndIndices.reserve(segments.size());
    for (std::size_t i = 0; i < segments.size(); i++){
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
        for (std::size_t i = 0; i < endpointsAndIndices.size(); i++) {
            isEndpointIndexOdd[endpointsAndIndices[i].second] = (i % 2);
        }
        for (std::size_t i = 0; i < endpointsAndIndices.size(); i += 2) {
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
        int index = 0;
        std::size_t length = 0;
        do {
            if ((wrapsAroundEnds && index % 2 == 1) || (!wrapsAroundEnds && index % 2 == 0)) {
                index = integerMod(index + 1, segments.size());
            } else
                index = integerMod(index - 1, segments.size());
            int pair = endpointsAndIndices[index].second % 2 == 0 ? endpointsAndIndices[index].second + 1 :
            endpointsAndIndices[index].second - 1;
            auto it = std::lower_bound(endpoints.begin(), endpoints.end(), segments[pair]->endpoint());
            index = integerMod(it - endpoints.begin(), segments.size());
            length += 2;
        } while (index != 0);
        if (length < segments.size()) {
            isCandidateForWrapsAroundEnds[wrapsAroundEnds] = false;
        }
    }


    // checking that the curve is minimal (can't be simplified trivially) and that it is simple

    std::vector<const IntervalNeighborhoods*> inhVector;
    inhVector.reserve(segments.size());
    for (std::size_t i = 0; i < segments.size(); i++) {
        inhVector.push_back(&segments[i]->intervalNeighborhoods());
    }
    IntervalNeighborhoods inhIntersection = IntervalNeighborhoods::intersect(inhVector);

    for (short wrapsAroundEnds = 0; wrapsAroundEnds < 2; wrapsAroundEnds++){
        if (isCandidateForWrapsAroundEnds[wrapsAroundEnds]) {
            for (std::size_t i = wrapsAroundEnds ? 1 : 0; i < segments.size(); i += 2) {
                int next = (i + 1) % segments.size();
                bool throughTopDivPoint = segments[endpointsAndIndices[i].second]->direction() == Direction::DOWN &&
                        segments[endpointsAndIndices[next].second]->direction() == Direction::DOWN ? true : false;


                if (!inhIntersection.containsIntervalThroughADivPoint(endpointsAndIndices[i].first,
                                                                  endpointsAndIndices[next].first,
                                                                  throughTopDivPoint))
                {
                    isCandidateForWrapsAroundEnds[wrapsAroundEnds] = false;
                }
            }
        }
    }

    return isCandidateForWrapsAroundEnds;
}



void balazs::TransverseCurveDatabase::applyToStoredTransverseCurves(void (*function)(const TransverseCurve&)){
   assert(function != nullptr);
   for ( auto curve : m_transverseCurves )
        function(curve);
}



void balazs::TransverseCurveDatabase::generateTransverseCurves(int maxdepth, int numLeafComponents, const SSCMode& sscmode, void (*function)(const TransverseCurve&)){
    //m_sepSegmentDatabase.generateSepSegments(maxdepth);
    SepSegmentCollectionList collections(m_sepSegmentDatabase, maxdepth, numLeafComponents,
                                      sscmode);

    for (const SepSegmentCollection& sepSegmentCollection : collections) {
        std::array<bool, 2> isWrapsAroundEndsGood = whichTransverseCurvesExist(sepSegmentCollection);
        for (short wrapsAroundEnds = 0; wrapsAroundEnds < 2; wrapsAroundEnds++ ) {
            if (isWrapsAroundEndsGood[wrapsAroundEnds]) {
                auto ret = m_transverseCurves.emplace(foliation(), sepSegmentCollection, wrapsAroundEnds).first;
                if (function != nullptr) {
                    function(*ret);
                }
            }
        }
    }
}








