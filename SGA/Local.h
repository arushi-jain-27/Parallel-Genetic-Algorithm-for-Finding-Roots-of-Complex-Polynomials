#ifndef POLYNOMIAL_ROOT_FINDING_PGA_LOCAL_H
#define POLYNOMIAL_ROOT_FINDING_PGA_LOCAL_H


#include <iostream>
#include <cstdlib>
#include <random>
#include <limits>
#include <ctime>
#include <functional>
#include <complex>
#include "World/Individual.h"
#include "World/Polynomial.h"






class rng {
private:
    rng() = default;

public:
    static std::random_device device;
    static std::mt19937_64 generator;
    static double getRealUniformDist(double lower, double upper);
    static double getRealNormalDist(double lower, double upper);

};



namespace mFitnessFunctions {
    extern double applyAbsoluteValue(const Individual& individual);
    extern std::function<double(const Individual&)> makeAbsoluteValue(const Polynomial& polynomial);
}


namespace mInformationRetrieval{
    extern void getInformation(int argc,
                               char** argv,
                               unsigned long &pop_s,
                               double &acpt_e,
                               double &mut_rate,
                               double &mut_rad,
                               double &start_rad);
}



#endif //POLYNOMIAL_ROOT_FINDING_PGA_LOCAL_H
