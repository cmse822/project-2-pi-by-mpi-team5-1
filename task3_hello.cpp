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