#include <iostream>
#include "mpi.h"
using namespace std;

int main(int argc, char *argv[]) {
    int numtasks;
    int rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // code from the lecture
    if(rank == 0){
        std::cout << "Process" << rank << " report: there are " << numtasks << " processes in total." << std::endl;
    }

    MPI_Finalize();

    return 0;
}
// bash-4.2$ mpic++ task3_question2.5.cpp -o task3_question2.5_result
// bash-4.2$ mpirun -np 4 ./task3_question2.5_result 
// Process0 report: there are 4 processes in total.