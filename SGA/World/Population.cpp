
#include <algorithm>
#include <complex>
#include <exception>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <utility>
#include <thread>
#include <vector>
#include "Population.h"
using namespace std;



// CONSTRUCTORS
Population::Population(function<double(const Individual&)> function, unsigned long pop_size) {
    this->_population_size = pop_size;
    this->_population = vector<Individual>(pop_size);
    this->_fitness_function = move(function);
    this->_combined_population = vector<Individual*>(pop_size);
    this->_combined_population_size = _population_size;
}

Population::Population(function<double(const Individual&)> function,
                       Polynomial polynomial,
                       unsigned long pop_size,
                       double accepted_error,
                       double mut_rate,
                       double mut_radius,
                       unsigned long start_generation) {


    this->_fitness_function = move(function);
    this->_polynomial = move(polynomial);

    this->_population = vector<Individual>(pop_size);
    this->_population_size = pop_size;


    this->_combined_population = vector<Individual*>(pop_size);
    this->_combined_population_size = _population_size;

    this->_accepted_error = accepted_error;
    this->_mutation_rate = mut_rate;
    this->_mutation_radius = mut_radius;
    this->_generation = start_generation;

}

Population::Population(const Population& that) {
    this->_fitness_function = that._fitness_function;

    this->_population_size = that._population_size;
    this->_accepted_error = that._accepted_error;
    this->_generation = that._generation;
    this->_mutation_rate = that._mutation_rate;
    this->_mutation_radius = that._mutation_radius;


    this->clear();
    this->_population = vector<Individual>(that._population_size);


    for (int i = 0; i < this->_population_size; i++) {
        _population[i] = Individual(that._population[i]);
    }
    _population_size = _population.size();


    for (int i = 0; i < that._combined_population_size; i++){
        _combined_population[i] = that._combined_population[i];
    }
    _combined_population_size = _combined_population.size();

}




void Population::init(double start_radius){
    double offset = abs(start_radius);

    for (int i = 0; i < _population_size; i++) {
        _population[i] = Individual(rng::getRealUniformDist(0.0, 2 * offset) - offset,
                                    rng::getRealUniformDist(0.0, 2 * offset) - offset);
        _population[i].setFitness(_fitness_function(_population[i]));
        _combined_population[i] = &_population[i];
    }

    sort();
}

/*
// Uniform Probability Distribution to select parents
void Population::select(Individual* parents[2]) {
    Individual *ret[2] = {nullptr, nullptr}; //ret    := used to hold ret values until end to assign parents
    Individual *not_me = nullptr;              //not_me := used to avoid selecting same member twice
    unsigned long pop_cap = _combined_population_size;
    //total of all members' fitness
    double totalFitness = [this]() {
        double total = 0;
        for(const auto& p : _combined_population) total += p->getFitness();
        return total;
    }();
    auto probability = new double[pop_cap];    //probability[i] = fitness(i) / total_i
    auto accumulated = new double[pop_cap];    //accumulated[i] = accumulated[i - 1] + probability[i]
    //calculating probability
    for (int i = 0; i < pop_cap; i++) probability[i] = _combined_population[i]->getFitness() / totalFitness;
    //calculating accumulated
    accumulated[0] = probability[0];
    accumulated[pop_cap - 1] = 1;
    for (int i = 1; i < pop_cap - 1; i++) accumulated[i] = accumulated[i - 1] + probability[i];
    //for each parent (2)
    for (auto &ndx : ret) {
        //do while the parents are equal to each other
        do {
            //get random number [0, 1] from uniform distribution to pick from population
            double num = rng::getRealUniformDist(0.0, 1.0);
            //pick from population by setting i where num <= accumulated[i]
            unsigned long i = 0;
            while (i < pop_cap && num > accumulated[i++]) {}
            //if the index we picked was from
            ndx = _combined_population[--i];
        } while (ndx == not_me);
        //to make sure parents are not equal
        not_me = ret[0];
    }
    //copy parents
    parents[0] = ret[0];parents[1] = ret[1];
    //deleting
    delete[] probability; delete[] accumulated;

}


*/

//USING ELITISM TO SELECT PARENTS

void Population::select(Individual* parents[2]) {

    unsigned long pop_half = _combined_population_size/2;

    double num1 = rand() % pop_half;
    double num2 = rand() % pop_half;

        do {

            //get random number from [0, pop_half] to pick from 50% of the healthiest population
    		num1 = rand() % pop_half;
    		num2 = rand() % pop_half;



        } while (num1 == num2);


    //copy parents
    parents[0] = _combined_population[num1];
    parents[1] = _combined_population[num2];


}




