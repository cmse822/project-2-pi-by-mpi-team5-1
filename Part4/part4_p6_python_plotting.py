################################################################################################
"""
Part 6: For each dart count, make a plot of runtime versus processor count. Each line 
        represents a "strong scaling" study for your code. For each dart count, also plot the 
        "ideal" scaling line. Calculate the parallel scaling efficiency of your code for each dart 
        count. Does the parallel performance vary with dart count? Explain your answer.
"""
################################################################################################

import matplotlib.pyplot as plt
import numpy as np
import os
import re

# Check the current working directory
print("Current Directory:", os.getcwd())

# Making a few functions to generate the outputs

# Func to read the p4_6.output file
def read_output_file(filename):
    data = {}
    current_dart_count = None  
    with open(filename, 'r') as file:
        for line in file:
            if 'total processor count' in line:
                processor_count = int(line.split(': ')[1].strip())
            elif line.strip().isdigit():  
                current_dart_count = int(line.strip())
                if current_dart_count not in data:
                    data[current_dart_count] = {}
            elif 'Total runtime' in line:
                runtime = float(line.split('= ')[1].strip())
                if processor_count not in data[current_dart_count]:
                    data[current_dart_count][processor_count] = []
                data[current_dart_count][processor_count].append(runtime)
    return data

filename = 'Part4/p4_6.output'  
our_data = read_output_file(filename)

# Plot data and the ideal scaling line -- we're plotting the ideal scaling line based on notion of 'speed-up' being T1/Tn
# where T1 is the amount of time for 1 processor and n is the number of processors we're distributing the work to.
def plot_data_for_specific_dart_counts(data, specific_dart_counts):
    for dart_count in specific_dart_counts:
        proc_data = data[dart_count]
        proc_counts = sorted(proc_data.keys())
        runtimes = [np.mean(proc_data[proc]) for proc in proc_counts]  
        ideal_runtime = runtimes[0] / np.array(proc_counts)
        
        # Plotting runtime
        plt.figure(figsize=(10, 6))  
        plt.plot(proc_counts, runtimes, label=f'Actual Runtime for {dart_count:.0e} darts', marker='o')
        
        # Plottomg tje ideal scaling
        plt.plot(proc_counts, ideal_runtime, '--', label=f'Ideal Scaling for {dart_count:.0e} darts')
        
        # Calculating the parallel scaling eficiency and inserting its values -- efficiency = ideal runtime (T1/TN) / runtime == T1/(TN * runtime)
        efficiency = (ideal_runtime / np.array(runtimes)) * 100
        for i, eff in enumerate(efficiency):
            plt.text(proc_counts[i], runtimes[i], f"{eff:.2f}%", fontsize=9, verticalalignment='bottom')
        
        # Put it all in the plot
        plt.xlabel('Processor Count')
        plt.ylabel('Runtime (seconds)')
        plt.title(f'Runtime vs. Processor Count for {dart_count:.0e} Darts')
        plt.legend()
        plt.grid(True)
        plt.show()

# Apply the func
specific_dart_counts = [1E3, 1E6, 1E9]
plot_data_for_specific_dart_counts(our_data, specific_dart_counts)

# Write up:

"""
We have plotted the runtimes for each of the different dart counts 1E3 - 1E9 over the processor counts. Using the 
scale-up formula of T1/TN, we see varying results over the different dart counts as the number of processors increases.
For 1E3 darts, there are clear indications of inefficiency as processor counts increase -- this is likely due to several
factors.  Some of the probable reasons are overhead communication between the processors and likely due to Amdhal's law.
Overhead communication costs are, with a marginal change of a given processor, greater than the marginal improvements in 
run time as the number of processors increase beyond around 15 when we operate on 1E3 darts.  The fact that the actual runtime
approximates the ideal scaling for 1E6 and 1E9 darts supports Amdhal's law because the proportion of the parallelizable work
becomes larger as we increase the amount of darts.
"""

