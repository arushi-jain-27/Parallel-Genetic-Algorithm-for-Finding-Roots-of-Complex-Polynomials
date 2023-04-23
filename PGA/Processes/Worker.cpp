#include "Worker.h"
#include "../Local.h"

using namespace std;
Worker::Worker(int pid, string pname, Polynomial& polynomial, Population& population, unsigned long seed, double start_radius)
        : polynomial(polynomial), population(population), seed(seed), start_radius(start_radius) {
    this->pid = pid;
    this->pname = move(pname);
    for(int& i : this->buffer) i = 0;
    rng::seedGenerator(seed);
    for(int i = 0; i < 20; i++){
        rng::getRealNormalDist(0, 10000.0);
    }
}


int Worker::mainProcedure() {
    //code is the status code
    int code = NOTHING;
    bool continueEvolving = true;
    double summary[Population::SUM_SIZE];
   
    //initialize population
    population.init(start_radius);
    //initial population statistics
    population.getSummary(summary);
    //send statistics to master node
    MPI_Send(summary, Population::SUM_SIZE, MPI_DOUBLE, MASTER_PID, TAG_SUMMARY, MPI_COMM_WORLD);
    //evolutionary loop
    while (true){
 
        //wait to start
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(&code, 1, MPI_INTEGER, MASTER_PID, MPI_COMM_WORLD);
        if (code == STOP) break;
        //evolve population once
        int status = population.evolve();
        MPI_Recv(&code, 1, MPI_INTEGER, MASTER_PID, TAG_QUEUED_UP, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (status == Population::FOUND) {
            code = COMPLETE_MORE;
 
            MPI_Send(&code, 1, MPI_INTEGER, MASTER_PID, TAG_STATUS_UPDATE, MPI_COMM_WORLD);
            complex<double> found = (complex<double>) population.getBestFit();
            MPI_Send(&found, 1, MPI_DOUBLE_COMPLEX, MASTER_PID, TAG_FINAL, MPI_COMM_WORLD);
        }
        else if (status == Population::CONVERGED) {
            code = CONVERGED;
            MPI_Send(&code, 1, MPI_INTEGER, MASTER_PID, TAG_STATUS_UPDATE, MPI_COMM_WORLD);
        }
        else if (status == Population::NOT_FOUND) {
            code = CONTINUE;
            MPI_Send(&code, 1, MPI_INTEGER, MASTER_PID, TAG_STATUS_UPDATE, MPI_COMM_WORLD);
        }
        else {
            cout << "Unknown status code reported" << endl;
            code = CONTINUE;
            MPI_Send(&code, 1, MPI_INTEGER, MASTER_PID, TAG_STATUS_UPDATE, MPI_COMM_WORLD);
        }
        population.getSummary(summary);
        //send array to master node
        MPI_Send(summary, Population::SUM_SIZE, MPI_DOUBLE, MASTER_PID, TAG_SUMMARY, MPI_COMM_WORLD);
    }

    const complex<double> best = population.getBestFit().getChromosome();

    MPI_Send(&best, 1, MPI_DOUBLE_COMPLEX, MASTER_PID, TAG_FINAL, MPI_COMM_WORLD);

    return 0;
}

