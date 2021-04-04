#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<iterator>
#include<tuple>
#include <cassert>

template <typename T>
class Matrix {
private:
    std::vector<std::vector<T>> body;

    class iterator {
    private:
        Matrix<T>* M;
        size_t i = 0, j = 0;
        typename std::vector<T>::iterator current;

    public:
        explicit iterator(Matrix<T>* m) : M(m) {
            current = ((*m).body)[0].begin();
        }

        iterator operator++(int) {
            iterator tmp(*this);
            ++(*this);
            return tmp;
        }

        iterator operator++() {
            auto [rows, columns] = (*M).size();
            if (j == columns - 1 &&
                i == rows - 1) {
                j++;
                current = (*M)[(*M).size().first - 1].end();
            } else if (j == columns - 1 &&
                i != rows - 1) {
                i++;
                j = 0;
                current = ((*M).body)[i].begin();
            } else if (j != columns) {
                j++;
                current++;
            }
            return *this;
        }

        T& operator*() {
            return *current;
        }

        bool operator==(const iterator& other) const {
            return std::make_tuple(i, j) == std::make_tuple(other.i, other.j);
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    friend class Matrix;
    };

    class iteratorC {
    private:
        const Matrix<T>* M;
        size_t i = 0, j = 0;
        typename std::vector<T>::const_iterator current;

    public:
        explicit iteratorC(const Matrix<T>* m) : M(m) {
            current = ((*m).body)[0].begin();
        }

        iteratorC operator++(int) {
            iteratorC tmp(*this);
            ++(*this);
            return tmp;
        }

        iteratorC operator++() {
            auto [rows, columns] = (*M).size();
            if (j == columns - 1 &&
                i == rows - 1) {
                j++;
                current = (*M)[(*M).size().first - 1].end();
            } else if (j == columns - 1 &&
                i != rows - 1) {
                i++;
                j = 0;
                current = ((*M).body)[i].begin();
            } else if (j != columns) {
                j++;
                current++;
            }
            return *this;
        }

        T operator*() {
            return *current;
        }

        bool operator==(const iteratorC& other) const {
            return std::make_tuple(i, j) == std::make_tuple(other.i, other.j);
        }

        bool operator!=(const iteratorC& other) const {
            return !(*this == other);
        }
        friend class Matrix;
    };

public:
    friend class iterator;
    friend class iteratorC;
    iterator begin() {
        return iterator(this);
    }

    iterator end() {
        auto a = (*this).begin();
        a.i = (*this).size().first-1;
        a.j = (*this).size().second;
        a.current = (*this)[a.i].end();
        return a;
    }

    iteratorC begin() const {
        return iteratorC(this);
    }

    iteratorC end() const {
        iteratorC a = (*this).begin();
        a.i = (*this).size().first - 1;
        a.j = (*this).size().second;
        a.current = (*this)[a.i].end();
        return a;
    }

    std::vector<T> operator[](size_t i) const {
        return body[i];
    }

    std::vector<T>& operator[](size_t i) {
        return body[i];
    }

    Matrix(const std::vector<std::vector<T>>& mat) {
        body = mat;
    }
    std::pair<size_t, size_t> size() const {
        return std::make_pair(body.size(), body[0].size());
    }

    Matrix<T>& transpose() {
        auto [rows, columns] = size();
        std::vector<std::vector<T>> K;
        K.resize(columns, std::vector<T>(rows));
        for (size_t i = 0; i < columns; ++i) {
            for (size_t j = 0; j < rows; ++j) {
                K[i][j] = body[j][i];
            }
        }
        body = K;
        return *this;
    }

    Matrix<T> operator*(const Matrix<T>& other) {
        std::vector<std::vector<T>> K;
        assert(size().second == other.size().first);
        K.resize(size().first, std::vector<T>(other.size().second));
        for (size_t i = 0; i < size().first; ++i) {
            for (size_t j = 0; j < other.size().second; ++j) {
                for (size_t k = 0; k < size().second; ++k) {
                    K[i][j] += body[i][k] * other.body[k][j];
                }
            }
        }
        return Matrix(K);
    }

    Matrix<T>& operator*=(const Matrix<T>& other) {
        assert(size().second == other.size().first);
        *this = *this * other;
        return *this;
    }

    Matrix<T> transposed() const {
        Matrix<T> b(*this);
        return b.transpose();
    }

    Matrix<T>& operator+=(const Matrix<T>& rhs) {
        auto [rows, columns] = size();
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < columns; ++j) {
                body[i][j] += rhs.body[i][j];
            }
        }
        return *this;
    }
    Matrix<T> operator+(const Matrix<T>& rhs)  const {
        Matrix<T> A(*this);
        A += rhs;
        return A;
    }

    template<typename S>
    Matrix<T>& operator*=(const S& s) {
        auto [rows, columns] = size();
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < columns; ++j) {
                body[i][j] *= s;
            }
        }
        return *this;
    }

    template <typename N>
    Matrix<T> operator*(const N& t) const {
        Matrix<T> b(*this);
        b *= t;
        return b;
    }
    template <typename Y>
    friend std::ostream& operator<<(std::ostream& out, const Matrix<Y>& m);
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& m) {
    for (size_t i = 0; i < m.body.size(); ++i) {
        for (size_t j = 0; j < m.body[0].size(); ++j) {
            if (j != 0) {
                out << "\t";
            }
            out << m.body[i][j];
        }
        if (i != m.body.size() - 1) {
            out << "\n";
        }
    }
    return out;
}
template <typename T, typename N>
Matrix<T> operator*(const N& t, const Matrix<T>& a) {
    Matrix<T> b(a);
    b *= t;
    return b;
}