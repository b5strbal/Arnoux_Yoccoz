#ifndef FOLIATIONDISK_H
#define FOLIATIONDISK_H

#include "../math/WeighedTree.h"
#include "../intex/IntervalPairing.h"

namespace balazs{


/*!
 * \brief The FoliationDisk class stores a measured foliation on a disk that is transverse to the boundary.
 * \author Balazs Strenner, strenner@math.wisc.edu
 * \date April 19, 2013
 *
 * The boundary of the disk is assumed to have circumference 1, therefore points on the boundary are
 * represented by UnitIntervalPoints. The foliation must have singularities and therefore separatrices.
 * The separatrices divide the foliation into rectangles, foliated horizontally. One can
 * describe such a setting by an interval exchange map of the of the unit interval where the permutation
 * is an involution (see IntervalExchangeFoliationDisk). This parametrization is unique up to
 * Whitehead equivalence. We assume that the foliation does not have saddle connections.
 *
 * Although the interval exchange representation is convenient for computations, e.g. transformations like
 * rotations, it doesn't retain the combinatorics of the foliation very well. A WeighedTree is much
 * better in that sense. The idea is that for each rectangle we get from the decomposition by the
 * separatrices, we connect the two singularities on the opposite sides of the rectangle by a curve
 * transverse to the foliation. The result is a tree. To each edge we can assign a weight, the
 * measure of the edge according to the foliation. This way we get a WeighedTree. Singularities
 * of the foliation correspond to vertices of the WeighedTree. The number of prongs of a singularity
 * equals the degree of the vertex in the tree, so the combinatorics of the foliation is well reflected.
 *
 * We can also construct a FoliationDisk form a WeighedTree uniquely by reversing the above process.
 * Imagine that the edges of the tree are made just a little thicker, and air is pumped into the inside of the tree.
 * Since it is a tree, it expands to a disk. Assume that the boundary doesn't stretch,
 * so each side of each edge takes up as long part of the boundary circle as it's weighed.
 * The circumference of the circle will be twice the sum of the weights of the edges.
 * There is a unique measured foliation up to Whitehead equivalence that pairs up the segments of the
 * boundary circle that come from the same edge.
 *
 * Internally, both representations are stored, but since the WeighedTree representation is more
 * desciptive, that has to be used in the constructor. (Also, an interval exchange transformation has
 * to be quite special to result in a FoliationDisk: even if it is an involution, one has to make sure
 * that there are no "crossing" pairs, so some checking would be necessary to make sure the
 * construction can happen.)
 */

class FoliationDisk
{
public:
    FoliationDisk(const WeighedTree& wt);
    FoliationDisk(const FoliationDisk&) = delete;
    FoliationDisk& operator=(const FoliationDisk&) = delete;

    const WeighedTree& weighedTree() const { return m_weighedTree; }
    const IntervalPairing& intervalPairing() const { return m_intervalPairing; }
    int numSeparatrices() const { return m_intervalPairing.size(); }
   // const std::vector<Mod1NumberIntExchange>& divPoints() const { return m_intervalPairing.divPoints(); }
    friend std::ostream& operator<<(std::ostream& out, const FoliationDisk& d);

private:
    IntervalPairing m_intervalPairing; // the interval exchange representation
    WeighedTree m_weighedTree;                       // the WeighedTree representation
};

}

#endif // FOLIATIONDISK_H
