#ifndef POLYNOMIAL_ROOT_FINDING_PGA_POPULATION_H
#define POLYNOMIAL_ROOT_FINDING_PGA_POPULATION_H

#include <utility>
#include <vector>
#include <functional>
#include <random>
#include <cstring>

#include "Individual.h"
#include "Polynomial.h"

#include "../Local.h"

using namespace std;

class Population {
public:

    //local population
    vector<Individual> _population;
    unsigned long _population_size{};


    //combined population (for migration)
    vector<Individual*> _combined_population;
    unsigned long _combined_population_size{};

    //number of generation that this population evolve
    unsigned long _generation{};

    //mutation: rate and gradRadius
    double _mutation_rate{};
    double _mutation_radius{};

    //accepted error
    double _accepted_error{};

    //fitness function
    function<double(const Individual&)> _fitness_function;
    Polynomial _polynomial{};


    


    explicit Population(function<double(const Individual&)> function, unsigned long pop_size);

protected:

    //Genetic operators
    void select(Individual* parents[2]);
    Individual* select();
    void crossover(Individual offspring[2], Individual* parents[2]);
    complex<double> gradient(Individual *parent);
    void mutate(Individual& x);
    void replace(Population& replacement);

    int handleConvergence();


    //modify population
    void add(Individual& x);
    void addAll(const vector<Individual>& vector);

    void remove(Individual& x);

    Individual& get(unsigned long i);

public:

    

    //status codes on evolve()
    enum status {
        IGNORE    = -1,
        NOT_FOUND = 0,
        FOUND     = 1,
        CONVERGED = 2};


    //getSummary() indices and size
   
    static const int SUM_NDX_REAL     = 0;
    static const int SUM_NDX_IMAG     = 1;
    static const int SUM_NDX_MIN      = 2;
    static const int SUM_SIZE         = 3;

    //default values for hyper parameters
    static const unsigned long DEF_POPULATION_SIZE  = 2500;
    static constexpr double    DEF_ACCEPTED_ERROR   = 0.01;
    static constexpr double    DEF_MUTATION_RATE    = 0.33;
    static constexpr double    DEF_MUTATION_RADIUS  = 100;
  
    static constexpr double    DEF_START_RADIUS     = 1000000;
    static const unsigned long DEF_START_GENERATION = 0;
 


    //Constructors
    Population() = default;
    explicit Population(function<double(const Individual&)> function,
                        Polynomial    polynomial,
                        unsigned long pop_size         = DEF_POPULATION_SIZE,
                        double        accepted_error   = DEF_ACCEPTED_ERROR,
                        double        mut_rate         = DEF_MUTATION_RATE,
                        double        mut_radius       = DEF_MUTATION_RADIUS,
                        unsigned long start_generation = DEF_START_GENERATION);

    Population(const Population& that);

    Population& operator=(const Population&& that);
    ~Population() = default;

    void init(double start_radius = DEF_START_RADIUS);

    //Accessors
    inline unsigned long getPopulationSize() const { return _population_size; }
    inline unsigned long getGeneration()     const { return _generation;      }
    inline double        getMutationRate()   const { return _mutation_rate;   }
    inline double        getMutationRadius() const { return _mutation_radius; }
    inline double        getAcceptedError()  const { return _accepted_error;  }

    //Mutators
    inline void setPopulationSize(unsigned long new_pop_size)       { _population_size = new_pop_size;    }
    inline void setGeneration(unsigned long new_gen)                { _generation = new_gen;              }
    inline void setMutationRate(double new_mut_rate)                { _mutation_rate = new_mut_rate;      }
    inline void setMutationRadius(double new_mut_radius)            { _mutation_radius = new_mut_radius;  }
    inline void setAcceptedError(double new_accepted_val)           { _accepted_error = new_accepted_val; }
    inline void setFitnessFunction(const function<double(const Individual&)>& new_fit_funct)
                                                                    { _fitness_function = new_fit_funct;  }





    status evolve();



    //population status functions
    bool checkSolution() const;
    bool checkChromosomeConvergence() const;
    bool checkFitnessConvergence() const;
 

    static double fitPopulation(Population& population);
  


    //summary of population's history
    string printSummary() const;
    void getSummary(double summary[SUM_SIZE]) const;


    const Individual& getBestFit();


    //Operators
    Individual& operator[] (int ndx);
    friend ostream& operator<<(ostream &out, const Population &c);


    //Other function
    void sort(bool descending = false);
    void clear();
    void reset();



};

#endif //POLYNOMIAL_ROOT_FINDING_PGA_POPULATION_H
