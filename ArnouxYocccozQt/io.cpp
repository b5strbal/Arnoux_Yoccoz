#include "io.h"

/*
#include <iostream>

std::ostream &balazs::operator <<(std::ostream &Out, const IntervalPairing &intervalPairing)
{
    Out << "Lengths: " << intervalPairing.m_intExchange.lengths() << "\n";
    Out << "Permutation: " << intervalPairing.permutation();
    return Out;
}

std::ostream& balazs::operator<<(std::ostream& out, const Mod1Number& p){
    out << p.m_position << "a";
    if(p.m_epsilon > 0)
        out << "(+" << p.m_epsilon << "eps)";
    else if(p.m_epsilon < 0)
        out << "(" << p.m_epsilon << "eps)";
    return out;
}




std::ostream& balazs::operator<<(std::ostream& out, const interval_t& interval){
    out << "[" << interval.leftEndpoint << "," << interval.rightEndpoint << "]";
    return out;
}


std::ostream& balazs::operator<<(std::ostream& Out, const TwistedIntervalExchangeMap& twistedIntervalExchange){
    Out << "Lengths: " << twistedIntervalExchange.m_lengths << "\n";
    Out << "Permutation: " << twistedIntervalExchange.m_permutation << "\n";
    Out << "Twist:" << twistedIntervalExchange.m_twist << "\n";
    Out << "Translations: " << twistedIntervalExchange.m_translations;

    return Out;
}


std::ostream& balazs::operator<<(std::ostream& out, const Foliation& f){
    out << f.m_twistedIntervalExchange;
    return out;
}




std::ostream& balazs::operator<<(std::ostream& out, const FoliationDisk& d)
{
    out << d.m_intervalPairing << "\n";
    out << "Singularity type: " << d.m_weighedTree.degrees();
    return out;
}




std::ostream& balazs::operator <<(std::ostream &out, const FoliationRP2 &f)
{
    out << f.foliationDisk();
    return out;
}



std::ostream& balazs::operator<<(std::ostream& Out, const FoliationSphere& f){
    Out << "(Top Disk) " << f.topFoliation() << std::endl;
    Out << "(Bottom Disk) " << f.bottomFoliation() << std::endl;
    Out << "Twist: " << f.twist();

    return Out;
}


std::ostream& balazs::operator<<(std::ostream& out, const DisjointIntervals& d){
    for (std::size_t i = d.m_wrapsAroundEnds ? 1 : 0; i < d.m_endpoints.size() - 2; i += 2) {
        out << interval_t({d.m_endpoints[i], d.m_endpoints[i + 1]}) << " ";
    }
    if (d.m_wrapsAroundEnds) {
        out << interval_t({d.m_endpoints.back(), d.m_endpoints.front()});
    } else
        out << interval_t({d.m_endpoints[d.m_endpoints.size() - 2], d.m_endpoints.back()});
    return out;
}


std::ostream & balazs::operator<<(std::ostream &out, const IntervalNeighborhoods& inh)
{
    for (std::size_t i = 0; i < inh.m_cuttingPoints.size(); i++){
        if (inh.m_cuttingPoints[i].isEmpty) {
            out << "() ";
        } else
            out << "(" << inh.m_cuttingPoints[i].first.number() << ","
                << inh.m_cuttingPoints[i].second.number() << ") ";
    }
    return out;
}




std::ostream& balazs::operator<<(std::ostream& out, const SeparatrixSegment& s)
{
    out << "SEPARATRIX SEGMENT\n";
    out << "(" << s.m_startingSingularity << ", ";
    out << s.m_depth << ", ";
    out << (s.m_direction == VDirection::Down ? "down" : "up") << ", ";
    out << s.m_endpoint.number() << ")\n";

    return out;
}



void balazs::printGoodSepSegments(std::size_t maxdepth, bool verbose){
    if (maxdepth > 0) {
        generateSepSegments(maxdepth);
    } else
        maxdepth = INT_MAX;
    for (VDirection vDirection : {VDirection::Up, VDirection::Down}) {
        for (std::size_t i = 0; i < m_foliation.numIntervals(); i++) {
            std::cout << i << (vDirection == VDirection::Up ? " UP" : " DOWN") << "\n";
            for(auto it = m_goodShiftedSeparatrixSegments[HDirection::Right][vDirection][i].begin(); it !=
                m_goodShiftedSeparatrixSegments[HDirection::Right][vDirection][i].end() && it->depth() < maxdepth; it++)
            {
                if (verbose) {                  // only prints segments shifted to right
                    std::cout << *it << "\n\n";
                } else
                    std::cout << it->depth() << " ";
            }
            if (!verbose) {
                std::cout << "\n\n";
            }
        }
    }
}


std::ostream& balazs::operator<<(std::ostream& out, const TransverseCurve& tc)
{
    out << "Separatrix segments:\n";
    for (auto goodSegmentIndex : tc.m_goodSegmentIndices){
        out << *goodSegmentIndex << "\n";
    }
    out << "Connecting arcs: " << tc.m_disjointIntervals << "\n";
    out << "Length: " << tc.length();
    return out;
}


std::ostream& balazs::operator <<(std::ostream &out, const Permutation &perm)
{
    for (std::size_t i = 0; i < perm.size(); i++)
        out << perm[i] << " ";
    return out;
}
*/
