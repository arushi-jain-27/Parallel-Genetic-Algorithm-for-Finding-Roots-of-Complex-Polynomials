#include <cstring>
#include "Polynomial.h"

using namespace std;

/**
 * Polynomial file format:
 * - first line is an integer (d) where d is the degree of the polynomial
 * - following lines are d lines of complex numbers (a b)
 *    where a and b are real numbers; a is the real part and b is the complex part
 * @param filename
 * @return
 */
Polynomial::Polynomial(char* filename){
    this->filename.assign("");
    this->filename.assign(filename);
    ifstream infile(filename);

    this->degree = -2;
    infile >> this->degree;

    this->coefficients_len = this->degree + 1;
    this->coefficients = new complex<double>[this->coefficients_len];

    double real, imag;
    int i = 0;
    while (infile >> real >> imag){
        this->coefficients[i].real(real);
        this->coefficients[i].imag(imag);
        i++;
    }
}

Polynomial::~Polynomial() {
    delete[] coefficients;
}

Polynomial::Polynomial(const Polynomial &other) {
    this->filename = other.filename;
    this->degree = other.degree;
    delete[] this->coefficients;
    this->coefficients_len = other.coefficients_len;
    this->coefficients = new complex<double>[this->coefficients_len];
    for (int i = 0; i < this->coefficients_len; i++){
        this->coefficients[i] = other.coefficients[i];
    }
}


Polynomial& Polynomial::operator=(Polynomial &&other) noexcept {
    this->filename = other.filename;
    this->degree = other.degree;
    delete[] this->coefficients;
    this->coefficients_len = other.coefficients_len;
    this->coefficients = new complex<double>[this->coefficients_len];
    for (int i = 0; i < this->coefficients_len; i++){
        this->coefficients[i] = other.coefficients[i];
    }
}









ostream& operator<<(ostream &out, const Polynomial &p) {
    if (p.filename.empty()) throw Polynomial::FileNotFoundException("Polynomial was not populated from a file");
    for(int i = 0; i < p.coefficients_len; i++){
        bool isRealZero = p.coefficients[i].real() == 0;
        bool isImagZero = p.coefficients[i].imag() == 0;
        if (!isRealZero && !isImagZero) out << "(" << p.coefficients[i].real() << " + " << p.coefficients[i].imag() << "i)";
        else if (!isRealZero) out << p.coefficients[i].real();
        else if (!isImagZero) out << p.coefficients[i].imag() <<"i";
        else continue;
        if (i + 1 != p.coefficients_len) out << "*x^" << p.degree - i << " + ";
    }
    return out;
}





complex<double> Polynomial::operator()(const complex<double>& input) const {
    if (filename.empty()) return {0,0}; //throw an exception instead
    auto z = complex<double>(0,0);
    for (int i = degree, j = 0; j < coefficients_len; i--, j++)
        z = z + coefficients[j] * pow(input, i);

    return z;
}



