#include <mpi.h>
#include <iostream>
using namespace std;

void calculateAndPrintPi(int rounds, int rank, int size, double* pi_result) {
    int count = 0;
    double x, y;
    
    unsigned int seed = rank;
    for (int i = 0; i < rounds; ++i) {
        x = (double)rand_r(&seed) / RAND_MAX;
        y = (double)rand_r(&seed) / RAND_MAX;
        if (x*x + y*y <= 1){
            count+=1;
        }
    }

    double pi_estimate = 4.0 * count / rounds;

    MPI_Reduce(&pi_estimate, pi_result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        *pi_result /= size;
        cout<<"Estimated Pi = " << *pi_result << endl;
    }

}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int total_rounds = 1000000; 
    double pi_result;

    int rounds_per_rank = total_rounds / size;
    cout<<"size: "<< size <<" rounds: "<<rounds_per_rank<<endl;
    int extra = total_rounds % size; 
    if (rank < extra) {
        rounds_per_rank++;
    }

    double start_time = MPI_Wtime();
    calculateAndPrintPi(rounds_per_rank, rank, size, &pi_result);
    double end_time = MPI_Wtime();

    double local_run_time = end_time - start_time;
    cout<<"Rank: "<< rank << " Calculation took "<<local_run_time<< " seconds."<< endl;

    double total_run_time;
    MPI_Reduce(&local_run_time, &total_run_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        cout<<"Total runtime = " << total_run_time << endl;
    }

    MPI_Finalize();
    return 0;
}

// part 4 question 2
// bash-4.2$ mpic++ task4_eat_pi.cpp -o task4_result
// bash-4.2$ mpiexec -np 1 ./task4_result 
// Estimated Pi = 3.14232
// Rank: 0 Calculation took 0.0134617 seconds.
// Total runtime = 0.0134617

// bash-4.2$ mpiexec -np 2 ./task4_result 
// Estimated Pi = 3.14104
// Rank: 0 Calculation took 0.0135798 seconds.
// Total runtime = 0.0135798
// Rank: 1 Calculation took 0.0135532 seconds.

// bash-4.2$ mpiexec -np 3 ./task4_result 
// Rank: 1 Calculation took 0.0134183 seconds.
// Rank: 2 Calculation took 0.0138174 seconds.
// Estimated Pi = 3.14202
// Rank: 0 Calculation took 0.0141955 seconds.
// Total runtime = 0.0141955

// bash-4.2$ mpiexec -np 4 ./task4_result 
// Rank: 1 Calculation took 0.0133714 seconds.
// Rank: 3 Calculation took 0.0133574 seconds.
// Rank: 2 Calculation took 0.0135553 seconds.
// Estimated Pi = 3.1416
// Rank: 0 Calculation took 0.013842 seconds.
// Total runtime = 0.013842


// part 4 question 3
// bash-4.2$ mpic++ task4_eat_pi.cpp -o task4_result
// bash-4.2$ mpiexec -np 1 ./task4_result 
// size: 1 rounds: 1000000
// Estimated Pi = 3.14232
// Rank: 0 Calculation took 0.0141479 seconds.
// Total runtime = 0.0141479

// bash-4.2$ mpiexec -np 2 ./task4_result 
// size: 2 rounds: 500000
// size: 2 rounds: 500000
// Rank: 1 Calculation took 0.00722418 seconds.
// Estimated Pi = 3.13979
// Rank: 0 Calculation took 0.00725773 seconds.
// Total runtime = 0.00725773

// bash-4.2$ mpiexec -np 3 ./task4_result 
// size: 3 rounds: 333333
// size: 3 rounds: 333333
// size: 3 rounds: 333333
// Rank: 1 Calculation took 0.00448131 seconds.
// Rank: 2 Calculation took 0.00490655 seconds.
// Estimated Pi = 3.14265
// Rank: 0 Calculation took 0.00502171 seconds.
// Total runtime = 0.00502171

// bash-4.2$ mpiexec -np 4 ./task4_result 
// size: 4 rounds: 250000
// size: 4 rounds: 250000
// size: 4 rounds: 250000
// size: 4 rounds: 250000
// Rank: 1 Calculation took 0.00351888 seconds.
// Rank: 3 Calculation took 0.00349815 seconds.
// Rank: 2 Calculation took 0.00358403 seconds.
// Estimated Pi = 3.1423
// Rank: 0 Calculation took 0.00368696 seconds.
// Total runtime = 0.00368696
