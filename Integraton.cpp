#include <iostream>
#include <memory>
#include <string>
#include <cmath>

template <typename T>
class IntegrationMethod {
public:
    virtual double Integrate(double (*f)(double), double x, double y, int n) const  = 0;
    virtual ~IntegrationMethod() {}
};
template <typename T>
class RectangleRule : public IntegrationMethod<T> {
    double Integrate(double (*f)(double), double x, double y, int n) const override {
        auto segment = (y-x)/n;
        double sum = 0;
        for (int i = 0; i < n; i++) {
            sum += f(((x+i*(segment)) + (x + (i+1) * (segment)))/2)*segment;
        }
        return sum;
    }
};
template <typename T>
class TrapezoidalRule : public IntegrationMethod<T> {
    double Integrate(double (*f)(double), double x, double y, int n) const override {
        auto segment = (y - x) / n;
        double sum = 0;
        for (int i = 0; i < n; i++) {
            sum += (f(x + i * (segment)) + f(x + (i + 1) * (segment))) / 2 * segment;
        }
        return sum;
    }
};