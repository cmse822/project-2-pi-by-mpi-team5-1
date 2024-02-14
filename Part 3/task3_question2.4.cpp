#include <iostream>
#include "mpi.h"
#include <fstream>
#include <sstream>
using namespace std;

int main(int argc, char *argv[]) {
    int numtasks;
    int rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // std::cout << "Processor rank: " << rank << std::endl;
    // std::cout << "Processor numtasks: " << numtasks << std::endl;
    std::stringstream iss;
    iss << "process_" << rank << ".txt";
    std::string file_name = iss.str();

    std::ofstream write_file(file_name.c_str());
    if (!write_file.is_open()) {
        std::cerr << "Fail" << file_name;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    write_file << "Hello from process " << rank << " out of " << numtasks << " processes." << std::endl;

    write_file.close();

    MPI_Finalize();

    return 0;

}
// bash-4.2$ mpic++ task3_question2.4.cpp -o task3_question2.4_result
// bash-4.2$ mpirun -np 4 ./task3_question2.4_result 
// Processor rank: 3
// Processor numtasks: 4
// Processor rank: 0
// Processor numtasks: 4
// Processor rank: 1
// Processor numtasks: 4
// Processor rank: 2
// Processor numtasks: 4