/*

//Roulette Wheel to Select Parents
void Population::select(Individual* parents[2]) {
    Individual *ret[2] = {nullptr, nullptr}; //ret    := used to hold ret values until end to assign parents
    Individual *not_me = nullptr;              //not_me := used to avoid selecting same member twice
    unsigned long pop_cap = _combined_population_size;
	auto inverse = new double[pop_cap];    //probability[i] = fitness(i) / total_i
    auto probability = new double[pop_cap];    //accumulated[i] = accumulated[i - 1] + probability[i]
    auto accumulated = new double[pop_cap];
    //calculating probability
    double totalFitness = 0;
    for (int i = 0; i < pop_cap; i++)
	{
		inverse[i] = 1/_combined_population[i]->getFitness() ;
		totalFitness  = totalFitness + inverse[i];
	}
    for (int i = 0; i < pop_cap; i++) probability[i] = inverse[i] / totalFitness;
    accumulated[0] = probability[0];
    for (int i = 1; i < pop_cap; i++) accumulated[i] = accumulated[i-1] + probability[i];
    //for each parent (2)
    for (auto &ndx : ret) {
        //do while the parents are equal to each other
        do {
            //get random number [0, 1] from uniform distribution to pick from population
            double num = rng::getRealUniformDist(0.0, 1.0);
			//pick from population by setting i where num <= accumulated[i]
		    unsigned long i = 0;
		    while (i < pop_cap && num > accumulated[i++]) {  }
            //if the index we picked was from
            ndx = _combined_population[--i];
        } while (ndx == not_me);
        //to make sure parents are not equal
        not_me = ret[0];
    }
    //copy parents
    parents[0] = ret[0]; parents[1] = ret[1];
	//deleting
    delete[] probability; delete[] inverse; delete[] accumulated;
}

*/
/*
//Using Uniform Probability Distribution to generate offsprings
void Population::crossover(Individual offspring[2], Individual* parents[2]) {

    Individual ret[2];

    complex<double> p1 = parents[0]->getChromosome();
    complex<double> p2 = parents[1]->getChromosome();

    complex<double> diff = p1 - p2;
    double abs_diff = abs(diff);

    double real_lower = min(p1.real(), p2.real()) - abs_diff;
    double real_upper = max(p1.real(), p2.real()) + abs_diff;
    double imag_lower = min(p1.imag(), p2.imag()) - abs_diff;
    double imag_upper = max(p1.imag(), p2.imag()) + abs_diff;

    ret[0] = Individual(rng::getRealUniformDist(real_lower, real_upper),
                        rng::getRealUniformDist(imag_lower, imag_upper));
    ret[1] = Individual(rng::getRealUniformDist(real_lower, real_upper),
                        rng::getRealUniformDist(imag_lower, imag_upper));

    ret[0].setFitness(_fitness_function(ret[0]));
    ret[1].setFitness(_fitness_function(ret[1]));

    offspring[0] = ret[0];
    offspring[1] = ret[1];

}

*/

//Using Trisection Method to create offsprings
void Population::crossover(Individual offspring[2], Individual* parents[2]) {

    Individual ret[2];

    complex<double> p1 = parents[0]->getChromosome();
    complex<double> p2 = parents[1]->getChromosome();

    double real_third1 = (p1.real()+ 2*p2.real())/3;
    double imag_third1 = (p1.imag()+ 2*p2.imag())/3;
    double real_third2 = (2*p1.real()+ p2.real())/3;
    double imag_third2 = (2*p1.imag()+ p2.imag())/3;

    ret[0] = Individual(real_third1 , imag_third1);
    ret[1] = Individual(real_third2 , imag_third2);

    ret[0].setFitness(_fitness_function(ret[0]));
    ret[1].setFitness(_fitness_function(ret[1]));

    offspring[0] = ret[0];
    offspring[1] = ret[1];


}



void Population::mutate(Individual &x){

    if (rng::getRealUniformDist(0.0, 1.0) <= _mutation_rate) {
        double new_real;
        double new_imag;

        //pick numbers within range
        do {
            new_real = rng::getRealUniformDist(0.0, _mutation_radius);
            new_imag = rng::getRealUniformDist(0.0, _mutation_radius);

        } while (pow(new_real, 2) + pow(new_imag, 2) <= pow(_mutation_radius, 2));

        x.setReal(new_real + x.getReal());
        x.setImaginary(new_imag + x.getImaginary());
    }

}
void Population::replace(Population &replacement){



    _population.insert(end(_population), begin(replacement._population), end(replacement._population));

    //sort from least to greatest
    sort();

    //remove the worst
    for (long i = 0; i < replacement._population_size; i++)
        _population.pop_back();

}




bool Population::checkSolution() const {
    Individual best = _population[0];
	if (best.getFitness()<_accepted_error)
    return true;
return false;
}

