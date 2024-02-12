## Part one Warm-up

- question 1  
Based on the first touch policy, which states that, the first thread to touch the area in effect causes the data to be allocated on the memory of its socket. In the first serial loop, array_a is initialized on a single thread, and then running calculations to reassign values for array_a in parallel, where different sockets are trying to access array_a. Access to array_a would be slower due to the memory being non-local, it lead to performance degradation which defeats the purpose of parallelism.

- question 2


- question 3
```
int leftproc, rightproc;
double bfromleft, bfromright;

MPI_Comm_rank(MPI_COMM_WORLD,&myTaskID);
MPI_Comm_size(MPI_COMM_WORLD,&nTasks);

if (myTaskID==0) leftproc = MPI_PROC_NULL;
    else leftproc = myTaskID-1;
if (myTaskID==nTasks-1) rightproc = MPI_PROC_NULL;
    else rightproc = myTaskID+1;

if (myTaskID != 0) {
    MPI_Sendrecv( &b[0], &bfromright, leftproc);
}

if (myTaskID != nTasks - 1) {
    MPI_Sendrecv( &b[LocalProblemSize-1], &bfromleft,  rightproc );
}

for (i = 0; i < LocalProblemSize; i++) {
    double bleft, bright;

    if (i == 0 && myTaskID == 0) {
        bleft = NULL;
    } else if (i == 0) {
        bleft = bfromleft;
    } else {
        bleft = b[i-1];
    }

    if (i == LocalProblemSize - 1 && myTaskID == nTasks - 1) {
        bright = NULL; 
    } else if (i == LocalProblemSize - 1) {
        bright = bfromright;
    } else {
        bright = b[i+1];
    }

    if (myTaskID == 0 && i == 0) {
        /// first element
        a[i] = (b[i] + bright) / 2.0;
    } else if (myTaskID == nTasks - 1 && i == LocalProblemSize - 1) {
        /// last element
        a[i] = (b[i] + bleft) / 2.0;
    } else {
        a[i] = (b[i] + bleft + bright) / 3.0;
    }
}

```

- question 4


- question 5  
As mentioned in the discussion point above, one message is being sent instead of one, which means lesser bandwidth is required as there are lesser data waiting to be sent. There is also lower latency as the time taken to initiate and complete sending one message is lesser than four.

- question 6


