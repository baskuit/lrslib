import os
import time
import random

random.seed(11)

"""

Generates games in "lrslib/games" (if python is called from lrslib)

"""

n_files = 5
rows = 2
cols = 2
max = 2

def matrix_string (rows = 2, cols = 2, max=2):

    row_array = [
        [
            (random.randint(-max, max + 1), abs(random.randint(-max, max + 1)))
            for col in range(cols)
        ]
        for row in range(rows)
    ]
    row_string = "\n".join([" ".join([f"{entry[0]}/{entry[1]}"  for entry in row]) for row in row_array])
    col_string = "\n".join([" ".join([f"{entry[1] - entry[0]}/{entry[1]}"  for entry in row]) for row in row_array])

    string = f"{rows} {cols}\n\n" + row_string + "\n\n" + col_string + "\n"
    return string


for i in range(n_files):
    current_time = time.strftime(
        "%Y%m%d%H%M%S", time.gmtime()
    )  # Get the current UTC time
    filename = (
        f"{current_time}_{i}"  # Add an increment discriminator to the filename
    )
    filepath = os.path.join(os.getcwd(), "games", filename)  # Create the full path to the file

    with open(filepath, "w") as file:
        file.write(matrix_string(rows, cols, max))  # Write the current UTC time to the file
