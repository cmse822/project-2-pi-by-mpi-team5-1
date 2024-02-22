#include <iostream>
#include "mpi.h"
using namespace std;

int main(int argc, char *argv[]) {
    int numtasks;
    int rank;
    cout << "before init" << endl;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    cout << "hello world! " << "from process: " << rank << endl;

    MPI_Finalize();
    cout << "after finalize, " << "rank: " << rank << endl;
    return 0;
}

// to run the program 
// salloc -n 4 --time 00:30:00
// mpic++ task3_hello.cpp -o task3_hello
// mpiexec -n 4 ./task3_hello > task3_hello.output