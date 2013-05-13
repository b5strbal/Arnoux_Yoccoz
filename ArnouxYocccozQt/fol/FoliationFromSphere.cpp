#include "FoliationFromSphere.h"
#include "FoliationSphere.h"


balazs::FoliationFromSphere::FoliationFromSphere(const std::vector<floating_point_type> &lengths,
                                                 const Permutation &permutation,
                                                 floating_point_type twist) :
    Foliation(lengths, permutation, twist)
{
}


balazs::FoliationFromSphere balazs::FoliationFromSphere::fromFoliationSphere(const FoliationSphere &foliationSphere)
{
    std::vector<ConnectedPoints> allConnectedPoints;
    allConnectedPoints.reserve(foliationSphere.topFoliation().numSeparatrices() + foliationSphere.bottomFoliation().numSeparatrices());

    generateTopConnectingPairs(foliationSphere, allConnectedPoints);
    generateBottomConnectingPairs(foliationSphere, allConnectedPoints);


    std::sort(allConnectedPoints.begin(), allConnectedPoints.end(), [] (const ConnectedPoints& cp1, const ConnectedPoints& cp2)
              {
                  return cp1.topPoint < cp2.topPoint;
              } );
    std::vector<ConnectedPoints>& allConnectedPointsSortedByTop = allConnectedPoints;
    std::vector<Mod1Number> allBottomPoints;
    allBottomPoints.reserve(allConnectedPoints.size());

    for (ConnectedPoints cp : allConnectedPoints) {
        allBottomPoints.push_back(cp.bottomPoint);
    }

    std::sort(allBottomPoints.begin(), allBottomPoints.end());

    std::vector<floating_point_type> lengths(allConnectedPoints.size());
    for (std::size_t i = 0; i < allConnectedPoints.size() - 1; i++) {
        lengths[i] = distanceBetween( allConnectedPointsSortedByTop[i].topPoint, allConnectedPointsSortedByTop[i + 1].topPoint);
    }
    lengths[allConnectedPoints.size() - 1] = -allConnectedPointsSortedByTop[allConnectedPoints.size() - 1].topPoint;

    std::vector<std::size_t> permutationInput(allConnectedPoints.size());
    for (std::size_t i = 0; i < allConnectedPoints.size(); i++) {
        auto it = std::lower_bound(allBottomPoints.begin(), allBottomPoints.end(), allConnectedPointsSortedByTop[i].bottomPoint);
        permutationInput[i] = it - allBottomPoints.begin() ;
    }
    Permutation permutation(permutationInput);
    floating_point_type twist = allBottomPoints[0];

    return FoliationFromSphere(lengths, permutation, twist);
}










void balazs::FoliationFromSphere::generateTopConnectingPairs(const FoliationSphere& foliationSphere,
                                                         std::vector<ConnectedPoints>& allConnectedPoints)
{
    std::size_t numSeparatrices = foliationSphere.topFoliation().numSeparatrices();
    for (std::size_t i = 0; i < numSeparatrices; i++) {
        if (foliationSphere.topFoliation().intervalPairing().permutation()[integerMod(i, numSeparatrices)] !=
                integerMod(static_cast<int>(i) - 1, numSeparatrices)) {
            // otherwise the current separatrix emanates from a 1-pronged singularity which is not important

            ConnectedPoints newConnectedPoints;
            newConnectedPoints.topPoint = foliationSphere.topFoliation().intervalPairing().divPoints()[i];

            int indexOfConnectedSeparatrix =
                    integerMod(foliationSphere.topFoliation().intervalPairing().permutation()[i]+ 1, numSeparatrices);
            Mod1Number middlePoint =
                    foliationSphere.topFoliation().intervalPairing().divPoints()[indexOfConnectedSeparatrix];

            newConnectedPoints.bottomPoint = foliationSphere.bottomFoliation().intervalPairing().applyTo(middlePoint - foliationSphere.twist()) + foliationSphere.twist();


            allConnectedPoints.push_back( newConnectedPoints );
        }
    }
}



void balazs::FoliationFromSphere::generateBottomConnectingPairs(const FoliationSphere& foliationSphere,
                                                            std::vector<ConnectedPoints>& allConnectedPoints)
{
    std::size_t numSeparatrices = foliationSphere.bottomFoliation().numSeparatrices();
    for (std::size_t i = 0; i < numSeparatrices; i++) {
        if (foliationSphere.bottomFoliation().intervalPairing().permutation()[i] !=
                integerMod(static_cast<int>(i) - 1, numSeparatrices) ) {

            ConnectedPoints newConnectedPoints;
            newConnectedPoints.bottomPoint = static_cast<Mod1Number>(foliationSphere.bottomFoliation().intervalPairing().divPoints()[i]) + foliationSphere.twist();

            int indexOfConnectedSeparatrix = integerMod(foliationSphere.bottomFoliation().intervalPairing().permutation()[i] + 1, numSeparatrices);
            Mod1Number middlePoint = static_cast<Mod1Number>(foliationSphere.topFoliation().intervalPairing().divPoints()[indexOfConnectedSeparatrix]) + foliationSphere.twist();

            newConnectedPoints.topPoint = foliationSphere.topFoliation().intervalPairing().applyTo(middlePoint);

            allConnectedPoints.push_back( newConnectedPoints );
        }
    }
}
