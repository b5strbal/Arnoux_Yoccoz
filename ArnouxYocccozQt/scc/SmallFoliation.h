#ifndef SMALLFOLIATION_H
#define SMALLFOLIATION_H

#include <vector>
#include <map>
#include "../intex/Mod1NumberIntExchange.h"
#include "../math/Permutation.h"
#include "../Eigen/Dense"



namespace balazs{

class TransverseCurve;


class SmallFoliation
{
public:
    SmallFoliation(const TransverseCurve& tc, std::size_t referenceZeroIndex, bool flippedOver, bool orientationReversing);

    const std::vector<Mod1NumberIntExchange>& lengths() const;
    const std::vector<long double>& normalizedLengths() const;
    const Permutation& permutation() const { return m_permutation; }
    const Mod1NumberIntExchange& twist() const { return m_twist; }
    long double normalizedTwist() const { return m_normalizedTwist; }
    const std::map<HDirection, std::vector<std::vector<std::size_t>>>& stripHeights() const;
    const Eigen::MatrixXd& transitionMatrix() const;
    const Eigen::EigenSolver<Eigen::MatrixXd>::EigenvalueType& eigenvalues() const;
    const Eigen::EigenSolver<Eigen::MatrixXd>::EigenvectorsType& eigenvectors() const;

    enum WhatIsWrong {
        Permutation_Does_Not_Match,
        No_Appropriate_EigenValue,
        No_Positive_EigenVector,
        Nothing
    };

    WhatIsWrong isGoodCandidate() const;

private:
    void initLengths() const;
    void initTransitionMatrix() const;
    void initStripHeights() const;
    void initEigenSolver() const;

    const TransverseCurve& m_transverseCurve;
    mutable std::vector<Mod1NumberIntExchange> m_lengths;
    mutable std::vector<long double> m_normalizedLengths;
    Permutation m_permutation;
    mutable Mod1NumberIntExchange m_twist;
    long double m_normalizedTwist;
    Mod1NumberIntExchange m_totalLength;

    mutable std::map<HDirection, std::vector<std::vector<std::size_t>>> m_stripHeights;
    mutable Eigen::MatrixXd m_transitionMatrix;
    mutable std::unique_ptr<Eigen::EigenSolver<Eigen::MatrixXd>::EigenvalueType> m_eigenvalues;
    mutable std::unique_ptr<Eigen::EigenSolver<Eigen::MatrixXd>::EigenvectorsType> m_eigenvectors;

    bool m_movingForward;
    std::vector<Mod1NumberIntExchange> m_frontDivPoints;
    std::vector<Mod1NumberIntExchange> m_sortedFrontDivPoints;
};

}

#endif // SMALLFOLIATION_H
