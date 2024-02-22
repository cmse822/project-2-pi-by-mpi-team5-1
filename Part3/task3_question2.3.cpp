#include <iostream>
#include "mpi.h"
using namespace std;

int main(int argc, char *argv[]) {
    int numtasks;
    int rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int len_name;
    MPI_Get_processor_name(processor_name, &len_name);

    std::string proc_name(processor_name, len_name);

    std::cout << "Processor name: " << proc_name << std::endl;

    MPI_Finalize();

    return 0;
    // salloc -N 2 --ntasks-per-node=1 --time=00:10:00, command requests an interactive job with two nodes
    // mpirun -np 2 ./task3_question2.3 >> task3_question2.3.output
}
