
#include <iomanip>
#include "Master.h"

using namespace std;
Master::Master(int pid, string pname, Polynomial& polynomial)
        : polynomial(polynomial), finished(false), generation(0), shouldPrintHeader(true) {
    this->pid = pid;
    this->pname = move(pname);
    for(int& i : this->buffer) i = 0;

}




void Master::printHeader() {

    cout << "==============================================================================================================" << endl;
    cout << "Generation: " << generation << endl;
    cout << fixed << setw(14) << "Worker Id |"
    		  << fixed << setw(14) << "Fitness |"
              << fixed << setw(14) << "Root 	\n";
              
    cout << "--------------------------------------------------------------------------------------------------------------" << endl;
}


void Master::printReport(int id, double *summary, string note) {

    cout << scientific << setw(11) << setprecision(3) << id << "  |"
    		  << scientific << setw(11) << setprecision(3) << summary[Population::SUM_NDX_MIN] <<" |"
              << scientific << setw(11) << setprecision(3) << summary[Population::SUM_NDX_REAL] << " + " <<summary[Population::SUM_NDX_IMAG]<<"i \n "  ;            
              
}


int Master::mainProcedure(){
    auto startTime  = chrono::high_resolution_clock::now();
    auto epochStart = startTime;

  

    cout << "starting" << endl;

    int code = NOTHING;

    complex<double> final;
    int finder = -1;
    double summary[Population::SUM_SIZE];

    printHeader();

    //get initial population stats
    for (int i = 1; i < networkSize; i++){
        MPI_Recv(summary, Population::SUM_SIZE, MPI_DOUBLE, i, TAG_SUMMARY, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printReport(i, summary," ==> Initial population");
    }

    code = CONTINUE;
    generation++;

    while (true){

        epochStart = chrono::system_clock::now();
        //wait for workers to finish their evolutions
        MPI_Barrier(MPI_COMM_WORLD);
        if (finished) code = STOP;
        //allow workers to continue
        MPI_Bcast(&code, 1, MPI_INTEGER, MASTER_PID, MPI_COMM_WORLD);
        if (finished) break;
        printHeader();

        for(int wid = 1; wid < networkSize; wid++){
            //allow process wid to send report and other information to master
            MPI_Send(&code, 1, MPI_INTEGER, wid, TAG_QUEUED_UP, MPI_COMM_WORLD);
            //receive status from worker
            MPI_Recv(&code, 1, MPI_INTEGER, wid, TAG_STATUS_UPDATE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if(code == COMPLETE || code == COMPLETE_MORE) {
                finished = true;
            }
            if(code == COMPLETE_MORE) {
                MPI_Recv(&final, 1, MPI_DOUBLE_COMPLEX, wid, TAG_FINAL, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                finder = wid;
            }
            //get summary of progress
            MPI_Recv(summary, Population::SUM_SIZE, MPI_DOUBLE, wid, TAG_SUMMARY, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //print summary
            if (code == CONVERGED) printReport(wid, summary, " ==> Convergence");
            else printReport(wid, summary);

        }

        chrono::duration<double, milli> epochDuration = chrono::high_resolution_clock::now() - epochStart;

        cout << defaultfloat << setprecision(10)  << "Epoch during: " << 0.001 * epochDuration.count() << "s" << endl;



        generation++;
    }

    cout << "\n\n";
    cout << defaultfloat << "f(x) = " << polynomial << "\n\n";

    complex<double> finalResults[networkSize];
    for (int i = 1; i < networkSize; i++){
        MPI_Recv(&finalResults[i], 1, MPI_DOUBLE_COMPLEX, i, TAG_FINAL, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "Worker (" << i << ") best result = " << finalResults[i].real() << " + " << finalResults[i].imag() << "i ==> " << abs(polynomial(finalResults[i])) << endl;
    }


    cout << "\n";
    cout << fixed
              << "f(x) = 0 when x ~= " << final.real() << " + " << final.imag() << "i"
              << " with the fitness of " << abs(polynomial(final)) << endl;
    cout << "found by " << finder << endl;

    chrono::duration<double, milli> duration = chrono::high_resolution_clock::now() - startTime;

    cout << fixed << setprecision(10) << "time elapsed: " << 0.001 * duration.count() << "s" << endl;


}


