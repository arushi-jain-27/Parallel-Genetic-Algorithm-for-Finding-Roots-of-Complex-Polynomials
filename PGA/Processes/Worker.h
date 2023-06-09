#ifndef POLYNOMIAL_ROOT_FINDING_PGA_SERVER_H
#define POLYNOMIAL_ROOT_FINDING_PGA_SERVER_H


#include "Process.h"
#include "../World/Polynomial.h"
#include "../World/Population.h"

class Worker : public Process {
private:
    Polynomial& polynomial;
    Population& population;
    unsigned long seed;
    double start_radius;

public:
    Worker() = default;
    Worker(int pid, std::string pname,
           Polynomial& polynomial,
           Population& population,
           unsigned long seed,
           double start_radius = Population::DEF_START_RADIUS);
    ~Worker() override = default;

    int mainProcedure() override;
};


#endif //POLYNOMIAL_ROOT_FINDING_PGA_SERVER_H
