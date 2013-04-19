#ifndef FOLIATIONDISK_H
#define FOLIATIONDISK_H

#include "WeighedTree.h"
#include "IntervalExchangeMap.h"

/*!
 * \brief The FoliationDisk class stores a measured foliation on a disk that is transverse to the boundary.
 * \author Balazs Strenner, strenner@math.wisc.edu
 * \date April 19, 2013
 * \see Foliation, FoliationSphere, WeighedTree, UnitIntervalPoint, IntervalExchangeFoliationDisk
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
 * Internally, both representations are stored, but since the WeighedTree representation is more
 * desciptive, that has to be used in the constructor. (Also, an interval exchange transformation has
 * to be quite special to result in a FoliationDisk: even if it is an involution, one has to make sure
 * that there are no "crossing" pairs, so some checking would be necessary to make sure the
 * construction can happen.)
 */

class FoliationDisk
{
public:
    //! Constructs a FoliationDisk from a WeighedTree
    FoliationDisk(const WeighedTree& wt);

    //! Returns the WeighedTree representation.
    const WeighedTree& weighedTree() const { return m_weighedTree; }

    //! Returns the interval exchange representation.
    const IntervalExchangeFoliationDisk& intervalPairing() const { return m_intervalPairing; }

    //! Returns the number of separatrices.
    int numSeparatrices() const { return m_intervalPairing.size(); }

    //! Returns the list of division points (intersections of separatrices with the boundary)
    const std::vector<UnitIntervalPoint>& divPoints() const { return m_intervalPairing.divPoints(); }

    //! Prints the data of the object into a string
    std::string print() const;

private:
    IntervalExchangeFoliationDisk m_intervalPairing; // the interval exchange representation
    WeighedTree m_weighedTree;                       // the WeighedTree representation
};

#endif // FOLIATIONDISK_H
