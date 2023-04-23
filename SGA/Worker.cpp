#include "Worker.h"

using namespace std;

Worker::Worker(Polynomial& polynomial, Population& population, double start_radius)
        : polynomial(polynomial), population(population), start_radius(start_radius) {}

void Worker::printHeader() {

    cout << "===============================================================" << endl;
    cout << "Generation: " << generation << endl;
    cout << fixed << setw(14) << "Fitness |"
              << fixed << setw(14) << "Root 	\n";

    cout << "---------------------------------------------------------------" << endl;
}


void Worker::printReport(double *summary, string note) {

    cout << scientific << setw(11) << setprecision(3) << summary[Population::SUM_NDX_MIN] <<" |"
              << scientific << setw(11) << setprecision(3) << summary[Population::SUM_NDX_REAL] << " + " <<summary[Population::SUM_NDX_IMAG]<<"i \n "  ;
    cout << "---------------------------------------------------------------\n" << endl;
}

int Worker::mainProcedure() {

    auto startTime  = chrono::high_resolution_clock::now();
    auto epochStart = startTime;
    int code = NOTHING;
    double summary[Population::SUM_SIZE];
    complex<double> found;

      cout << "I" << endl;
      population.init(start_radius);
      cout << "J" << endl;
      population.getSummary(summary);

      generation = 1;

      while(true){

          epochStart = chrono::system_clock::now();

          if (code == STOP || code == COMPLETE_MORE) break;

          printHeader();

          int status = population.evolve();


          if (status == Population::FOUND) {
              code = COMPLETE_MORE;

              found = (complex<double>) population.getBestFit();
          }
          else if (status == Population::CONVERGED) {
              code = CONVERGED;
          }
          else if (status == Population::NOT_FOUND) {
              code = CONTINUE;
          }
          else {
              cout << "Unknown status code reported" << endl;
              code = CONTINUE;
          }

          population.getSummary(summary);

          printReport(summary);

          chrono::duration<double, milli> epochDuration = chrono::high_resolution_clock::now() - epochStart;

          cout << defaultfloat << setprecision(10)  << "Epoch during: " << 0.001 * epochDuration.count() << "s" << endl;

          generation++;
      }

      cout << "\n\n";
      cout << defaultfloat << "f(x) = " << polynomial << "\n\n";

      const complex<double> best = population.getBestFit().getChromosome();

      cout << "\n";
      cout << fixed
                << "f(x) = 0 when x ~= " << found.real() << " + " << found.imag() << "i"
                << " with the fitness of " << abs(polynomial(found)) << endl;

      chrono::duration<double, milli> duration = chrono::high_resolution_clock::now() - startTime;

      cout << fixed << setprecision(10) << "\ntime elapsed: " << 0.001 * duration.count() << "s" << endl;

      population.clear();



    return 0;
}
