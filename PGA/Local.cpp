#include <complex>
#include <cstdlib>
#include <cmath>
#include <valarray>
#include <cstring>
#include "Local.h"
using namespace std;

random_device rng::device;
mt19937_64 rng::generator(rng::device());

bool rng::isSeeded = false;

void rng::seedGenerator(unsigned long seed) {
    generator.seed(seed);
    isSeeded = true;
}

double rng::getRealUniformDist(double lower, double upper) {
    uniform_real_distribution<double> distribution(lower, upper);
    return distribution(generator);
}



double rng::getRealNormalDist(double lower, double upper) {
    normal_distribution<double> distribution(lower, upper);
    return distribution(generator);
}




function<double(const Individual&)> mFitnessFunctions::makeAbsoluteValue(const Polynomial& polynomial) {
    return [&polynomial](const Individual& individual)->double {
        return abs(polynomial(individual.getChromosome()));
    };
}


double mFitnessFunctions::applyAbsoluteValue(const Individual& individual) {
    return abs(individual.getChromosome());
}



void mInformationRetrieval::getInformation(int argc, char **argv, unsigned long &pop_s, double &acpt_e,
                                           double &mut_rate, double &mut_rad, double &start_rad) {
    for (int j = 0; j < argc; j++) {
        if (strcmp("-p", argv[j]) == 0 || strcmp("--population-size", argv[j]) == 0) {
            pop_s     = strtoul(argv[++j], nullptr, 0);
        }
        if (strcmp("-ae", argv[j]) == 0 || strcmp("--accepted-error", argv[j]) == 0) {
            acpt_e    = strtod(argv[++j], nullptr);
        }
        if (strcmp("-mr", argv[j]) == 0 || strcmp("--mutation-rate", argv[j]) == 0) {
            mut_rate  = strtod(argv[++j], nullptr);
        }
        if (strcmp("-ms", argv[j]) == 0 || strcmp("--mutation-radius", argv[j]) == 0) {
            mut_rad   = strtod(argv[++j], nullptr);
        }
        if (strcmp("-ss", argv[j]) == 0 || strcmp("--starting-radius", argv[j]) == 0) {
            start_rad = strtod(argv[++j], nullptr);
        }

    }
}







//NetworkSizeException::NetworkSizeException(const string &__arg) : runtime_error(__arg) {}