bool Population::checkChromosomeConvergence() const {
    double recur_real = _population[0].getReal();
    double recur_imag = _population[0].getImaginary();

    for (int i = 1; i < _population_size; i++){
        if (recur_real != _population[i].getReal() &&
            recur_imag != _population[i].getImaginary())
            return false;
    }

    return true;
}
bool Population::checkFitnessConvergence() const {

    double recur_fitness = _population[0].getFitness();

    for (int i = 1; i < _population_size; i++){
        if (recur_fitness != _population[i].getFitness())
            return false;
    }

    return true;
}



int Population::handleConvergence() {

    for (int i = 0; i < _population_size; i++) {
        double new_real;
        double new_imag;

        //pick numbers within range
        do {
            new_real = rng::getRealUniformDist(0.0, 2 * _mutation_radius) - _mutation_radius;
            new_imag = rng::getRealUniformDist(0.0, 2 * _mutation_radius) - _mutation_radius;
        } while (pow(new_real, 2) + pow(new_imag, 2) <= pow(_mutation_radius, 2));


        _population[i].setReal(new_real + _population[i].getReal());
        _population[i].setImaginary(new_imag + _population[i].getImaginary());
        _population[i].setFitness(_fitness_function(_population[i]));
    }

}

/*
Population::status Population::evolve(){

    status status = NOT_FOUND;

    //sort population
    sort();

    //check for a root
    if (checkSolution()) return FOUND;

    //check & handle convergence
    if (checkChromosomeConvergence()) {
        status = CONVERGED;
        handleConvergence();
    }


    Population new_generation(_fitness_function, 0);

    for (unsigned long i = 0; i < _population_size; i++) {
        Individual* parents[2];
        select(parents);

        Individual offspring[2];
        crossover(offspring, parents);

        mutate(offspring[0]);
        mutate(offspring[1]);

        new_generation._population.push_back(offspring[0]);
        new_generation._population.push_back(offspring[1]);


    }

    new_generation._population_size = new_generation._population.size();


    fitPopulation(new_generation);


    replace(new_generation);

    _generation++;

    _combined_population.empty();
    for (int i = 0; i < _population_size; i++){
        _combined_population[i] = &_population[i];
    }

    _combined_population_size = _population_size;

    return status;
}

*/
//Evolving Population
Population::status Population::evolve(){
    status status = NOT_FOUND;
    sort();	//sort population
    if (checkSolution()) return FOUND; //check for a root
	if (checkChromosomeConvergence()) {
        status = CONVERGED;
        handleConvergence();
    }//check & handle convergence
	Population new_generation(_fitness_function, 0);
  	unsigned long elite = 0.1*_population_size;
	for(unsigned long i = 0; i < elite; i++)
	new_generation._population.push_back(_population[i]);
	for (unsigned long i = elite; i < (_population_size - elite)/2; i++) {
        Individual* parents[2];
        select(parents);
        Individual offspring[2];
        crossover(offspring, parents);
        mutate(offspring[0]);
        mutate(offspring[1]);
        new_generation._population.push_back(offspring[0]);
        new_generation._population.push_back(offspring[1]);
    }
    new_generation._population_size = new_generation._population.size();
    fitPopulation(new_generation);
    _generation++;
	_population = new_generation._population;
    _combined_population.empty();
    for (int i = 0; i < _population_size; i++){
        _combined_population[i] = &_population[i];
    }
    _combined_population_size = _population_size;
    return status;
}

void Population::getSummary(double summary[SUM_SIZE]) const {


    double pop_min = _population.front().getFitness();


    summary[SUM_NDX_MIN] = pop_min;
    summary[SUM_NDX_REAL] = _population[0].getChromosome().real();
    summary[SUM_NDX_IMAG] = _population[0].getChromosome().imag();
}

const Individual& Population::getBestFit(){
    return _population[0];
}


double Population::fitPopulation(Population& population) {
    double total = 0;
    for (Individual& individual : population._population) {
        individual.setFitness(population._fitness_function(individual));
        total += individual.getFitness();
    }
    return total / population._population_size;
}


void Population::sort(bool descending) {
    if (descending) {
        std::sort(_population.begin(), _population.end(), greater<Individual>());
        std::sort(_combined_population.begin(), _combined_population.end(),
                  [](Individual *i, Individual *j) { return i->getFitness() > j->getFitness(); });
    } else {
        std::sort(_population.begin(), _population.end(), less<Individual>());
        std::sort(_combined_population.begin(), _combined_population.end(),
                  [](Individual *i, Individual *j) { return i->getFitness() < j->getFitness(); });
    }
}


void Population::clear() {
    _population.clear();
}



void Population::reset() {
    _combined_population.clear();
    _combined_population_size = _population_size;
    for(int i = 0; i < _population_size; i++){
        _combined_population[i] = &_population[i];
    }
}
