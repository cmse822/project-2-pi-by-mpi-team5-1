/***************************************************************************
Team 5: Project 2
Authors: Ryan Jayne, Erika Zheng, Junchi Zhu, Sona Chitchyan

Code adapted from original work from Fox et al.(1988) Solving Problems on 
Concurrent Processors, vol.1 page 207.  
***************************************************************************/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void srandom (unsigned seed);  
double dboard (int darts);

#define DARTS 10000    /* number of throws at dartboard */
#define ROUNDS 100     /* number of times "darts" is iterated */

int main(int argc, char *argv[])
{
    int rank, size;
    double pi, avepi;
    double local_pi, global_pi;
    double start_time, end_time, total_time;
    int local_rounds, i;

    MPI_Init(&argc, &argv); // Initialize MPI environment
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Getting rank of process
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Getting number of processes

    // Here we'll calculate the number of rounds for each of the processes
    local_rounds = ROUNDS / size;

    if (rank == 0) {
        printf("Starting parallel version of pi calculation using dartboard algorithm...\n");
    }
    srandom(5); // Seed the random number generator differently for each process

    start_time = MPI_Wtime(); // Starts timing

    avepi = 0.0;
    for (i = 0; i < local_rounds; i++) {
        local_pi = dboard(DARTS / size); // Each process calculates its portion
        
        // Gather results from all processes to the root process
        // Key part here! -- Here is where we gather results from all 
        // processes to the root process.  Here's a breadown:
        // &local_score -- address of the local var that holds the results from each pro
        // &total_score -- address of the var where reduction operation is stored
        // '1' -- each process sends one integer
        // MPI_INT -- is the data type being sent
        // MPI_SUM -- this is the operation we're doing -- sum
        // '0' -- rank of the root process -- this rank process will recieve the results        
        // Gather results from all processes to the root process
        MPI_Reduce(&local_pi, &global_pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        if (rank == 0) { // Because rank 0 is the location that takes the results, as specified above
            avepi += global_pi / size; // Accumulate average of the global_pi value
        }
    }

    if (rank == 0) {
        avepi /= local_rounds; // Final average calculation
        printf("   After %d rounds, average value of pi = %10.8f\n", ROUNDS, avepi);
    }

    end_time = MPI_Wtime(); // End timing
    total_time = end_time - start_time;

    if (rank == 0) {
        printf("\nReal value of PI: 3.1415926535897 \n");
        printf("Total runtime: %f seconds\n", total_time);
    }

    MPI_Finalize(); // Finalize MPI environment
    return 0;
}

/*****************************************************************************
 * dboard
 *****************************************************************************/
#define sqr(x)	((x)*(x))
long random(void);

double dboard(int darts)
{
   double x_coord,       /* x coordinate, between -1 and 1  */
          y_coord,       /* y coordinate, between -1 and 1  */
          pi,            /* pi  */
          r;             /* random number scaled between 0 and 1  */
   int score,            /* number of darts that hit circle */
       n;
   unsigned int cconst;  /* must be 4-bytes in size */
/*************************************************************************
 * The cconst variable must be 4 bytes. We check this and bail if it is
 * not the right size
 ************************************************************************/
   if (sizeof(cconst) != 4) {
      printf("Wrong data size for cconst variable in dboard routine!\n");
      printf("See comments in source file. Quitting.\n");
      exit(1);
      }
   /* 2 bit shifted to MAX_RAND later used to scale random number between 0 and 1 */
   cconst = 2 << (31 - 1);
   score = 0;

/***********************************************************************
 * Throw darts at board.  Done by generating random numbers
 * between 0 and 1 and converting them to values for x and y
 * coordinates and then testing to see if they "land" in
 * the circle."  If so, score is incremented.  After throwing the
 * specified number of darts, pi is calculated.  The computed value
 * of pi is returned as the value of this function, dboard.
 ************************************************************************/

   for (n = 1; n <= darts; n++) {
      /* generate random numbers for x and y coordinates */
      r = (double)random()/cconst;
      x_coord = (2.0 * r) - 1.0;
      r = (double)random()/cconst;
      y_coord = (2.0 * r) - 1.0;

      /* if dart lands in circle, increment score */
      if ((sqr(x_coord) + sqr(y_coord)) <= 1.0)
         score++;
      }

   /* calculate pi */
   pi = 4.0 * (double)score/(double)darts;
   return(pi);
} 
