
#ifndef POLYNOMIAL_ROOT_FINDING_PGA_POLYNOMIAL_H
#define POLYNOMIAL_ROOT_FINDING_PGA_POLYNOMIAL_H

#include "Individual.h"

#include <complex>
#include <string>
#include <fstream>
using namespace std;

class Polynomial{
private:
    string filename;
    int degree;
    complex<double>* coefficients;
    int coefficients_len;

public:
    class FileNotFoundException: public std::runtime_error{
    public:
        explicit FileNotFoundException(const std::string &msg = "File was not found")
                : runtime_error(msg) {}
    };


    Polynomial() = default;
    explicit Polynomial(char* filename);
    ~Polynomial();

    Polynomial(const Polynomial& other);
    Polynomial(const Polynomial&& other) noexcept;
    Polynomial& operator=(const Polynomial& other);
    Polynomial& operator=(Polynomial&& other) noexcept;

    inline int getDegree() { return degree; }
    inline string getFilename() { return filename; }
    inline const complex<double>* getCoefficients() { return coefficients; };


    string to_string() const; //override operation<<
    friend ostream& operator<<(ostream &out, const Polynomial& p);
    complex<double> operator()(const complex<double>& input) const;

};


#endif //POLYNOMIAL_ROOT_FINDING_PGA_POLYNOMIAL_H
