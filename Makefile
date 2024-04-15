CC=gcc
CFLAGS=-Wall -std=c99 -fopenmp
MERGESORT=merge_sort
BINARYSEARCH=binary_search
MATRIXMULT=matrix_multiplication

MERGESORTSRC=src/sorting/parallel_merge_sort/merge_sort.c
BINARYSEARCHSRC=src/search/parallel_binary_search/binary_search.c
MATRIXMULTSRC=src/other_apps/parallel_matrix_multiplication/matrix_multiplication.c

MERGESORTINPUTS=src/sorting/parallel_merge_sort/inputs
BINARYSEARCHINPUTS=src/search/parallel_binary_search/inputs
MATRIXMULTINPUTS=src/other_apps/parallel_matrix_multiplication/inputs

mergesort:
	$(CC) $(CFLAGS) -o $(MERGESORT) $(MERGESORTSRC)
	./$(MERGESORT) $(MERGESORTINPUTS)/small_input.txt 1
	./$(MERGESORT) $(MERGESORTINPUTS)/small_input.txt 2
	./$(MERGESORT) $(MERGESORTINPUTS)/small_input.txt 4
	./$(MERGESORT) $(MERGESORTINPUTS)/small_input.txt 8

	./$(MERGESORT) $(MERGESORTINPUTS)/medium_input.txt 1
	./$(MERGESORT) $(MERGESORTINPUTS)/medium_input.txt 2
	./$(MERGESORT) $(MERGESORTINPUTS)/medium_input.txt 4
	./$(MERGESORT) $(MERGESORTINPUTS)/medium_input.txt 8

	./$(MERGESORT) $(MERGESORTINPUTS)/large_input.txt 1
	./$(MERGESORT) $(MERGESORTINPUTS)/large_input.txt 2
	./$(MERGESORT) $(MERGESORTINPUTS)/large_input.txt 4
	./$(MERGESORT) $(MERGESORTINPUTS)/large_input.txt 8

	./$(MERGESORT) $(MERGESORTINPUTS)/extreme_large_input.txt 1
	./$(MERGESORT) $(MERGESORTINPUTS)/extreme_large_input.txt 2
	./$(MERGESORT) $(MERGESORTINPUTS)/extreme_large_input.txt 4
	./$(MERGESORT) $(MERGESORTINPUTS)/extreme_large_input.txt 8
binarysearch:
	$(CC) $(CFLAGS) -o $(BINARYSEARCH) $(BINARYSEARCHSRC)
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/small_input.txt 1 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/small_input.txt 2 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/small_input.txt 4 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/small_input.txt 8 50

	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/medium_input.txt 1 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/medium_input.txt 2 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/medium_input.txt 4 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/medium_input.txt 8 50

	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/large_input.txt 1 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/large_input.txt 2 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/large_input.txt 4 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/large_input.txt 8 50

	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/extreme_large_input.txt 1 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/extreme_large_input.txt 2 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/extreme_large_input.txt 4 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/extreme_large_input.txt 8 50

matrixmult:
	$(CC) $(CFLAGS) -o $(MATRIXMULT) $(MATRIXMULTSRC)
	./$(MATRIXMULT) $(MATRIXMULTINPUTS)/matrix_A_100.txt $(MATRIXMULTINPUTS)/matrix_B_100.txt 1
	./$(MATRIXMULT) $(MATRIXMULTINPUTS)/matrix_A_100.txt $(MATRIXMULTINPUTS)/matrix_B_100.txt 2
	./$(MATRIXMULT) $(MATRIXMULTINPUTS)/matrix_A_100.txt $(MATRIXMULTINPUTS)/matrix_B_100.txt 4
	./$(MATRIXMULT) $(MATRIXMULTINPUTS)/matrix_A_100.txt $(MATRIXMULTINPUTS)/matrix_B_100.txt 8

	./$(MATRIXMULT) $(MATRIXMULTINPUTS)/matrix_A_1000.txt $(MATRIXMULTINPUTS)/matrix_B_1000.txt 1
	./$(MATRIXMULT) $(MATRIXMULTINPUTS)/matrix_A_1000.txt $(MATRIXMULTINPUTS)/matrix_B_1000.txt 2
	./$(MATRIXMULT) $(MATRIXMULTINPUTS)/matrix_A_1000.txt $(MATRIXMULTINPUTS)/matrix_B_1000.txt 4
	./$(MATRIXMULT) $(MATRIXMULTINPUTS)/matrix_A_1000.txt $(MATRIXMULTINPUTS)/matrix_B_1000.txt 8

	./$(MATRIXMULT) $(MATRIXMULTINPUTS)/matrix_A_10000.txt $(MATRIXMULTINPUTS)/matrix_B_10000.txt 1
	./$(MATRIXMULT) $(MATRIXMULTINPUTS)/matrix_A_10000.txt $(MATRIXMULTINPUTS)/matrix_B_10000.txt 2
	./$(MATRIXMULT) $(MATRIXMULTINPUTS)/matrix_A_10000.txt $(MATRIXMULTINPUTS)/matrix_B_10000.txt 4
	./$(MATRIXMULT) $(MATRIXMULTINPUTS)/matrix_A_10000.txt $(MATRIXMULTINPUTS)/matrix_B_10000.txt 8

clean:
	rm -f $(MERGESORT) $(BINARYSEARCH) $(MATRIXMULT)
