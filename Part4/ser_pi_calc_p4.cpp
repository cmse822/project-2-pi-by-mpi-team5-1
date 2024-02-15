#include <mpi.h>
#include <iostream>
#include <vector>
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
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int len_name;
    MPI_Get_processor_name(processor_name, &len_name);
    std::string proc_name(processor_name, len_name);
    std::cout << "Processor name: " << proc_name << std::endl;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    cout<<"total processor count: "<< size << endl;
    int darts = 10000;
    int rounds = 100;
    vector<int> darts_list = {1000,1000000,1000000000};
    
    double pi_result;
    
    for (auto dart:darts_list){
        MPI_Barrier(MPI_COMM_WORLD); 
        int total_rounds = dart*rounds; 
        int rounds_per_rank = total_rounds / size;
        cout << dart << endl;
        int extra = total_rounds % size; 
        if (rank < extra) {
            rounds_per_rank++;
        }

        double start_time = MPI_Wtime();
        calculateAndPrintPi(rounds_per_rank, rank, size, &pi_result);
        double end_time = MPI_Wtime();

        double local_run_time = end_time - start_time;
        MPI_Barrier(MPI_COMM_WORLD); 
        cout<<"Rank: "<< rank << " Calculation took "<<local_run_time<< " seconds."<< endl;

        double total_run_time;
        MPI_Reduce(&local_run_time, &total_run_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD); 
        
        if (rank == 0) {
            cout<<"Total runtime = " << total_run_time << endl;
        }
    }
    MPI_Finalize();
    return 0;
}

