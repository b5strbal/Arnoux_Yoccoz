#ifndef TRANSVERSECURVEDATABASE_H
#define TRANSVERSECURVEDATABASE_H

#include <set>
#include <mutex>
#include <array>
#include "TransverseCurve.h"

namespace balazs{


class TransverseCurveDatabase
{
public:
    TransverseCurveDatabase(SepSegmentDatabase& sepSegmentDatabase, std::shared_ptr<SSCMode> sscMode);
    TransverseCurveDatabase(const TransverseCurveDatabase&) = delete;
    TransverseCurveDatabase& operator=(const TransverseCurveDatabase&) = delete;

 //   void applyToStoredTransverseCurves(void (*function)(const TransverseCurve&));
    void generateTransverseCurves(int maxdepth, int maxInvolvedSingularities, const bool &quit, std::mutex& mutex);
    const Foliation& foliation() const;

private:
    SepSegmentDatabase& m_sepSegmentDatabase;
    std::shared_ptr<SSCMode> m_sscMode;
    std::set<TransverseCurve> m_transverseCurves;

    std::array<bool, 2> whichTransverseCurvesExist(const SepSegmentCollection& segments);
};

}


#endif // TRANSVERSECURVEDATABASE_H
