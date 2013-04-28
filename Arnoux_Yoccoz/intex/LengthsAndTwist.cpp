#include "LengthsAndTwist.h"
#include <numeric>

balazs::LengthsAndTwist::LengthsAndTwist(const std::vector<floating_point_type> &lengths, floating_point_type twist) :
    m_lengths(lengths),
    m_twist(twist)
{
    if (lengths.size() == 0){
        throw std::runtime_error("The number of intervals must be at least 1.");
    }
    for (unsigned int i = 0; i < lengths.size(); i++) {
        if (lengths[i] <= 0)
            throw std::runtime_error("The length parameters an interval exchange map must be positive.");
    }

    floating_point_type total = std::accumulate(lengths.begin(), lengths.end());
    for (auto x : m_lengths) {
        x /= total;
    }
}
