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

    const std::vector<Mod1NumberIntExchange>& lengths() const { return m_lengths; }
    const std::vector<long double>& normalizedLengths() const { return m_normalizedLengths; }
    const Permutation& permutation() const { return m_permutation; }
    const Mod1NumberIntExchange& twist() const { return m_twist; }
    long double normalizedTwist() const { return m_normalizedTwist; }
    const std::map<HDirection, std::vector<std::vector<std::size_t>>>& stripHeights() const { return m_stripHeights; }
    const Eigen::MatrixXd& transitionMatrix() const;
    const Eigen::EigenSolver<Eigen::MatrixXd>::EigenvalueType& eigenvalues();
    const Eigen::EigenSolver<Eigen::MatrixXd>::EigenvectorsType& eigenvectors();

    enum WhatIsWrong {
        Permutation_Does_Not_Match,
        No_Appropriate_EigenValue,
        No_Positive_EigenVector,
        Nothing
    };

    WhatIsWrong isGoodCandidate();

private:
    void initTransitionMatrix();
    void initEigenSolver();

    const TransverseCurve& m_transverseCurve;
    std::vector<Mod1NumberIntExchange> m_lengths;
    std::vector<long double> m_normalizedLengths;
    Permutation m_permutation;
    Mod1NumberIntExchange m_twist;
    long double m_normalizedTwist;
    Mod1NumberIntExchange m_totalLength;

    std::map<HDirection, std::vector<std::vector<std::size_t>>> m_stripHeights;
    Eigen::MatrixXd m_transitionMatrix;
    std::unique_ptr<Eigen::EigenSolver<Eigen::MatrixXd>::EigenvalueType> m_eigenvalues;
    std::unique_ptr<Eigen::EigenSolver<Eigen::MatrixXd>::EigenvectorsType> m_eigenvectors;
};

}

#endif // SMALLFOLIATION_H
