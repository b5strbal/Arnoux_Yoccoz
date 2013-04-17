#ifndef CHOOSE_H
#define CHOOSE_H

#include <vector>

class Choose{
public:
    Choose(int n, int k);
    Choose& operator++();
    inline bool isAfterLast() const { return m_chosenIndices.empty(); }
   // operator const std::vector<int>&(){ return m_chosenIndices; }
    int operator[](int i) { return m_chosenIndices[i]; }

    const int* begin() const { return &m_chosenIndices[0]; }
    const int* end() const { return begin() + m_k; }
private:
    int m_n;
    int m_k;
    std::vector<int> m_chosenIndices;
};

#endif // CHOOSE_H
