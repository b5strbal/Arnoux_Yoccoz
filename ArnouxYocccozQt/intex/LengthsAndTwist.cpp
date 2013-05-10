#include "LengthsAndTwist.h"
#include <numeric>

balazs::LengthsAndTwist::LengthsAndTwist(const std::vector<floating_point_type> &lengths, floating_point_type twist) :
    m_lengths(lengths),
    m_twist(twist)
{
    if (lengths.size() == 0){
        throw std::runtime_error("The number of intervals must be at least 1.");
    }
    for (std::size_t i = 0; i < lengths.size(); i++) {
        if (lengths[i] <= 0)
            throw std::runtime_error("The length parameters of an interval exchange map must be positive.");
    }

    floating_point_type total = std::accumulate(lengths.begin(), lengths.end(), 0);
    for (auto x : m_lengths) {
        x /= total;
    }
}
