#ifndef CONTAININGINTERVAL_H
#define CONTAININGINTERVAL_H

#include <vector>

template <typename type1, typename type2>
std::size_t containingInterval(const std::vector<type1>& orderedList, const type2& point){
    int interval = std::upper_bound(orderedList.begin(), orderedList.end(), point) - orderedList.begin() - 1;
    return interval == -1 ? orderedList.size() - 1 : interval;
}


#endif // CONTAININGINTERVAL_H
