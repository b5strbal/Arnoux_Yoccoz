#include "SmallFoliation.h"
#include "TransverseCurve.h"
#include "SeparatrixSegment.h"
#include "../fol/Foliation.h"
#include <cassert>

balazs::SmallFoliation::SmallFoliation(const balazs::TransverseCurve &tc,
                                       std::size_t referenceZeroIndex, bool flippedOver, bool orientationReversing)
    : m_transverseCurve(tc),
      m_totalLength(tc.disjointIntervals().totalLength()),
      m_movingForward((!orientationReversing && !flippedOver) || (orientationReversing && flippedOver))
{
    assert(referenceZeroIndex < tc.foliation().numIntervals());

    const std::vector<Mod1NumberIntExchange>& bottomIntersections = m_movingForward ? tc.bottomRightIntersections() :
                                                                                    tc.bottomLeftIntersections();

    const std::vector<Mod1NumberIntExchange>& frontIntersections = flippedOver ? bottomIntersections :
                                                                                 tc.topIntersections();
    const std::vector<Mod1NumberIntExchange>& backIntersections = flippedOver ? tc.topIntersections() :
                                                                                 bottomIntersections;

    Mod1NumberIntExchange referenceZero = frontIntersections[referenceZeroIndex];



    // m_frontDivPoints
    m_frontDivPoints.reserve(tc.foliation().numIntervals());
    {
        Mod1NumberIntExchange distance;
        for(std::size_t i = 0; i < tc.foliation().numIntervals(); i++){
            distance = m_movingForward ? tc.distanceOnCurve(referenceZero, frontIntersections[i]) :
                                       tc.distanceOnCurve(frontIntersections[i], referenceZero);
            m_frontDivPoints.push_back(distance);
        }
    }
    m_sortedFrontDivPoints = m_frontDivPoints;
    std::sort(m_sortedFrontDivPoints.begin(), m_sortedFrontDivPoints.end());



    // backDivPoints
    std::vector<Mod1NumberIntExchange> backDivPoints;
    backDivPoints.reserve(tc.foliation().numIntervals());
    {
        Mod1NumberIntExchange distance;
        for(std::size_t i = 0; i < tc.foliation().numIntervals(); i++){
            distance = m_movingForward ? tc.distanceOnCurve(referenceZero, backIntersections[i]) :
                                       tc.distanceOnCurve(backIntersections[i], referenceZero);
            backDivPoints.push_back(distance);
        }
    }
    std::vector<Mod1NumberIntExchange> sortedBackDivPoints = backDivPoints;
    std::sort(sortedBackDivPoints.begin(), sortedBackDivPoints.end());


    // m_twist
    m_twist = sortedBackDivPoints[0];

    // m_normalizedTwist
    m_normalizedTwist = m_twist / m_totalLength;


    // m_permutation
    std::vector<std::size_t> permutationInput;
    for(std::size_t i = 0; i < tc.foliation().numIntervals(); i++){
        std::size_t singularity = std::find(m_frontDivPoints.begin(), m_frontDivPoints.end(),
                                            m_sortedFrontDivPoints[i]) - m_frontDivPoints.begin();
        permutationInput.push_back(std::lower_bound(sortedBackDivPoints.begin(), sortedBackDivPoints.end(),
                                               backDivPoints[singularity]) - sortedBackDivPoints.begin());
    }
    m_permutation = Permutation(permutationInput);
}

const std::vector<balazs::Mod1NumberIntExchange> &balazs::SmallFoliation::lengths() const
{
    initLengths();
    return m_lengths;
}

const std::vector<long double> &balazs::SmallFoliation::normalizedLengths() const
{
    initLengths();
    return m_normalizedLengths;
}

const std::map<balazs::HDirection, std::vector<std::vector<std::size_t> > > &balazs::SmallFoliation::stripHeights() const
{
    initStripHeights();
    return m_stripHeights;
}




const Eigen::MatrixXd &balazs::SmallFoliation::transitionMatrix() const
{
    initTransitionMatrix();
    return m_transitionMatrix;
}

const Eigen::EigenSolver<Eigen::MatrixXd>::EigenvalueType &balazs::SmallFoliation::eigenvalues() const
{
    initEigenSolver();
    return *m_eigenvalues;
}

const Eigen::EigenSolver<Eigen::MatrixXd>::EigenvectorsType &balazs::SmallFoliation::eigenvectors() const
{
    initEigenSolver();
    return *m_eigenvectors;
}



bool allEntriesPositive(const Eigen::VectorXcd& vector){
    static const long double allowedError = 0.000000001L;

    for(int i = 0; i < vector.rows(); i++){
        if(fabs(vector(i).imag()) > allowedError || vector(i).real() < allowedError){
            return false;
        }
    }
    return true;
}




balazs::SmallFoliation::WhatIsWrong balazs::SmallFoliation::isGoodCandidate() const
{

    static const long double allowedError = 0.000000001L;


    WhatIsWrong currentProblem = Permutation_Does_Not_Match;

    if(permutation() != m_transverseCurve.foliation().intExchange().permutationWithMinimalTwist())
        return currentProblem;

    currentProblem = No_Appropriate_EigenValue;

    initEigenSolver();

    for(int i = 0; i < m_eigenvalues->rows(); i++){
        if(fabs((*m_eigenvalues)[i].imag()) < allowedError && (*m_eigenvalues)[i].real() > allowedError &&
                (*m_eigenvalues)[i].real() < 1 - allowedError){
            currentProblem = std::max(currentProblem, No_Positive_EigenVector);

            if(allEntriesPositive(-m_eigenvectors->col(i)) || allEntriesPositive(m_eigenvectors->col(i)))
                return Nothing;
        }
    }
    return currentProblem;
}

