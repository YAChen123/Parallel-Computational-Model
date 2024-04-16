import numpy as np

def create_matrix_file(n, filename):
    matrix = np.random.randint(0, 100, size=(n, n))  # Random integers between 0 and 99
    with open(filename, 'w') as f:
        f.write("{}\n".format(n))  # Changed to use str.format()
        for row in matrix:
            f.write(' '.join(map(str, row.tolist())) + '\n')

create_matrix_file(100, 'matrix_A_100.txt')
create_matrix_file(100, 'matrix_B_100.txt')
create_matrix_file(1000, 'matrix_A_1000.txt')
create_matrix_file(1000, 'matrix_B_1000.txt')
create_matrix_file(10000, 'matrix_A_10000.txt')
create_matrix_file(10000, 'matrix_B_10000.txt')


