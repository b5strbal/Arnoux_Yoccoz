#ifndef TRANSVERSECURVEDATA_H
#define TRANSVERSECURVEDATA_H

#include <memory>
#include <vector>

namespace balazs{

class TransverseCurve;
class SepSegmentDatabase;
class Mod1NumberIntExchange;
class Permutation;
class SeparatrixSegment;
struct SepSegmentIndex;

class TransverseCurveData
{
public:
    TransverseCurveData(const TransverseCurve& tc, SepSegmentDatabase& ssDatabase);
    ~TransverseCurveData();
    TransverseCurveData(TransverseCurveData&& other);
    const TransverseCurve& transverseCurve() const;

    const SeparatrixSegment& touchingSepSegment(const SepSegmentIndex& ssIndex) const;
    const std::vector<Mod1NumberIntExchange>& lengths() const;
    const Permutation& intervalPermutation() const;
    const Permutation& singularityPermutation() const;
    const Mod1NumberIntExchange& twist() const;
    Mod1NumberIntExchange length() const;

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};


}

#endif // TRANSVERSECURVEDATA_H
