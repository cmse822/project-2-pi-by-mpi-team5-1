################################################################################################
"""
Part 5: For each processor count, plot the resulting errors in your computed values 
        of pi compared to the true value as functions of the number of darts used to 
        compute it. Use log-log scaling on this plot. What is the rate of convergence of 
        your algorithm for computing pi? Does this value make sense? Does the error or rate 
        of convergence to the correct answer vary with processor count? Should it?
"""
################################################################################################

# Currently the data in the .output files needs a bit of processing/manipulating

import numpy as np
import matplotlib.pyplot as plt
import os

# Check the current working directory
print("Current Directory:", os.getcwd())

# Getting true val of pi to compare
true_pi = np.pi
data = {}

# The correct file (for number 5 is p4_6.output)
with open('Part4/p4_6.output', 'r') as file:
    lines = file.readlines()
    
    processor_count = 0
    darts = 0  
    
    # Process each line in the output file
    for line in lines:
        if 'total processor count:' in line:
            processor_count = int(line.split(':')[1].strip())
            data[processor_count] = []
        elif line.strip().isdigit():  
            darts = int(line.strip())
        elif 'Estimated Pi' in line:
            estimated_pi = float(line.split('=')[1].strip())
            error = abs(estimated_pi - true_pi)
            # Storing darts and error in the dictionary for each processor count
            data[processor_count].append((darts, error))

# Plotting 
plt.figure(figsize=(10, 6))

for processor_count, values in data.items():
    # Sort values by number of darts for consistent plotting
    values.sort(key=lambda x: x[0])  
    darts = [v[0] for v in values]
    errors = [v[1] for v in values]
    
    plt.loglog(darts, errors, label=f'{processor_count} Processors', marker='o', linestyle='-')

plt.xlabel('Number of Darts')
plt.ylabel('Absolute Error')
plt.title('Error in Computed/Estimated Pi vs. Number of Darts (Log-Log Scale)')
plt.legend()
plt.grid(True, which="both", ls="--")

plt.show()