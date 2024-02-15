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

# Check where we are
print("Current Directory:", os.getcwd())

# The true value of Pi for comparison
true_pi = np.pi

# A dictionary to store error data, keyed by processor count
data = {}

# Read one of the output files -- currently for computing node amd20
with open('Part4/p7_amd20.output', 'r') as file:
    lines = file.readlines()
    
    # Initialize vars
    processor_count = 0
    darts = 0  
    
    # Process each line in the output file
    for line in lines:
        if 'total processor count:' in line:
            processor_count = int(line.split(':')[1].strip())
            data[processor_count] = []
        elif line.strip().isdigit():  
            # Extracting number of darts from lines that contain only digits
            darts = int(line.strip())
        elif 'Estimated Pi' in line:
            # Extracting our estimated Pi value and calculate the error
            estimated_pi = float(line.split('=')[1].strip())
            error = abs(estimated_pi - true_pi)
            # Storing darts and error in the dictionary
            if darts > 0:  
                data[processor_count].append((darts, error))

# Plotting
plt.figure(figsize=(10, 6))

# Plot error vs. number of darts for each processor count
for processor_count, values in data.items():
    # Sort values by number of darts 
    values = sorted(values, key=lambda x: x[0])  
    darts = [v[0] for v in values]
    errors = [v[1] for v in values]
    
    # Log-log plot for each processor count
    plt.loglog(darts, errors, label=f'{processor_count} Processors', marker='o', linestyle='-')

plt.xlabel('Number of Darts')
plt.ylabel('Absolute Error')
plt.title('Error in Computed/Estimated Pi vs. Number of Darts (Log-Log Scale)')
plt.legend()
plt.grid(True, which="both", ls="--")
plt.show()