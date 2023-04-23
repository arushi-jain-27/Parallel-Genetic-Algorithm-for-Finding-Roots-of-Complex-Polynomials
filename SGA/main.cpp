
#include <iostream>
#include <cmath>
#include <random>

#include "World.h"
#include "Local.h"

#include "Worker.h"


using namespace std;

void printPopulationInfo(const Polynomial& polynomial,
                         const unsigned long populationSize,
                         const double acceptedError,
                         const double mutationRate,
                         const double mutationRadius,
                         const double startingRadius){

    cout << endl;
    cout << "------------------------------------------------" << endl;
    cout <<   "polynomial      = " << polynomial
              << "\npopulation size = " << populationSize
              << "\naccepted error  = " << acceptedError
              << "\nmutation rate   = " << mutationRate
              << "\nmutation radius = " << mutationRadius
              << "\nstarting radius = " << startingRadius << endl;
    cout << "------------------------------------------------\n\n";

}

int main(int argc, char* argv[]) {

    Polynomial polynomial = Polynomial(argv[1]);
    unsigned long populationSize = Population::DEF_POPULATION_SIZE;
    double acceptedError         = Population::DEF_ACCEPTED_ERROR;
    double mutationRate          = Population::DEF_MUTATION_RATE;
    double mutationRadius        = Population::DEF_MUTATION_RADIUS;
    double startingRadius        = Population::DEF_START_RADIUS;

    try {
        mInformationRetrieval::getInformation(argc, argv,
                                              populationSize,
                                              acceptedError,
                                              mutationRate, mutationRadius,
                                              startingRadius);
    }
    catch(exception &e){
        cout << "Parsing error" << endl;
        exit(11);
    }


        printPopulationInfo(polynomial, populationSize, acceptedError,mutationRate, mutationRadius, startingRadius);

        auto fitnessFunction = mFitnessFunctions::makeAbsoluteValue(polynomial);
/*
        Population population = Population(fitnessFunction,
                                           populationSize,
                                           acceptedError,
                                           mutationRate,
                                           mutationRadius); */

        //for(int i = 0;i < polynomial.getDegree();i++) {

        Population population = Population(fitnessFunction,populationSize);
        population.setAcceptedError(acceptedError);
        population.setMutationRate(mutationRate);
        population.setMutationRadius(mutationRadius);
        population.setGeneration(0);

        Worker worker = Worker(polynomial, population, startingRadius);
        cout << "A" <<endl;

        worker.mainProcedure();
        //}

    return 0;
}
