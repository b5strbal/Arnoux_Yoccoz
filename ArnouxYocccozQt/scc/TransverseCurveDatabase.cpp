#include "TransverseCurveDatabase.h"
#include "../fol/Foliation.h"
#include "SepSegmentCollectionList.h"
#include "SeparatrixSegment.h"
#include "SepSegmentDatabase.h"

#include <cassert>
//#include <mutex>

balazs::TransverseCurveDatabase::TransverseCurveDatabase(SepSegmentDatabase &sepSegmentDatabase, std::shared_ptr<SSCMode> sscMode) :
    m_sepSegmentDatabase(sepSegmentDatabase),
    m_sscMode(sscMode)
{
}

const balazs::Foliation& balazs::TransverseCurveDatabase::foliation() const
{
    return m_sepSegmentDatabase.foliation();
}


std::array<bool, 2> balazs::TransverseCurveDatabase::whichTransverseCurvesExist(const SepSegmentCollection& segments){

    assertValidCollection(segments);


    std::array<bool, 2> isCandidateForWrapsAroundEnds = {{true, true}};
    std::vector<std::pair<Mod1NumberIntExWithInfo, int>> endpointsAndIndices;

    endpointsAndIndices.reserve(segments.size());
    for (std::size_t i = 0; i < segments.size(); i++){
        endpointsAndIndices.emplace_back(segments[i]->endpoint(), i);
    }
    std::sort(endpointsAndIndices.begin(), endpointsAndIndices.end());



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


    std::vector<Mod1Number> endpoints;
    endpoints.reserve(endpointsAndIndices.size());
    for (auto& x : endpointsAndIndices) {
        endpoints.push_back(x.first);
    }

    // checking that no two endpoints are too close
    if(arePointsTooClose(endpoints)){
        return {{false, false}};
    }

    // checking that the curve is connected
    for (short wrapsAroundEnds = 0; wrapsAroundEnds < 2; wrapsAroundEnds++){
        std::size_t index = 0;
        std::size_t length = 0;
        do {
            if ((wrapsAroundEnds && index % 2 == 1) || (!wrapsAroundEnds && index % 2 == 0)) {
                index = (index + 1) % segments.size();
            } else
                index = (index + (segments.size()- 1)) % segments.size();
            int pair = endpointsAndIndices[index].second % 2 == 0 ? endpointsAndIndices[index].second + 1 :
            endpointsAndIndices[index].second - 1;
            auto it = std::lower_bound(endpoints.begin(), endpoints.end(), segments[pair]->endpoint());
            index = (it - endpoints.begin()) % segments.size();
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
    IntervalNeighborhoods inhIntersection(inhVector);

    for (short wrapsAroundEnds = 0; wrapsAroundEnds < 2; wrapsAroundEnds++){
        if (isCandidateForWrapsAroundEnds[wrapsAroundEnds]) {
            for (std::size_t i = wrapsAroundEnds ? 1 : 0; i < segments.size(); i += 2) {
                int next = (i + 1) % segments.size();
                bool throughTopDivPoint = segments[endpointsAndIndices[i].second]->vDirection() == VDirection::Down &&
                        segments[endpointsAndIndices[next].second]->vDirection() == VDirection::Down ? true : false;


                if (!inhIntersection.containsIntervalThroughADivPoint(endpointsAndIndices[i].first,
                                                                  endpointsAndIndices[next].first,
                                                                  throughTopDivPoint))
                {
                    isCandidateForWrapsAroundEnds[wrapsAroundEnds] = false;
                }
//                if(segments[endpointsAndIndices[i].second]->vDirection() !=
//                        segments[endpointsAndIndices[next].second]->vDirection()){
//                    // maybe cheching this simplification, but it might not work well with the singularity wrap mode
//                }
            }
        }
    }

    return isCandidateForWrapsAroundEnds;
}




void balazs::TransverseCurveDatabase::generateTransverseCurves(int maxdepth,
                                                               int maxInvolvedSingularities,
                                                               const bool& quit, std::mutex& mutex)
{
    SepSegmentCollectionList collections(m_sepSegmentDatabase, maxdepth, maxInvolvedSingularities,
                                      m_sscMode);

    for (const SepSegmentCollection& sepSegmentCollection : collections) {

        std::array<bool, 2> isWrapsAroundEndsGood = whichTransverseCurvesExist(sepSegmentCollection);
        for (short wrapsAroundEnds = 0; wrapsAroundEnds < 2; wrapsAroundEnds++ ) {
            if (isWrapsAroundEndsGood[wrapsAroundEnds]) {
                try{
                    m_transverseCurves.emplace(sepSegmentCollection, wrapsAroundEnds, m_sepSegmentDatabase);
                } catch(const std::exception&) {
                    // if touching segments cannot be generated for the transverse curve, i.e. it cannot be constructed
                    // we silently ignore it.
                }
            }
        }
        std::lock_guard<std::mutex> lk(mutex);
        if(quit){
            break;
        }
    }
}



