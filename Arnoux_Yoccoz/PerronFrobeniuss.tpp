//--------//
// Matrix //
//--------//


template <typename Type>
Matrix<Type>::Matrix(const std::vector<std::vector<Type>>& entries){
    if (entries.empty()) {
        throw std::runtime_error("A matrix must have at least one row.");
    }
    if (entries[0].empty()) {
        throw std::runtime_error("The width of a matrix must be at least 1.");
    }
    for (int i = 1; i < entries.size(); i++) {
        if (entries[1].size() != entries[0].size()) {
            throw std::runtime_error("In a matrix the length of the rows must be equal.");
        }
    }
    m_entries = entries;
}


template <typename Type>
Matrix<Type>::Matrix(int height, int width) :
    Matrix<Type>(std::vector<std::vector<Type>>(height, std::vector<Type>(width)))
{
}




template <typename Type>
template <typename OtherType>
Matrix<Type>::Matrix(const Matrix<OtherType>& matrix) :
    m_entries(matrix.height(), std::vector<Type>(matrix.width()))
{
    for (int i = 0; i < height(); i++) {
        for (int j = 0; j < width(); j++) {
            m_entries[i][j] = matrix.getEntry(i, j);
        }
    }
}






template <typename Type>
Matrix<Type> operator*(const Matrix<Type>& m1, const Matrix<Type>& m2){
    if (m1.width() != m2.height()) {
        throw std::runtime_error("Dimensions of matrices don't match for multiplication.");
    }
    Matrix<Type> product(m1.height(), m2.width());
    for (int i = 0; i < m1.height(); i++) {
        for (int j = 0; j < m2.width(); j++) {
            Type sum = 0;
            for (int k = 0; k < m1.width(); k++) {
                sum += m1.getEntry(i, k) * m2.getEntry(k, j);
            }
            product.setEntry(i, j, sum);
        }
    }
    return product;
}




template <typename Type>
std::ostream& operator<<(std::ostream& Out, const Matrix<Type>& matrix){
    for (int i = 0; i < matrix.height(); i++) {
        for (int j = 0; j < matrix.width(); j++) {
            Out << matrix.m_entries[i][j] << " ";
        }
        Out << std::endl;
    }
    return Out;
}



template <typename Type>
Matrix<Type> Matrix<Type>::transpose() const{
    std::vector<std::vector<Type>> copy(height(), std::vector<Type>(width()));
    
    for (int i = 0; i < height(); i++) {
        for (int j = 0; j < width(); j++) {
            copy[i][j] = m_entries[j][i];
        }
    }
    return Matrix<Type>(copy);
}




template <typename Type>
Type Matrix<Type>::maxEntry() const{
    Type maxValue = m_entries[0][0];
    for (int i = 0; i < height(); i++) {
        for (int j = 0; j < width(); j++) {
            if (maxValue < m_entries[i][j]) {
                maxValue = m_entries[i][j];
            }
        }
    }
    return maxValue;
}




template <typename Type>
Vector<Type> Matrix<Type>::column(int columnIndex) const{
    if (columnIndex < 0 || columnIndex >= width()) {
        throw std::runtime_error("Invalid column index for Matrix object.");
    }
    Vector<Type> myVector(height());
    for (int i = 0; i < height(); i++) {
        myVector.setEntry(i, getEntry(i, columnIndex));
    }
    return myVector;
}




template <typename Type>
bool isCloseEnough(const Matrix<Type>& matrix1, const Matrix<Type>& matrix2){
    assert(matrix1.width() == matrix1.width());
    assert(matrix1.height() == matrix1.height());

    for (int i = 0; i < matrix1.height(); i++) {
        for (int j = 0; j < matrix2.width(); j++) {
            if (fabs(matrix1.getEntry(i, j) - matrix2.getEntry(i, j)) > ALLOWED_ERROR) {
                return false;
            }
        }
    }
    return true;
}



void normalize(Matrix<floating_point_type>& matrix){
    floating_point_type maxValue = matrix.maxEntry();
    assert(maxValue > 0);
    for (int i = 0; i < matrix.height(); i++) {
        for (int j = 0; j < matrix.width(); j++) {
            matrix.setEntry(i, j, matrix.getEntry(i, j)/maxValue);
        }
    }
}





//--------//
// Vector //
//--------//




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



