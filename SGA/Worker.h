#ifndef POLYNOMIAL_ROOT_FINDING_PGA_SERVER_H
#define POLYNOMIAL_ROOT_FINDING_PGA_SERVER_H

#include <iomanip>
#include <chrono>
#include <vector>
#include "World/Polynomial.h"
#include "World/Population.h"
#include "Local.h"

class Worker {
private:
    Polynomial& polynomial;
    Population& population;
    double start_radius;
    long long generation;

public:
  enum status {
      NOTHING = 0,
      CONTINUE = 1,
      COMPLETE = 2,
      NOTHING_MORE = 3,
      CONTINUE_MORE = 4,
      COMPLETE_MORE = 5,
      SUMMARY = 6,

      CONVERGED = 7,
      STOP = 8,


  };

    Worker() = default;
    Worker(Polynomial& polynomial,
           Population& population,
           double start_radius);

    void printHeader();
    void printReport(double summary[Population::SUM_SIZE], std::string note = "");

    int mainProcedure();
};


#endif //POLYNOMIAL_ROOT_FINDING_PGA_SERVER_H
