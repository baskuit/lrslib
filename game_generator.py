import os
import time
import random

random.seed(11)

"""

Generates games in "lrslib/games" (if python is called from lrslib)

"""

def matrix_string (rows = 2, cols = 2, max=2):

    row_array = [
        [
            (random.randint(-max, max + 1), abs(random.randint(-max, max + 1)))
            for col in range(cols)
        ]
        for row in range(rows)
    ]
    row_array = [
        [
            (entry[0], entry[1] + (entry[1] == 0))
            for entry in row
        ]
        for row in row_array
    ]
    row_string = "\n".join([" ".join([f"{entry[0]}/{entry[1]}"  for entry in row]) for row in row_array])
    col_string = "\n".join([" ".join([f"{entry[1] - entry[0]}/{entry[1]}"  for entry in row]) for row in row_array])

    string = f"{rows} {cols}\n\n" + row_string + "\n\n" + col_string + "\n"
    return string


def make_batch (rows, cols, n_files, max=2, dir="new_games"):

    for i in range(n_files):
        current_time = time.strftime(
            "%Y%m%d%H%M%S", time.gmtime()
        )  # Get the current UTC time
        filename = (
            f"{current_time}_{i}_{random.randint(0, 2**10)}"  # Add an increment discriminator to the filename
        )
        filepath = os.path.join(os.getcwd(), dir, filename)  # Create the full path to the file

        with open(filepath, "w") as file:
            file.write(matrix_string(rows, cols, max))  # Write the current UTC time to the file

if __name__ == "__main__":

    game_dir = os.path.join(os.getcwd(), 'new_games')
    if not os.path.exists(game_dir):
        os.mkdir(game_dir)

    row_sched = [2, 3, 4, 5, 6, 7, 8, 9]
    col_sched = row_sched

    n_files = 5
    max = 2

    for rows, cols in zip(row_sched, col_sched):
        make_batch(rows, cols, n_files, max)

    # for rows, cols in zip(row_sched, col_sched):
    #     make_batch(rows, cols, n_files, 20)
