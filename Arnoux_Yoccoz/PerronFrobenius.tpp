//
//  PF.cpp
//  Arnoux_Yoccoz
//
//  Created by Balazs Strenner on 3/21/13.
//  Copyright (c) 2013 Balazs Strenner. All rights reserved.
//









template <typename Type>
const Matrix<Type>& Vector<Type>::assertVector(const Matrix<Type>& matrix){
    if (matrix.width() > 1) {
        throw std::runtime_error("Can't make a vector from a matrix with width larger than 1.");
    }
    return matrix;
}



template <typename Type>
std::vector<Type> Vector<Type>::toStandardVector() const
{
    std::vector<Type> vector(this->height());
    for (int i = 0; i < this->height(); i++) {
        vector[i] = getEntry(i);
    }
    return vector;
}






//--------------//
// SquareMatrix //
//--------------//




template <typename Type>
const Matrix<Type>& SquareMatrix<Type>::assertSquareMatrix(const Matrix<Type>& matrix){
    if (matrix.height() != matrix.width()) {
        throw std::runtime_error("A SquareMatrix object can only be initialized by a square shaped matrix.");
    }
    return matrix;
}



template <typename Type>
bool SquareMatrix<Type>::isEveryVertexReachableFrom(int vertex) const{
    // Doing a breadth-first search from vectex
    
    assert(vertex >= 0 && vertex < size());
    int CountReachableVertices = 1;
    
    enum vertex_status{
        ALREADY_SEARCHED_NEIGHBORS,
        NEIGHBORS_TO_BE_SEARCHED,
        NOT_YET_REACHED
    };
    
    std::vector<vertex_status> Status(size(), NOT_YET_REACHED);
    Status[vertex] = NEIGHBORS_TO_BE_SEARCHED;
    std::deque<int> VerticesToSearch(1, vertex);
    
    while (VerticesToSearch.size() > 0) {
        int CurrentVertex = VerticesToSearch.front();
        for (int i = 0; i < size(); i++) {
            if (this->getEntry(CurrentVertex, i) > 0 && Status[i] == NOT_YET_REACHED) {
                Status[i] = NEIGHBORS_TO_BE_SEARCHED;
                CountReachableVertices++;
                VerticesToSearch.push_back(i);
            }
        }
        Status[CurrentVertex] = ALREADY_SEARCHED_NEIGHBORS;
        VerticesToSearch.pop_front();
    }
    
    if (CountReachableVertices == size()) {
        return true;
    }
    return false;
}


template <typename Type>
bool SquareMatrix<Type>::isReachableFromEveryVertex(int vertex) const{
    return this->transpose().isEveryVertexReachableFrom(vertex);
}



