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

################################### Processing the data: #######################################

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

################################### Plotting: ################################################
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

################################### Convergence: ################################################
def calculate_convergence_rates(errors, darts):
    rates = []
    for i in range(1, len(errors)):
        e_n = errors[i-1]
        e_n_plus_1 = errors[i]
        h_n = 1 / darts[i-1]
        h_n_plus_1 = 1 / darts[i]
        
        rate = np.log(abs(e_n_plus_1 / e_n)) / np.log(abs(h_n_plus_1 / h_n))
        rates.append(rate)
    return rates

for processor_count, values in data.items():
    values.sort(key=lambda x: x[0])  # sorting
    darts = [v[0] for v in values]
    errors = [v[1] for v in values]
    
    # Calculateing convergence rates
    rates = calculate_convergence_rates(errors, darts)
    
    # Give the convergence rates:
    print(f"Convergence rates for {processor_count} processors:")
    for i, rate in enumerate(rates, start=1):
        print(f"  Between {darts[i-1]} and {darts[i]} darts: {rate:.4f}")

################################ Explanation of results: ##########################################
"""
Below is the printed results from the Convergence portion of the code:

Convergence rates for 1 processors:
  Between 1000 and 1000000 darts: 0.5289
  Between 1000000 and 1000000000 darts: 0.4325
Convergence rates for 2 processors:
  Between 1000 and 1000000 darts: 0.4470
  Between 1000000 and 1000000000 darts: 0.4791
Convergence rates for 4 processors:
  Between 1000 and 1000000 darts: 0.5429
  Between 1000000 and 1000000000 darts: 0.3031
Convergence rates for 8 processors:
  Between 1000 and 1000000 darts: 0.3042
  Between 1000000 and 1000000000 darts: 0.4882
Convergence rates for 16 processors:
  Between 1000 and 1000000 darts: 0.7087
  Between 1000000 and 1000000000 darts: 0.1110
Convergence rates for 32 processors:
  Between 1000 and 1000000 darts: 0.6150
  Between 1000000 and 1000000000 darts: 0.5357
Convergence rates for 64 processors:
  Between 1000 and 1000000 darts: 0.4856
  Between 1000000 and 1000000000 darts: 0.2252

The results of the plot and the convergence rates are varied. On AMD20, we see that processor counts 4, 16, and 64
show diminishing convergence rates from 1E6 to 1E9 whereas processor counts 1, 2, 8, and 32 show faster convergences
fro 1E6 to 1E9 and have a lower absolute error in the estimates of Pi.  It is clear that as we increase the number of 
simuluations that we approximate the true values of Pi, however why there is an irregular pattern about which processor
counts have better/worse approximations of Pi could be due to the randomness of the simulations.
"""
        