void balazs::SmallFoliation::initLengths() const
{
    if(!m_lengths.empty()) return;

    // m_lengths
    m_lengths.reserve(m_permutation.size());
    for(std::size_t i = 0; i < m_permutation.size() - 1; i++){
        m_lengths.push_back(m_sortedFrontDivPoints[i + 1] - m_sortedFrontDivPoints[i]);
    }
    m_lengths.push_back(m_totalLength - m_sortedFrontDivPoints.back());

    // m_normalizedLengths
    m_normalizedLengths.reserve(m_lengths.size());
    for(const auto& x : m_lengths){
        m_normalizedLengths.emplace_back(x / m_totalLength);
    }
}



void balazs::SmallFoliation::initTransitionMatrix() const
{
    if(m_transitionMatrix.cols() != 0) return;

    initLengths();

    m_transitionMatrix.resize(m_lengths.size() + 1, m_lengths.size() + 1);
    for(std::size_t i = 0; i < m_lengths.size(); i++){
        m_lengths[i].adjustCoefficients();
        for(std::size_t j = 0; j < m_lengths.size(); j++){
            m_transitionMatrix(i, j) = m_lengths[i].coefficients()[j];
        }
        m_transitionMatrix(i, m_lengths.size()) = m_lengths[i].twistCoeff();
    }
    m_twist.adjustCoefficients();
    for(std::size_t j = 0; j < m_lengths.size(); j++){
        m_transitionMatrix(m_lengths.size(), j) = m_twist.coefficients()[j];
    }
    m_transitionMatrix(m_lengths.size(), m_lengths.size()) = m_twist.twistCoeff();
}



void balazs::SmallFoliation::initStripHeights() const
{
    if(!m_stripHeights.empty()) return;

    for(HDirection hDirection : { HDirection::Left, HDirection::Right}){
        m_stripHeights[hDirection].resize(m_transverseCurve.foliation().numIntervals());
        for(std::size_t i = 0; i < m_transverseCurve.foliation().numIntervals(); i++){
            m_stripHeights[hDirection][i].resize(m_transverseCurve.foliation().numIntervals());
        }
    }

    for(std::size_t i = 0; i < m_transverseCurve.foliation().numIntervals(); i++){
        std::size_t singularity = std::find(m_frontDivPoints.begin(), m_frontDivPoints.end(),
                                            m_sortedFrontDivPoints[i]) - m_frontDivPoints.begin();
        for(std::size_t j = 0; j < m_transverseCurve.foliation().numIntervals(); j++){
            std::map<HDirection, HDirection> source;
            source[HDirection::Right] = m_movingForward ? HDirection::Right : HDirection::Left;
            source[HDirection::Left] = m_movingForward ? HDirection::Left : HDirection::Right;

            for(HDirection hDirection : { HDirection::Left, HDirection::Right}){

            const SeparatrixSegment& upCentered = m_transverseCurve.touchingSepSegment({source[hDirection], VDirection::Up, singularity},SepSegmentDatabase::Centered);
            const SeparatrixSegment& downCentered = m_transverseCurve.touchingSepSegment({source[hDirection], VDirection::Down, singularity},SepSegmentDatabase::Centered);
            const SeparatrixSegment& upShifted = m_transverseCurve.touchingSepSegment({source[hDirection], VDirection::Up, singularity},SepSegmentDatabase::ShiftedEvenMore);
            const SeparatrixSegment& downShifted = m_transverseCurve.touchingSepSegment({source[hDirection], VDirection::Down, singularity},SepSegmentDatabase::ShiftedEvenMore);

            if(upCentered.depth() < upShifted.depth()){
                m_stripHeights[hDirection][i][j] = downCentered.intervalIntersectionCount()[j] -
                        downShifted.intervalIntersectionCount()[j];
            } else

            if(downCentered.depth() < downShifted.depth()){
                m_stripHeights[hDirection][i][j] = upCentered.intervalIntersectionCount()[j] -
                        upShifted.intervalIntersectionCount()[j];
            } else {
                m_stripHeights[hDirection][i][j] = upShifted.intervalIntersectionCount()[j] +
                        downShifted.intervalIntersectionCount()[j];
            }

            }
        }
    }
}



void balazs::SmallFoliation::initEigenSolver() const
{
    if(!m_eigenvalues){
        initTransitionMatrix();
        Eigen::EigenSolver<Eigen::MatrixXd> es(m_transitionMatrix);
        if(es.info() != Eigen::Success){
            throw std::runtime_error("Eigenvalues cannot be computed.");
        }
        m_eigenvalues.reset(new Eigen::EigenSolver<Eigen::MatrixXd>::EigenvalueType(es.eigenvalues()));
        m_eigenvectors.reset(new Eigen::EigenSolver<Eigen::MatrixXd>::EigenvectorsType(es.eigenvectors()));
        for(int i = 0; i < m_eigenvalues->size(); i++){
            double sum = 0;
            for(int j = 0; j < m_eigenvalues->size()-1; j++){
                sum += abs(m_eigenvectors->col(i)[j]);
            }
            for(int j = 0; j < m_eigenvalues->size(); j++){
                m_eigenvectors->col(i)[j] /= sum;
            }
        }
    }
}






