#ifndef INTERVALEXCHANGEFOLIATIONDISK_H
#define INTERVALEXCHANGEFOLIATIONDISK_H

#include "IntervalExchangeBase.h"
#include "WeighedTree.h"


class IntervalExchangeFoliationDisk : public IntervalExchangeBase

{
public:
    static IntervalExchangeFoliationDisk fromWeighedTree(const WeighedTree& wt);

    virtual Mod1Number applyTo(const Mod1Number& point) const;
    virtual Mod1NumberIntExchange applyTo(const Mod1NumberIntExchange& point) const;
    virtual Mod1Number applyInverseTo(const Mod1Number& point) const { return applyTo(point); }
    virtual Mod1NumberIntExchange applyInverseTo(const Mod1NumberIntExchange& point) const;
    const Permutation& permutation() const { return m_permutation; }

private:
    IntervalExchangeFoliationDisk(const std::vector<floating_point_type>& lengths, const Permutation& permutation);

    static void fillInLengthsAndPairing(std::vector<floating_point_type>& lengths,
                                 std::vector<unsigned int> &pairing,
                                 int StartingIndex,
                                 WeighedTree::Node* pNode);

};

#endif // INTERVALEXCHANGEFOLIATIONDISK_H
