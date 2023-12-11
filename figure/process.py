import pandas as pd

# Function to process the data and find the maximum ratio for each algorithm compared to 'Ours'


def find_max_ratio_across_datasets(file_path):
    # Read the data from the file
    with open(file_path, 'r') as file:
        lines = file.readlines()

    # Initialize a dictionary to store the dataframes for each dataset
    dataframes = {}
    # Initialize a dictionary to store the maximum ratio for each algorithm
    max_ratios = {'Cuckoo': 0, 'Coco': 0, 'USS': 0, 'Elastic': 0}

    # Process each line
    for line in lines:
        if line.startswith('ratio'):
            # Dataset and load factor
            parts = line.split()
            dataset_name = parts[0][7:]
            load_factor = float(parts[1])
            if dataset_name not in dataframes:
                dataframes[dataset_name] = pd.DataFrame()
        else:
            # Algorithm data
            parts = line.split()
            algorithm, mse = parts[0], float(parts[3])
            dataframes[dataset_name].at[load_factor, algorithm] = mse

    # Calculate the maximum ratio of 'Ours' compared to other algorithms for each dataset
    for df in dataframes.values():
        for algorithm in max_ratios.keys():
            current_max_ratio = (df[algorithm] / df['Ours']).max()
            max_ratios[algorithm] = max(
                max_ratios[algorithm], current_max_ratio)

    return max_ratios


# Assuming the data file is named 'memory_error_data.txt' and is located in the /mnt/data/ directory.
file_path = './memory_error_data.txt'
max_ratio_values = find_max_ratio_across_datasets(file_path)

print(max_ratio_values)
