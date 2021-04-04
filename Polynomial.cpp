#include <iostream>
#include <cmath>
#include <vector>
template <typename T>
class Polynomial {
private:
    std::vector<T> data;
    size_t size = 0;

public:
    template <typename Y>
    friend std::ostream& operator<<(std::ostream& out, const Polynomial<Y>& m);
    Polynomial(const T& value = T()) {
        data = { value };
        size = 1;
        (*this).Simplify();
    }

    Polynomial(const std::vector<T>& _data) {
        data = _data;
        size = data.size();
        (*this).Simplify();
    }

    template <typename It>
    Polynomial(const It& begin, const It& last) {
        data = {};
        for (auto i = begin; i != last; i++) {
            data.push_back(*i);
            size++;
        }
        (*this).Simplify();
    }

    T operator[](size_t i) const {
        if (i + 1 > size) {
            return T(0);
        }
        return data[i];
    }

    long int Degree() const {
        return size - 1;
    }

    Polynomial<T>& Simplify() {
        auto i = size;
        while (i > 0 && data[i - 1] == T(0)) {
            data.pop_back();
            size--;
            i--;
        }
        return *this;
    }

    Polynomial<T> Simplify() const {
        Polynomial<T> copy(*this);
        auto i = size;
        while (i > 0 && copy.data[i - 1] == T(0)) {
            copy.data.pop_back();
            copy.size--;
            i--;
        }
        return *this;
    }

    T operator()(const T x) const {
        T output = T();
        T power = T(1);
        for (size_t i = 0; i < size; i++) {
            output += (power) * (*this)[i];
            power *= x;
        }
        return output;
    }

    typename std::vector<T>::iterator begin() {
        return data.begin();
    }

    typename std::vector<T>::iterator end() {
        return data.end();
    }

    typename std::vector<T>::const_iterator begin() const {
        return data.begin();
    }

    typename std::vector<T>::const_iterator end() const {
        return data.end();
    }

    friend Polynomial<T>& operator+=(Polynomial<T>& first, const Polynomial<T>& second) {
        size_t n;
        if (first.size < second.size) {
            first.data.resize(second.size);
            first.size = second.size;
        }
        n = second.size;
        while (n > 0) {
            first.data[n - 1] += second.data[n - 1];
            n--;
        }
        return first.Simplify();
    }

    friend Polynomial<T> operator*(const Polynomial<T>& a, const Polynomial<T>& b) {
        std::vector<T> c;
        c.resize(a.size + b.size);
        for (size_t i = 0; i < a.size; ++i) {
            for (size_t j = 0; j < b.size; ++j) {
                c[i + j] += a[i] * b[j];
            }
        }
        return Polynomial<T>(c).Simplify();
    }

    friend Polynomial<T>& operator*=(Polynomial<T>& a, const Polynomial<T>& b) {
        a = (a * b);
        return a;
    }

    friend Polynomial<T>& operator-=(Polynomial<T>& first, const Polynomial<T>& second) {
        size_t n;
        if (first.size < second.size) {
            first.data.resize(second.size);
            first.size = second.size;
        }
        n = second.size;
        while (n > 0) {
            first.data[n - 1] -= second.data[n - 1];
            n--;
        }
        return first.Simplify();
    }

    friend Polynomial<T> operator+(const Polynomial<T>& first, const Polynomial<T>& second) {
        Polynomial<T> output = first;
        output += second;
        return output;
    }

    friend Polynomial<T> operator-(const Polynomial<T>& first, const Polynomial<T>& second) {
        Polynomial<T> output = first;
        output -= second;
        return output;
    }

    friend bool operator==(const Polynomial<T>& first, const Polynomial<T>& second) {
        if (first.size == second.size) {
            size_t n = first.size;
            while (n > 0) {
                if (first.data[n - 1] != second.data[n - 1]) {
                    return false;
                }
                n--;
            }
            return true;
        }
        return false;
    }

    friend bool operator!=(const Polynomial<T>& first, const Polynomial<T>& second) {
        return !(first == second);
    }

    friend Polynomial<T> operator&(const Polynomial<T>& first, const Polynomial<T>& second) {
        Polynomial<T> copy = Polynomial(T(1));
        Polynomial<T> output = Polynomial(T(0));
        for (size_t i = 0; i < first.size; i++) {
            output += copy * first[i];
            copy *= second;
        }
        return output.Simplify();
    }
};

template <typename T>
std::ostream& operator << (std::ostream& out, const Polynomial<T>& m) {
    size_t power = m.size;
    for (auto i = m.size; i > 0; i--) {
        if (m[i] != T(0)) {
            if (m[i] != T(1)) {
                if (m[i] > T(0)) {
                    if (power != m.size - 1) {
                        out << "+" << m[i] << "*";
                    } else {
                        out << m[i] << "*";
                    }
                } else if (m[i] == T(-1)) {
                    out << "-";
                } else {
                    out << m[i] << "*";
                }
            } else if (power != m.size - 1) {
                out << "+";
            }
            if (power != 1 && power != 0 && i < m.size && m[i] != T(0)) {
                out << "x^" << power;
            } else if (power == 1 && i < m.size && m[i] != T(0)) {
                out << "x";
            }
        }
        power--;
    }
    if (m.size > 1) {
        if (m.data[0] < T(0) && m.size > 0) {
            out << m.data[0];
        }
        if (m.data[0] > T(0) && m.size > 1) {
            out << "+";
            out << m.data[0];
        }
    }
    if (m.size == 1) {
        out << m.data[0];
    }
    if (m.size == 0) {
        out << T(0);
    }
    return out;
}